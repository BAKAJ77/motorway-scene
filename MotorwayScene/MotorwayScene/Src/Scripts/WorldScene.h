#pragma once
#include <memory>
#include <vector>
#include <glm/glm.hpp>

class Player;
class FrameBuffer;

class WorldScene
{
private:
	Player* m_player;
private:
	void GenerateShadowMap() const;
	void RenderScene() const;

	/*
		GenerateTrees() : Generates specified number of transformations for the trees within bounds given.
		[numGenerate] - The number of transformations to be generated
		[minBound] - The minimum bound for a translation to be (NOTE: The y component will be used as the Z coordinate)
		[maxBound] - The maximum bound for a translation to be (NOTE: The y component will be used as the Z coordinate)
	*/
	std::vector<glm::mat4> GenerateTrees(uint32_t numGenerate, const glm::vec2& minBound, const glm::vec2& maxBound,
		float spawnHeight = 1.0f) const; // This is for generating random trees, creating a forest
	std::vector<glm::mat4> GenerateAdjacentTranslations(float distance, float scale, float xValue, float yValue = 0.0f,
		float rotationAngle = 90.0f, float flippedAngle = -90.0f) const;
private:
	void DrawPavements() const;
	void DrawRoadBarriers() const;
	void DrawRoadLine() const;
	void DrawMainRoad() const;
	void DrawFloorPlane() const;
	void DrawStreetLamps() const;

	void DrawTrees() const;
	void DrawDistantSun() const; // Must be drawn last
public:
	WorldScene();
	~WorldScene();

	void InitScript(Player& m_player);

	void SetupShaders();
	void SetupTextures();
	void SetupModels();

	void UpdateTick(const float& deltaTime);
	void Render() const;
};