#include "IndexBuffer.h"
#include <GL/glew.h>

IndexBuffer::IndexBuffer(const unsigned int* data, unsigned int count)
	: m_count(count)
{
	glGenBuffers(1, &m_rendererID);
	bind();
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(unsigned int), data, GL_STATIC_DRAW);
}

IndexBuffer::~IndexBuffer()
{
	glDeleteBuffers(1, &m_rendererID);
}

void IndexBuffer::bind() const
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_rendererID);
}

void IndexBuffer::unbind() const
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}