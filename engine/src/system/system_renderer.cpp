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
    //glUseProgram(this->shader.ID);
    // prepare transformations
    position = getGlfwCoordinates(position, size);
    glm::mat4 model = glm::mat4(1.0f);

    // First translate to the sprite's position
    model = glm::translate(model, glm::vec3(position, 0.0f));

   // Then translate back by half the size to set the rotation origin to the center of the sprite
    model = glm::translate(model, glm::vec3(0.5f * size.x, 0.5f * size.y, 0.0f));

    //model = glm::translate(model, glm::vec3(0.5f * size.x, 0.5f * size.y, 0.0f)); // move origin of rotation to center of quad
    model = glm::rotate(model, glm::radians(rotate), glm::vec3(0.0f, 0.0f, 1.0f)); // then rotate

    // Translate back to the original position
    model = glm::translate(model, glm::vec3(-0.5f * size.x, -0.5f * size.y, 0.0f));

    // Apply the original scale
    model = glm::scale(model, glm::vec3(size, 1.0f));

    CheckGLError();

    // Set the model matrix
    this->shader.set_mat4("model", model);
    CheckGLError();

    // render textured quad
    this->shader.set_vec3f("spriteColor", color);
    //glUseProgram(this->shader.ID);
    CheckGLError();

    // Bind the texture
    glActiveTexture(GL_TEXTURE0);
    texture.Bind();
    CheckGLError();

    // Draw the sprite
    glBindVertexArray(this->quadVAO);
    CheckGLError();
    glDrawArrays(GL_TRIANGLES, 0, 6);
    CheckGLError();
    glBindVertexArray(0);
    CheckGLError();
}

//void RenderSystem::DrawObject(const TextureComponent& texture, glm::vec2 position, glm::vec2 size, float rotate, glm::vec3 color, float direction)
//{
//    // prepare transformations
//    glm::mat4 model = glm::mat4(1.0f);
//
//    // Adjust the position based on the direction
//    if (direction == -1)
//    {
//        position.x += size.x;
//    }
//
//    model = glm::translate(model, glm::vec3(position, 0.0f));  // first translate (sprite position)
//
//    //model = glm::translate(model, glm::vec3(0.5f * size.x, 0.5f * size.y, 0.0f)); // move origin of rotation to center of quad
//    model = glm::rotate(model, glm::radians(rotate), glm::vec3(0.0f, 0.0f, 1.0f)); // then rotate
//
//    // Flip the sprite based on the direction
//    if (direction != 0)
//    {
//        model = glm::scale(model, glm::vec3(direction * size.x, size.y, 1.0f)); // scale and flip
//    }
//    else
//    {
//        model = glm::scale(model, glm::vec3(size, 1.0f)); // scale without flipping
//    }
//
//    // Set the model matrix
//    this->shader.set_mat4("model", model);
//
//    // render textured quad
//    this->shader.set_vec3f("spriteColor", color);
//
//    // Bind the texture
//    glActiveTexture(GL_TEXTURE0);
//    CheckGLError();
//    texture.Bind();
//    CheckGLError();
//
//    // Draw the sprite
//    glBindVertexArray(this->quadVAO);
//    CheckGLError();
//    glDrawArrays(GL_TRIANGLES, 0, 6);
//    CheckGLError();
//    glBindVertexArray(0);
//    CheckGLError();
//}
//void RenderSystem::DrawEntitySprite(const TextureComponent& texture, TransformComponent& transform, Camera& camera, glm::vec3 color)
//{
//    this->shader.use();
//    // Create a view matrix based on the camera's position
//    glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(-camera.position.x, -camera.position.y, 0.0f));
//    // Scale the view matrix based on the camera's zoom
//    view = glm::scale(view, glm::vec3(1.0f / camera.zoom, 1.0f / camera.zoom, 1.0f));
//    // Pass the view matrix to the shader
//    this->shader.set_mat4("view", view);
//    // prepare transformations
//    glm::mat4 model = glm::mat4(1.0f);
//
//    // translate to the sprite position
//    model = glm::translate(model, glm::vec3(transform->position, 0.0f));  
//
//    // translate back by half the size to set the rotation origin to the center of the sprite
//    model = glm::translate(model, glm::vec3(0.5f * transform->size.x, 0.5f * transform->size.y, 0.0f));
//
//    // Flip the sprite based on the direction
//    if (transform->direction != 0)
//    {
//        model = glm::scale(model, glm::vec3(transform->direction, 1.0f, 1.0f)); // flip without distorting
//    }
//
//    // translate back to the original position
//    model = glm::translate(model, glm::vec3(-0.5f * transform->size.x, -0.5f * transform->size.y, 0.0f));
//
//    // then rotate
//    model = glm::rotate(model, glm::radians(transform->rotation), glm::vec3(0.0f, 0.0f, 1.0f)); 
//
//    // Apply the original scale
//    model = glm::scale(model, glm::vec3(transform->size, 1.0f)); 
//
//    // Set the model matrix
//    this->shader.set_mat4("model", model);
//
//    // render textured quad
//    this->shader.set_vec3f("spriteColor", color);
//
//    // Bind the texture
//    glActiveTexture(GL_TEXTURE0);
//    texture.Bind();
//
//    // Draw the sprite
//    glBindVertexArray(this->quadVAO);
//    glDrawArrays(GL_TRIANGLES, 0, 6);
//    glBindVertexArray(0);
//}
void RenderSystem::DrawEntity(const Entity& entity)
{
    if (!CheckRenderComponents(entity)) { return; }

	auto transform = entity.getComponent<TransformComponent>();
	auto texture = entity.getComponent<TextureComponent>();
	auto camera = entity.getComponent<Camera>();

    // Get the world position of the entity
    glm::vec2 worldPosition = GetWorldPosition(entity);

	this->shader.use();
	
	camera->position.x = transform->position.x;
	camera->position.y = transform->position.y;

    // Create a view matrix based on the camera's position
	//glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(-camera->position.x, -camera->position.y, 0.0f));
	glm::mat4 view = glm::lookAt(
		glm::vec3(camera->position.x, camera->position.y, camera->zoom), // Assuming camera->zoom is the same for x and y
		glm::vec3(camera->position.x, camera->position.y, 0.0f),
		glm::vec3(0.0f, 1.0f, 0.0f));
    
    // Scale the view matrix based on the camera's zoom
    view = glm::scale(view, glm::vec3(1.0f / camera->zoom, 1.0f / camera->zoom, 1.0f));

	// Pass the view matrix to the shader
	this->shader.set_mat4("view", view);
	// prepare transformations
	glm::mat4 model = glm::mat4(1.0f);

	// translate to the sprite position
	model = glm::translate(model, glm::vec3(transform->position, 0.0f));

	// translate back by half the size to set the rotation origin to the center of the sprite
	model = glm::translate(model, glm::vec3(0.5f * transform->size.x, 0.5f * transform->size.y, 0.0f));

	// Flip the sprite based on the direction
	if (transform->direction != 0)
	{
		model = glm::scale(model, glm::vec3(transform->direction, 1.0f, 1.0f)); // flip without distorting
	}

	// translate back to the original position
	model = glm::translate(model, glm::vec3(-0.5f * transform->size.x, -0.5f * transform->size.y, 0.0f));

	// then rotate
	model = glm::rotate(model, glm::radians(transform->rotation), glm::vec3(0.0f, 0.0f, 1.0f));

	// Apply the original scale
	model = glm::scale(model, glm::vec3(transform->size, 1.0f));

	// translate to the sprite position using the world position
	model = glm::translate(model, glm::vec3(worldPosition, 0.0f));

	// Set the model matrix
	this->shader.set_mat4("model", model);

	// render textured quad
	this->shader.set_vec3f("spriteColor", Colors::White);

	// Bind the texture
	glActiveTexture(GL_TEXTURE0);
	texture->Bind();

	// Draw the sprite
	glBindVertexArray(this->quadVAO);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glBindVertexArray(0);
    CheckGLError();

}
bool nyl::RenderSystem::CheckRenderComponents(const Entity& entity)
{
	// Get the entity's transform component
	auto transform = entity.getComponent<TransformComponent>();
	if (!transform) {
		NYL_CORE_ERROR("Error: Entity does not have a TransformComponent");
		return false;
	}

	// Get the entity's texture component
	auto texture = entity.getComponent<TextureComponent>();
	if (!texture) {
		NYL_CORE_ERROR("Error: Entity does not have a TextureComponent");
		return false;
	}
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

    glBindVertexArray(this->outlineVAO);
    glDrawArrays(GL_LINE_LOOP, 0, 4);
    glBindVertexArray(0);
    CheckGLError();
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
void RenderSystem::CheckGLError()
{
    GLenum err;
    while ((err = glGetError()) != GL_NO_ERROR) 
    {
        NYL_CORE_ERROR("OpenGL error: " + std::to_string(err));
    }
}
glm::vec2 RenderSystem::getGlfwCoordinates(glm::vec2 worldPos, glm::vec2 size)
{
    // Convert from world coordinates (origin at center) to GLFW coordinates (origin at top-left)
    glm::vec2 glfwPos;
    glfwPos.x = (worldPos.x + windowSize.x / 2.0f) - size.x / 2.0f;
    glfwPos.y = (windowSize.y / 2.0f - worldPos.y) - size.y / 2.0f;
    return glfwPos;
}
glm::vec2 RenderSystem::GetWorldPosition(const Entity& entity) 
{
	auto transform = entity.getComponent<TransformComponent>();
	auto camera = entity.getComponent<Camera>();

	// If entity doesn't have a transform component, return (0, 0)
	if (!transform) {
		return glm::vec2(0.0f, 0.0f);
	}

	// If entity has a camera component, adjust for camera position
	if (camera) {
		return glm::vec2(transform->position.x - camera->position.x, transform->position.y - camera->position.y);
	}

	// Otherwise, return entity's position as is
	return glm::vec2(transform->position.x, transform->position.y);
}