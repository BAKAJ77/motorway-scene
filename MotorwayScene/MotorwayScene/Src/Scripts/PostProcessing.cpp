#include "PostProcessing.h"
#include "Graphics/BufferObjects.h"
#include "Graphics/ObjectRenderer.h"
#include "Utils/ResourceManager.h"

namespace Config
{
	constexpr uint32_t WIDTH = 1600;
	constexpr uint32_t HEIGHT = 900;
	
	constexpr int MAX_SAMPLES = 4;
	constexpr float GAMMA = 2.2f;
}

PostProcess::PostProcess() 
{
	this->InitScript();
}

PostProcess::~PostProcess() {}

PostProcess* PostProcess::GetPtr()
{
	static PostProcess singleton;
	return &singleton;
}

void PostProcess::InitScript()
{
	Resource::LoadShader("PostProcessingMS", "Resources/Shaders/PostProcessing.glsl.vsh",
		"Resources/Shaders/PostProcessing.glsl.fsh");

	auto colorAttachment = Buffer::GenerateTBO(Config::WIDTH, Config::HEIGHT, GL_SRGB, GL_RGB, GL_UNSIGNED_BYTE, false, true,
		Config::MAX_SAMPLES);
	auto depthStenilAttachment = Buffer::GenerateRBO(Config::WIDTH, Config::HEIGHT, GL_DEPTH24_STENCIL8, true,
		Config::MAX_SAMPLES);

	m_FBO = Buffer::GenerateFBO();
	m_FBO->AttachTextureBuffer("PostProcessMS", colorAttachment, GL_COLOR_ATTACHMENT0);
	m_FBO->AttachRenderBuffer(depthStenilAttachment, GL_DEPTH_STENCIL_ATTACHMENT);
}

void PostProcess::RenderToFBO() const
{
	m_FBO->BindBuffer();
	glViewport(0, 0, 1600, 900);
}

void PostProcess::RenderPostProcess() const
{
	m_FBO->UnbindBuffer();

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	Resource::GetShader("PostProcessingMS")->BindShader();
	Resource::GetShader("PostProcessingMS")->SetUniform("gammaValue", Config::GAMMA);
	Resource::GetShader("PostProcessingMS")->SetUniform("numSamples", Config::MAX_SAMPLES);

	m_FBO->GetColorBuffer("PostProcessMS")->BindBuffer("sceneTexture", 0);
	ObjectRenderer::GetPtr()->RenderQuad();
}