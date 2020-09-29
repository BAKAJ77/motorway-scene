#include "BufferObjects.h"
#include "Utils/LoggingManager.h"
#include "Utils/ResourceManager.h"

///////////////////////////////////////////////////////////////////////////////////////////

VertexBuffer::VertexBuffer(const void* data, GLsizeiptr size, GLenum usage)
{
	glGenBuffers(1, &m_ID);
	glBindBuffer(GL_ARRAY_BUFFER, m_ID);
	glBufferData(GL_ARRAY_BUFFER, size, data, usage);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

VertexBuffer::~VertexBuffer()
{
	glDeleteBuffers(1, &m_ID);
}

void VertexBuffer::ModifySubData(const void* data, GLintptr offset, GLsizeiptr size)
{
	glBindBuffer(GL_ARRAY_BUFFER, m_ID);
	glBufferSubData(GL_ARRAY_BUFFER, offset, size, data);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void VertexBuffer::BindBuffer() const
{
	glBindBuffer(GL_ARRAY_BUFFER, m_ID);
}

void VertexBuffer::UnbindBuffer() const
{
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

const uint32_t& VertexBuffer::GetID() const
{
	return m_ID;
}

///////////////////////////////////////////////////////////////////////////////////////////

IndexBuffer::IndexBuffer(const void* data, GLsizeiptr size, GLenum usage)
{
	glGenBuffers(1, &m_ID);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ID);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, data, usage);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

IndexBuffer::~IndexBuffer()
{
	glDeleteBuffers(1, &m_ID);
}

void IndexBuffer::ModifySubData(const void* data, GLintptr offset, GLsizeiptr size)
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ID);
	glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, offset, size, data);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void IndexBuffer::BindBuffer() const
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ID);
}

void IndexBuffer::UnbindBuffer() const
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

const uint32_t& IndexBuffer::GetID() const
{
	return m_ID;
}

///////////////////////////////////////////////////////////////////////////////////////////

RenderBuffer::RenderBuffer(uint32_t width, uint32_t height, GLenum format, bool multisample, int samples)
{
	glGenRenderbuffers(1, &m_ID);
	glBindRenderbuffer(GL_RENDERBUFFER, m_ID);
	
	if (multisample)
		glRenderbufferStorageMultisample(GL_RENDERBUFFER, samples, format, width, height);
	else
		glRenderbufferStorage(GL_RENDERBUFFER, format, width, height);

	glBindRenderbuffer(GL_RENDERBUFFER, 0);
}

RenderBuffer::~RenderBuffer()
{
	glDeleteRenderbuffers(1, &m_ID);
}

void RenderBuffer::BindBuffer() const
{
	glBindRenderbuffer(GL_RENDERBUFFER, m_ID);
}

void RenderBuffer::UnbindBuffer() const
{
	glBindRenderbuffer(GL_RENDERBUFFER, 0);
}

const uint32_t& RenderBuffer::GetID() const
{
	return m_ID;
}

///////////////////////////////////////////////////////////////////////////////////////////

TextureBuffer::TextureBuffer(uint32_t width, uint32_t height, GLenum internalFormat, GLenum format, GLenum type, bool cubemap, 
	bool multisample, int samples)
{
	if (cubemap)
		m_target = GL_TEXTURE_CUBE_MAP;
	else
	{
		if (multisample)
			m_target = GL_TEXTURE_2D_MULTISAMPLE;
		else
			m_target = GL_TEXTURE_2D;
	}

	glGenTextures(1, &m_ID);
	glBindTexture(m_target, m_ID);

	glTexParameteri(m_target, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(m_target, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(m_target, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(m_target, GL_TEXTURE_WRAP_T, GL_REPEAT);

	if (!cubemap)
	{
		if (multisample)
			glTexImage2DMultisample(m_target, samples, internalFormat, width, height, GL_TRUE);
		else
			glTexImage2D(m_target, 0, internalFormat, width, height, 0, format, type, nullptr);
	}
	else
	{
		constexpr uint32_t NUM_FACES = 6;
		glTexParameteri(m_target, GL_TEXTURE_WRAP_R, GL_REPEAT);
		
		for (uint32_t i = 0; i < NUM_FACES; i++)
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, internalFormat, width, height, 0, format, type,
				nullptr);
	}

	glBindTexture(m_target, 0);
}

TextureBuffer::~TextureBuffer()
{
	glDeleteTextures(1, &m_ID);
}

void TextureBuffer::SetWrapping(GLenum wrapX, GLenum wrapY, GLenum wrapZ) const
{
	glBindTexture(m_target, m_ID);
	glTexParameteri(m_target, GL_TEXTURE_WRAP_S, wrapX);
	glTexParameteri(m_target, GL_TEXTURE_WRAP_T, wrapY);

	if(m_target == GL_TEXTURE_CUBE_MAP)
		glTexParameteri(m_target, GL_TEXTURE_WRAP_R, wrapZ);

	glBindTexture(m_target, 0);
}

void TextureBuffer::SetFiltering(GLenum min, GLenum mag) const
{
	glBindTexture(m_target, m_ID);
	glTexParameteri(m_target, GL_TEXTURE_MIN_FILTER, min);
	glTexParameteri(m_target, GL_TEXTURE_MAG_FILTER, mag);
	glBindTexture(m_target, 0);
}

void TextureBuffer::SetBorderColor(const glm::vec4& color) const
{
	glBindTexture(m_target, m_ID);

	glTexParameteri(m_target, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(m_target, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	if(m_target == GL_TEXTURE_CUBE_MAP)
		glTexParameteri(m_target, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_BORDER);

	float borderColor[] = { color.r, color.g, color.b, color.a };
	glTexParameterfv(m_target, GL_TEXTURE_BORDER_COLOR, borderColor);

	glBindTexture(m_target, 0);
}

void TextureBuffer::BindBuffer(const std::string& samplerName, uint32_t samplerUnit) const
{
	ShaderManager::GetPtr()->GetBoundShader()->SetUniform(samplerName, (int)samplerUnit);

	glActiveTexture(GL_TEXTURE0 + samplerUnit);
	glBindTexture(m_target, m_ID);
}

void TextureBuffer::UnbindBuffer() const
{
	glBindTexture(m_target, 0);
}

const uint32_t& TextureBuffer::GetID() const
{
	return m_ID;
}

const GLenum& TextureBuffer::GetTarget() const
{
	return m_target;
}

///////////////////////////////////////////////////////////////////////////////////////////

FrameBuffer::FrameBuffer(bool noColorAttachments) :
	m_noColorAttachments(noColorAttachments)
{
	glGenFramebuffers(1, &m_ID);
}

FrameBuffer::~FrameBuffer()
{
	glDeleteFramebuffers(1, &m_ID);
}

void FrameBuffer::AttachTextureBuffer(const std::string& key, std::shared_ptr<TextureBuffer> colorAttachment, GLenum attachmentType)
{
	glBindFramebuffer(GL_FRAMEBUFFER, m_ID);
	
	if (colorAttachment->GetTarget() == GL_TEXTURE_CUBE_MAP)
	{
		glFramebufferTexture(GL_FRAMEBUFFER, attachmentType, colorAttachment->GetID(), 0);
	}
	else
	{
		glFramebufferTexture2D(GL_FRAMEBUFFER, attachmentType, colorAttachment->GetTarget(), colorAttachment->GetID(), 0);
	}

	if (m_noColorAttachments)
	{
		glDrawBuffer(GL_NONE);
		glReadBuffer(GL_NONE);
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	m_TBOAttachments[key] = colorAttachment;
}

void FrameBuffer::AttachRenderBuffer(std::shared_ptr<RenderBuffer> depthStencilRBO, GLenum attachmentType)
{
	glBindFramebuffer(GL_FRAMEBUFFER, m_ID);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, attachmentType, GL_RENDERBUFFER, depthStencilRBO->GetID());
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	m_RBOAttachment = depthStencilRBO;
}

void FrameBuffer::BindBuffer() const
{
	glBindFramebuffer(GL_FRAMEBUFFER, m_ID);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		OutputLog("The bound framebuffer (id: " + std::to_string(m_ID) + ") isn't complete!",
			Logging::Severity::FATAL);
}

void FrameBuffer::UnbindBuffer() const
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

const uint32_t& FrameBuffer::GetID() const
{
	return m_ID;
}

std::shared_ptr<TextureBuffer> FrameBuffer::GetColorBuffer(const std::string& key) const
{
	return m_TBOAttachments[key];
}

std::shared_ptr<RenderBuffer> FrameBuffer::GetRenderBuffer() const
{
	return m_RBOAttachment;
}

///////////////////////////////////////////////////////////////////////////////////////////

namespace Buffer
{
	std::shared_ptr<VertexBuffer> GenerateVBO(const void* data, GLsizeiptr size, GLenum usage)
	{
		return std::make_shared<VertexBuffer>(data, size, usage);
	}

	std::shared_ptr<IndexBuffer> GenerateIBO(const void* data, GLsizeiptr size, GLenum usage)
	{
		return std::make_shared<IndexBuffer>(data, size, usage);
	}

	std::shared_ptr<TextureBuffer> GenerateTBO(uint32_t width, uint32_t height, GLenum internalFormat, GLenum format, GLenum type,
		bool cubemap, bool multisample, int samples)
	{
		return std::make_shared<TextureBuffer>(width, height, internalFormat, format, type, cubemap, multisample, samples);
	}

	std::shared_ptr<RenderBuffer> GenerateRBO(uint32_t width, uint32_t height, GLenum format, bool multisample, int samples)
	{
		return std::make_shared<RenderBuffer>(width, height, format, multisample, samples);
	}

	std::shared_ptr<FrameBuffer> GenerateFBO(bool noColorAttachments)
	{
		return std::make_shared<FrameBuffer>(noColorAttachments);
	}
}

///////////////////////////////////////////////////////////////////////////////////////////