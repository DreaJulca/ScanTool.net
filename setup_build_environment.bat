@echo off
echo ScanTool.net Enhanced Build Setup
echo ==================================

echo.
echo Step 1: Setting up MinGW...
if not exist "C:\MinGW" (
    echo Creating MinGW directory...
    mkdir "C:\MinGW"
)

echo.
echo Step 2: Extract Allegro library...
if exist "build_tools\allegro-mingw-4.2.3.zip" (
    echo Extracting Allegro to C:\MinGW...
    powershell -Command "Expand-Archive -Path 'build_tools\allegro-mingw-4.2.3.zip' -DestinationPath 'C:\MinGW' -Force"
    echo Allegro library extracted.
) else (
    echo ERROR: allegro-mingw-4.2.3.zip not found in build_tools!
    pause
    exit /b 1
)

echo.
echo Step 3: Extract DirectX libraries...
if exist "build_tools\dx70_mgw.zip" (
    echo Extracting DirectX libraries to C:\MinGW...
    powershell -Command "Expand-Archive -Path 'build_tools\dx70_mgw.zip' -DestinationPath 'C:\MinGW' -Force"
    echo DirectX libraries extracted.
) else (
    echo WARNING: dx70_mgw.zip not found - some features may not work
)

echo.
echo Step 4: Setting up environment...
set PATH=%PATH%;C:\MinGW\bin
set MINGDIR=C:\MinGW

echo.
echo Step 5: Verify setup...
if exist "C:\MinGW\lib\liballeg.a" (
    echo ✓ Allegro library found
) else (
    echo ✗ Allegro library NOT found
)

if exist "C:\MinGW\bin\gcc.exe" (
    echo ✓ MinGW GCC found
) else (
    echo ✗ MinGW GCC NOT found - you may need to install MinGW separately
)

echo.
echo Setup complete! You can now run build_enhanced.bat to compile.
echo.
pause