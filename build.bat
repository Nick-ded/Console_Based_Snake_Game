@echo off
echo ============================================
echo   Snake Ultra Edition - Build Script
echo ============================================

REM Try to find g++ in common MinGW locations
set GXX=
for %%p in (
    "C:\msys64\mingw64\bin\g++.exe"
    "C:\msys64\ucrt64\bin\g++.exe"
    "C:\MinGW\bin\g++.exe"
    "C:\MinGW64\bin\g++.exe"
    "C:\TDM-GCC-64\bin\g++.exe"
    "C:\Program Files\mingw-w64\x86_64-8.1.0-posix-seh-rt_v6-rev0\mingw64\bin\g++.exe"
) do (
    if exist %%p (
        set GXX=%%p
        goto :found
    )
)

REM Check if g++ is in PATH
where g++ >nul 2>&1
if %errorlevel%==0 (
    set GXX=g++
    goto :found
)

echo ERROR: g++ not found!
echo.
echo Please install MinGW-w64:
echo   Option 1: MSYS2  - https://www.msys2.org/
echo             Then run: pacman -S mingw-w64-x86_64-gcc
echo   Option 2: WinLibs - https://winlibs.com/
echo             Download and extract, add bin\ to PATH
echo.
pause
exit /b 1

:found
echo Using compiler: %GXX%
echo.

set SRC=src\main.cpp src\game.cpp src\console.cpp src\sound.cpp src\particles.cpp src\maze.cpp src\leaderboard.cpp src\ui.cpp
set FLAGS=-std=c++17 -O2 -Wall -Wno-unused-variable
set OUT=snake_ultra.exe

echo Compiling...
%GXX% %FLAGS% -o %OUT% %SRC%

if %errorlevel%==0 (
    echo.
    echo ============================================
    echo   BUILD SUCCESS: %OUT%
    echo ============================================
    echo.
    echo Run with:  snake_ultra.exe
    echo.
) else (
    echo.
    echo BUILD FAILED. See errors above.
    echo.
)
pause
