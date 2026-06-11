#pragma once

#include <string>
#include <vector>

#include "core.h"

namespace nyl
{
    /**
     * @brief Native OS file/folder dialogs (via tinyfiledialogs).
     *
     * Lets the editor pick projects, scenes and assets without a terminal —
     * part of making NYL usable without an external IDE. All return "" on cancel.
     */
    class NYL_API FileDialog
    {
    public:
        // filterPatterns e.g. { "*.nylproj" }; filterDesc e.g. "NYL projects"
        static std::string OpenFile(const std::string& title,
                                    const std::vector<std::string>& filterPatterns = {},
                                    const std::string& filterDesc = "");
        static std::string SaveFile(const std::string& title,
                                    const std::string& defaultPath = "",
                                    const std::vector<std::string>& filterPatterns = {},
                                    const std::string& filterDesc = "");
        static std::string SelectFolder(const std::string& title,
                                        const std::string& defaultPath = "");
    };
}
