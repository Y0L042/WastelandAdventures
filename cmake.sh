# #!/bin/bash
#
# export CC=gcc
# export CCX=g++
#
# # Create and configure the build directory
# cmake -B build -G Ninja 
#
# # Build the project
# cmake --build build
#
# cp ./build/compile_commands.json ./compile_commands.json
#
# echo "Running executable:"
# echo ""
# # Run the executable
# ./bin/main.exe
# # cmake --build build --target run



#!/bin/bash

export CC=gcc
export CCX=g++

# Function to build the project
build() {
	echo "Building..."
    # Create and configure the build directory
    cmake -B build -G Ninja 

    # Build the project
    cmake --build build

    # Copy the compile_commands.json file
	echo "Copying compile_commands.json..."
    cp ./build/compile_commands.json ./compile_commands.json
	echo "Building finished."
}

# Function to run the executable
run() {
	build
    echo "Running executable:"
    echo ""
    # Run the executable
    ./bin/main.exe
}

# Function to clean the build
clean() {
    echo "Cleaning build directory..."
    rm -rf build
    rm -f compile_commands.json
	echo "Cleaning complete."
}

# Main logic to handle different commands
case "$1" in
    buildonly)
        build
        ;;
    run)
        run
        ;;
    clean)
        clean
        ;;
    *)
        echo "Usage: $0 {buildonly|run|clean}"
        exit 1
        ;;
esac
