#pragma once

#include <map>
#include <string>

#include <glad/glad.h>

#include "texture.h"
#include "shader.h"
#include "system_shader.h"
#include "animation.h"

#include "core.h"

namespace nyl
{

/**
 * @class ResourceManager
 * @brief A static class responsible for managing resources such as shaders and textures.
 *
 * The ResourceManager class provides static functions to load, retrieve, and clear resources.
 * It stores shaders and textures in maps for easy access.
 * The class follows the singleton pattern, as it has a private constructor and its members and functions are publicly available (static).
 */
class NYL_API ResourceManager
    {
    public:
        static std::map<std::string, ShaderComponent*> Shaders;
        static std::map<std::string, TextureComponent*> Textures;
        //static EntityManager* entityManager;
        static ShaderSystem* shaderSystem;

        static ShaderComponent* LoadShader(const char* vShaderFile, const char* fShaderFile, const char* gShaderFile, std::string name);
        static ShaderComponent* GetShader(std::string name);
        static TextureComponent* LoadTexture(const char* file, bool alpha, std::string name);
        static TextureComponent* GetTexture(std::string name);
        /// 1x1 white texture (lazily created) for solid-colour quads: particles,
        /// text backgrounds, debug shapes. Requires an active GL context.
        static TextureComponent* GetWhiteTexture();
        static void Clear();

        // ---- resource root (project/asset directory) ----
        // Lets the game/editor load assets by a path relative to a project root,
        // instead of relying on compile-time source paths. Used by scene
        // serialization (Phase 6) and the editor (Phase 8).
        static void SetResourceRoot(const std::string& path);
        static const std::string& GetResourceRoot();
        static std::string ResolvePath(const std::string& relative);

    private:
        ResourceManager() { }
        static std::string s_ResourceRoot;
        static ShaderComponent* loadShaderFromFile(const char* vShaderFile, const char* fShaderFile, const char* gShaderFile = nullptr);
        static TextureComponent* loadTextureFromFile(const char* file, bool alpha);

};
}