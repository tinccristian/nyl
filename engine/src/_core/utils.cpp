#include "utils.h"

#ifdef _WIN32
#include <windows.h>
#endif

std::string getFullPath(const std::string& relativePath) {
    std::filesystem::path currentFile(__FILE__);
    std::filesystem::path dir = currentFile.parent_path();
    std::filesystem::path fullPath = dir / relativePath;
    return fullPath.string();
}

std::string getExecutableDir() {
#ifdef _WIN32
    char buffer[MAX_PATH];
    DWORD len = GetModuleFileNameA(nullptr, buffer, MAX_PATH);
    std::string path(buffer, len);
    auto pos = path.find_last_of("/\\");
    return (pos == std::string::npos) ? std::string(".") : path.substr(0, pos);
#else
    return ".";
#endif
}