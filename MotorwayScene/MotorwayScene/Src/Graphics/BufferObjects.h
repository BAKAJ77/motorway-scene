#pragma once
#include <glad/glad.h>
#include <unordered_map>
#include <glm/glm.hpp>
#include <memory>

typedef unsigned int uint32_t;

class VertexBuffer
{
private:
	uint32_t m_ID;
public:
	VertexBuffer(const void* data, GLsizeiptr size, GLenum usage);
	~VertexBuffer();

	void ModifySubData(const void* data, GLintptr offset, GLsizeiptr size);

	void BindBuffer() const;
	void UnbindBuffer() const;
public:
	const uint32_t& GetID() const;
};

class IndexBuffer
{
private:
	uint32_t m_ID;
public:
	IndexBuffer(const void* data, GLsizeiptr size, GLenum usage);
	~IndexBuffer();

	void ModifySubData(const void* data, GLintptr offset, GLsizeiptr size);

	void BindBuffer() const;
	void UnbindBuffer() const;
public:
	const uint32_t& GetID() const;
};

class RenderBuffer
{
private:
	uint32_t m_ID;
public:
	RenderBuffer(uint32_t width, uint32_t height, GLenum format, bool multisample, int samples);
	~RenderBuffer();

	void BindBuffer() const;
	void UnbindBuffer() const;
public:
	const uint32_t& GetID() const;
};

class TextureBuffer
{
private:
	uint32_t m_ID;
	GLenum m_target;
public:
	TextureBuffer(uint32_t width, uint32_t height, GLenum internalFormat, GLenum format, GLenum type, bool cubemap, 
		bool multisample, int samples);
	~TextureBuffer();

	void SetWrapping(GLenum wrapX, GLenum wrapY, GLenum wrapZ = GL_REPEAT) const;
	void SetFiltering(GLenum min, GLenum mag) const;
	void SetBorderColor(const glm::vec4& color) const;

	void BindBuffer(const std::string& samplerName, uint32_t samplerUnit) const;
	void UnbindBuffer() const;
public:
	const uint32_t& GetID() const;
	const GLenum& GetTarget() const;
};

class FrameBuffer
{
private:
	uint32_t m_ID;
	bool m_noColorAttachments;

	mutable std::unordered_map<std::string, std::shared_ptr<TextureBuffer>> m_TBOAttachments;
	mutable std::shared_ptr<RenderBuffer> m_RBOAttachment;
public:
	FrameBuffer(bool noColorAttachments);
	~FrameBuffer();

	void AttachTextureBuffer(const std::string& key, std::shared_ptr<TextureBuffer> colorAttachment, GLenum attachmentType);
	void AttachRenderBuffer(std::shared_ptr<RenderBuffer> depthStencilRBO, GLenum attachmentType);

	void BindBuffer() const;
	void UnbindBuffer() const;
public:
	const uint32_t& GetID() const;

	std::shared_ptr<TextureBuffer> GetColorBuffer(const std::string& key) const;
	std::shared_ptr<RenderBuffer> GetRenderBuffer() const;
};

namespace Buffer
{
	std::shared_ptr<VertexBuffer> GenerateVBO(const void* data, GLsizeiptr size, GLenum usage);
	std::shared_ptr<IndexBuffer> GenerateIBO(const void* data, GLsizeiptr size, GLenum usage);

	std::shared_ptr<TextureBuffer> GenerateTBO(uint32_t width, uint32_t height, GLenum internalFormat, GLenum format, 
		GLenum type = GL_UNSIGNED_BYTE, bool cubemap = false, bool multisample = false, int samples = 2);
	std::shared_ptr<RenderBuffer> GenerateRBO(uint32_t width, uint32_t height, GLenum format, bool multisample = false,
		int samples = 2);
	std::shared_ptr<FrameBuffer> GenerateFBO(bool noColorAttachments = false);
}