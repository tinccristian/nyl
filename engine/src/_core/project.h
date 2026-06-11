#pragma once

#include <string>
#include <vector>

#include "core.h"

namespace nyl
{
    /**
     * @brief Editable, serializable project settings (a `.nylproj` file).
     */
    struct ProjectSettings
    {
        std::string name = "Untitled";
        int         windowWidth = 1280;
        int         windowHeight = 720;
        std::string assetDir = "assets";       // relative to the project root
        std::string sceneDir = "scenes";       // relative to the project root
        std::string startupScene = "main.scene";
    };

    /**
     * @brief A NYL project: a folder containing a `.nylproj` settings file, an
     * assets folder and a scenes folder.
     *
     * This is the unit the editor creates / opens / configures so games can be
     * authored without an external IDE. Single "current project" model.
     */
    class NYL_API Project
    {
    public:
        // scaffold a new project (folders + .nylproj + empty main scene), then open it
        static bool Create(const std::string& rootDir, const std::string& name);
        static bool Load(const std::string& projectFilePath);
        static bool Save();
        static void Close();
        static bool IsOpen();

        static const std::string& RootDir();
        static const std::string& ProjectFile();
        static ProjectSettings&   Settings();

        static std::string AssetDirPath();                  // rootDir/assetDir
        static std::string SceneDirPath();                  // rootDir/sceneDir
        static std::string AssetPath(const std::string& relative);
        static std::string ScenePath(const std::string& relative);

        static std::vector<std::string> ListScenes(); // *.scene file names in sceneDir
        static std::vector<std::string> ListAssets(); // image file names in assetDir
    };
}
