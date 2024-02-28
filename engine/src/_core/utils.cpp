#include "utils.h"

std::string getFullPath(const std::string& relativePath) {
    std::filesystem::path currentFile(__FILE__);
    std::filesystem::path dir = currentFile.parent_path();
    std::filesystem::path fullPath = dir / relativePath;
    return fullPath.string();
}