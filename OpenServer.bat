@echo off
cd /d "%~dp0Binaries\Server"

REM Check if Locator.exe is already running
tasklist /FI "IMAGENAME eq Locator.exe" 2>NUL | find /I /N "Locator.exe">NUL
if "%ERRORLEVEL%"=="0" (
    echo Locator.exe is already running.
) else (
    start /MIN "" "%CD%\Locator.exe"
)

REM Check if Matchserver.exe is already running
tasklist /FI "IMAGENAME eq Matchserver.exe" 2>NUL | find /I /N "Matchserver.exe">NUL
if "%ERRORLEVEL%"=="0" (
    echo Matchserver.exe is already running.
) else (
    start /MIN "" "%CD%\Matchserver.exe"
)

REM Check if Matchagent.exe is already running
tasklist /FI "IMAGENAME eq Matchagent.exe" 2>NUL | find /I /N "Matchagent.exe">NUL
if "%ERRORLEVEL%"=="0" (
    echo Matchagent.exe is already running.
) else (
    start /MIN "" "%CD%\Matchagent.exe"
)

exit
