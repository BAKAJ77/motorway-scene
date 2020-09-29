#include "VertexArray.h"
#include <cassert>

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////

VertexArray::VertexArray()
{
	glGenVertexArrays(1, &m_ID);
}

VertexArray::~VertexArray()
{
	glDeleteVertexArrays(1, &m_ID);
}

void VertexArray::AttachBufferObjects(std::shared_ptr<VertexBuffer> vbo, std::shared_ptr<IndexBuffer> ibo)
{
	assert(vbo != nullptr); // A valid VBO must be given

	glBindVertexArray(m_ID);
	vbo->BindBuffer();
	if (ibo)
		ibo->BindBuffer();

	// Configure the vertex attribs with the layouts stored
	for (const auto& layout : m_attribLayouts)
	{
		glEnableVertexAttribArray(layout.index);
		glVertexAttribPointer(layout.index, layout.size, layout.type, layout.normalized, layout.stride,
			layout.offset);
		glVertexAttribDivisor(layout.index, layout.divisor);
	}

	m_attribLayouts.clear();

	glBindVertexArray(0);
	vbo->UnbindBuffer();
	if (ibo)
		ibo->UnbindBuffer();
}

void VertexArray::BindVertexArray() const
{
	glBindVertexArray(m_ID);
}

void VertexArray::UnbindVertexArray() const
{
	glBindVertexArray(0);
}

const uint32_t& VertexArray::GetID() const
{
	return m_ID;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////

namespace Buffer
{
	std::shared_ptr<VertexArray> GenerateVAO()
	{
		return std::make_shared<VertexArray>();
	}
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////