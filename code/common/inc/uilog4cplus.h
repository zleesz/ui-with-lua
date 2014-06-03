#pragma once
#include <Shlwapi.h>
#include <tchar.h>
#include <string>

//////////////////////// log4cplus ////////////////////////////////
#include <log4cplus/logger.h>
#include <log4cplus/configurator.h>

#define LOG_INIT()                              log4cplus::PropertyConfigurator::doConfigure(LOG4CPLUS_TEXT("log4cplus.cfg"))
#define LOG_INIT_EX(cfgfile)                    log4cplus::PropertyConfigurator::doConfigure(cfgfile)

// for log in global
#define LOGGER_DEC(logger)                      extern log4cplus::Logger logger
#define LOGGER_IMP_EX(logger, loggername)       log4cplus::Logger logger = log4cplus::Logger::getInstance( loggername )
#define LOGGER_DEFAULT()						log4cplus::Logger::getInstance( _T("") )
#define LOGGER_METHOD(logger)                   LOG4CPLUS_TRACE_METHOD(logger, LOG4CPLUS_C_STR_TO_TSTRING(__FUNCSIG__))
#define LOGGER_TRACE(logger,msg)                LOG4CPLUS_TRACE(logger, '[' << ::GetCurrentProcessId() << "][" << ::GetCurrentThreadId() << "][" << LOG4CPLUS_C_STR_TO_TSTRING(__FUNCTION__) << "] " << msg )
#define LOGGER_DEBUG(logger,msg)                LOG4CPLUS_DEBUG(logger, '[' << ::GetCurrentProcessId() << "][" << ::GetCurrentThreadId() << "][" << LOG4CPLUS_C_STR_TO_TSTRING(__FUNCTION__) << "] " << msg )
#define LOGGER_INFO(logger,msg)                 LOG4CPLUS_INFO(logger,  '[' << ::GetCurrentProcessId() << "][" << ::GetCurrentThreadId() << "][" << LOG4CPLUS_C_STR_TO_TSTRING(__FUNCTION__) << "] " << msg )
#define LOGGER_WARN(logger,msg)                 LOG4CPLUS_WARN(logger,  '[' << ::GetCurrentProcessId() << "][" << ::GetCurrentThreadId() << "][" << LOG4CPLUS_C_STR_TO_TSTRING(__FUNCTION__) << "] " << msg )
#define LOGGER_ERROR(logger,msg)                LOG4CPLUS_ERROR(logger, '[' << ::GetCurrentProcessId() << "][" << ::GetCurrentThreadId() << "][" << LOG4CPLUS_C_STR_TO_TSTRING(__FUNCTION__) << "] " << msg )
#define LOGGER_FATAL(logger,msg)                LOG4CPLUS_FATAL(logger, '[' << ::GetCurrentProcessId() << "][" << ::GetCurrentThreadId() << "][" << LOG4CPLUS_C_STR_TO_TSTRING(__FUNCTION__) << "] " << msg )

// for log in class
#define LOG_CLS_DEC_EX(loggername) \
	static inline HINSTANCE GetCurrentModuleHandle(void) \
	{ \
	static HINSTANCE hCurrentModule = NULL; \
	if(NULL == hCurrentModule) \
		{ \
		MEMORY_BASIC_INFORMATION m = { 0 }; \
		VirtualQuery(&hCurrentModule, &m, sizeof(MEMORY_BASIC_INFORMATION)); \
		hCurrentModule = (HINSTANCE) m.AllocationBase; \
		} \
		return hCurrentModule; \
	} \
	static log4cplus::Logger & getLogger() \
	{ \
	static log4cplus::Logger s_logger = log4cplus::Logger::getInstance( loggername ); \
	return s_logger; \
	}
#define LOG_CLS_DEC() \
	static TCHAR* getModuleName() \
	{ \
	static TCHAR s_szModuleFileName[MAX_PATH] = {0}; \
	if(_T('\0') == s_szModuleFileName[0]) \
		{ \
		HINSTANCE hInst = GetCurrentModuleHandle(); \
		::GetModuleFileName(hInst, s_szModuleFileName, MAX_PATH); \
		TCHAR* tszName1 = ::PathFindFileName(s_szModuleFileName); \
		_tcsncpy_s(s_szModuleFileName, tszName1, MAX_PATH); \
		::PathRemoveExtension(s_szModuleFileName); \
		} \
		return s_szModuleFileName; \
	} \
	LOG_CLS_DEC_EX( getModuleName() ) \
	LOG_CLS_AUTO_DEC()

#define LOG_METHOD() \
	__if_exists(this) { LOGGER_METHOD(getLogger()); } \
	__if_not_exists(this) { LOGGER_METHOD(LOGGER_DEFAULT()); }
#define LOG_TRACE(msg) \
	__if_exists(this) { LOGGER_TRACE(getLogger(), msg); } \
	__if_not_exists(this) { LOGGER_TRACE(LOGGER_DEFAULT(), msg); }
#define LOG_DEBUG(msg) \
	__if_exists(this) { LOGGER_DEBUG(getLogger(), msg); } \
	__if_not_exists(this) { LOGGER_DEBUG(LOGGER_DEFAULT(), msg); }
#define LOG_INFO(msg) \
	__if_exists(this) { LOGGER_INFO(getLogger(), msg); } \
	__if_not_exists(this) { LOGGER_INFO(LOGGER_DEFAULT(), msg); }
#define LOG_WARN(msg) \
	__if_exists(this) { LOGGER_WARN(getLogger(), msg); } \
	__if_not_exists(this) { LOGGER_WARN(LOGGER_DEFAULT(), msg); }
#define LOG_ERROR(msg) \
	__if_exists(this) { LOGGER_ERROR(getLogger(), msg); } \
	__if_not_exists(this) { LOGGER_ERROR(LOGGER_DEFAULT(), msg); }
#define LOG_FATAL(msg) \
	__if_exists(this) { LOGGER_FATAL(getLogger(), msg); } \
	__if_not_exists(this) { LOGGER_FATAL(LOGGER_DEFAULT(), msg); }

#define LOG_AUTO_REAL(file, line) \
	CLogAuto logAuto( LOG4CPLUS_C_STR_TO_TSTRING(file), line, LOG4CPLUS_C_STR_TO_TSTRING(__FUNCSIG__) );
#define LOG_AUTO() \
	LOG_AUTO_REAL(__FILE__, __LINE__)
#define LOG_CLS_AUTO_DEC() \
class CLogAuto \
{ \
public: \
	CLogAuto(log4cplus::tstring file, LONG line, log4cplus::tstring tszFuncSig) : \
		m_pszFuncSig(tszFuncSig), \
		m_pszFile(file) \
	{ \
	char szLine[20] = {0}; \
	sprintf_s(szLine, "%d", line); \
	m_pszLine = LOG4CPLUS_C_STR_TO_TSTRING(szLine); \
	LOG4CPLUS_TRACE(getLogger(), '[' << ::GetCurrentProcessId() << "][" << ::GetCurrentThreadId() << "][" << "¡ú" << m_pszFuncSig << ']' << " [" << m_pszFile << ":" << m_pszLine << "]"); \
	} \
	~CLogAuto() \
	{ \
	LOG4CPLUS_TRACE(getLogger(), '[' << ::GetCurrentProcessId() << "][" << ::GetCurrentThreadId() << "][" << "¡û" << m_pszFuncSig << ']' << " [" << m_pszFile << ":" << m_pszLine << "]"); \
	} \
private: \
	log4cplus::tstring m_pszFuncSig; \
	log4cplus::tstring m_pszFile; \
	log4cplus::tstring m_pszLine; \
}
//////////////////////////////////////////////////////////////////////////
