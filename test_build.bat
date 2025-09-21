@echo off
echo ScanTool.net Test Build Script
echo =================================
echo Testing core compilation without problematic files...

REM Set MinGW environment variables
SET PATH=%PATH%;C:\MINGW\BIN
SET MINGDIR=C:\MINGW

echo.
echo Checking build requirements...
where gcc >nul 2>&1
if %errorlevel% neq 0 (
    echo ❌ MinGW GCC not found in PATH
    echo Please install MinGW and add it to your PATH
    pause
    exit /b 1
)
echo ✅ MinGW GCC found

REM Check for Allegro headers
if not exist "%MINGDIR%\include\allegro.h" (
    echo ❌ Allegro headers not found in %MINGDIR%\include\
    pause
    exit /b 1
)
echo ✅ Allegro headers found

echo.
echo Cleaning previous build...
if exist *.o del *.o
if exist scantool.exe del scantool.exe

echo.
echo Building core modules...
echo.

REM Define common compiler flags
SET CFLAGS=-c -O2 -Wall -I. -I"%MINGDIR%\include"

echo Compiling allegro_common.c...
gcc %CFLAGS% allegro_common.c -o allegro_common.o
if %errorlevel% neq 0 goto :build_failed

echo Compiling main.c...
gcc %CFLAGS% main.c -o main.o
if %errorlevel% neq 0 goto :build_failed

echo Compiling main_menu.c...
gcc %CFLAGS% main_menu.c -o main_menu.o
if %errorlevel% neq 0 goto :build_failed

echo Compiling trouble_code_reader.c...
gcc %CFLAGS% trouble_code_reader.c -o trouble_code_reader.o
if %errorlevel% neq 0 goto :build_failed

echo Compiling sensors.c...
gcc %CFLAGS% sensors.c -o sensors.o
if %errorlevel% neq 0 goto :build_failed

echo Compiling options.c...
gcc %CFLAGS% options.c -o options.o
if %errorlevel% neq 0 goto :build_failed

echo Compiling about.c...
gcc %CFLAGS% about.c -o about.o
if %errorlevel% neq 0 goto :build_failed

echo Compiling error_handlers.c...
gcc %CFLAGS% error_handlers.c -o error_handlers.o
if %errorlevel% neq 0 goto :build_failed

echo Compiling get_port_names.c...
gcc %CFLAGS% get_port_names.c -o get_port_names.o
if %errorlevel% neq 0 goto :build_failed

echo Compiling listports.c...
gcc %CFLAGS% listports.c -o listports.o
if %errorlevel% neq 0 goto :build_failed

echo Compiling reset.c...
gcc %CFLAGS% reset.c -o reset.o
if %errorlevel% neq 0 goto :build_failed

echo Compiling custom_gui.c...
gcc %CFLAGS% custom_gui.c -o custom_gui.o
if %errorlevel% neq 0 goto :build_failed

REM Skip serial.c for now as it has Windows API conflicts
echo Skipping serial.c (Windows API conflicts)...

rem Compile enhanced modules if they exist
if exist ecu_programming.c (
    echo Compiling ECU programming module...
    gcc %CFLAGS% ecu_programming.c -o ecu_programming.o
    if %errorlevel% neq 0 goto :build_failed
)

if exist advanced_diagnostics.c (
    echo Compiling advanced diagnostics module...
    gcc %CFLAGS% advanced_diagnostics.c -o advanced_diagnostics.o
    if %errorlevel% neq 0 goto :build_failed
)

if exist oil_cooler_sensors.c (
    echo Compiling oil cooler sensors module...
    gcc %CFLAGS% oil_cooler_sensors.c -o oil_cooler_sensors.o
    if %errorlevel% neq 0 goto :build_failed
)

if exist enhanced_communication.c (
    echo Compiling enhanced communication module...
    gcc %CFLAGS% enhanced_communication.c -o enhanced_communication.o
    if %errorlevel% neq 0 goto :build_failed
)

if exist realtime_charts.c (
    echo Compiling realtime charts module...
    gcc %CFLAGS% realtime_charts.c -o realtime_charts.o
    if %errorlevel% neq 0 goto :build_failed
)

echo.
echo ✅ Core compilation successful!
echo ⚠️  Note: serial.c was skipped due to Windows API conflicts
echo 📁 Object files created: %~dp0*.o
echo.

dir *.o

echo.
echo 🔧 Next step: Fix serial.c Windows API conflicts or create alternative implementation
echo.
pause
exit /b 0

:build_failed
echo.
echo ❌ Build failed!
echo Check the error messages above.
echo.
pause
exit /b 1