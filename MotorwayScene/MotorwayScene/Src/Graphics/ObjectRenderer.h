#pragma once
#include <memory>
#include <string>
#include <glm/glm.hpp>

class VertexBuffer;
class VertexArray;

class ObjectRenderer
{
private:
	std::shared_ptr<VertexBuffer> m_quadVBO;
	std::shared_ptr<VertexBuffer> m_cubeVBO;

	std::shared_ptr<VertexArray> m_quadVAO;
	std::shared_ptr<VertexArray> m_cubeVAO;

	size_t m_quadBufferSize, m_cubeBufferSize;
private:
	ObjectRenderer();
	~ObjectRenderer();

	void InitQuadObject();
	void InitCubeObject();
public:
	static ObjectRenderer* GetPtr();

	void LoadModel(const std::string& key, const std::string& modelPath, const std::string& textureDir = "", 
		float shininess = 64.0f, const glm::mat4 * instancedData = nullptr, size_t numInstances = 0);

	void RenderQuad(int textureRepeatX = 1, int textureRepeatY = 1) const;
	void RenderCube(int textureRepeatX = 1, int textureRepeatY = 1, int textureRepeatZ = 1) const;
	void RenderModel(const std::string& key, const std::string& structUniform) const;
};