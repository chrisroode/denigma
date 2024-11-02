#include <filesystem>
#include <iostream>
#include <string>
#include <sys/stat.h>
#include "minizip/unzip.h"
#include <errno.h>
#include <zlib.h>


#ifdef _WIN32
	#include <direct.h> // For _mkdir on Windows
#else
	#include <unistd.h>
	#include <dirent.h>
#endif


//Shout out to Deguerre https://github.com/Deguerre
struct ScoreFileCrypter {
	const uint32_t initialState = 0x28006D45;

	void CryptBuffer(uint8_t* buffer, std::size_t length)
	{
		uint32_t state = initialState;
		int i = 0;
		while (length-- > 0) {
			if ( i % 0x20000 == 0 ) {
				state = initialState;
			}
			state = state * 0x41c64e6d + 0x3039; // BSD rand()!
			uint16_t upper = state >> 16;
			uint8_t c = upper + upper / 255;
			*buffer++ ^= c;
			i++;
		}
	}
};



void create_directories(const std::string& filePath) {
    std::filesystem::path file(filePath);
    std::filesystem::path parentDir = file.parent_path();

    // Create the parent directory if it doesn't exist
    if (!parentDir.empty() && !std::filesystem::exists(parentDir)) {
        std::error_code ec; // To capture any error during creation
        std::filesystem::create_directories(parentDir, ec); // Creates parentDir and all necessary parent directories
        if (ec) {
            std::cerr << "Error creating directories: " << ec.message() << std::endl;
        }
    }
}




bool extract_zip(const std::string& zipFile, const std::string& outputDir) {
	unzFile zip = unzOpen64(zipFile.c_str());
	if (zip == nullptr) {
		std::cerr << "Error: Cannot open zip file " << zipFile << std::endl;
		return false;
	}

	if (unzGoToFirstFile(zip) != UNZ_OK) {
		std::cerr << "Error: Cannot go to first file in zip" << std::endl;
		unzClose(zip);
		return false;
	}

	do {
		char fileName[256];
		unz_file_info fileInfo;
		
		if (unzGetCurrentFileInfo(zip, &fileInfo, fileName, sizeof(fileName), nullptr, 0, nullptr, 0) != UNZ_OK) {
			std::cerr << "Error: Cannot get file info" << std::endl;
			unzClose(zip);
			return false;
		}
		// std::cout << "Unzipping " << fileName << std::endl;
		/*
		std::cout << "-----------\nversion " << fileInfo.version
			<< "\nversion_needed " << fileInfo.version_needed
			<< "\nflag " << fileInfo.flag
			<< "\ncompression_method " << fileInfo.compression_method
			<< "\n compressed_size " << fileInfo.compressed_size
			<< "\n mz_dos_date " << fileInfo.mz_dos_date
			<< "\n crc " << fileInfo.crc
			<< "\n compressed_size " << fileInfo.compressed_size
			<< "\n uncompressed_size " << fileInfo.uncompressed_size
			<< "\n size_filename " << fileInfo.size_filename << " " << fileName
			<< "\n size_file_extra " << fileInfo.size_file_extra
			<< "\n size_file_comment " << fileInfo.size_file_comment
			<< "\n disk_num_start " << fileInfo.disk_num_start
			<< "\n internal_fa " << fileInfo.internal_fa
			<< "\n external_fa " << fileInfo.external_fa << "\n\n";
		*/
		std::string fullPath = outputDir + "/" + fileName;

		create_directories(fullPath);
		if (!(fileInfo.external_fa & 0x10)) { // If it's a directory
			int state = unzOpenCurrentFile(zip);
			if (state != UNZ_OK) {
				std::cerr << "Error: Cannot open current file in zip. Err Code " << state << std::endl;
				unzClose(zip);
				return false;
			}

			FILE* outFile = fopen(fullPath.c_str(), "wb");
			if (outFile == nullptr) {
				std::cerr << "Error: Cannot open output file " << fullPath << std::endl;
				unzCloseCurrentFile(zip);
				unzClose(zip);
				return false;
			}

			char buffer[8192];
			int bytesRead = 0;
			while ((bytesRead = unzReadCurrentFile(zip, buffer, sizeof(buffer))) > 0) {
				fwrite(buffer, 1, bytesRead, outFile);
			}

			fclose(outFile);
			unzCloseCurrentFile(zip);
		}
	} while (unzGoToNextFile(zip) == UNZ_OK);

	unzClose(zip);
	return true;
}




bool decode_score_dat(const std::string& openPath, const std::string& writePath, const std::string& gzPath) {
	std::cout << "open " << openPath << " and then write to " << writePath << "\n";
	FILE* inFile = fopen(openPath.c_str(), "r");

	fseek(inFile, 0, SEEK_END);
	std::size_t fileSize = ftell(inFile);
	rewind(inFile);

	uint8_t* buffer = static_cast<uint8_t*>(malloc(fileSize));
	if (!buffer) {
		perror("Failed to allocate buffer");
		fclose(inFile);
		return false;
	}

	// Step 4: Read the file contents into the buffer
	if (fread(buffer, 1, fileSize, inFile) != fileSize) {
		perror("Failed to read file");
		free(buffer);
		fclose(inFile);
		return false;
	}

	// Step 5: Close the file and return the buffer
	fclose(inFile);

	ScoreFileCrypter theTool;
	std::cout << "file size " << fileSize << "\n";

	theTool.CryptBuffer(buffer,fileSize);

	FILE* gzipFile = fopen(gzPath.c_str(),"wb");
	if (!gzipFile) {
		perror("Failed to open file");
		return false;
	}
	 std::size_t bytesWritten = fwrite(buffer, sizeof(uint8_t), fileSize, gzipFile);
	if (bytesWritten != fileSize) {
		perror("Failed to write to file");
		  fclose(gzipFile);
		  return false;
	}

	// Close the file
	fclose(gzipFile);


	gzFile gzRead = gzopen(gzPath.c_str(), "rb");
	if (!gzRead) {
		perror("Failed to open gzip file");
		return false;
	}

	const std::size_t chunkSize = 16384;
	std::size_t bufferSize = chunkSize;
	uint8_t* xmlBuffer = static_cast<uint8_t*>(malloc(bufferSize));
	if (!xmlBuffer) {
		perror("Failed to allocate buffer");
		gzclose(gzRead);
		return false;
	}

	std::size_t totalRead = 0;
	int bytesRead;
	while ((bytesRead = gzread(gzRead, xmlBuffer + totalRead, chunkSize)) > 0) {
		totalRead += bytesRead;

		// If buffer is full, expand it
		if (totalRead == bufferSize) {
			bufferSize += chunkSize;
			xmlBuffer = static_cast<uint8_t*>(realloc(xmlBuffer, bufferSize));
			if (!buffer) {
				perror("Failed to reallocate buffer");
				gzclose(gzRead);
				return false;
			}
		}
	}

	// Clean up
	gzclose(gzRead);
	int uncompressedSize = totalRead;
	std::cout << "decompressed Size " << uncompressedSize << "\n";

	//Bookmark
	FILE* xmlFile = fopen(writePath.c_str(),"wb");
	if (!xmlFile) {
		perror("Failed to open file");
		return false;
	}
	bytesWritten = fwrite(xmlBuffer, sizeof(uint8_t), uncompressedSize, xmlFile);
	if (bytesWritten != uncompressedSize) {
		perror("Failed to write to file");
		  fclose(xmlFile);
		  return false;
	}

	// Close the file
	fclose(xmlFile);
	if (remove(gzPath.c_str()) != 0) {
        perror("Error deleting the temporary file");
    } else {
        printf("Temporary file deleted successfully\n");
    }

	return true;
}


int main(int argc, char* argv[]) {
	
	if (argc < 2) {
		std::cerr << "Usage: " << argv[0] << " <musx file>" << std::endl;
		return 1;
	}

	std::string zipFilePath = argv[1];
	std::string outputDir = zipFilePath.substr(0, zipFilePath.find_last_of('.'));

	if (!extract_zip(zipFilePath, outputDir)) {
		std::cerr << "Failed to extract zip file " << zipFilePath << " to " << outputDir << std::endl;
		return 1;
	}

	std::cout << "Decrypting and expanding score.dat\n";
	std::string datOpen = outputDir + "/score.dat";
	std::string datGud = outputDir + "/score.enigmaxml";
	std::string datGzzzz = outputDir + "/score.gz";
	if (!decode_score_dat(datOpen,datGud,datGzzzz)) {
		std::cerr << "BONK! we failed";
		return 1;
	}



	std::cout << "Extraction complete!" << std::endl;
	return 0; 
}
