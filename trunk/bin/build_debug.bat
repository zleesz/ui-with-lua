
del ui.log 2> nul
echo  ���� UI.sln
call "%VS90COMNTOOLS%..\IDE\devenv.exe" "%~0\..\..\\code\src\UI.sln" /build debug  /out ui.log
type ui.log
echo off
set LOGFILE=%cd%\ui.log
findstr /C:"ʧ�� 0 ��" "%LOGFILE%"
if %ERRORLEVEL% NEQ 0 (
	pause & exit
)
del ui.log
