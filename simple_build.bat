@echo off
echo ScanTool.net Simple Build Script
echo ====================================
echo Setting up environment...

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
echo Building ScanTool.net Enhanced Edition...
echo.
echo Compiling source files...

REM Define common compiler flags
SET CFLAGS=-c -O2 -Wall -I. -I"%MINGDIR%\include"

gcc %CFLAGS% allegro_common.c -o allegro_common.o
if %errorlevel% neq 0 goto :build_failed

gcc %CFLAGS% main.c -o main.o
if %errorlevel% neq 0 goto :build_failed

gcc %CFLAGS% main_menu.c -o main_menu.o
if %errorlevel% neq 0 goto :build_failed

gcc %CFLAGS% serial.c -o serial.o
if %errorlevel% neq 0 goto :build_failed

gcc %CFLAGS% trouble_code_reader.c -o trouble_code_reader.o
if %errorlevel% neq 0 goto :build_failed

gcc %CFLAGS% sensors.c -o sensors.o
if %errorlevel% neq 0 goto :build_failed

gcc %CFLAGS% options.c -o options.o
if %errorlevel% neq 0 goto :build_failed

gcc %CFLAGS% about.c -o about.o
if %errorlevel% neq 0 goto :build_failed

gcc %CFLAGS% error_handlers.c -o error_handlers.o
if %errorlevel% neq 0 goto :build_failed

gcc %CFLAGS% get_port_names.c -o get_port_names.o
if %errorlevel% neq 0 goto :build_failed

gcc %CFLAGS% listports.c -o listports.o
if %errorlevel% neq 0 goto :build_failed

gcc %CFLAGS% reset.c -o reset.o
if %errorlevel% neq 0 goto :build_failed

gcc %CFLAGS% custom_gui.c -o custom_gui.o
if %errorlevel% neq 0 goto :build_failed

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
echo Linking executable...
gcc -o scantool.exe *.o -L"%MINGDIR%\lib" -lalleg -luser32 -lgdi32 -lcomdlg32 -lole32 -ldinput -lddraw -ldxguid -lwinmm -ldsound
if %errorlevel% neq 0 goto :build_failed

echo.
echo Copying required files...
if exist scantool.dat copy scantool.dat .\
if exist "%MINGDIR%\bin\alleg42.dll" copy "%MINGDIR%\bin\alleg42.dll" .\

echo.
echo ✅ Build completed successfully!
echo ✅ Executable: scantool.exe
if exist alleg42.dll echo ✅ Allegro DLL copied
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
