#pragma once

#include <map>
#include <string>

#include <glad/glad.h>

#include "Texture.h"
#include "Shader.h"
#include "Core.h"

namespace Nyl
{

/**
 * @class EntityManager
 * @brief A static class responsible for managing resources such as shaders and textures.
 *
 * The EntityManager class provides static functions to load, retrieve, and clear resources.
 * It stores shaders and textures in maps for easy access.
 * The class follows the singleton pattern, as it has a private constructor and its members and functions are publicly available (static).
 */
class NYL_API EntityManager
{
    public:
        // resource storage
        static std::map<std::string, Shader >    Shaders;
        static std::map<std::string, Texture>    Textures;
        // loads (and generates) a shader program from file loading vertex, fragment (and geometry) shader's source code. If gShaderFile is not nullptr, it also loads a geometry shader
        static Shader    LoadShader(const char* vShaderFile, const char* fShaderFile, const char* gShaderFile, std::string name);
        // retrieves a stored shader
        static Shader    GetShader(std::string name);
        // loads (and generates) a texture from file
        static Texture LoadTexture(const char* file, bool alpha, std::string name);
        // retrieves a stored texture
        static Texture GetTexture(std::string name);
        // properly de-allocates all loaded resources
        static void      Clear();
    private:
        // private constructor, that is we do not want any actual resource manager objects. Its members and functions should be publicly available (static).
        EntityManager() { }
        // loads and generates a shader from file
        static Shader    loadShaderFromFile(const char* vShaderFile, const char* fShaderFile, const char* gShaderFile = nullptr);
        // loads a single texture from file
        static Texture loadTextureFromFile(const char* file, bool alpha);
    };

}