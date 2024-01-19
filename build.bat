:: Uncomment the following lines to use different compilers
:: cmake -G Ninja -DCMAKE_C_COMPILER=clang -DCMAKE_CXX_COMPILER=clang++ ..
:: cmake -G Ninja -DCMAKE_C_COMPILER=icc -DCMAKE_CXX_COMPILER=icpc ..

:: Uncomment the following lines to use different build types
:: cmake -G Ninja -DCMAKE_BUILD_TYPE=Debug ..
:: cmake -G Ninja -DCMAKE_BUILD_TYPE=Release ..
:: cmake -G Ninja -DCMAKE_BUILD_TYPE=RelWithDebInfo ..
:: cmake -G Ninja -DCMAKE_BUILD_TYPE=MinSizeRel ..

:: The default configuration
@echo off
cd /d "%~dp0build"
cmake -G Ninja -DCMAKE_BUILD_TYPE=Debug -DCMAKE_C_COMPILER=gcc -DCMAKE_CXX_COMPILER=g++ ..
ninja -d stats