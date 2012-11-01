
del ui.log 2> nul
echo  ±àÒë UI.sln
call "%VS90COMNTOOLS%..\IDE\devenv.exe" "%~0\..\..\\code\src\UI.sln" /build debug  /out ui.log
type ui.log
echo off
set LOGFILE=%cd%\ui.log
findstr /C:"Ê§°Ü 0 ¸ö" "%LOGFILE%"
if %ERRORLEVEL% NEQ 0 (
	pause & exit
)
del ui.log
