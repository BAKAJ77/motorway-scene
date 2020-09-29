#pragma once
#include "Core/PerspectiveCamera.h"

class Player
{
private:
	std::shared_ptr<WindowFrame> m_window;
	PerspectiveCamera m_camera;

	glm::vec3 m_velocity;

	float m_baseSpeed, m_currentSpeed;
	bool m_flyingEnabled;
private:
	void HandleMovement(const float& deltaTime);
	void HandleEvents();
public:
	Player();
	~Player();

	void InitScript(std::shared_ptr<WindowFrame> window, float speed);
	void UpdateTick(const float& deltaTime);
public:
	const PerspectiveCamera& GetCamera() const;
};