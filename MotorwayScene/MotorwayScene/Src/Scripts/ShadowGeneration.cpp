#include "ShadowGeneration.h"
#include "Utils/ResourceManager.h"

#include <glm/gtc/matrix_transform.hpp>

namespace 
{
	const uint32_t SHADOW_RESOLUTION = 2160;

	const float SHADOW_MAP_MAX_LENGTH = 25.0f;
	const float SHADOW_MAP_NEAR_LIMIT = -55.0f;
	const float SHADOW_MAP_FAR_LIMIT = 50.0f;
}

ShadowGeneration::ShadowGeneration()
{
	this->InitScript();
}

ShadowGeneration::~ShadowGeneration() {}

void ShadowGeneration::InitScript()
{
	Resource::LoadShader("DepthMapping", "Resources/Shaders/DepthMapping.glsl.vsh", "Resources/Shaders/DepthMapping.glsl.fsh");

	auto depthMap = Buffer::GenerateTBO(SHADOW_RESOLUTION, SHADOW_RESOLUTION, GL_DEPTH_COMPONENT, GL_DEPTH_COMPONENT, GL_FLOAT);
	depthMap->SetFiltering(GL_NEAREST, GL_NEAREST);
	depthMap->SetBorderColor(glm::vec4(1.0f));

	m_depthMapFBO = Buffer::GenerateFBO(true);
	m_depthMapFBO->AttachTextureBuffer("DepthMap", depthMap, GL_DEPTH_ATTACHMENT);
}

ShadowGeneration* ShadowGeneration::GetPtr()
{
	static ShadowGeneration singleton;
	return &singleton;
}

void ShadowGeneration::RenderDepthMap(const glm::vec3& lightDir, const glm::vec3& playerPos) const
{
	m_depthMapFBO->BindBuffer();

	glViewport(0, 0, SHADOW_RESOLUTION, SHADOW_RESOLUTION);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_DEPTH_BUFFER_BIT);

	m_lightView = glm::lookAt(playerPos - lightDir, glm::vec3(playerPos), glm::vec3(0.0f, 1.0f, 0.0f));
	m_lightProjection = glm::ortho(-SHADOW_MAP_MAX_LENGTH, SHADOW_MAP_MAX_LENGTH, -SHADOW_MAP_MAX_LENGTH, SHADOW_MAP_MAX_LENGTH, 
		SHADOW_MAP_NEAR_LIMIT, SHADOW_MAP_FAR_LIMIT);

	Resource::GetShader("DepthMapping")->BindShader();
	Resource::GetBoundShader()->SetUniform("lightMatrixVP", m_lightProjection * m_lightView);
}

void ShadowGeneration::StopDepthMapRender() const
{
	m_depthMapFBO->UnbindBuffer();
}

const std::shared_ptr<TextureBuffer> ShadowGeneration::GetDepthMap() const
{
	return m_depthMapFBO->GetColorBuffer("DepthMap");
}

const glm::mat4 ShadowGeneration::GetLightMatrix() const
{
	return m_lightProjection * m_lightView;
}