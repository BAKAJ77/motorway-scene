#include "CubemapComponent.h"
#include "Utils/ResourceManager.h"
#include "Utils/LoggingManager.h"

#include <glad/glad.h>
#include <stb_image.h>

CubemapComponent::CubemapComponent(const std::array<std::string, 6>& paths)
{
	glGenTextures(1, &m_ID);
	glBindTexture(GL_TEXTURE_CUBE_MAP, m_ID);

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	for (uint32_t index = 0; index < 6; index++)
	{
		int width = 0, height = 0, channels = 0;
		GLubyte* textureData = stbi_load(paths[index].c_str(), &width, &height, &channels, 0);
		if (textureData)
		{
			if (channels > 3)
				glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + index, 0, GL_SRGB_ALPHA, width, height, 0, GL_RGBA,
					GL_UNSIGNED_BYTE, textureData);
			else
				glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + index, 0, GL_SRGB, width, height, 0, GL_RGB,
					GL_UNSIGNED_BYTE, textureData);
		}
		else
		{
			OutputLog("Failed to load cubemap face: " + paths[index], Logging::Severity::FATAL);
		}
	}

	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
}

CubemapComponent::~CubemapComponent()
{
	glDeleteTextures(1, &m_ID);
}

void CubemapComponent::BindCubemap(const std::string& samplerName, uint32_t samplerUnit) const
{
	ShaderManager::GetPtr()->GetBoundShader()->SetUniform(samplerName, (int)samplerUnit);
	glBindTexture(GL_TEXTURE_CUBE_MAP, m_ID);
}