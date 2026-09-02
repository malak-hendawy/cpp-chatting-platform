@echo off
set "SCRIPT_DIR=%~dp0"
cd /d "%SCRIPT_DIR%"
echo Attempting to build GUI Application in %SCRIPT_DIR%...

REM Check if cl is already available
where cl >nul 2>nul
if %errorlevel% equ 0 (
    goto :Build
)

echo 'cl' not found in path. Attempting to initialize VS Dev Cmd...

REM Try to find VsDevCmd.bat in standard locations (Community 2022)
set "VSPATH=C:\Program Files\Microsoft Visual Studio\2022\Community\Common7\Tools\VsDevCmd.bat"

if exist "%VSPATH%" (
    echo Found VS 2022 Community. Initializing...
    call "%VSPATH%"
    goto :Build
)

echo.
echo ERROR: Could not find 'cl' or 'VsDevCmd.bat'.
echo Please run this from the "Developer Command Prompt for VS 2022".
pause
exit /b 1

:Build
echo Environment Ready. Compiling...
cl /EHsc /D_UNICODE /DUNICODE GuiMain.cpp ChatSystem.cpp User.cpp /Fe:GuiChat.exe

if %errorlevel% neq 0 (
    echo Build Failed!
    pause
    exit /b 1
)

echo.
echo Build Successful! Application starting...
start GuiChat.exe
