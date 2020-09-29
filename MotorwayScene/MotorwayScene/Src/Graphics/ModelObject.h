#pragma once
#include <memory>
#include <string>
#include <vector>
#include <glm/glm.hpp>
#include <assimp/scene.h>

class VertexBuffer;
class IndexBuffer;
class VertexArray;
class TextureComponent;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////

enum class TextureType
{
	DIFFUSE,
	SPECULAR
};

struct Texture
{
	TextureType m_type;
	std::shared_ptr<TextureComponent> m_component;
};

struct VertexData
{
	glm::vec3 m_position;
	glm::vec3 m_normal;
	glm::vec2 m_textureCoord;
};

struct Material
{
	std::vector<Texture> m_textures;
	glm::vec3 m_ambient, m_diffuse, m_specular;

	float m_shininess;
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class Mesh
{
private:
	std::shared_ptr<VertexBuffer> m_meshVBO;
	std::shared_ptr<IndexBuffer> m_meshIBO;
	std::shared_ptr<VertexArray> m_meshVAO;

	std::shared_ptr<VertexBuffer> m_instancedVBO;
	size_t m_numInstances;
	
	Material m_material;
	uint32_t m_numIndices;
public:
	Mesh(const std::vector<VertexData>& vertices, const std::vector<uint32_t>& indices, Material material,
		const glm::mat4* instancedData = nullptr, size_t numInstances = 0); // The instancedData is supposed to be an array of model matrices
	~Mesh();

	void DrawMesh(const std::string& structUniform) const;
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class Model
{
private:
	std::vector<Mesh> m_meshes;
	const std::string m_path, m_textureDir;

	const float m_shininess;
private:
	void ProcessNode(aiNode* node, const aiScene* modelScene);

	Mesh GenerateMesh(aiMesh* mesh, const aiScene* modelScene) const;
	std::vector<Texture> GetMaterialTextures(aiMaterial* mat, aiTextureType textureType) const;
	Material GetGenericMaterial(aiMaterial* mat) const; // Returns material that doesn't include texture maps
public:
	// The texture directory string must end with a back/forward slash
	Model();
	Model(const std::string& path, const std::string& textureDir, float shininess = 64.0f,
		const glm::mat4* instancedData = nullptr, size_t numInstances = 0);
	~Model();

	void DrawModel(const std::string& structUniform) const;
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////