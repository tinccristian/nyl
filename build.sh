#!/bin/bash

run=false
build_type=Debug
info=false

# Calculate the start time
start=$(date +%s%N)

# Parse command line arguments
for arg in "$@"; do
    case $arg in
        -clean)
            echo "Performing a clean build..."
            cd build/Debug
            ninja clean
            if [ $? -ne 0 ]; then
                    echo "Clean failed with error $?"
                    exit $?
            fi
            # Calculate and display the elapsed time
            end=$(date +%s%N)
            elapsed=$((end-start))
            s=$((elapsed/1000000000))
            mins=$((secs/60))
            h=$((mins/60))
            ms=$((elapsed%1000000/10000))
            echo "Clean completed in: ${h}h$((h%60))m${s}s${ms}ms"
            exit $? # Exit with exit status of ninja clean
            ;;
        -run)
            run=true
            ;;
        -r)
            build_type=Release
            ;;
        -info)
            info=true
            ;;
        *)
            ;;
    esac
done

# Change to the appropriate build directory based on the build type
cd "$(dirname "$0")" || exit
mkdir -p build/"$build_type"
cd build/"$build_type" || exit

# Configure CMake
echo "Configuring CMake..."
cmake -G Ninja -DCMAKE_BUILD_TYPE="$build_type" -DCMAKE_C_COMPILER=gcc -DCMAKE_CXX_COMPILER=g++ ../..

# Start the build
echo "Starting $build_type build..."
echo "Build started at: $(date +%T)"

if [ "$info" = true ]; then
    ninja -j 6 -d stats > build_info.txt
    if [ $? -ne 0 ]; then
        echo "$build_type build failed with error $?"
        exit $?
    fi
else
    ninja -j 6 -d stats
    if [ $? -ne 0 ]; then
        echo "$build_type build failed with error $?"
        exit $?
    fi
fi

# Calculate and display the elapsed time
end=$(date +%s%N)
elapsed=$((end-start))
s=$((elapsed/1000000000))
mins=$((secs/60))
h=$((mins/60))
ms=$((elapsed%1000000/10000))
echo "$build_type build completed in: ${h}h$((h%60))m${s}s${ms}ms"

# Run the executable if the -run argument was provided
if [ "$run" = true ]; then
    echo "Running ../output/platformer.exe"
    cd ../ || exit
    cd output || exit
    ./platformer.exe
fi
