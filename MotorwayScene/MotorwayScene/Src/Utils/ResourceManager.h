#pragma once
#include "Graphics/ShaderPrograms.h"
#include "Graphics/TextureComponent.h"
#include "Graphics/CubemapComponent.h"
#include "Graphics/ModelObject.h"

#include <memory>

/////////////////////////////////////////////////////////////////////////////////////////////////////////////

class ShaderManager
{
	friend class ShaderProgram;
private:
	mutable std::unordered_map<std::string, std::shared_ptr<ShaderProgram>> m_shaders;
protected:
	ShaderManager();
	~ShaderManager();
public:
	static ShaderManager* GetPtr();

	void LoadShader(const std::string& key, const std::string& vertexPath, const std::string& fragmentPath, 
		const std::string& geometryPath = "") const;
	std::shared_ptr<ShaderProgram> GetShader(const std::string& key) const;
	std::shared_ptr<ShaderProgram> GetBoundShader() const; // Returns current bound shader
};

class TextureManager
{
private:
	mutable std::unordered_map<std::string, std::shared_ptr<TextureComponent>> m_textures;
protected:
	TextureManager();
	~TextureManager();
public:
	static TextureManager* GetPtr();

	void LoadTexture(const std::string& key, const std::string& path, bool srgb, bool flipVertical) const;
	std::shared_ptr<TextureComponent> GetTexture(const std::string& key);
};

class CubemapManager
{
private:
	mutable std::unordered_map<std::string, std::shared_ptr<CubemapComponent>> m_cubemaps;
protected:
	CubemapManager();
	~CubemapManager();
public:
	static CubemapManager* GetPtr();

	void LoadCubemap(const std::string& key, const std::array<std::string, 6>& paths) const;
	std::shared_ptr<CubemapComponent> GetCubemap(const std::string& key);
};

class ModelManager
{
private:
	mutable std::unordered_map<std::string, Model> m_models;
protected:
	ModelManager();
	~ModelManager();
public:
	static ModelManager* GetPtr();

	void LoadModel(const std::string& key, const std::string& modelPath, const std::string& textureDir, float shininess,
		const glm::mat4* instancedData, size_t numInstances) const;
	const Model* GetModel(const std::string& key);
};

/////////////////////////////////////////////////////////////////////////////////////////////////////////////

namespace Resource
{
	void LoadShader(const std::string& key, const std::string& vertexPath, const std::string& fragmentPath,
		const std::string& geometryPath = "");
	std::shared_ptr<ShaderProgram> GetShader(const std::string& key);
	std::shared_ptr<ShaderProgram> GetBoundShader();

	void LoadTexture(const std::string& key, const std::string& path, bool srgb = false, bool flipVertical = false);
	std::shared_ptr<TextureComponent> GetTexture(const std::string& key);

	void LoadCubemap(const std::string& key, const std::array<std::string, 6>& paths);
	std::shared_ptr<CubemapComponent> GetCubemap(const std::string& key);

	void LoadModel(const std::string& key, const std::string& modelPath, const std::string& textureDir = "", 
		float shininess = 64.0f, const glm::mat4* instancedData = nullptr, size_t numInstances = 0);
	const Model* GetModel(const std::string& key);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////