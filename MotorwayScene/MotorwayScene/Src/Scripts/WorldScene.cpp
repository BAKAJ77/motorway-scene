#include "WorldScene.h"
#include "Player.h"
#include "PostProcessing.h"
#include "ShadowGeneration.h"

#include "Utils/ResourceManager.h"
#include "Graphics/SceneLighting.h"
#include "Graphics/ObjectRenderer.h"
#include "Utils/RandomGenerator.h"

#include <glad/glad.h>

namespace World
{
	const glm::vec3 SKY_COLOR = { 0.31f, 0.4f, 0.42f };
	const glm::vec3 LIGHT_RAY_DIR = { 2.0f, -1.0f, 2.0f };

	const float STREET_BOUND_MIN_X = -8.5f;
	const float STREET_BOUND_MAX_X = 8.5f;
}

WorldScene::WorldScene() :
	m_player(nullptr)
{}

WorldScene::~WorldScene() {}

void WorldScene::InitScript(Player& player)
{
	m_player = &player;

	this->SetupShaders();
	this->SetupTextures();
	this->SetupModels();
}

void WorldScene::SetupShaders()
{
	Resource::LoadShader("ObjectShaders", "Resources/Shaders/ObjectShader.glsl.vsh",
		"Resources/Shaders/ObjectShader.glsl.fsh");
}

void WorldScene::SetupTextures()
{
	Resource::LoadTexture("SnowDiffuse", "Resources/Textures/snow-diffuse.jpg", true);
	Resource::LoadTexture("SnowSpecular", "Resources/Textures/snow-specular.jpg", false);

	Resource::LoadTexture("AsphaltDiffuse", "Resources/Textures/asphalt-diffuse.jpg", true);
	Resource::LoadTexture("AsphaltSpecular", "Resources/Textures/asphalt-specular.jpg", false);

	Resource::LoadTexture("CobbleDiffuse", "Resources/Textures/cobblestone-diffuse.jpg", true);
}

void WorldScene::SetupModels() 
{
	const auto treeTransformations = this->GenerateTrees(20000, glm::vec2(-500, -500), glm::vec2(500, 500), 1.9f);
	ObjectRenderer::GetPtr()->LoadModel("Tree", "Resources/Models/LowPolyTree/lowpolytree.obj", "None", 64.0f,
		&treeTransformations[0], treeTransformations.size());

	const auto barrierTransformations = this->GenerateAdjacentTranslations(3.0f, 0.1f, 4.36f);
	ObjectRenderer::GetPtr()->LoadModel("CrashBarrier", "Resources/Models/CrashBarrier/crash-barrier.obj",
		"Resources/Textures/CrashBarrier/", 64.0f, &barrierTransformations[0], barrierTransformations.size());

	const auto lampTransformations = this->GenerateAdjacentTranslations(15.0f, 0.25f, 7.0f, 0.0f, 180.0f, 0.0f);
	ObjectRenderer::GetPtr()->LoadModel("StreetLamp", "Resources/Models/StreetLamp/street-lamp.obj", "", 128.0f,
		&lampTransformations[0], lampTransformations.size());

	ObjectRenderer::GetPtr()->LoadModel("DistantSun", "Resources/Models/DistantSun/sun.obj", "None");
}

void WorldScene::UpdateTick(const float& deltaTime) {}

void WorldScene::Render() const
{
	this->GenerateShadowMap();
	this->RenderScene();
}

void WorldScene::GenerateShadowMap() const
{
	ShadowGeneration::GetPtr()->RenderDepthMap(World::LIGHT_RAY_DIR, m_player->GetCamera().GetPosition());

	this->DrawFloorPlane();
	this->DrawMainRoad();
	this->DrawRoadLine();
	this->DrawRoadBarriers();
	this->DrawPavements();
	this->DrawStreetLamps();

	this->DrawTrees();

	ShadowGeneration::GetPtr()->StopDepthMapRender();
}

void WorldScene::RenderScene() const
{
	PostProcess::GetPtr()->RenderToFBO();

	glClearColor(World::SKY_COLOR.r, World::SKY_COLOR.g, World::SKY_COLOR.b, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	Resource::GetShader("ObjectShaders")->BindShader();
	Resource::GetShader("ObjectShaders")->SetUniform("cameraPos", m_player->GetCamera().GetPosition());
	Resource::GetShader("ObjectShaders")->SetUniform("skyColor", World::SKY_COLOR);

	Resource::GetShader("ObjectShaders")->SetUniform("vpMatrix", m_player->GetCamera().GetMatrix());
	Resource::GetShader("ObjectShaders")->SetUniform("lightMatrixVP", ShadowGeneration::GetPtr()->GetLightMatrix());
	ShadowGeneration::GetPtr()->GetDepthMap()->BindBuffer("depthMap", 7);

	Lighting::SetDirLight("dirLight", World::LIGHT_RAY_DIR, glm::vec3(0.025f), glm::vec3(0.15f),
		glm::vec3(0.75f));

	this->DrawFloorPlane();
	this->DrawMainRoad();
	this->DrawRoadLine();
	this->DrawRoadBarriers();
	this->DrawPavements();
	this->DrawStreetLamps();
	
	this->DrawTrees();
	this->DrawDistantSun();

	PostProcess::GetPtr()->RenderPostProcess();
}

void WorldScene::DrawDistantSun() const
{
	glm::vec3 sunPosition = m_player->GetCamera().GetPosition() - (12.0f * World::LIGHT_RAY_DIR);

	glm::mat4 model;
	model = glm::translate(model, sunPosition);
	model = glm::scale(model, glm::vec3(0.02f));

	Resource::GetBoundShader()->SetUniform("model", model);
	ObjectRenderer::GetPtr()->RenderModel("DistantSun", "mat");
}

void WorldScene::DrawStreetLamps() const 
{
	ObjectRenderer::GetPtr()->RenderModel("StreetLamp", "mat");
}

void WorldScene::DrawTrees() const
{
	ObjectRenderer::GetPtr()->RenderModel("Tree", "mat");
}

void WorldScene::DrawPavements() const
{
	constexpr float PAVEMENT_WIDTH = 2.0f;
	Lighting::SetMaterial("mat", Resource::GetTexture("CobbleDiffuse"), nullptr, 0.0f);

	// Left pavement
	glm::mat4 model;
	model = glm::translate(model, glm::vec3(-4.55 - (PAVEMENT_WIDTH / 2.0f), -0.1f, 0.0f));
	model = glm::scale(model, glm::vec3(PAVEMENT_WIDTH, 0.3f, 1000.0f));

	Resource::GetBoundShader()->SetUniform("model", model);
	ObjectRenderer::GetPtr()->RenderCube(2, 1, 1000);

	// Right pavement
	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(4.55 + (PAVEMENT_WIDTH / 2.0f), -0.1f, 0.0f));
	model = glm::scale(model, glm::vec3(PAVEMENT_WIDTH, 0.3f, 1000.0f));

	Resource::GetBoundShader()->SetUniform("model", model);
	ObjectRenderer::GetPtr()->RenderCube(2, 1, 1000);
}

void WorldScene::DrawRoadBarriers() const
{
	ObjectRenderer::GetPtr()->RenderModel("CrashBarrier", "mat");
}

void WorldScene::DrawRoadLine() const
{
	Lighting::SetMaterial("mat", nullptr, nullptr, 64.0f, glm::vec3(0.3f), glm::vec3(1.0f),
		glm::vec3(0.0f));

	glm::mat4 model;
	model = glm::translate(model, glm::vec3(0.0f, 0.025f, 0.0f));
	model = glm::scale(model, glm::vec3(0.1f, 1.0f, 1000.0f));
	model = glm::rotate(model, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));

	Resource::GetBoundShader()->SetUniform("model", model);
	ObjectRenderer::GetPtr()->RenderQuad();
}

void WorldScene::DrawMainRoad() const
{
	Lighting::SetMaterial("mat", Resource::GetTexture("AsphaltDiffuse"), Resource::GetTexture("AsphaltSpecular"),
		64.0f);

	// Left lane
	glm::mat4 model;
	model = glm::translate(model, glm::vec3(-2.3f, -0.1f, 0.0f));
	model = glm::scale(model, glm::vec3(4.5f, 0.25f, 1000.0f));

	Resource::GetBoundShader()->SetUniform("model", model);
	ObjectRenderer::GetPtr()->RenderCube(4, 1, 1000);

	// Right lane
	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(2.3f, -0.1f, 0.0f));
	model = glm::scale(model, glm::vec3(4.5f, 0.25f, 1000.0f));

	Resource::GetBoundShader()->SetUniform("model", model);
	ObjectRenderer::GetPtr()->RenderCube(4, 1, 1000);
}

void WorldScene::DrawFloorPlane() const
{
	Lighting::SetMaterial("mat", Resource::GetTexture("SnowDiffuse"), Resource::GetTexture("SnowSpecular"),
		64.0f);

	glm::mat4 model;
	model = glm::translate(model, glm::vec3(0.0f));
	model = glm::rotate(model, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	model = glm::scale(model, glm::vec3(1000.0f));

	Resource::GetBoundShader()->SetUniform("model", model);
	ObjectRenderer::GetPtr()->RenderQuad(1000, 1000);
}

std::vector<glm::mat4> WorldScene::GenerateTrees(uint32_t numGenerate, const glm::vec2& minBound,
	const glm::vec2& maxBound, float spawnHeight) const
{
	std::vector<glm::mat4> transformations;
	transformations.reserve(numGenerate);

	for (uint32_t i = 0; i < numGenerate; i++)
	{
		glm::vec3 position = glm::vec3(0.0f, spawnHeight, 0.0f);
		while (position.x > World::STREET_BOUND_MIN_X && position.x < World::STREET_BOUND_MAX_X)
		{
			position.x = Random::GenerateFloat(minBound.x, maxBound.x);
			position.z = Random::GenerateFloat(minBound.y, maxBound.y);
		}

		glm::mat4 model;
		model = glm::translate(model, position);
		model = glm::scale(model, glm::vec3(1.0f));

		transformations.emplace_back(model);
	}

	return transformations;
}

std::vector<glm::mat4> WorldScene::GenerateAdjacentTranslations(float distance, float scale, float xValue, float yValue,
	float rotationAngle, float flippedAngle) const
{
	std::vector<glm::mat4> transformations;

	// Generate the left side barriers
	for (float z = -500.0f; z <= 500.0f; z += distance)
	{
		// Generate the left-side barriers
		glm::mat4 model;
		model = glm::translate(model, glm::vec3(-xValue, yValue, z));
		model = glm::scale(model, glm::vec3(scale));
		model = glm::rotate(model, glm::radians(rotationAngle), glm::vec3(0.0f, 1.0f, 0.0f));

		transformations.emplace_back(model);

		// Generate the right-side barriers
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(xValue, yValue, z));
		model = glm::scale(model, glm::vec3(scale));
		model = glm::rotate(model, glm::radians(flippedAngle), glm::vec3(0.0f, 1.0f, 0.0f));

		transformations.emplace_back(model);
	}

	return transformations;
}