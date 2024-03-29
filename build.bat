@echo off
setlocal enabledelayedexpansion

set run=false
set build_type=Debug
set info=false

:: Calculate the start time
for /f "tokens=1-4 delims=:.," %%a in ("%time%") do (
    set /a "start=(((%%a*60)+1%%b %% 100)*60+1%%c %% 100)*100+1%%d %% 100"
)

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

    IF "%%a"=="-info" (
        set info=true
    )
)

:: Change to the appropriate build directory based on the build type
cd /d "%~dp0"
mkdir build\%build_type%
cd build\%build_type%

:: Configure CMake
echo Configuring CMake...
cmake -G Ninja -DCMAKE_BUILD_TYPE=%build_type% -DCMAKE_C_COMPILER=gcc -DCMAKE_CXX_COMPILER=g++ ../..

:: Start the build
echo Starting %build_type% build...
echo Build started at: %time%

IF "%info%"=="true" (
    ninja -j 6 -d stats > build_info.txt
    IF ERRORLEVEL 1 (
        echo %build_type% build failed with error %errorlevel%
        exit /b %errorlevel%
    )
) ELSE (
    ninja -j 6 -d stats
    IF ERRORLEVEL 1 (
        echo %build_type% build failed with error %errorlevel%
        exit /b %errorlevel%
    )
)

:: Calculate and display the elapsed time
for /f "tokens=1-4 delims=:.," %%a in ("%time%") do (
    set /a "end=(((%%a*60)+1%%b %% 100)*60+1%%c %% 100)*100+1%%d %% 100"
)

set /a elapsed=end-start
set /a mins=elapsed/(60*100), secs=(elapsed%%(60*100))/100, hundredths=(elapsed%%100)

echo %build_type% build completed in : %mins% minutes, %secs%.%hundredths% seconds

:: Run the executable if the -run argument was provided
IF "%run%"=="true" (
    echo Running ../output/platformer.exe
    cd ../
    cd output
    platformer.exe
)