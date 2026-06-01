@echo off
REM Build script for MinGW (Windows)
REM Requires g++ on PATH (MinGW)

set OUTPUT=EVPlatform.exe
set FLAGS=-std=c++11 -O2 -Wall

echo Building %OUTPUT%...

g++ %FLAGS% *.cpp -o %OUTPUT%
if errorlevel 1 (
    echo Build failed.
    pause
    exit /b 1
) else (
    echo Build succeeded.
    echo Running %OUTPUT%...
    %OUTPUT%
)
