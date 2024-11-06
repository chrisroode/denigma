# Minimum version of CMake required
cmake_minimum_required(VERSION 3.15)

# Project name and version
project(denigma VERSION 1.0)

# Specify C++ standard and minimum macOS version
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_OSX_DEPLOYMENT_TARGET 10.15)

# Define output directory for executables
set(OUTPUT_DIR "${CMAKE_BINARY_DIR}/build")
set(MINIZIP_DIR "minizip")

# Set output directory for compiled executable
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${OUTPUT_DIR})

# Include directories for your source files
include_directories(src)

# Find required libraries
find_library(BZ2 bz2 REQUIRED)
find_library(ZLIB z REQUIRED)
find_library(ZSTD zstd REQUIRED)
find_library(COMPRESSION compression REQUIRED)
find_library(ICONV iconv REQUIRED)
find_library(LZMA lzma REQUIRED)
find_library(CRYPTO crypto REQUIRED)

# Add minizip as an external project
add_subdirectory(${MINIZIP_DIR} ${CMAKE_BINARY_DIR}/minizip)

# Add executable target
add_executable(denigma src/main.cpp)

# Link libraries to the target
target_link_libraries(denigma PRIVATE
    ${BZ2}
    ${ZLIB}
    ${ZSTD}
    ${COMPRESSION}
    ${ICONV}
    ${LZMA}
    ${CRYPTO}
    minizip
)

# Add minizip build directory as an alias for minizip include
target_include_directories(denigma PRIVATE
    $<BUILD_INTERFACE:${CMAKE_BINARY_DIR}>
)
