#include "project.h"
#include "log.h"

#include <filesystem>
#include <fstream>
#include <sstream>
#include <algorithm>

#include <nlohmann/json.hpp>

namespace nyl
{
    namespace fs = std::filesystem;
    using json = nlohmann::json;

    static ProjectSettings s_settings;
    static std::string     s_rootDir;
    static std::string     s_projectFile;
    static bool            s_open = false;

    static bool hasExtension(const fs::path& p, std::initializer_list<const char*> exts)
    {
        std::string e = p.extension().string();
        std::transform(e.begin(), e.end(), e.begin(), [](unsigned char c) { return (char)std::tolower(c); });
        for (const char* x : exts) if (e == x) return true;
        return false;
    }

    bool Project::Create(const std::string& rootDir, const std::string& name)
    {
        std::error_code ec;
        fs::path root(rootDir);
        fs::create_directories(root, ec);
        fs::create_directories(root / "assets", ec);
        fs::create_directories(root / "scenes", ec);

        s_settings = ProjectSettings{};
        s_settings.name = name;
        s_settings.startupScene = "main.scene";
        s_rootDir = root.string();
        s_projectFile = (root / (name + ".nylproj")).string();
        s_open = true;

        // starter empty scene
        {
            std::ofstream scene(root / "scenes" / "main.scene");
            scene << "{\n  \"entities\": [],\n  \"version\": 1\n}\n";
        }

        return Save();
    }

    bool Project::Load(const std::string& projectFilePath)
    {
        std::ifstream in(projectFilePath);
        if (!in)
        {
            NYL_CORE_ERROR("Project: cannot open '{}'", projectFilePath);
            return false;
        }
        std::stringstream ss; ss << in.rdbuf();
        json root = json::parse(ss.str(), nullptr, false);
        if (root.is_discarded())
        {
            NYL_CORE_ERROR("Project: failed to parse '{}'", projectFilePath);
            return false;
        }

        ProjectSettings s;
        s.name         = root.value("name", std::string("Untitled"));
        s.windowWidth  = root.value("windowWidth", 1280);
        s.windowHeight = root.value("windowHeight", 720);
        s.assetDir     = root.value("assetDir", std::string("assets"));
        s.sceneDir     = root.value("sceneDir", std::string("scenes"));
        s.startupScene = root.value("startupScene", std::string());

        s_settings = s;
        s_projectFile = fs::absolute(projectFilePath).string();
        s_rootDir = fs::path(s_projectFile).parent_path().string();
        s_open = true;
        NYL_CORE_INFO("Project '{}' loaded ({})", s_settings.name, s_rootDir);
        return true;
    }

    bool Project::Save()
    {
        if (!s_open) return false;
        json root;
        root["name"]         = s_settings.name;
        root["windowWidth"]  = s_settings.windowWidth;
        root["windowHeight"] = s_settings.windowHeight;
        root["assetDir"]     = s_settings.assetDir;
        root["sceneDir"]     = s_settings.sceneDir;
        root["startupScene"] = s_settings.startupScene;

        std::ofstream out(s_projectFile);
        if (!out)
        {
            NYL_CORE_ERROR("Project: cannot write '{}'", s_projectFile);
            return false;
        }
        out << root.dump(2);
        NYL_CORE_INFO("Project saved to {}", s_projectFile);
        return true;
    }

    void Project::Close()
    {
        s_open = false;
        s_rootDir.clear();
        s_projectFile.clear();
        s_settings = ProjectSettings{};
    }

    bool Project::IsOpen() { return s_open; }
    const std::string& Project::RootDir() { return s_rootDir; }
    const std::string& Project::ProjectFile() { return s_projectFile; }
    ProjectSettings& Project::Settings() { return s_settings; }

    std::string Project::AssetDirPath() { return (fs::path(s_rootDir) / s_settings.assetDir).string(); }
    std::string Project::SceneDirPath() { return (fs::path(s_rootDir) / s_settings.sceneDir).string(); }
    std::string Project::AssetPath(const std::string& rel) { return (fs::path(AssetDirPath()) / rel).string(); }
    std::string Project::ScenePath(const std::string& rel) { return (fs::path(SceneDirPath()) / rel).string(); }

    std::vector<std::string> Project::ListScenes()
    {
        std::vector<std::string> out;
        if (!s_open) return out;
        std::error_code ec;
        for (const auto& entry : fs::directory_iterator(SceneDirPath(), ec))
            if (entry.is_regular_file() && hasExtension(entry.path(), { ".scene" }))
                out.push_back(entry.path().filename().string());
        std::sort(out.begin(), out.end());
        return out;
    }

    std::vector<std::string> Project::ListAssets()
    {
        std::vector<std::string> out;
        if (!s_open) return out;
        std::error_code ec;
        for (const auto& entry : fs::directory_iterator(AssetDirPath(), ec))
            if (entry.is_regular_file() && hasExtension(entry.path(), { ".png", ".jpg", ".jpeg", ".bmp" }))
                out.push_back(entry.path().filename().string());
        std::sort(out.begin(), out.end());
        return out;
    }
}
