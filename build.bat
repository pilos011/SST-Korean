@echo off
:: ============================================================
:: build.bat - Super Star Trek (Korean) Build Script
:: Requires: MinGW-w64 GCC (via MSYS2)
:: ============================================================

setlocal EnableDelayedExpansion

echo ============================================================
echo   Super Star Trek Korean - Build
echo ============================================================
echo.

:: ---- Find GCC ----
set GCC=
for %%P in (gcc.exe) do set GCC=%%~$PATH:P

if not defined GCC (
    if exist "C:\msys64\mingw64\bin\gcc.exe" set GCC=C:\msys64\mingw64\bin\gcc.exe
    if exist "C:\msys64\ucrt64\bin\gcc.exe"  set GCC=C:\msys64\ucrt64\bin\gcc.exe
    if exist "C:\mingw64\bin\gcc.exe"         set GCC=C:\mingw64\bin\gcc.exe
    if exist "C:\mingw\bin\gcc.exe"           set GCC=C:\mingw\bin\gcc.exe
    if exist "C:\Program Files\msys64\ucrt64\bin\gcc.exe"           set GCC="C:\Program Files\msys64\ucrt64\bin\gcc.exe"
)

if not defined GCC (
    echo [ERROR] GCC not found.
    echo.
    echo Please install MinGW-w64:
    echo   1. Download MSYS2 from https://www.msys2.org
    echo   2. In MSYS2 terminal run:
    echo      pacman -S mingw-w64-x86_64-gcc
    echo   3. Add C:\msys64\mingw64\bin to system PATH
    echo.
    pause
    exit /b 1
)

echo [INFO] GCC found: %GCC%
"%GCC%" --version 2>&1 | findstr /r "gcc"
echo.

:: ---- Source files ----
set SRCDIR=src
set SOURCES=%SRCDIR%\sst.c %SRCDIR%\setup.c %SRCDIR%\battle.c %SRCDIR%\events.c %SRCDIR%\finish.c %SRCDIR%\moving.c %SRCDIR%\planets.c %SRCDIR%\reports.c %SRCDIR%\ai.c %SRCDIR%\lang.c %SRCDIR%\win_compat.c

set OUTPUT=sst.exe
set CFLAGS=-O2 -Wno-unused-result -Wno-unused-variable -Wno-unused-function -Wno-multichar -Wno-array-bounds -finput-charset=UTF-8 -fexec-charset=UTF-8
set LDFLAGS=-lm

echo [BUILD] Compiling...
echo.

"%GCC%" %CFLAGS% %SOURCES% -o %OUTPUT% %LDFLAGS% -I%SRCDIR%

if errorlevel 1 (
    echo.
    echo [FAILED] Build failed. Check errors above.
    pause
    exit /b 1
)

echo.
echo [SUCCESS] Build complete: %OUTPUT%
echo.
echo ============================================================
echo   How to run: sst.exe
echo   Save game:  freeze  to save,  frozen  to load
echo ============================================================
echo.

if "%1"=="-run" (
    start "" "%OUTPUT%"
)

pause
