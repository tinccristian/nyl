#include"VBO.h"


// empty constructor
// ------------------------------------------------------------------------
VBO::VBO() { ID = 0; }
// constructor that generates a Vertex Buffer Object and links it to vertices
// ------------------------------------------------------------------------
VBO::VBO(GLfloat* vertices, GLsizeiptr size)
{
	glGenBuffers(1, &ID);
	glBindBuffer(GL_ARRAY_BUFFER, ID);
	glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);
}
VBO::~VBO() { Delete(); }

// binds the VBO
// ------------------------------------------------------------------------
void VBO::Bind()
{
	glBindBuffer(GL_ARRAY_BUFFER, ID);
}

// unbinds the VBO
// ------------------------------------------------------------------------
void VBO::Unbind()
{
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

// deletes the VBO
// ------------------------------------------------------------------------
void VBO::Delete()
{
	glDeleteBuffers(1, &ID);
}