// nyl includes
#include "core_log.h"
#include "resource_manager.h"

// thirdparty
#include <stb/stb_image.h>

// std
#include <iostream>
#include <sstream>
#include <fstream>

namespace Nyl {

// <><><><><>          instantiate static variables          <><><><><>
std::map<std::string, ShaderComponent*> ResourceManager::Shaders;
std::map<std::string, TextureComponent*> ResourceManager::Textures;
ShaderSystem* ResourceManager::shaderSystem = nullptr;


ShaderComponent* ResourceManager::LoadShader(const char* vShaderFile, const char* fShaderFile, const char* gShaderFile, std::string name)
{
    ShaderComponent* shaderComponent = loadShaderFromFile(vShaderFile, fShaderFile, gShaderFile);
    Shaders[name] = shaderComponent;
    return shaderComponent;
}

ShaderComponent* ResourceManager::GetShader(std::string name)
{
    if (Shaders.find(name) == Shaders.end())
    {
        NYL_CORE_ERROR("Shader {0} not found.", name);
    }
    return Shaders[name];
}

TextureComponent* ResourceManager::LoadTexture(const char* file, bool alpha, std::string name)
{
    Textures[name] = loadTextureFromFile(file, alpha);
    return Textures[name];
}

TextureComponent* ResourceManager::GetTexture(std::string name)
{
    if (Textures.find(name) == Textures.end())
    {
        NYL_CORE_ERROR("Texture {0} not found.", name);
    }
    return Textures[name];
}

void ResourceManager::Clear()
{
    // (properly) delete all shaders	
    for (auto iter : Shaders)
    {
        glDeleteProgram(iter.second->ID);
        delete iter.second;
    }
    Shaders.clear();
    // (properly) delete all textures
    for (auto iter : Textures)
    {
        glDeleteTextures(1, &iter.second->ID);
        delete iter.second;
    }
    Textures.clear();
}

ShaderComponent* ResourceManager::loadShaderFromFile(const char* vShaderFile, const char* fShaderFile, const char* gShaderFile)
{
    // 1. retrieve the vertex/fragment source code from filePath
    std::string vertexCode;
    std::string fragmentCode;
    std::string geometryCode;
    try
    {
        // open files
        std::ifstream vertexShaderFile(vShaderFile);
        std::ifstream fragmentShaderFile(fShaderFile);
        std::stringstream vShaderStream, fShaderStream;
        // read file's buffer contents into streams
        vShaderStream << vertexShaderFile.rdbuf();
        fShaderStream << fragmentShaderFile.rdbuf();
        // close file handlers
        vertexShaderFile.close();
        fragmentShaderFile.close();
        // convert stream into string
        vertexCode = vShaderStream.str();
        fragmentCode = fShaderStream.str();
        // if geometry shader path is present, also load a geometry shader
        if (gShaderFile != nullptr)
        {
            std::ifstream geometryShaderFile(gShaderFile);
            std::stringstream gShaderStream;
            gShaderStream << geometryShaderFile.rdbuf();
            geometryShaderFile.close();
            geometryCode = gShaderStream.str();
        }
    }
    catch (std::exception e)
    {
        NYL_CORE_ERROR("SHADER: Failed to read shader files : {0}",e.what());
    }
    const char* vShaderCode = vertexCode.c_str();
    const char* fShaderCode = fragmentCode.c_str();
    const char* gShaderCode = geometryCode.c_str();
    // 2. now create shader object from source code
    ShaderComponent* shader = new ShaderComponent();
    shader->vertexSource = vertexCode;
    shader->fragmentSource = fragmentCode;

    // Compile the shader
    shader->compile();

    return shader;
}

TextureComponent* ResourceManager::loadTextureFromFile(const char* file, bool alpha)
{
    // create texture object
    TextureComponent* texture = new TextureComponent();
    if (alpha)
    {
        texture->object_format = GL_RGBA;
        texture->image_format = GL_RGBA;
    }
    // load image
    int width, height, nrChannels;
    unsigned char* data = stbi_load(file, &width, &height, &nrChannels, 0);
    if (data)
    {
        NYL_CORE_INFO("Image width:{0}, height {1} loaded.", width, height);
        // now generate texture
        texture->Generate(width, height, data);
        // and finally free image data
        stbi_image_free(data);
    }
    else
    {
        NYL_CORE_ERROR("Failed to load texture file {0}: {1}", file, stbi_failure_reason());
        delete texture;
        return nullptr;
    }
    return texture;
}
}
