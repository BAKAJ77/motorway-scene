#include "TextureComponent.h"
#include "Utils/ResourceManager.h"
#include "Utils/LoggingManager.h"

#include <glad/glad.h>
#include <stb_image.h>

TextureComponent::TextureComponent(const std::string& path, bool srgb, bool flipVertical) :
	m_path(path)
{
	glGenTextures(1, &m_ID);
	glBindTexture(GL_TEXTURE_2D, m_ID);

	this->SetupTextureConfig();

	stbi_set_flip_vertically_on_load(flipVertical);
	GLubyte* textureData = stbi_load(path.c_str(), &m_width, &m_height, &m_numChannels, 0);
	if (textureData)
	{
		if (m_numChannels > 3)
		{
			if (srgb)
				glTexImage2D(GL_TEXTURE_2D, 0, GL_SRGB_ALPHA, m_width, m_height, 0, GL_RGBA, GL_UNSIGNED_BYTE,
					textureData);
			else
				glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_width, m_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, textureData);
		}
		else if (m_numChannels == 3)
		{
			if (srgb)
				glTexImage2D(GL_TEXTURE_2D, 0, GL_SRGB, m_width, m_height, 0, GL_RGB, GL_UNSIGNED_BYTE, textureData);
			else
				glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, m_width, m_height, 0, GL_RGB, GL_UNSIGNED_BYTE, textureData);
		}
		else
		{
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, m_width, m_height, 0, GL_RED, GL_UNSIGNED_BYTE, textureData);
		}

		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
		OutputLog("Failed to load texture: " + path, Logging::Severity::FATAL);

	stbi_image_free(textureData);
	glBindTexture(GL_TEXTURE_2D, 0);
}

TextureComponent::~TextureComponent()
{
	glDeleteTextures(1, &m_ID);
}

void TextureComponent::SetupTextureConfig() const
{
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}

void TextureComponent::BindTexture(const std::string& samplerName, uint32_t samplerUnit) const
{
	ShaderManager::GetPtr()->GetBoundShader()->SetUniform(samplerName, (int)samplerUnit);

	glActiveTexture(GL_TEXTURE0 + samplerUnit);
	glBindTexture(GL_TEXTURE_2D, m_ID);
}

void TextureComponent::UnbindTexture() const
{
	glBindTexture(GL_TEXTURE_2D, 0);
}

const uint32_t& TextureComponent::GetID() const
{
	return m_ID;
}

const std::string& TextureComponent::GetPath() const
{
	return m_path;
}

const int& TextureComponent::GetWidth() const
{
	return m_width;
}

const int& TextureComponent::GetHeight() const
{
	return m_height;
}

const int& TextureComponent::GetNumChannels() const
{
	return m_numChannels;
}