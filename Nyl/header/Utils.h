#pragma once

#include "Log.h"

// std
#include <iostream>
#include <fstream>
#include <string>
#include <filesystem> //C++17

namespace fs = std::filesystem;

namespace Nyl
{
    std::string load_file_contents(const std::string& filename) {
        try {
            // check if the file exists
            if (!fs::exists(filename)) {
                NYL_CORE_ERROR("File not found: " + filename);
                throw std::runtime_error("File not found: " + filename);
            }

            // open the file using a memory-mapped file
            std::ifstream file(filename, std::ios::ate | std::ios::binary);
            if (!file.is_open()) {
                NYL_CORE_ERROR("File not found: " + filename);
                throw std::runtime_error("Failed to open file: " + filename);
            }

            // get the file size
            size_t fileSize = file.tellg();
            file.seekg(0);

            // Allocate memory for the file content
            std::string fileContent(fileSize, '\0');

            // Read the file content into the string
            file.read(fileContent.data(), fileSize);

            // close the file
            file.close();

            return fileContent;
        }
        catch (const std::exception& e) {
            std::cerr << "Error: " << e.what() << std::endl;
            return "";
        }
    }
}