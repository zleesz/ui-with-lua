// System.cpp : Defines the entry point for the DLL application.
//

// Unicode support by Jim Park & Olivier Marcoux

#include "stdafx.h"
#include "System.h"
#define _DECL_DLLMAIN   /* enable prototypes for DllMain and _CRT_INIT */
#include <process.h>
#include <crtdbg.h>
#include <objbase.h>
#include <tchar.h>

typedef TCHAR NSIS_STRING[1024];
#ifdef _MSC_VER
#  pragma bss_seg(NSIS_VARS_SECTION)
NSIS_STRING g_usrvarssection[1];
#  pragma bss_seg()
#  pragma comment(linker, "/section:" NSIS_VARS_SECTION ",rwd")
#else

// Parse Section Type 
#define PST_PROC    0
#define PST_PARAMS  1
#define PST_RETURN  2
#define PST_OPTIONS 3

#define PCD_NONE    0
#define PCD_PROC    1
#define PCD_PARAMS  2
#define PCD_DONE    3   // Just Continue

const int PARAMSIZEBYTYPE_PTR = (4==sizeof(void*)) ? 1 : 2;
const int ParamSizeByType[8] = {
    0, // PAT_VOID (Size will be equal to 1) //BUGBUG64?
    1, // PAT_INT
    2, // PAT_LONG
    sizeof(void*) / 4, // PAT_STRING //BUGBUG64?
    sizeof(void*) / 4, // PAT_WSTRING //BUGBUG64?
    sizeof(void*) / 4, // PAT_GUID //BUGBUG64?
    0, // PAT_CALLBACK (Size will be equal to 1) //BUGBUG64?
    0 // PAT_REGMEM //BUGBUG64?
};

// Thomas needs to look at this.
static const int ByteSizeByType[8] = {
    1, // PAT_VOID
    1, // PAT_INT
    1, // PAT_LONG
    1, // PAT_STRING
    2, // PAT_WSTRING (special case for &wN notation: N is a number of WCHAR, not a number of bytes)
    1, // PAT_GUID
    1, // PAT_CALLBACK
    1 // PAT_REGMEM
};

int LastStackPlace;
int LastStackReal;
DWORD LastError;
volatile SystemProc *LastProc;
int CallbackIndex;
CallbackThunk* g_CallbackThunkListHead;

// Return to callback caller with stack restore
char retexpr[4]; //BUGBUG64?
HANDLE retaddr;
const int g_stringsize = 1024;
LPTSTR g_variables[g_stringsize] = NULL;

TCHAR *AllocString()
{
	return (TCHAR*) GlobalAlloc(GPTR,g_stringsize*sizeof(TCHAR));
}

TCHAR *AllocStr(TCHAR *str)
{
	return lstrcpyn(AllocString(), str, g_stringsize);
}

void *copymem(void *output, void *input, size_t cbSize)
{
	BYTE *out = (BYTE*) output;
	BYTE *in = (BYTE*) input;
	if ((input != NULL) && (output != NULL))
	{
		while (cbSize-- > 0) *(out++) = *(in++);
	}
	return output;
}

HANDLE GlobalCopy(HANDLE Old)
{
	size_t size = GlobalSize(Old);
	return copymem(GlobalAlloc(GPTR, size), Old, size);
}

TCHAR *GetResultStr(SystemProc *proc)
{
    TCHAR *buf = AllocString();
    if (proc->ProcResult == PR_OK) lstrcpy(buf, _T("ok"));
    else if (proc->ProcResult == PR_ERROR) lstrcpy(buf, _T("error"));
    else if (proc->ProcResult == PR_CALLBACK) wsprintf(buf, _T("callback%d"), proc->CallbackIndex);
    return buf;
}
__int64 myatoi64(TCHAR *s)
{
	__int64 v=0;
	// Check for right input
	if (!s) return 0;
	if (*s == _T('0') && (s[1] == _T('x') || s[1] == _T('X')))
	{
		s++;
		for (;;)
		{
			int c=*(++s);
			if (c >= _T('0') && c <= _T('9')) c-=_T('0');
			else if (c >= _T('a') && c <= _T('f')) c-=_T('a')-10;
			else if (c >= _T('A') && c <= _T('F')) c-=_T('A')-10;
			else break;
			v<<=4;
			v+=c;
		}
	}
	else if (*s == _T('0') && s[1] <= _T('7') && s[1] >= _T('0'))
	{
		for (;;)
		{
			int c=*(++s);
			if (c >= _T('0') && c <= _T('7')) c-=_T('0');
			else break;
			v<<=3;
			v+=c;
		}
	}
	else
	{
		int sign=0;
		if (*s == _T('-')) sign++; else s--;
		for (;;)
		{
			int c=*(++s) - _T('0');
			if (c < 0 || c > 9) break;
			v*=10;
			v+=c;
		}
		if (sign) v = -v;
	}

	// Support for simple ORed expressions
	if (*s == _T('|')) 
	{
		v |= myatoi64(s+1);
	}

	return v;
}

void myitoa64(__int64 i, TCHAR *buffer)
{
	TCHAR buf[128], *b = buf;

	if (i < 0)
	{
		*(buffer++) = _T('-');
		i = -i;
	}
	if (i == 0) *(buffer++) = _T('0');
	else 
	{
		while (i > 0) 
		{
			*(b++) = _T('0') + ((TCHAR) (i%10));
			i /= 10;
		}
		while (b > buf) *(buffer++) = *(--b);
	}
	*buffer = 0;
}

enum
{
	__INST_LAST = 20
};

#define isvalidnsisvarindex(varnum) ( ((unsigned int)(varnum)) < (__INST_LAST) )

TCHAR *system_getuservariable(int varnum)
{
	if (!isvalidnsisvarindex(varnum)) return AllocString();
	return AllocStr(g_variables+varnum*g_stringsize);
}

TCHAR *system_setuservariable(int varnum, TCHAR *var)
{
	if (var && isvalidnsisvarindex(varnum)) {
		lstrcpy(g_variables + varnum*g_stringsize, var);
	}
	return var;
}

#ifndef COUNTOF
#define COUNTOF(a) ( sizeof(a) / sizeof(a[0]) )
#endif

#define myatoi(str) ( (int) myatoi64(str) ) 
#define StrToIntPtr(str) ( (INT_PTR)myatoi((str)) )
#define IntPtrToStr(p,buf) myitoa64((p),(buf))
#define BUGBUG64(brokencast) (brokencast) // Cast that needs fixing on x64

/**
 * This function is useful for Unicode support.  Since the Windows
 * GetProcAddress function always takes a char*, this function wraps
 * the windows call and does the appropriate translation when
 * appropriate.
 *
 * @param dllHandle Handle to the DLL loaded by LoadLibraryEx.
 * @param funcName The name of the function to get the address of.
 * @return The pointer to the function.  Null if failure.
 */
void * NSISGetProcAddress(HMODULE dllHandle, TCHAR* funcName)
{
#ifdef _UNICODE
  char* ansiName = NULL;
  int   len;
  void* funcPtr;

  len = WideCharToMultiByte(CP_ACP, 0, funcName, -1, ansiName, 0, NULL, NULL);
  ansiName = (char*) GlobalAlloc(GPTR, len);
  WideCharToMultiByte(CP_ACP, 0, funcName, -1, ansiName, len, NULL, NULL);
  funcPtr = GetProcAddress(dllHandle, ansiName);
  GlobalFree(ansiName);
  return funcPtr;
#else
  return GetProcAddress(dllHandle, funcName);
#endif
}

#ifdef _WIN64
/*
TODO: CallProc/Back not implemeted.
Fake the behavior of the System plugin for the LoadImage API function so MUI works.
BUGBUG: MUI is leaking DeleteObject and failing GetClientRect
*/
SystemProc* CallProc(SystemProc *proc)
{
    INT_PTR ret, *place;
    LastError = lstrcmp(proc->ProcName, sizeof(TCHAR) > 1 ? _T("LoadImageW") : _T("LoadImageA"));
    if (!LastError)
    {
        ret = (INT_PTR) LoadImage((HINSTANCE)proc->Params[1].Value,
          (LPCTSTR)proc->Params[2].Value, (UINT)proc->Params[3].Value,
          (int)proc->Params[4].Value, (int)proc->Params[5].Value,
          (UINT)proc->Params[6].Value);
        LastError = GetLastError();
    }
    else
        proc->ProcResult = PR_ERROR, ret = 0;
    place = (INT_PTR*) proc->Params[0].Value;
    if (proc->Params[0].Option != -1) place = (INT_PTR*) &(proc->Params[0].Value);
    if (place) *place = ret;
    return proc;
}

SystemProc* CallBack(SystemProc *proc)
{
    proc->ProcResult = PR_ERROR;
    return proc;
}
#endif // ~_WIN64

void Call(TCHAR *variables)
{
    // Prepare input
    SystemProc *proc = PrepareProc(TRUE, variables);
    if (proc == NULL) return;

    if (proc->ProcResult != PR_CALLBACK) ParamAllocate(proc);
    ParamsIn(proc, variables);

    // Make the call
    if (proc->ProcResult != PR_ERROR)
    {
        switch (proc->ProcType)
        {
        case PT_NOTHING:
            if (proc->ProcResult == PR_CALLBACK) 
                proc = CallBack(proc);
            break;
        case PT_PROC:
        case PT_VTABLEPROC:
            proc = CallProc(proc); break;
        }
    }

    // Process output
    if ((proc->Options & POPT_ALWRETURN) != 0)
    {
        // Always return flag set - return separate return and result
        ParamsOut(proc);
        GlobalFree(GetResultStr(proc));
    }
    else
    {
        if (proc->ProcResult != PR_OK)
        {
            ProcParameter pp = proc->Params[0]; // Save old return param

            // Return result instead of return value
            proc->Params[0].Value = (INT_PTR) GetResultStr(proc);
            proc->Params[0].Type = PAT_TSTRING;

            ParamsOut(proc); // Return all params
            proc->Params[0] = pp; // Restore old return param
        }
        else
            ParamsOut(proc);
    }

    if (proc->ProcResult != PR_CALLBACK)
    {
        ParamsDeAllocate(proc);

        // if not callback - check for unload library option
        if ((proc->Options & POPT_UNLOAD) && proc->ProcType == PT_PROC && proc->Dll)
            FreeLibrary(proc->Dll); // and unload it :)

        // In case of POPT_ERROR - first pop will be proc error
        //if ((proc->Options & POPT_ERROR) != 0) system_pushint(LastError);
    }    

    // If proc is permanent?
    if ((proc->Options & POPT_PERMANENT) == 0)
		GlobalFree((HGLOBAL) proc); // No, free it
}

__int64 GetIntFromString(TCHAR **p)
{
    TCHAR buffer[128], *b = buffer;
    (*p)++; // First character should be skipped
    while (((**p >= _T('a')) && (**p <= _T('f'))) || ((**p >= _T('A')) && (**p <= _T('F'))) || ((**p >= _T('0')) && (**p <= _T('9'))) || (**p == _T('X')) || (**p == _T('-')) || (**p == _T('x')) || (**p == _T('|')))
		*(b++) = *((*p)++);
    *b = 0;
    (*p)--; // We should point at last digit
    return myatoi64(buffer);
}

SystemProc *PrepareProc(BOOL NeedForCall, TCHAR *p)
{
    int SectionType = PST_PROC, // First section is always proc spec
        ProcType = PT_NOTHING, // Default proc spec
        ChangesDone = 0,
        ParamIndex = 0,
        temp = 0, temp2, temp3;
    INT_PTR temp4;
    BOOL param_defined = FALSE;
    SystemProc *proc = NULL;
    TCHAR *ibuf, *ib, *sbuf, *cbuf, *cb;
    unsigned int UsedTString = 0;

    // Retrieve proc specs
    cb = (cbuf = AllocString()); // Current String buffer
    sbuf = AllocString(); // Safe String buffer
    ib = ibuf = p; // Input string

    // Parse the string
    while (SectionType != -1)
    {
        // Check for Section Change
        BOOL changed = TRUE;
        ChangesDone = SectionType;

        if (SectionType != PST_PROC && proc == NULL)
            // no proc after PST_PROC is done means something is wrong with
            // the call syntax and we'll get a crash because everything needs
            // proc from here on.
            break;

        switch (*ib)
        {
        case 0x0: SectionType = -1; break;
        case _T('#'): // "...#" redefines proc unless preceded by ":", then it's an ordinal (dll::#123)
          if (ib <= ibuf || *(ib-1) != _T(':') || PST_PROC != SectionType)
            SectionType = PST_PROC, ProcType = PT_NOTHING;
          else
            changed = FALSE;
          break;
        case _T('('): 
            SectionType = PST_PARAMS; 
            // fake-real parameter: for COM interfaces first param is Interface Pointer
            ParamIndex = ((ProcType == PT_VTABLEPROC)?(2):(1)); 
            temp3 = temp = 0;
            param_defined = FALSE;
            break;
        case _T(')'): SectionType = PST_RETURN; temp3 = temp = 0; break;
        case _T('?'): SectionType = PST_OPTIONS; temp = 1; break;
        default:
            changed = FALSE;
        }

        // Check for changes
        if (changed)
        {
            switch (ChangesDone)
            {
            case PST_PROC:
                *cb = 0;
                // Adopt proc
                if (proc == NULL)
                {
                    proc = (SystemProc *) GlobalAlloc(GPTR, sizeof(SystemProc));
                    proc->Options = 0;
                    proc->ParamCount = 0;
                }
                // Default parameters
                *proc->DllName = 0;
                *proc->ProcName = 0;
                proc->Dll = NULL;
                proc->Proc = NULL;
                proc->ProcType = ProcType;
                proc->ProcResult = PR_OK;
    
                // Section changed and previos section was Proc
                switch (ProcType)
                {
                case PT_NOTHING:
                    // Is it previous proc or just unknown proc?
                    if (cb != cbuf)
                    {
                        // Previous proc (for clear up)
                        SystemProc *pr = NULL;

                        if (proc != NULL) GlobalFree(proc);
                        // Get already defined proc                                      
                        proc = (SystemProc *) StrToIntPtr(cbuf);
                        if (!proc) break;

                        // Find the last clone at proc queue
                        while (proc && (proc->Clone != NULL)) proc = (pr = proc)->Clone;

                        // Clear parents record for child callback proc
                        if (pr != NULL) pr->Clone = NULL;

                        // Never Redefine?
                        if ((proc->Options & POPT_NEVERREDEF) != 0)
                        {
                            // Create new proc as copy
                            proc = GlobalCopy(proc);
                            // NeverRedef options is never inherited
                            proc->Options &= (~POPT_NEVERREDEF) & (~POPT_PERMANENT);
                        } else proc->Options |= POPT_PERMANENT; // Proc is old -> permanent
                    }
                    break;
                case PT_PROC:
                case PT_VTABLEPROC:
                    lstrcpy(proc->DllName, sbuf);
                case PT_STRUCT:
                    lstrcpy(proc->ProcName, cbuf);
                    break;
                }
                break;
            case PST_PARAMS:
                if (param_defined)
                    proc->ParamCount = ParamIndex;
                else
                    // not simply zero because of vtable calls
                    proc->ParamCount = ParamIndex - 1;
            case PST_RETURN:
            case PST_OPTIONS:
                break;
            }
            ib++;
            cb = cbuf;
            continue;
        }

        // Parse the section
        ChangesDone = PCD_NONE;
        switch (SectionType)
        {
        // Proc sections parser
        case PST_PROC:
            switch (*ib)
            {
            case _T(':'):
            case _T('-'):
                // Is it '::'
                if ((*(ib) == _T('-')) && (*(ib+1) == _T('>')))
                {
                    ProcType = PT_VTABLEPROC;    
                } else
                {
                    if ((*(ib+1) != _T(':')) || (*(ib) == _T('-'))) break;
                    ProcType = PT_PROC;
                }
                ib++; // Skip next ':'

                if (cb > cbuf)
                {
                    *cb = 0;
                    lstrcpy(sbuf, cbuf);
                } else  *sbuf = 0; // No dll - system proc
                
                // Ok
                ChangesDone = PCD_DONE;
                break;
            case _T('*'):
                // Structure defenition
                ProcType = PT_STRUCT;
                ChangesDone = PCD_DONE;
                break;
            }
            break;

        // Params and return sections parser
        case PST_RETURN:
            ParamIndex = 0; // Uses the same logic as PST_PARAMS section
        case PST_PARAMS:
            temp2 = -1; temp4 = 0; // Our type placeholder
            switch (*ib)
            {
            case _T(' '):
                break;
            case _T('_'): // No param cutting specifier
                if (proc->ParamCount > ParamIndex) ParamIndex = proc->ParamCount;
                temp3 = temp = 0; // Clear parameter options
                if (proc->ParamCount != ((ProcType == PT_VTABLEPROC) ? 1 : 0))
                {
                  // only define params if the last count wasn't zero
                  // this prevents errornous param count for:
                  //   'user32::CloseClipboard()(_)'
                  // for vtable calls, param count should not be one
                  param_defined = TRUE;
                }
                break;
            case _T(','): // Next param
                temp3 = temp = 0; // Clear parameter options
                ParamIndex++;
                param_defined = TRUE;
                break;
            case _T('&'):
                temp = 1; break; // Special parameter option
            case _T('*'):
                temp = -1; break; // Pointer parameter option

            // Types
            case _T('@'): temp2 = PAT_REGMEM; break;
            case _T('v'):
            case _T('V'): temp2 = PAT_VOID; break;

#ifndef _WIN64
            case _T('p'):
#endif
            case _T('i'):
            case _T('I'): temp2 = PAT_INT; break;
#ifdef _WIN64
            case _T('p'):
#endif
            case _T('l'):
            case _T('L'): temp2 = PAT_LONG; break;
            case _T('m'):
            case _T('M'): temp2 = PAT_STRING; break;
            case _T('t'):
            case _T('T'):
                temp2 = PAT_TSTRING;
                ++UsedTString;
                break;
            case _T('g'):
            case _T('G'): temp2 = PAT_GUID; break;
            case _T('w'):
            case _T('W'): temp2 = PAT_WSTRING; break;
            case _T('k'):
            case _T('K'): temp2 = PAT_CALLBACK; break;

            // Input output specifiers
            case _T('.'): temp3++; break; // skip specifier

            case _T('R'):
                temp4 = ((INT_PTR) GetIntFromString(&ib))+1;
                if (temp4 < 11) temp4 += 10; 
                break;
            case _T('r'): temp4 = ((INT_PTR) GetIntFromString(&ib))+1; break; // Register

            case _T('-'):
            case _T('0'): case _T('1'): case _T('2'): case _T('3'): case _T('4'):
            case _T('5'): case _T('6'): case _T('7'): case _T('8'): case _T('9'):
                // Numeric inline
                if (temp3 == 0)
                {
                    ib--;
                    // It's stupid, I know, but I'm too lazy to do another thing
                    myitoa64(GetIntFromString(&(ib)),(TCHAR *)(temp4 = (INT_PTR) AllocString()));
                }
                break;

            case _T('\"'): case _T('\''): case _T('`'):
                // Character inline
                {
                    TCHAR start = *ib;
                    cb = cbuf;
                    // copy inline
                    while (!((*(++ib) == start) && (*(ib+1) != start)) && (*ib))
                    {
                        if ((*ib) == start) ++ib;
                        *(cb++) = *(ib);
                    }
                    // finish and save
                    *cb = 0; 
                    temp4 = (INT_PTR) AllocStr(cbuf);
                }
                break;

            case _T('s'):
            case _T('S'): temp4 = -1; break;    // Stack
            }

            // Param type changed?
            if (temp2 != -1)
            {
                const int psbt = ParamSizeByType[temp2];
                param_defined = TRUE;
                proc->Params[ParamIndex].Type = temp2;
                proc->Params[ParamIndex].Size = // Pointer sized or from type
                    (temp == -1)?(PARAMSIZEBYTYPE_PTR):((psbt>0)?(psbt):(1)); //BUGBUG64: Is it safe to fallback to 1 for CALLBACK?
                // Get the parameter real special option value
                if (temp == 1) temp = ((int) GetIntFromString(&ib)) + 1;
                proc->Params[ParamIndex].Option = temp;
                proc->Params[ParamIndex].Value = 0;
                proc->Params[ParamIndex].Input = IOT_NONE;
                proc->Params[ParamIndex].Output = IOT_NONE;
            }
          
            // Param source/dest changed?
            if (temp4 != 0)
            {
                param_defined = TRUE;
                if (temp3 == 0)
                {
                    // it may contain previous inline input
                    if (!((proc->Params[ParamIndex].Input > -1) && (proc->Params[ParamIndex].Input <= __INST_LAST)))
                        GlobalFree((HGLOBAL) proc->Params[ParamIndex].Input);
                    proc->Params[ParamIndex].Input = temp4;
                }
				else  if (temp3 == 1)
				{
                    proc->Params[ParamIndex].Output = (int) temp4; // Note: As long as we never assign a pointer to temp4 when parsing a destination the cast to int is OK.
				}
                // Next parameter is output or something else
                temp3++;
            }

            ChangesDone = PCD_DONE;
            break;

        // Options sections parser
        case PST_OPTIONS:
            temp2 = 0;
            switch (*ib)
            {
            case _T(' '):
                break;
            case _T('!'): temp = -temp; break;
            case _T('c'):
#ifndef _WIN64
                temp2 = POPT_CDECL;
#endif
                break;
            case _T('r'):
                temp2 = POPT_ALWRETURN;
                break;
            case _T('n'):
                temp2 = POPT_NEVERREDEF;
                break;
            case _T('s'):
                temp2 = POPT_GENSTACK;
                break;
            case _T('e'):
                temp2 = POPT_ERROR;
                break;
            case _T('u'):
                temp2 = POPT_UNLOAD;
                break;
            }

            // New Options
            if (temp2 != 0)
            {
                if (temp == 1) proc->Options |= temp2;
                else proc->Options &= ~temp2;
                // Back to default (turn on nothing) state
                temp = 1; temp2 = 0;
            }

            ChangesDone = PCD_DONE;
            break;
        }

        // Nothing done, just copy char to buffer
        if (ChangesDone == PCD_NONE) *(cb++) = *(ib);
        // Something done, buffer = ""
        else cb = cbuf;

        // Increase input pointer
        ib++;
    }

    GlobalFree(ibuf);
    GlobalFree(cbuf);
    GlobalFree(sbuf);

    // Ok, the final step: check proc for existance
    if (proc != NULL && proc->Proc == NULL)
    {
        switch (proc->ProcType)
        {
        case PT_NOTHING: break;
        case PT_VTABLEPROC:
            {
                // Use direct system proc address
                INT_PTR addr;

                proc->Dll = (HMODULE) StrToIntPtr(proc->DllName);
  
                if (proc->Dll == 0)
                {
                    proc->ProcResult = PR_ERROR;
                    break;
                }

                addr = (INT_PTR) proc->Dll;

                // fake-real parameter: for COM interfaces first param is Interface Pointer
                proc->Params[1].Output = IOT_NONE;
                proc->Params[1].Input = (INT_PTR) AllocStr(proc->DllName);
                proc->Params[1].Size = PARAMSIZEBYTYPE_PTR;
                proc->Params[1].Type = PAT_PTR;
                proc->Params[1].Option = 0;

                // addr - pointer to interface vtable
                addr = *((INT_PTR *)addr);
                // now addr contains the pointer to first item at VTABLE
                // add the index of proc
                addr = addr + (INT_PTR)(myatoi64(proc->ProcName)*sizeof(void*));
                proc->Proc = *((HANDLE*)addr);
            }
            break;
        case PT_PROC:
            if (*proc->DllName == 0)
            {
                // Use direct system proc address
                if ((proc->Proc = (HANDLE) StrToIntPtr(proc->ProcName)) == 0)
                    proc->ProcResult = PR_ERROR;
            }
			else
            {
                // Get DLL address
                if ((proc->Dll = GetModuleHandle(proc->DllName)) == NULL)
                    if ((proc->Dll = LoadLibrary(proc->DllName)) == NULL)
                    {
                        proc->ProcResult = PR_ERROR;
                        break;
                    }

                // Get proc address
                proc->Proc = NSISGetProcAddress(proc->Dll, proc->ProcName);
                if (!proc->Proc && *proc->ProcName == _T('#'))
                {
                  int ordinal = myatoi(proc->ProcName+1);
                  if (ordinal && IS_INTRESOURCE(ordinal))
                    proc->Proc = GetProcAddress(proc->Dll, MAKEINTRESOURCEA(ordinal));
                }
                if (UsedTString || !proc->Proc)
                {
                    FARPROC tproc;
                    TCHAR*ProcName = proc->ProcName; // This buffer has room for us to party on
                    unsigned int cch = lstrlen(ProcName);
#ifdef _UNICODE
                    STRSET2CH(ProcName+cch, _T('W'), _T('\0'));
#else
                    STRSET2CH(ProcName+cch, _T('A'), _T('\0'));
#endif
                    tproc = NSISGetProcAddress(proc->Dll, ProcName);
                    if (tproc)
                        proc->Proc = tproc;
                    else
                        if (!proc->Proc)
							proc->ProcResult = PR_ERROR;
                }
            }
            break;
        }
    }

    return proc;
}

void ParamAllocate(SystemProc *proc)
{
    int i;
    for (i = 0; i <= proc->ParamCount; i++)
        if (!proc->Params[i].Value && proc->Params[i].Option == -1)
            proc->Params[i].Value = (INT_PTR) GlobalAlloc(GPTR, 4*ParamSizeByType[proc->Params[i].Type]);
}

void ParamsIn(SystemProc *proc, TCHAR *variables)
{
    int i;
    HGLOBAL* place;
    TCHAR *realbuf;
#ifndef _UNICODE
    LPWSTR wstr;
#endif

    i = (proc->ParamCount > 0)?(1):(0);
    while (TRUE)
    {
        ProcParameter *par = &proc->Params[i];
        // Step 1: retrive value
        if ((par->Input == IOT_NONE) || (par->Input == IOT_INLINE))
		{
            realbuf = AllocStr(_T(""));
		}
		else if ((par->Input > 0) && (par->Input <= __INST_LAST))
		{
			realbuf = system_getuservariable((int)par->Input - 1);
		}
        else
        {
            // Inline input, will be freed as realbuf
            realbuf = (TCHAR*) par->Input;
            par->Input = IOT_INLINE;
        }

        // Retreive pointer to place
        if (par->Option == -1)
			place = (HGLOBAL*) par->Value;
        else
			place = (HGLOBAL*) &(par->Value);

        // by default no blocks are allocated
        par->allocatedBlock = NULL;

        // Step 2: place it
        switch (par->Type)
        {
        case PAT_VOID:
            par->Value = 0;
            break;
        case PAT_INT:
            *(int*)place = myatoi(realbuf);
            break;
        case PAT_LONG:
            *(__int64*)place = myatoi64(realbuf);
            break;
        case PAT_TSTRING:
            *place = par->allocatedBlock = AllocStr(realbuf);
            break;
#ifdef _UNICODE
        case PAT_STRING:
            *place = par->allocatedBlock = GlobalAlloc(GPTR, g_stringsize);
            WideCharToMultiByte(CP_ACP, 0, realbuf, g_stringsize, *(LPSTR*)place, g_stringsize, NULL, NULL);
            break;
        case PAT_GUID:
            *place = par->allocatedBlock = GlobalAlloc(GPTR, 16);
            CLSIDFromString(realbuf, *(LPCLSID*)place);
            break;
#else
        case PAT_WSTRING:
        case PAT_GUID:
            wstr = (LPWSTR) GlobalAlloc(GPTR, g_stringsize*sizeof(WCHAR));
            MultiByteToWideChar(CP_ACP, 0, realbuf, g_stringsize, wstr, g_stringsize);
            if (par->Type == PAT_GUID)
            {
                *place = par->allocatedBlock = GlobalAlloc(GPTR, 16);
                CLSIDFromString(wstr, *(LPCLSID*)place);
                GlobalFree((HGLOBAL) wstr);
            }
			else
			{
                *place = par->allocatedBlock = (HGLOBAL)wstr;
			}
            break;
#endif
        case PAT_CALLBACK:
            // Generate new or use old callback
            if (lstrlen(realbuf) > 0)
                par->Value = (INT_PTR) CreateCallback((SystemProc*) StrToIntPtr(realbuf));
            break;
        case PAT_REGMEM:
            {
              int straddr = myatoi(realbuf);
              par->Value = (INT_PTR) straddr;
              par->Value += sizeof(void*) > 4 ? sizeof(_T("-9223372036854775807")) : sizeof(_T("-2147483647"));
            }
            break;
        }
        GlobalFree(realbuf);

        if (i == 0)
			break;
        if (i == proc->ParamCount)
			i = 0;
		else
			i++;
    }
}

void ParamsDeAllocate(SystemProc *proc)
{
    int i;
    for (i = proc->ParamCount; i >= 0; i--)
        if (proc->Params[i].Value && proc->Params[i].Option == -1)
        {
            GlobalFree((HGLOBAL) (proc->Params[i].Value));
            proc->Params[i].Value = 0;
        }
}

void ParamsOut(SystemProc *proc)
{
    INT_PTR *place;
    LPWSTR wstr;
    int i;
    TCHAR *realbuf = AllocString();

    i = proc->ParamCount;
    do
    {
        // Retreive pointer to place
        if (proc->Params[i].Option == -1)
            place = (INT_PTR*) proc->Params[i].Value;
        else 
            place = (INT_PTR*) &(proc->Params[i].Value);

        // Step 1: retrive value
        switch (proc->Params[i].Type)
        {
        case PAT_VOID:
            *realbuf = _T('\0');
            break;
#ifndef _WIN64
        case PAT_REGMEM:
#endif
        case PAT_INT:
            wsprintf(realbuf, _T("%d"), (int)(*((INT_PTR*) place)));
            break;
#ifdef _WIN64
        case PAT_REGMEM:
#endif
        case PAT_LONG:
            myitoa64(*((__int64*) place), realbuf);
            break;
        case PAT_STRING:
#ifdef _UNICODE
            MultiByteToWideChar(CP_ACP, 0, *((char**) place), g_stringsize, realbuf, g_stringsize-1);
            realbuf[g_stringsize-1] = _T('\0'); // make sure we have a null terminator
#else
            lstrcpyn(realbuf,*((TCHAR**) place), g_stringsize); // note: lstrcpyn always include a null terminator (unlike strncpy)
#endif
            break;
        case PAT_GUID:
#ifdef _UNICODE
            StringFromGUID2(*((REFGUID*)place), realbuf, g_stringsize);
#else
            {
                WCHAR guidstrbuf[39];
                int guidcch = StringFromGUID2(*((REFGUID*)place), guidstrbuf, g_stringsize);
                WideCharToMultiByte(CP_ACP, 0, guidstrbuf, guidcch, realbuf, g_stringsize, NULL, NULL);
            }
#endif
            break;
        case PAT_WSTRING:
            wstr = *((LPWSTR*)place);
#ifdef _UNICODE
            lstrcpyn(realbuf, wstr, g_stringsize); // note: lstrcpyn always include a null terminator (unlike strncpy)
#else
            WideCharToMultiByte(CP_ACP, 0, wstr, g_stringsize, realbuf, g_stringsize-1, NULL, NULL);
            realbuf[g_stringsize-1] = _T('\0'); // make sure we have a null terminator
#endif
            break;
        case PAT_CALLBACK:
            wsprintf(realbuf, _T("%d"), BUGBUG64(proc->Params[i].Value));
            break;
        }

        // memory cleanup
        if ((proc->Params[i].allocatedBlock != NULL) && ((proc->ProcType != PT_STRUCT)
            || (proc->Params[i].Option > 0)))
            GlobalFree(proc->Params[i].allocatedBlock);

        // Step 2: place it
        if (proc->Params[i].Output == IOT_NONE)
		{
		}
        else if (proc->Params[i].Output > 0)
        {
            system_setuservariable(proc->Params[i].Output - 1, realbuf);


        }

        i--;
    } 
    while (i >= 0);

    GlobalFree(realbuf);
}

HANDLE CreateCallback(SystemProc *cbproc)
{
    char *mem;

    if (cbproc->Proc == NULL)
    {
        // Set callback index
        cbproc->CallbackIndex = ++(CallbackIndex);
        cbproc->Options |= POPT_PERMANENT;

        mem = (char *) (cbproc->Proc = VirtualAlloc(NULL, sizeof(CallbackThunk), MEM_COMMIT, PAGE_EXECUTE_READWRITE));

        ((CallbackThunk*)mem)->pNext=g_CallbackThunkListHead;
        g_CallbackThunkListHead=(CallbackThunk*)mem;

        *(mem++) = (char) 0xB8; // Mov eax, const
        *((int *)mem) = (int) cbproc;
        mem += sizeof(int);
        *(mem++) = (char) 0xe9; // Jmp relative
        *((int *)mem) = (int) RealCallBack;
        *((int *)mem) -= ((int) mem) + 4;
    }

    // Return proc address
    return cbproc->Proc;
}

/*
Returns size by which the stack should be expanded
*/
unsigned int GetNewStackSize(void)
{
    return NEW_STACK_SIZE;
}

/*
Returns non-zero value if GENSTACK option is set
*/
unsigned int GetGenStackOption(SystemProc *proc)
{
    return (proc->Options & POPT_GENSTACK);
}

/*
Returns non-zero value if CDECL option is set
*/
unsigned int GetCDeclOption(SystemProc *proc)
{
    return (proc->Options & POPT_CDECL);
}

/*
Returns non-zero value if Error option is set
*/
unsigned int GetErrorOption(SystemProc *proc)
{
    return (proc->Options & POPT_ERROR);
}

/*
Returns offset for element Proc of SystemProc structure
*/
UINT_PTR GetProcOffset(void)
{
    return (UINT_PTR)(&(((SystemProc *)0)->Proc));
}

/*
Returns offset for element Clone of SystemProc structure
*/
unsigned int GetCloneOffset(void)
{
    return (unsigned int)(&(((SystemProc *)0)->Clone));
}

/*
Returns offset for element ProcName of SystemProc structure
*/
unsigned int GetProcNameOffset(void)
{
    return (unsigned int)(&(((SystemProc *)0)->ProcName));
}

/*
Returns offset for element ArgsSize of SystemProc structure
*/
unsigned int GetArgsSizeOffset(void)
{
    return (unsigned int)(&(((SystemProc *)0)->ArgsSize));
}

/*
Returns number of parameters
*/
unsigned int GetParamCount(SystemProc *proc)
{
    return proc->ParamCount;
}

/*
Returns offset for element Params of SystemProc structure
*/
UINT_PTR GetParamsOffset(void)
{
    return (UINT_PTR)(&(((SystemProc *)0)->Params));
}

/*
Returns size of ProcParameter structure
*/
UINT_PTR GetSizeOfProcParam(void)
{
    return (sizeof(ProcParameter));
}

/*
Returns offset for element Size of ProcParameter structure
*/
unsigned int GetSizeOffsetParam(void)
{
    return (unsigned int)(&(((ProcParameter *)0)->Size));
}

/*
Returns offset for element Value of ProcParameter structure
*/
UINT_PTR GetValueOffsetParam(void)
{
    return (UINT_PTR)(&(((ProcParameter *)0)->Value));
}

#ifndef _WIN64
/*
Returns offset for element _value of ProcParameter structure
*/
unsigned int Get_valueOffsetParam(void)
{
    return (unsigned int)(&(((ProcParameter *)0)->_value));
}
#endif

/*
Sets "CLONE" option
*/
void SetCloneOption(SystemProc *proc)
{
    proc->Options |= POPT_CLONE;
}

/*
Sets Result of procedure call to be "OK"
*/
void SetProcResultOk(SystemProc *proc)
{
    proc->ProcResult = PR_OK;
}

/*
Sets Result of procedure call to be "CALLBACK"
*/
void SetProcResultCallback(SystemProc *proc)
{
    proc->ProcResult = PR_CALLBACK;
}


/*
Use of system _DllMainCRTStartup to avoid endless recursion for the debug
report macro _RPT0. 

The system _DllMainCRTStartup initializes the C runtime environment. 
In particular the value for _osplatform is initialized. In the function 
_get_winmajor called in the execution of the _RPT0 macro an assertion 
failure is raised if _osplatform is not set. The assertion is reported by 
the same means as used for the _RPT0 macro. This leads to an endless recursion.
*/

void main()
{
	// change the protection of return command
	VirtualProtect(&retexpr, sizeof(retexpr), PAGE_EXECUTE_READWRITE, (PDWORD)&LastStackPlace);

	// initialize some variables
	LastStackPlace = 0, LastStackReal = 0;
	LastError = 0;
	LastProc = NULL;
	CallbackIndex = 0, g_CallbackThunkListHead = NULL;
	retexpr[0] = (char) 0xC2;
	retexpr[2] = 0x00;

	Call(_T("shell32::PathGetShortPath(t r3 r3)"));
	g_variables;
	OutputDebugStringA("aaaa");
}
