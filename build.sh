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
            ninja clean
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
mins=$((elapsed/(60*100)))
secs=$((elapsed%(60*100)/100))
hundredths=$((elapsed%100))

echo "$build_type build completed in: $mins minutes, $secs.$hundredths seconds"

# Run the executable if the -run argument was provided
if [ "$run" = true ]; then
    echo "Running ../output/platformer.exe"
    cd ../ || exit
    cd output || exit
    ./platformer.exe
fi
