@echo off
echo ScanTool.net Unified Build Script
echo ===================================
echo Setting up environment...

set MINGW_PATH=C:\MinGW
set PATH=%MINGW_PATH%\bin;%PATH%

echo.
echo Checking build requirements...
if not exist "%MINGW_PATH%\bin\mingw32-gcc.exe" (
    echo ✗ MinGW GCC not found at %MINGW_PATH%
    goto :error
)
echo ✓ MinGW GCC found

if not exist "%MINGW_PATH%\include\allegro.h" (
    echo ✗ Allegro library not found
    goto :error
)
echo ✓ Allegro library found

echo.
echo Building ScanTool.net with unified compilation...
mingw32-make -f makefile_unified

if %ERRORLEVEL% neq 0 (
    echo.
    echo ✗ Build failed!
    echo Check the error messages above.
    goto :error
)

echo.
echo ✓ Build successful!
echo Executable: ScanTool.exe
goto :end

:error
echo.
echo Build aborted due to errors.
pause
exit /b 1

:end
pause
