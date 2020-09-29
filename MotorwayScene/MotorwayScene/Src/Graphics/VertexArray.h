#pragma once
#include "BufferObjects.h"
#include <vector>
#include <memory>

struct VertexAttribLayout
{
	GLuint index;
	GLint size;
	GLenum type;
	GLboolean normalized;

	GLsizei stride;
	const void* offset;

	GLuint divisor;
};

class VertexArray
{
private:
	uint32_t m_ID;
	std::vector<VertexAttribLayout> m_attribLayouts;
public:
	VertexArray();
	~VertexArray();

	void AttachBufferObjects(std::shared_ptr<VertexBuffer> vbo, std::shared_ptr<IndexBuffer> ibo = nullptr);

	void BindVertexArray() const;
	void UnbindVertexArray() const;
public:
	template<typename T>
	void PushAttribLayout(GLuint index, GLint size, GLsizei stride, GLsizei offset = 0, GLuint divisor = 0,
		GLboolean normalized = GL_FALSE);
	template<>
	void PushAttribLayout<GLfloat>(GLuint index, GLint size, GLsizei stride, GLsizei offset, GLuint divisor,
		GLboolean normalized);
	template<>
	void PushAttribLayout<GLuint>(GLuint index, GLint size, GLsizei stride, GLsizei offset, GLuint divisor,
		GLboolean normalized);
	template<>
	void PushAttribLayout<GLubyte>(GLuint index, GLint size, GLsizei stride, GLsizei offset, GLuint divisor,
		GLboolean normalized);
public:
	const uint32_t& GetID() const;
};

namespace Buffer
{
	std::shared_ptr<VertexArray> GenerateVAO();
}

#include "VertexArray.tpp"