# Minimum version of CMake required
cmake_minimum_required(VERSION 3.15)

# Project name and version
project(denigma VERSION 1.0)

# Specify C++ standard and minimum macOS version
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_OSX_DEPLOYMENT_TARGET 10.15)

set(BUILD_SHARED_LIBS OFF)

# Define output directory for executables
set(OUTPUT_DIR "${CMAKE_BINARY_DIR}/build")
set(MINIZIP_DIR "minizip")

# Set output directory for compiled executable
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${OUTPUT_DIR})

# Include directories for your source files
include_directories(src)

# Find required libraries
find_package(BZip2 REQUIRED)
find_package(ZLIB REQUIRED)
find_package(ZSTD REQUIRED)
find_package(Iconv REQUIRED)
find_library(LZMA lzma REQUIRED)
find_library(CRYPTO crypto REQUIRED)


# Add minizip as an external project
add_subdirectory(${MINIZIP_DIR} ${CMAKE_BINARY_DIR}/minizip)

# Add executable target
add_executable(denigma src/main.cpp)

# Link libraries to the target
target_link_libraries(denigma PRIVATE
    ${BZIP2_LIBRARES}
    ${ZLIB_LIBRARIES}
    ${ZSTD_LIBRARIES}
    ${ICONV_LIBRARIES}
    ${LZMA}
    ${CRYPTO}
    minizip
)

# Ensure the include directory is added
target_include_directories(denigma PRIVATE ${ZLIB_INCLUDE_DIRS})

# Add minizip build directory as an alias for minizip include
target_include_directories(denigma PRIVATE
    $<BUILD_INTERFACE:${CMAKE_BINARY_DIR}>
)
