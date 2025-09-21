@echo off
echo ScanTool.net Enhanced Build Script
echo ==================================

echo Setting up environment...
set PATH=%PATH%;C:\MinGW\bin
set MINGDIR=C:\MinGW

echo.
echo Checking build requirements...

REM Check if MinGW is available
if not exist "C:\MinGW\bin\gcc.exe" (
    echo ERROR: MinGW GCC not found!
    echo Please run setup_build_environment.bat first
    pause
    exit /b 1
)

REM Check if Allegro is available  
if not exist "C:\MinGW\lib\liballeg.a" (
    echo ERROR: Allegro library not found!
    echo Please run setup_build_environment.bat first
    pause
    exit /b 1
)

echo ✓ MinGW GCC found
echo ✓ Allegro library found

echo.
echo Creating build directory...
if not exist "compiled" mkdir compiled

echo.
echo Building ScanTool.net Enhanced Edition...
echo.

REM Build with enhanced features
mingw32-make NOWERROR=1 MINGDIR=C:\MinGW

if %ERRORLEVEL% == 0 (
    echo.
    echo ✓ Build successful!
    echo.
    echo Copying runtime files...
    
    REM Copy data files
    if exist "scantool.dat" copy "scantool.dat" "compiled\"
    if exist "codes.dat" copy "codes.dat" "compiled\"
    
    REM Copy Allegro DLL
    if exist "%MINGDIR%\bin\alleg42.dll" copy "%MINGDIR%\bin\alleg42.dll" "compiled\"
    
    echo.
    echo Enhanced ScanTool.net is ready!
    echo Location: compiled\ScanTool.exe
    echo.
    echo New features include:
    echo - ECU Programming and Calibration
    echo - Real-time Data Charts
    echo - Oil Cooler Temperature/Pressure Monitoring
    echo - Advanced Diagnostic Procedures
    echo - Maintenance Service Resets
    echo.
) else (
    echo.
    echo ✗ Build failed!
    echo Check the error messages above.
    echo.
)

pause