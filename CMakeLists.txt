cmake_minimum_required(VERSION 3.10)

project(WastelandAdventures C)

# Set the target executable name
set(TARGET_NAME main)

# Platform detection (Windows only)
if (WIN32)
    set(PLATFORM windows)
else()
    message(FATAL_ERROR "Unsupported platform")
endif()

# Include directories
include_directories(
    ${CMAKE_SOURCE_DIR}/vendor/raylib5/include
    ${CMAKE_SOURCE_DIR}/vendor/cjson/include
    ${CMAKE_SOURCE_DIR}/vendor/fmt/include
	${CMAKE_SOURCE_DIR}/vendor/flecs/include
)

# Library paths
link_directories(
    ${CMAKE_SOURCE_DIR}/vendor/raylib5/lib/${PLATFORM}
    ${CMAKE_SOURCE_DIR}/vendor/cjson/lib/${PLATFORM}
	${CMAKE_SOURCE_DIR}/vendor/flecs/lib
)

# Binary directory
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)

# Enable compile_commands.json generation
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Source files
file(GLOB SRC_C ${CMAKE_SOURCE_DIR}/src/*.c)

# Set CFLAGS and LDFLAGS for Windows
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -g -fno-asynchronous-unwind-tables -std=gnu99")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -static-libgcc -static-libstdc++")

# Enable colored diagnostics for GCC and Clang
if(CMAKE_C_COMPILER_ID MATCHES "GNU|Clang")
    add_compile_options(-fdiagnostics-color=always)
endif()

# Create the executable
add_executable(${TARGET_NAME} ${SRC_C})

# Link the libraries
target_link_libraries(${TARGET_NAME}
    raylib.a
    cjson.a
	flecs_static.a
    gdi32
    winmm
    Ws2_32
)

# Additional build types
set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS} -DDEBUG")
set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS} -DRELEASE")


# Installation configuration
install(TARGETS ${TARGET_NAME} DESTINATION bin)

# Custom command to copy the binary to ROOT/bin
add_custom_command(TARGET ${TARGET_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_SOURCE_DIR}/bin
    COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:${TARGET_NAME}> ${CMAKE_SOURCE_DIR}/bin
    COMMENT "Copying binary to the ROOT/bin directory"
)

# Custom target for "make run" functionality
add_custom_target(run
    COMMAND ${CMAKE_SOURCE_DIR}/bin/${TARGET_NAME}.exe
    DEPENDS ${TARGET_NAME}
    COMMENT "Running the executable"
)

# Custom target for release build
add_custom_target(release
    COMMAND ${CMAKE_COMMAND} -DCMAKE_BUILD_TYPE=Release ${CMAKE_SOURCE_DIR}
    COMMAND ${CMAKE_COMMAND} --build ${CMAKE_BINARY_DIR} --config Release
    COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_BINARY_DIR}/builds/build_${PLATFORM}
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_BINARY_DIR}/bin ${CMAKE_BINARY_DIR}/builds/build_${PLATFORM}
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/assets ${CMAKE_BINARY_DIR}/builds/build_${PLATFORM}/assets
    COMMAND git log -1 > ${CMAKE_BINARY_DIR}/builds/build_${PLATFORM}/git_commit_info.txt
    COMMENT "Creating release build"
)


