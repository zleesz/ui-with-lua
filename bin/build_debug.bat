
echo  ���� UI.sln
call "%VS90COMNTOOLS%..\IDE\devenv.exe" "%~0\..\..\code\src\UI.sln" /build debug  /out "%~0\..\ui.log"
type %~0\..\ui.log
echo off
set LOGFILE=%~0\..\ui.log
findstr /C:"ʧ�� 0 ��" "%LOGFILE%"
if %ERRORLEVEL% NEQ 0 (
	pause & exit
)
del %~0\..\ui.log

exit
