@echo off
mkdir build-vs22
cmake -S . -B build-vs22 -G "Visual Studio 17 2022"
if %ERRORLEVEL% neq 0 (
    echo Error: CMake failed to generate Visual Studio project files.
    exit /b %ERRORLEVEL%
)