@echo off
set "ROOT=%~dp0"
echo [VS Code Build] Starting GUI Build...
call "%ROOT%DS PROJ\build_gui.bat"

if %errorlevel% neq 0 (
    echo [VS Code Build] ERROR: Build failed.
    exit /b 1
)

echo [VS Code Build] SUCCESS!
exit /b 0
