#include "ObjectRenderer.h"
#include "Graphics/VertexArray.h"
#include "Utils/ResourceManager.h"

ObjectRenderer::ObjectRenderer() :
	m_quadBufferSize(0), m_cubeBufferSize(0)
{
	this->InitQuadObject();
	this->InitCubeObject();
}

ObjectRenderer::~ObjectRenderer() {}

void ObjectRenderer::InitQuadObject()
{
	float vertexPosData[]
	{
		0.5f, -0.5f, 0.0f,
	   -0.5f, -0.5f, 0.0f,
		0.5f,  0.5f, 0.0f,
	   -0.5f,  0.5f, 0.0f
	};

	float normalData[]
	{
		0.0f, 0.0f, -1.0f,
		0.0f, 0.0f, -1.0f,
		0.0f, 0.0f, -1.0f,
		0.0f, 0.0f, -1.0f
	};

	const size_t TEXTURE_COORD_SIZE = (8 * sizeof(float));

	m_quadBufferSize = sizeof(vertexPosData) + sizeof(normalData) + TEXTURE_COORD_SIZE;
	m_quadVBO = Buffer::GenerateVBO(vertexPosData, m_quadBufferSize, GL_STATIC_DRAW);

	m_quadVBO->ModifySubData(normalData, sizeof(vertexPosData), sizeof(normalData));

	m_quadVAO = Buffer::GenerateVAO();
	m_quadVAO->PushAttribLayout<float>(0, 3, 3 * sizeof(float));
	m_quadVAO->PushAttribLayout<float>(1, 3, 3 * sizeof(float), sizeof(vertexPosData));
	m_quadVAO->PushAttribLayout<float>(2, 2, 2 * sizeof(float), m_quadBufferSize - TEXTURE_COORD_SIZE);

	m_quadVAO->AttachBufferObjects(m_quadVBO);
}

void ObjectRenderer::InitCubeObject()
{
	float vertexPosData[]
	{
		// Front face
		-0.5f, -0.5f,  0.5f,
		-0.5f,  0.5f,  0.5f,
		 0.5f,  0.5f,  0.5f,
		 0.5f,  0.5f,  0.5f,
		-0.5f, -0.5f,  0.5f,
		 0.5f, -0.5f,  0.5f,

		 // Back face
		 -0.5f, -0.5f, -0.5f,
		 -0.5f,  0.5f, -0.5f,
		  0.5f,  0.5f, -0.5f,
		  0.5f,  0.5f, -0.5f,
		 -0.5f, -0.5f, -0.5f,
		  0.5f, -0.5f, -0.5f,

		  // Left face
		  -0.5f, -0.5f,  0.5f,
		  -0.5f, -0.5f, -0.5f,
		  -0.5f,  0.5f, -0.5f,
		  -0.5f,  0.5f, -0.5f,
		  -0.5f, -0.5f,  0.5f,
		  -0.5f,  0.5f,  0.5f,

		  // Right face
		   0.5f, -0.5f,  0.5f,
		   0.5f, -0.5f, -0.5f,
		   0.5f,  0.5f, -0.5f,
		   0.5f,  0.5f, -0.5f,
		   0.5f, -0.5f,  0.5f,
		   0.5f,  0.5f,  0.5f,

		   // Top face
		   -0.5f,  0.5f,  0.5f,
		   -0.5f,  0.5f, -0.5f,
			0.5f,  0.5f,  0.5f,
			0.5f,  0.5f,  0.5f,
		   -0.5f,  0.5f, -0.5f,
			0.5f,  0.5f, -0.5f,

			// Bottom face
			-0.5f, -0.5f,  0.5f,
			-0.5f, -0.5f, -0.5f,
			 0.5f, -0.5f,  0.5f,
			 0.5f, -0.5f,  0.5f,
			-0.5f, -0.5f, -0.5f,
			 0.5f, -0.5f, -0.5f,
	};

	float normalData[]
	{
		// Front face
		0.0f, 0.0f, 1.0f,
		0.0f, 0.0f, 1.0f,
		0.0f, 0.0f, 1.0f,
		0.0f, 0.0f, 1.0f,
		0.0f, 0.0f, 1.0f,
		0.0f, 0.0f, 1.0f,

		// Back face
		0.0f, 0.0f, -1.0f,
		0.0f, 0.0f, -1.0f,
		0.0f, 0.0f, -1.0f,
		0.0f, 0.0f, -1.0f,
		0.0f, 0.0f, -1.0f,
		0.0f, 0.0f, -1.0f,

		// Left face
		-1.0f, 0.0f, 0.0f,
		-1.0f, 0.0f, 0.0f,
		-1.0f, 0.0f, 0.0f,
		-1.0f, 0.0f, 0.0f,
		-1.0f, 0.0f, 0.0f,
		-1.0f, 0.0f, 0.0f,

		// Right face
		1.0f, 0.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		1.0f, 0.0f, 0.0f,

		// Top face
		0.0f, 1.0f, 0.0f,
		0.0f, 1.0f, 0.0f,
		0.0f, 1.0f, 0.0f,
		0.0f, 1.0f, 0.0f,
		0.0f, 1.0f, 0.0f,
		0.0f, 1.0f, 0.0f,

		// Bottom face
		0.0f, -1.0f, 0.0f,
		0.0f, -1.0f, 0.0f,
		0.0f, -1.0f, 0.0f,
		0.0f, -1.0f, 0.0f,
		0.0f, -1.0f, 0.0f,
		0.0f, -1.0f, 0.0f
	};

	const size_t TEXTURE_COORD_SIZE = (72 * sizeof(float));

	m_cubeBufferSize = sizeof(vertexPosData) + sizeof(normalData) + TEXTURE_COORD_SIZE;
	m_cubeVBO = Buffer::GenerateVBO(vertexPosData, m_cubeBufferSize, GL_STATIC_DRAW);

	m_cubeVBO->ModifySubData(normalData, sizeof(vertexPosData), sizeof(normalData));

	m_cubeVAO = Buffer::GenerateVAO();
	m_cubeVAO->PushAttribLayout<float>(0, 3, 3 * sizeof(float));
	m_cubeVAO->PushAttribLayout<float>(1, 3, 3 * sizeof(float), sizeof(vertexPosData));
	m_cubeVAO->PushAttribLayout<float>(2, 2, 2 * sizeof(float), m_cubeBufferSize - TEXTURE_COORD_SIZE);

	m_cubeVAO->AttachBufferObjects(m_cubeVBO);
}

ObjectRenderer* ObjectRenderer::GetPtr()
{
	static ObjectRenderer renderer;
	return &renderer;
}

void ObjectRenderer::LoadModel(const std::string& key, const std::string& modelPath, const std::string& textureDir,
	float shininess, const glm::mat4* instancedData, size_t numInstances)
{
	Resource::LoadModel(key, modelPath, textureDir, shininess, instancedData, numInstances);
}

void ObjectRenderer::RenderQuad(int textureRepeatX, int textureRepeatY) const
{
	if (textureRepeatX > 1 || textureRepeatY > 1)
	{
		float textureCoordData[]
		{
			1.0f * textureRepeatX, 0.0f,
			0.0f, 0.0f,
			1.0f * textureRepeatX, 1.0f * textureRepeatY,
			0.0f, 1.0f * textureRepeatY
		};

		m_quadVBO->ModifySubData(textureCoordData, m_quadBufferSize - sizeof(textureCoordData), sizeof(textureCoordData));
	}
	else
	{
		// Reset the texture coord data for future use
		float textureCoordData[]
		{
			1.0f, 0.0f,
			0.0f, 0.0f,
			1.0f, 1.0f,
			0.0f, 1.0f
		};

		m_quadVBO->ModifySubData(textureCoordData, m_quadBufferSize - sizeof(textureCoordData), sizeof(textureCoordData));
	}

	m_quadVAO->BindVertexArray();
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
}

void ObjectRenderer::RenderCube(int textureRepeatX, int textureRepeatY, int textureRepeatZ) const
{
	if (textureRepeatX > 1 || textureRepeatY > 1 || textureRepeatZ > 1)
	{
		float textureCoordData[]
		{
			// Front face
			0.0f, 0.0f,
			0.0f, 1.0f * textureRepeatY,
			1.0f * textureRepeatX, 1.0f * textureRepeatY,
			1.0f * textureRepeatX, 1.0f * textureRepeatY,
			0.0f, 0.0f,
			1.0f * textureRepeatX, 0.0f,

			// Back face
			0.0f, 0.0f,
			0.0f, 1.0f * textureRepeatY,
			1.0f * textureRepeatX, 1.0f * textureRepeatY,
			1.0f * textureRepeatX, 1.0f * textureRepeatY,
			0.0f, 0.0f,
			1.0f * textureRepeatX, 0.0f,

			// Left face
			0.0f, 1.0f * textureRepeatY,
			0.0f, 0.0f,
			1.0f * textureRepeatZ, 0.0f,
			1.0f * textureRepeatZ, 0.0f,
			0.0f, 1.0f * textureRepeatY,
			1.0f * textureRepeatZ, 1.0f * textureRepeatY,

			// Right face
			0.0f, 1.0f * textureRepeatY,
			0.0f, 0.0f,
			1.0f * textureRepeatZ, 0.0f,
			1.0f * textureRepeatZ, 0.0f,
			0.0f, 1.0f * textureRepeatY,
			1.0f * textureRepeatZ, 1.0f * textureRepeatY,

			// Top face
			0.0f, 1.0f * textureRepeatZ,
			0.0f, 0.0f,
			1.0f * textureRepeatX, 1.0f * textureRepeatZ,
			1.0f * textureRepeatX, 1.0f * textureRepeatZ,
			0.0f, 0.0f,
			1.0f * textureRepeatX, 0.0f,

			// Bottom face
			0.0f, 1.0f * textureRepeatZ,
			0.0f, 0.0f,
			1.0f * textureRepeatX, 1.0f * textureRepeatZ,
			1.0f * textureRepeatX, 1.0f * textureRepeatZ,
			0.0f, 0.0f,
			1.0f * textureRepeatX, 0.0f
		};

		m_cubeVBO->ModifySubData(textureCoordData, m_cubeBufferSize - sizeof(textureCoordData), sizeof(textureCoordData));
	}
	else
	{
		// Reset the texture coord data for future use
		float textureCoordData[]
		{
			// Front face
			0.0f, 0.0f,
			0.0f, 1.0f,
			1.0f, 1.0f,
			1.0f, 1.0f,
			0.0f, 0.0f,
			1.0f, 0.0f,

			// Back face
			0.0f, 0.0f,
			0.0f, 1.0f,
			1.0f, 1.0f,
			1.0f, 1.0f,
			0.0f, 0.0f,
			1.0f, 0.0f,

			// Left face
			0.0f, 1.0f,
			0.0f, 0.0f,
			1.0f, 0.0f,
			1.0f, 0.0f,
			0.0f, 1.0f,
			1.0f, 1.0f,

			// Right face
			0.0f, 1.0f,
			0.0f, 0.0f,
			1.0f, 0.0f,
			1.0f, 0.0f,
			0.0f, 1.0f,
			1.0f, 1.0f,

			// Top face
			0.0f, 1.0f,
			0.0f, 0.0f,
			1.0f, 1.0f,
			1.0f, 1.0f,
			0.0f, 0.0f,
			1.0f, 0.0f,

			// Bottom face
			0.0f, 1.0f,
			0.0f, 0.0f,
			1.0f, 1.0f,
			1.0f, 1.0f,
			0.0f, 0.0f,
			1.0f, 0.0f
		};

		m_cubeVBO->ModifySubData(textureCoordData, m_cubeBufferSize - sizeof(textureCoordData), sizeof(textureCoordData));
	}

	m_cubeVAO->BindVertexArray();
	glDrawArrays(GL_TRIANGLES, 0, 36);
}

void ObjectRenderer::RenderModel(const std::string& key, const std::string& structUniform) const
{
	Resource::GetModel(key)->DrawModel(structUniform);
}