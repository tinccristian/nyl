#ifndef VAO_H
#define VAO_H

#include<glad/glad.h>
#include"VBO.h"

class VAO
{
public:
	// ID reference for the Vertex Array Object
	GLuint ID;

	// Links a VBO to the VAO using a certain layout
	void LinkVBO(VBO& VBO, GLuint layout);
	// Binds the VAO
	void Bind();
	// Unbinds the VAO
	void Unbind();
	// Deletes the VAO
	void Delete();
};
#endif