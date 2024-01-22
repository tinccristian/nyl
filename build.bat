:: This script is used to build the project using Ninja and CMake.
:: It supports the following command line arguments:
:: -clean: Performs a clean build by running 'ninja clean'.
:: -run: Runs the executable after building.
:: -r: Sets the build type to Release. If not set, the build type defaults to Debug.

@echo off
cd /d "%~dp0build"

setlocal enabledelayedexpansion

set run=false
set build_type=Debug

:: Parse command line arguments
for %%a in (%*) do (
    IF "%%a"=="-clean" (
        echo Performing a clean build...
        ninja clean
    )

    IF "%%a"=="-run" (
        set run=true
    )

    IF "%%a"=="-r" (
        set build_type=Release
    )
)

:: Configure CMake
echo Configuring CMake...
cmake -G Ninja -DCMAKE_BUILD_TYPE=%build_type% -DCMAKE_C_COMPILER=gcc -DCMAKE_CXX_COMPILER=g++ ..

:: Start the build
echo Starting build...
for /f "tokens=1-4 delims=:.," %%a in ("%time%") do (
    set /a "start=(((%%a*60)+1%%b %% 100)*60+1%%c %% 100)*100+1%%d %% 100"
)

echo Build started at: %time%

ninja -j 0 -d stats

:: Calculate and display the elapsed time
for /f "tokens=1-4 delims=:.," %%a in ("%time%") do (
    set /a "end=(((%%a*60)+1%%b %% 100)*60+1%%c %% 100)*100+1%%d %% 100"
)

set /a elapsed=end-start
set /a mins=elapsed/(60*100), secs=(elapsed%%(60*100))/100, hundredths=(elapsed%%100)

echo [Build completed in : %mins% minutes, %secs%.%hundredths% seconds]

:: Run the executable if the -run argument was provided
IF "%run%"=="true" (
    echo Running /output/Antares.exe
    cd output
    Antares.exe
)