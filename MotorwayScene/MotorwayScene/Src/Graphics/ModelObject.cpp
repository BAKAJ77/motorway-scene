#include "ModelObject.h"
#include "Graphics/VertexArray.h"
#include "Graphics/TextureComponent.h"
#include "Utils/ResourceManager.h"
#include "Utils/LoggingManager.h"

#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>

namespace Instancing
{
	const glm::mat4* instancedData = nullptr;
	size_t numInstances = 0;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

Mesh::Mesh(const std::vector<VertexData>& vertices, const std::vector<uint32_t>& indices, Material material,
	const glm::mat4* instancedData, size_t numInstances) :
	m_material(material), m_numIndices(indices.size()), m_numInstances(numInstances)
{
	m_meshVBO = Buffer::GenerateVBO(&vertices[0], sizeof(VertexData) * vertices.size(), GL_STATIC_DRAW);
	m_meshIBO = Buffer::GenerateIBO(&indices[0], sizeof(uint32_t) * indices.size(), GL_STATIC_DRAW);

	m_meshVAO = Buffer::GenerateVAO();
	m_meshVAO->PushAttribLayout<float>(0, 3, sizeof(VertexData));
	m_meshVAO->PushAttribLayout<float>(1, 3, sizeof(VertexData), offsetof(VertexData, m_normal));
	m_meshVAO->PushAttribLayout<float>(2, 2, sizeof(VertexData), offsetof(VertexData, m_textureCoord));

	m_meshVAO->AttachBufferObjects(m_meshVBO, m_meshIBO);

	if (instancedData)
	{
		m_instancedVBO = Buffer::GenerateVBO(instancedData, numInstances * sizeof(glm::mat4), GL_STATIC_DRAW);

		m_meshVAO->PushAttribLayout<float>(3, 4, sizeof(glm::mat4), 0, 1);
		m_meshVAO->PushAttribLayout<float>(4, 4, sizeof(glm::mat4), sizeof(glm::vec4), 1);
		m_meshVAO->PushAttribLayout<float>(5, 4, sizeof(glm::mat4), 2 * sizeof(glm::vec4), 1);
		m_meshVAO->PushAttribLayout<float>(6, 4, sizeof(glm::mat4), 3 * sizeof(glm::vec4), 1);

		m_meshVAO->AttachBufferObjects(m_instancedVBO);
	}
}

Mesh::~Mesh() {}

void Mesh::DrawMesh(const std::string& structUniform) const
{
	const std::string UNIFORM_PREFIX = structUniform + ".";
	const auto& MESH_TEXTURES = m_material.m_textures;
	auto currentShader = ShaderManager::GetPtr()->GetBoundShader();

	currentShader->SetUniform(UNIFORM_PREFIX + "useTextures", !MESH_TEXTURES.empty());
	currentShader->SetUniform(UNIFORM_PREFIX + "renderingModel", true);
	currentShader->SetUniform(UNIFORM_PREFIX + "shininess", m_material.m_shininess);

	if (MESH_TEXTURES.empty())
	{
		currentShader->SetUniform(UNIFORM_PREFIX + "ambient", m_material.m_ambient);
		currentShader->SetUniform(UNIFORM_PREFIX + "diffuse", m_material.m_diffuse);
		currentShader->SetUniform(UNIFORM_PREFIX + "specular", m_material.m_specular);
	}
	else
	{
		bool usingSpecularMap = false;
		int numDiffuse = 0, numSpecular = 0;

		for (size_t i = 0; i < MESH_TEXTURES.size(); i++)
		{
			if (MESH_TEXTURES[i].m_type == TextureType::DIFFUSE)
				MESH_TEXTURES[i].m_component->BindTexture(UNIFORM_PREFIX + "diffuseTexture" + std::to_string(numDiffuse++), i);
			else if (MESH_TEXTURES[i].m_type == TextureType::SPECULAR)
			{
				usingSpecularMap = true;
				MESH_TEXTURES[i].m_component->BindTexture(UNIFORM_PREFIX + "specularTexture" + std::to_string(numSpecular++), i);
			}
		}

		if (!usingSpecularMap)
			currentShader->SetUniform(UNIFORM_PREFIX + "specular", m_material.m_specular);

		currentShader->SetUniform(UNIFORM_PREFIX + "useSpecularMap", usingSpecularMap);
	}

	m_meshVAO->BindVertexArray();
	if (m_numInstances > 0)
	{
		currentShader->SetUniform("usingInstancing", true);
		glDrawElementsInstanced(GL_TRIANGLES, m_numIndices, GL_UNSIGNED_INT, nullptr, m_numInstances);
	}
	else
		glDrawElements(GL_TRIANGLES, m_numIndices, GL_UNSIGNED_INT, nullptr);

	currentShader->SetUniform("usingInstancing", false);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

Model::Model() :
	m_path(""), m_textureDir(""), m_shininess(64.0f)
{}

Model::Model(const std::string& path, const std::string& textureDir, float shininess, const glm::mat4* instancedData, 
	size_t numInstances) :
	m_path(path), m_textureDir(textureDir), m_shininess(shininess)
{
	Instancing::instancedData = instancedData;
	Instancing::numInstances = numInstances;

	Assimp::Importer modelImporter;
	const aiScene* modelScene = modelImporter.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);
	if (!modelScene || modelScene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !modelScene->mRootNode)
		OutputLog("Failed to load model: " + path, Logging::Severity::FATAL);
	else
		this->ProcessNode(modelScene->mRootNode, modelScene);
}

Model::~Model() {}

void Model::ProcessNode(aiNode* node, const aiScene* modelScene)
{
	for (size_t i = 0; i < node->mNumMeshes; i++)
		m_meshes.emplace_back(this->GenerateMesh(modelScene->mMeshes[node->mMeshes[i]], modelScene));

	for (size_t i = 0; i < node->mNumChildren; i++)
		this->ProcessNode(node->mChildren[i], modelScene);
}

Mesh Model::GenerateMesh(aiMesh* mesh, const aiScene* modelScene) const
{
	std::vector<VertexData> vertices;
	std::vector<uint32_t> indices;
	Material material;

	// Load all the vertex data
	for (size_t i = 0; i < mesh->mNumVertices; i++)
	{
		VertexData vertex;

		auto& position = vertex.m_position;
		position.x = mesh->mVertices[i].x;
		position.y = mesh->mVertices[i].y;
		position.z = mesh->mVertices[i].z;

		auto& normal = vertex.m_normal;
		normal.x = mesh->mNormals[i].x;
		normal.y = mesh->mNormals[i].y;
		normal.z = mesh->mNormals[i].z;

		if (mesh->mTextureCoords[0])
		{
			auto& textureCoord = vertex.m_textureCoord;
			textureCoord.x = mesh->mTextureCoords[0][i].x;
			textureCoord.y = mesh->mTextureCoords[0][i].y;
		}
		else
			vertex.m_textureCoord = glm::vec2(0.0f);

		vertices.emplace_back(vertex);
	}

	// Load all the index data
	for (size_t i = 0; i < mesh->mNumFaces; i++)
	{
		aiFace face = mesh->mFaces[i];
		for (size_t j = 0; j < face.mNumIndices; j++)
			indices.emplace_back(face.mIndices[j]);
	}

	// Finally load the material
	if (mesh->mMaterialIndex >= 0)
	{
		auto diffuseMaps = this->GetMaterialTextures(modelScene->mMaterials[mesh->mMaterialIndex], aiTextureType_DIFFUSE);
		auto specularMaps = this->GetMaterialTextures(modelScene->mMaterials[mesh->mMaterialIndex], aiTextureType_SPECULAR);
		material.m_textures.insert(material.m_textures.end(), diffuseMaps.begin(), diffuseMaps.end());
		material.m_textures.insert(material.m_textures.end(), specularMaps.begin(), specularMaps.end());

		// Check if the mesh material has a diffuse map, if not then just use generic phong color vectors
		bool diffuseMapFound = false;
		for (auto& texture : material.m_textures)
		{
			if (texture.m_type == TextureType::DIFFUSE)
			{
				diffuseMapFound = true;
				break;
			}
		}

		if (!diffuseMapFound)
		{
			material = this->GetGenericMaterial(modelScene->mMaterials[mesh->mMaterialIndex]);
			material.m_textures.clear();
		}

		material.m_shininess = m_shininess;
	}

	return Mesh(vertices, indices, material, Instancing::instancedData, Instancing::numInstances);
}

std::vector<Texture> Model::GetMaterialTextures(aiMaterial* mat, aiTextureType textureType) const
{
	bool loadAsSRGB = (textureType == aiTextureType_DIFFUSE);

	std::vector<Texture> textures;
	for (size_t i = 0; i < mat->GetTextureCount(textureType); i++)
	{
		Texture textureMap;
		aiString path;

		switch (textureType)
		{
		case aiTextureType_DIFFUSE:
			textureMap.m_type = TextureType::DIFFUSE;
			break;
		case aiTextureType_SPECULAR:
			textureMap.m_type = TextureType::SPECULAR;
			break;
		}
		
		mat->GetTexture(textureType, i, &path);
		Resource::LoadTexture(path.C_Str(), m_textureDir + path.C_Str(), loadAsSRGB);
		textureMap.m_component = Resource::GetTexture(path.C_Str());

		textures.emplace_back(textureMap);
	}

	return textures;
}

Material Model::GetGenericMaterial(aiMaterial* mat) const
{
	Material material;
	aiColor3D colorRGB(0.0f, 0.0f, 0.0f);

	mat->Get(AI_MATKEY_COLOR_AMBIENT, colorRGB);
	material.m_ambient = glm::vec3(colorRGB.r, colorRGB.g, colorRGB.b);

	mat->Get(AI_MATKEY_COLOR_DIFFUSE, colorRGB);
	material.m_diffuse = glm::vec3(colorRGB.r, colorRGB.g, colorRGB.b);

	mat->Get(AI_MATKEY_COLOR_SPECULAR, colorRGB);
	material.m_specular = glm::vec3(colorRGB.r, colorRGB.g, colorRGB.b);

	return material;
}

void Model::DrawModel(const std::string& structUniform) const
{
	for (auto& mesh : m_meshes)
		mesh.DrawMesh(structUniform);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////