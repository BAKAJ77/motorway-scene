#pragma once
#include "VertexArray.h"

template<typename T>
void VertexArray::PushAttribLayout(GLuint index, GLint size, GLsizei stride, GLsizei offset, GLuint divisor,
	GLboolean normalized)
{
	static_assert(false);
}

template<>
void VertexArray::PushAttribLayout<GLfloat>(GLuint index, GLint size, GLsizei stride, GLsizei offset, GLuint divisor,
	GLboolean normalized)
{
	VertexAttribLayout layout { index, size, GL_FLOAT, normalized, stride, (void*)offset, divisor };
	m_attribLayouts.emplace_back(layout);
}

template<>
void VertexArray::PushAttribLayout<GLuint>(GLuint index, GLint size, GLsizei stride, GLsizei offset, GLuint divisor,
	GLboolean normalized)
{
	VertexAttribLayout layout { index, size, GL_UNSIGNED_INT, normalized, stride, (void*)offset, divisor };
	m_attribLayouts.emplace_back(layout);
}

template<>
void VertexArray::PushAttribLayout<GLubyte>(GLuint index, GLint size, GLsizei stride, GLsizei offset, GLuint divisor,
	GLboolean normalized)
{
	VertexAttribLayout layout { index, size, GL_UNSIGNED_BYTE, normalized, stride, (void*)offset, divisor };
	m_attribLayouts.emplace_back(layout);
}