#include "system_renderer.h"
#include "resource_manager.h"
#include "log.h"

using namespace nyl;

RenderSystem::RenderSystem(ShaderComponent &shader, float screenWidth, float screenHeight)
    : shader(shader) , windowSize(glm::vec2(screenWidth, screenHeight))
{
    this->initRenderData();
}

RenderSystem::~RenderSystem()
{
    glDeleteVertexArrays(1, &this->quadVAO);
    glDeleteVertexArrays(1, &this->outlineVAO);
}
void RenderSystem::initRenderData()
{
	// configure VAO/VBO for filled rectangle
	unsigned int VBO;
	float vertices[] = {
		// pos      // tex
		0.0f, 1.0f, 0.0f, 1.0f,
		1.0f, 0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 0.0f,

		0.0f, 1.0f, 0.0f, 1.0f,
		1.0f, 1.0f, 1.0f, 1.0f,
		1.0f, 0.0f, 1.0f, 0.0f
	};

	glGenVertexArrays(1, &this->quadVAO);
	glGenBuffers(1, &VBO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindVertexArray(this->quadVAO);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	// configure VAO/VBO for rectangle outline
	float outlineVertices[] = {
		0.0f, 0.0f, // Bottom-left vertex
		1.0f, 0.0f, // Bottom-right vertex
		1.0f, 1.0f, // Top-right vertex
		0.0f, 1.0f  // Top-left vertex
	};

	glGenVertexArrays(1, &this->outlineVAO);
	glGenBuffers(1, &VBO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(outlineVertices), outlineVertices, GL_STATIC_DRAW);

	glBindVertexArray(this->outlineVAO);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void RenderSystem::DrawSprite(const TextureComponent& texture, glm::vec2 position, glm::vec2 size, float rotate, glm::vec3 color)
{

    this->shader.use();                 // fuck you
    // prepare transformations
    position = getGlfwCoordinates(position, size);
    glm::mat4 model = glm::mat4(1.0f);

    // first translate to the sprite's position
    model = glm::translate(model, glm::vec3(position, 0.0f));

    // then translate back by half the size to set the rotation origin to the center of the sprite
    model = glm::translate(model, glm::vec3(0.5f * size.x, 0.5f * size.y, 0.0f));
    // rotate
    model = glm::rotate(model, glm::radians(rotate), glm::vec3(0.0f, 0.0f, 1.0f));

    // translate back to the original position
    model = glm::translate(model, glm::vec3(-0.5f * size.x, -0.5f * size.y, 0.0f));

    // apply the original scale
    model = glm::scale(model, glm::vec3(size, 1.0f));

    // set shader properties
    this->shader.set_mat4("model", model);
    this->shader.set_vec3f("spriteColor", color);
    this->shader.set_vec2f("texOffset", glm::vec2(0.0f, 0.0f));
    this->shader.set_vec2f("texScale", glm::vec2(1.0f, 1.0f));
    // bind the texture
    glActiveTexture(GL_TEXTURE0);
    texture.Bind();
    CheckGLError("Before binding the texture");

    // draw the sprite
    CheckGLError("Before drawing");
    glBindVertexArray(this->quadVAO);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    CheckGLError("After drawing");
    glBindVertexArray(0);
    CheckGLError("Final");
}

void RenderSystem::DrawEntity(const Entity& entity, float deltaTime)
{
    if (!CheckRenderComponents(entity)) { return; }

    auto transform = entity.getComponent<TransformComponent>();
    auto camera = entity.getComponent<Camera>();

    std::shared_ptr<TextureComponent> texture = nullptr;
    Animation* currentAnimation = nullptr;

    if (entity.hasComponent<AnimatedComponent>()) {
        auto animatedComponent = entity.getComponent<AnimatedComponent>();
        animatedComponent->Update(deltaTime);
        currentAnimation = animatedComponent->GetCurrentAnimation();
        texture = animatedComponent->GetCurrentTexture();
    }
    else if (entity.hasComponent<TextureComponent>()) {
        texture = entity.getComponent<TextureComponent>();
    }

    if (!texture) {
        NYL_CORE_ERROR("Error: Entity has no valid texture");
        return;
    }

    // use the shader program first
    this->shader.use();
    CheckGLError("After shader use");

    // calculate and set view matrix
    camera->position.x = transform->position.x;
    camera->position.y = transform->position.y;
    glm::mat4 view = glm::lookAt(
        glm::vec3(camera->position.x, camera->position.y, camera->zoom),
        glm::vec3(camera->position.x, camera->position.y, 0.0f),
        glm::vec3(0.0f, 1.0f, 0.0f));
    view = glm::scale(view, glm::vec3(1.0f / camera->zoom, 1.0f / camera->zoom, 1.0f));
    this->shader.set_mat4("view", view);

    // calculate and set model matrix
    glm::vec2 worldPosition = GetWorldPosition(entity);
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(transform->position, 0.0f));
    model = glm::translate(model, glm::vec3(0.5f * transform->size.x, 0.5f * transform->size.y, 0.0f));
    if (transform->direction != 0)
    {
        model = glm::scale(model, glm::vec3(transform->direction, 1.0f, 1.0f));
    }
    model = glm::translate(model, glm::vec3(-0.5f * transform->size.x, -0.5f * transform->size.y, 0.0f));
    model = glm::rotate(model, glm::radians(transform->rotation), glm::vec3(0.0f, 0.0f, 1.0f));
    model = glm::scale(model, glm::vec3(transform->size, 1.0f));
    model = glm::translate(model, glm::vec3(worldPosition, 0.0f));
    this->shader.set_mat4("model", model);

    // set animation properties for the shader
    if (currentAnimation)
    {
        float frameWidth = static_cast<float>(currentAnimation->frameWidth) / texture->width;
        float frameOffsetX = frameWidth * currentAnimation->currentFrame;
        this->shader.set_vec2f("texOffset", glm::vec2(frameOffsetX, 0.0f));
        this->shader.set_vec2f("texScale", glm::vec2(frameWidth, 1.0f));
    }
    else
    {
        this->shader.set_vec2f("texOffset", glm::vec2(0.0f, 0.0f));
        this->shader.set_vec2f("texScale", glm::vec2(1.0f, 1.0f));
    }

    // set color
    this->shader.set_vec3f("spriteColor", Colors::White);

    CheckGLError("After setting uniforms");

    // bind the texture
    glActiveTexture(GL_TEXTURE0);
    texture->Bind();
    CheckGLError("After binding texture");

    // draw the sprite
    glBindVertexArray(this->quadVAO);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(0);
    CheckGLError("After drawing");
}
bool nyl::RenderSystem::CheckRenderComponents(const Entity& entity)
{
	// get the entity's transform component
	auto transform = entity.getComponent<TransformComponent>();
	if (!transform) {
		NYL_CORE_ERROR("Error: Entity does not have a TransformComponent");
		return false;
	}
	// get the entity's texture component
	auto texture = entity.getComponent<TextureComponent>();
    if (!entity.hasComponent<TextureComponent>() && !entity.hasComponent<AnimatedComponent>()) {
        NYL_CORE_ERROR("Error: Entity does not have a TextureComponent or AnimatedComponent");
        return false;
    }
    // get the entity's camera component
	auto camera = entity.getComponent<Camera>();
	if (!camera) {
		NYL_CORE_ERROR("Error: Entity does not have a Camera");
		return false;
	}
    return true;
}
void RenderSystem::DrawRectangleOutline(glm::vec2 position, glm::vec2 size, float rotate, glm::vec3 color)
{
    this->shader.use();
    // prepare transformations
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(position, 0.0f));  // first translate

    model = glm::rotate(model, glm::radians(rotate), glm::vec3(0.0f, 0.0f, 1.0f)); // then rotate

    model = glm::scale(model, glm::vec3(size, 1.0f)); // scale

    this->shader.set_mat4("model", model);

    // render rectangle outline
    this->shader.set_vec3f("debugColor", color);

	CheckGLError("Before drawing");
    glBindVertexArray(this->outlineVAO);
    glDrawArrays(GL_LINE_LOOP, 0, 4);
    glBindVertexArray(0);
	CheckGLError("After drawing");
}
void RenderSystem::update() 
{
    //for (Entity& entity : entities) {
    //    auto renderComponent = entity.getComponent<RenderComponent>();
    //    auto cameraComponent = entity.getComponent<Camera>();
    //    
    //    if (renderComponent) {
    //        // Draw the entity using the data in the renderComponent...
    //        DrawSprite(renderComponent->texture, renderComponent->position, renderComponent->size, renderComponent->rotate, renderComponent->color);
    //    }
    //}
}
void RenderSystem::CheckGLError(const char* operation) {
	GLenum error;
	while ((error = glGetError()) != GL_NO_ERROR) {
		std::string errorStr;
		switch (error) {
		case GL_INVALID_ENUM:                  errorStr = "INVALID_ENUM"; break;
		case GL_INVALID_VALUE:                 errorStr = "INVALID_VALUE"; break;
		case GL_INVALID_OPERATION:             errorStr = "INVALID_OPERATION"; break;
		case GL_STACK_OVERFLOW:                errorStr = "STACK_OVERFLOW"; break;
		case GL_STACK_UNDERFLOW:               errorStr = "STACK_UNDERFLOW"; break;
		case GL_OUT_OF_MEMORY:                 errorStr = "OUT_OF_MEMORY"; break;
		case GL_INVALID_FRAMEBUFFER_OPERATION: errorStr = "INVALID_FRAMEBUFFER_OPERATION"; break;
		}
		NYL_CORE_ERROR("OpenGL error after {0}: {1} ({2})", operation, errorStr, error);
	}
}
glm::vec2 RenderSystem::getGlfwCoordinates(glm::vec2 worldPos, glm::vec2 size)
{
    // convert from world coordinates (origin at center) to GLFW coordinates (origin at top-left)
    glm::vec2 glfwPos;
    glfwPos.x = (worldPos.x + windowSize.x / 2.0f) - size.x / 2.0f;
    glfwPos.y = (windowSize.y / 2.0f - worldPos.y) - size.y / 2.0f;
    return glfwPos;
}
glm::vec2 RenderSystem::GetWorldPosition(const Entity& entity) 
{
	auto transform = entity.getComponent<TransformComponent>();
	auto camera = entity.getComponent<Camera>();

	// if entity doesn't have a transform component, return (0, 0)
	if (!transform) {
		return glm::vec2(0.0f, 0.0f);
	}

	// if entity has a camera component, adjust for camera position
	if (camera) {
		return glm::vec2(transform->position.x - camera->position.x, transform->position.y - camera->position.y);
	}

	// otherwise, return entity's position as is
	return glm::vec2(transform->position.x, transform->position.y);
}