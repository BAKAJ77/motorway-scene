#include "ResourceManager.h"
#include <glad/glad.h>

////////////////////////////////////////////////////////////////////////////////////////////////////////

ShaderManager::ShaderManager() {}

ShaderManager::~ShaderManager() {}

ShaderManager* ShaderManager::GetPtr()
{
	static ShaderManager singleton;
	return &singleton;
}

void ShaderManager::LoadShader(const std::string& key, const std::string& vertexPath, 
	const std::string& fragmentPath, const std::string& geometryPath) const
{
	// Only load the shader if it hasn't been
	bool shaderLoaded = false;
	for (const auto shader : m_shaders)
	{
		if (shader.first == key || (shader.second->m_vertexPath == vertexPath &&
			shader.second->m_fragmentPath == fragmentPath && shader.second->m_geometryPath == geometryPath))
		{
			shaderLoaded = true;
			break;
		}
	}

	if (!shaderLoaded)
		m_shaders[key] = std::make_shared<ShaderProgram>(vertexPath, fragmentPath, geometryPath);
}

std::shared_ptr<ShaderProgram> ShaderManager::GetShader(const std::string& key) const
{
	return m_shaders[key];
}

std::shared_ptr<ShaderProgram> ShaderManager::GetBoundShader() const
{
	for (const auto& shader : m_shaders)
	{
		if (shader.second->m_ID == Shader::currentBound)
			return shader.second;
	}

	return nullptr;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////

TextureManager::TextureManager() {}

TextureManager::~TextureManager() {}

TextureManager* TextureManager::GetPtr()
{
	static TextureManager singleton;
	return &singleton;
}

void TextureManager::LoadTexture(const std::string& key, const std::string& path, bool srgb, bool flipVertical) const
{
	// Only load the texture if it hasn't been
	bool textureLoaded = false;
	for (const auto texture : m_textures)
	{
		if (texture.first == key || texture.second->GetPath() == path)
		{
			textureLoaded = true;
			break;
		}
	}

	if (!textureLoaded)
		m_textures[key] = std::make_shared<TextureComponent>(path, srgb, flipVertical);
}

std::shared_ptr<TextureComponent> TextureManager::GetTexture(const std::string& key)
{
	return m_textures[key];
}

////////////////////////////////////////////////////////////////////////////////////////////////////////

CubemapManager::CubemapManager() {}

CubemapManager::~CubemapManager() {}

CubemapManager* CubemapManager::GetPtr()
{
	static CubemapManager singleton;
	return &singleton;
}

void CubemapManager::LoadCubemap(const std::string& key, const std::array<std::string, 6>& paths) const
{
	// Only load the cubemap if it hasn't been
	bool cubemapLoaded = false;
	for (const auto cubemap : m_cubemaps)
	{
		if (cubemap.first == key)
		{
			cubemapLoaded = true;
			break;
		}
	}

	if (!cubemapLoaded)
		m_cubemaps[key] = std::make_shared<CubemapComponent>(paths);
}

std::shared_ptr<CubemapComponent> CubemapManager::GetCubemap(const std::string& key)
{
	return m_cubemaps[key];
}

////////////////////////////////////////////////////////////////////////////////////////////////////////

ModelManager::ModelManager() {}

ModelManager::~ModelManager() {}

ModelManager* ModelManager::GetPtr()
{
	static ModelManager singleton;
	return &singleton;
}

void ModelManager::LoadModel(const std::string& key, const std::string& modelPath, const std::string& textureDir, 
	float shininess, const glm::mat4* instancedData, size_t numInstances) const
{
	// Only load the model if it hasn't been
	bool modelLoaded = false;
	for (const auto model : m_models)
	{
		if (model.first == key)
		{
			modelLoaded = true;
			break;
		}
	}

	if (!modelLoaded)
		m_models.insert(std::pair<std::string, Model>(key, Model(modelPath, textureDir, shininess, instancedData, numInstances)));
}

const Model* ModelManager::GetModel(const std::string& key)
{
	return &m_models[key];
}

////////////////////////////////////////////////////////////////////////////////////////////////////////

namespace Resource
{
	void LoadShader(const std::string& key, const std::string& vertexPath, const std::string& fragmentPath,
		const std::string& geometryPath)
	{
		ShaderManager::GetPtr()->LoadShader(key, vertexPath, fragmentPath, geometryPath);
	}

	std::shared_ptr<ShaderProgram> GetShader(const std::string& key)
	{
		return ShaderManager::GetPtr()->GetShader(key);
	}

	std::shared_ptr<ShaderProgram> GetBoundShader()
	{
		return ShaderManager::GetPtr()->GetBoundShader();
	}

	void LoadTexture(const std::string& key, const std::string& path, bool srgb, bool flipVertical)
	{
		TextureManager::GetPtr()->LoadTexture(key, path, srgb, flipVertical);
	}

	std::shared_ptr<TextureComponent> GetTexture(const std::string& key)
	{
		return TextureManager::GetPtr()->GetTexture(key);
	}

	void LoadCubemap(const std::string& key, const std::array<std::string, 6>& paths)
	{
		CubemapManager::GetPtr()->LoadCubemap(key, paths);
	}

	std::shared_ptr<CubemapComponent> GetCubemap(const std::string& key)
	{
		return CubemapManager::GetPtr()->GetCubemap(key);
	}

	void LoadModel(const std::string& key, const std::string& modelPath, const std::string& textureDir, float shininess,
		const glm::mat4* instancedData, size_t numInstances)
	{
		return ModelManager::GetPtr()->LoadModel(key, modelPath, textureDir, shininess, instancedData, numInstances);
	}

	const Model* GetModel(const std::string& key)
	{
		return ModelManager::GetPtr()->GetModel(key);
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////