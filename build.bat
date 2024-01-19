:: Example compilers
:: cmake -G Ninja -DCMAKE_C_COMPILER=clang -DCMAKE_CXX_COMPILER=clang++ ..
:: cmake -G Ninja -DCMAKE_C_COMPILER=icc -DCMAKE_CXX_COMPILER=icpc ..

:: Example build types
:: cmake -G Ninja -DCMAKE_BUILD_TYPE=Debug ..
:: cmake -G Ninja -DCMAKE_BUILD_TYPE=Release ..
:: cmake -G Ninja -DCMAKE_BUILD_TYPE=RelWithDebInfo ..
:: cmake -G Ninja -DCMAKE_BUILD_TYPE=MinSizeRel ..

:: The default configuration
@echo off
cd /d "%~dp0build"

setlocal enabledelayedexpansion

set run=false

for %%a in (%*) do (
    IF "%%a"=="-clean" (
        echo performing a clean build...
        ninja clean
    )

    IF "%%a"=="-run" (
        set run=true
    )
)

cmake -G Ninja -DCMAKE_BUILD_TYPE=Debug -DCMAKE_C_COMPILER=gcc -DCMAKE_CXX_COMPILER=g++ ..

echo Starting build...
for /f "tokens=1-4 delims=:.," %%a in ("%time%") do (
    set /a "start=(((%%a*60)+1%%b %% 100)*60+1%%c %% 100)*100+1%%d %% 100"
)

ninja -d stats

for /f "tokens=1-4 delims=:.," %%a in ("%time%") do (
    set /a "end=(((%%a*60)+1%%b %% 100)*60+1%%c %% 100)*100+1%%d %% 100"
)

set /a elapsed=end-start

set /a mins=elapsed/(60*100), secs=(elapsed%%(60*100))/100, hundredths=(elapsed%%100)

echo [Build completed in : %mins% minutes, %secs%.%hundredths% seconds]

IF "%run%"=="true" (
    echo Running /output/Antares.exe
    cd output
    Antares.exe
)