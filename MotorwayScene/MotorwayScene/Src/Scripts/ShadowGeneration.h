#pragma once
#include "Graphics/BufferObjects.h"
#include <memory>
#include <glm/glm.hpp>

class FrameBuffer;

class ShadowGeneration
{
private:
	std::shared_ptr<FrameBuffer> m_depthMapFBO;
	mutable glm::mat4 m_lightView, m_lightProjection;
private:
	ShadowGeneration();
	~ShadowGeneration();

	void InitScript();
public:
	static ShadowGeneration* GetPtr();

	void RenderDepthMap(const glm::vec3& lightDir, const glm::vec3& playerPos) const;
	void StopDepthMapRender() const;
public:
	const std::shared_ptr<TextureBuffer> GetDepthMap() const;
	const glm::mat4 GetLightMatrix() const;
};