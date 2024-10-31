# It's a static file. So there...

# Compiler settings
MACOS_VERSION := 10.15  # Minimum macOS version that supports <filesystem>
CXX := clang++
CXXFLAGS := -std=c++17 -mmacosx-version-min=$(MACOS_VERSION)
OUTPUT_DIR := build
MINIZIP_DIR := minizip


# Create the output directory in a platform-independent way
all: $(OUTPUT_DIR)/denigma

# Target to build the main program
$(OUTPUT_DIR)/denigma: src/main.cpp $(MINIZIP_DIR)/build/libminizip.a
	@mkdir -p $(OUTPUT_DIR) || (mkdir $(OUTPUT_DIR) && echo "Created output directory")
	$(CXX) $(CXXFLAGS) -o $@ src/main.cpp $(MINIZIP_DIR)/build/libminizip.a -lbz2 -lz -lzstd -lcompression -liconv -llzma -lcrypto

# Ensure minizip is built using CMake
$(MINIZIP_DIR)/build/libminizip.a:
	@mkdir -p $(MINIZIP_DIR)/build || (mkdir $(MINIZIP_DIR)/build && echo "Created minizip build directory")
	cd $(MINIZIP_DIR) && cmake -B build -D MZ_BUILD_TESTS=ON -D CMAKE_OSX_DEPLOYMENT_TARGET=$(MACOS_VERSION)
	cd $(MINIZIP_DIR) && cmake --build build

# Clean up built files
clean:
	rm -rf $(OUTPUT_DIR) $(MINIZIP_DIR)/build
