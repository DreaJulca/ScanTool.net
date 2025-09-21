@echo off
echo ScanTool.net Enhanced Build Script - Fixed Version
echo ======================================================

echo Setting up environment...
set PATH=%PATH%;C:\MinGW\bin
set C_INCLUDE_PATH=C:\MinGW\include
set LIBRARY_PATH=C:\MinGW\lib

echo.
echo Checking build requirements...
where gcc >nul 2>&1
if %errorlevel% neq 0 (
    echo ❌ MinGW GCC not found in PATH
    pause
    exit /b 1
) else (
    echo ✅ MinGW GCC found
)

if exist "C:\MinGW\include\allegro.h" (
    echo ✅ Allegro library found
) else (
    echo ❌ Allegro library not found
    pause  
    exit /b 1
)

echo.
echo Creating build directory...
if not exist "build" mkdir build

echo.
echo Building ScanTool.net Enhanced Edition...
echo.

REM Compile new support modules first
echo Compiling support modules...
gcc -c -O -Wall -IC:\MinGW\include allegro_wrapper.c -o allegro_wrapper.o
if %errorlevel% neq 0 goto :error

gcc -c -O -Wall -IC:\MinGW\include realtime_charts.c -o realtime_charts.o  
if %errorlevel% neq 0 goto :error

gcc -c -O -Wall -IC:\MinGW\include enhanced_communication.c -o enhanced_communication.o
if %errorlevel% neq 0 goto :error

echo Support modules compiled successfully.
echo.

REM Use the existing makefile for the rest
echo Running main build...
mingw32-make clean
mingw32-make

if %errorlevel% equ 0 (
    echo.
    echo ✅ Build completed successfully!
    echo ✅ ScanTool.exe has been created
    echo.
) else (
    goto :error
)

echo Build completed at %date% %time%
pause
exit /b 0

:error
echo.
echo ❌ Build failed!
echo Check the error messages above.
echo.
pause
exit /b 1
