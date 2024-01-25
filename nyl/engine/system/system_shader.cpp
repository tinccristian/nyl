#include "system_shader.h"

#include <glad/glad.h>
#include <glm/ext/matrix_float4x4.hpp>
#include <glm/gtc/type_ptr.hpp>

ShaderSystem::ShaderSystem() {}



void ShaderSystem::update() {
    for (Entity& entity : entities) {
        if (entity.hasComponent<ShaderComponent>()) {
            auto shaderComponent = entity.getComponent<ShaderComponent>();
            // Use the shader
            glUseProgram(shaderComponent->ID);
        }
    }
}