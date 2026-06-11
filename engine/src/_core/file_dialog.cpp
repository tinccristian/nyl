#include "file_dialog.h"

#include "tinyfiledialogs.h" // already wrapped in extern "C" for C++

namespace nyl
{
    static std::string toStr(const char* c) { return c ? std::string(c) : std::string(); }

    std::string FileDialog::OpenFile(const std::string& title,
                                     const std::vector<std::string>& filterPatterns,
                                     const std::string& filterDesc)
    {
        std::vector<const char*> pats;
        pats.reserve(filterPatterns.size());
        for (const std::string& p : filterPatterns) pats.push_back(p.c_str());

        const char* result = tinyfd_openFileDialog(
            title.c_str(), "",
            static_cast<int>(pats.size()),
            pats.empty() ? nullptr : pats.data(),
            filterDesc.empty() ? nullptr : filterDesc.c_str(),
            0);
        return toStr(result);
    }

    std::string FileDialog::SaveFile(const std::string& title,
                                     const std::string& defaultPath,
                                     const std::vector<std::string>& filterPatterns,
                                     const std::string& filterDesc)
    {
        std::vector<const char*> pats;
        pats.reserve(filterPatterns.size());
        for (const std::string& p : filterPatterns) pats.push_back(p.c_str());

        const char* result = tinyfd_saveFileDialog(
            title.c_str(), defaultPath.c_str(),
            static_cast<int>(pats.size()),
            pats.empty() ? nullptr : pats.data(),
            filterDesc.empty() ? nullptr : filterDesc.c_str());
        return toStr(result);
    }

    std::string FileDialog::SelectFolder(const std::string& title, const std::string& defaultPath)
    {
        const char* result = tinyfd_selectFolderDialog(title.c_str(), defaultPath.c_str());
        return toStr(result);
    }
}
