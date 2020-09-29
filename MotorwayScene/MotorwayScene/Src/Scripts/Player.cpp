#include "Player.h"
#include "Core/WindowFrame.h"

#include <GLFW/glfw3.h>

Player::Player() :
	m_flyingEnabled(false), m_baseSpeed(1.0f), m_currentSpeed(0.0f)
{}

Player::~Player() {}

void Player::InitScript(std::shared_ptr<WindowFrame> window, float speed)
{
	m_window = window;
	m_baseSpeed = speed;
	m_camera = Camera::GeneratePerspective(window, glm::vec3(0.0f, 1.25f, 3.0f), 45.0f);
}

void Player::HandleMovement(const float& deltaTime)
{
	const float finalSpeed = m_currentSpeed * deltaTime;
	m_velocity = glm::vec3(0.0f);
	
	if (m_window->WasKeyPressed(GLFW_KEY_W)) // To move forwards
	{
		m_velocity.x = (m_camera.GetFrontDir().x / cos(glm::radians(m_camera.GetEulerPitch()))) * finalSpeed;
		m_velocity.z = (m_camera.GetFrontDir().z / cos(glm::radians(m_camera.GetEulerPitch()))) * finalSpeed;
	}
	else if (m_window->WasKeyPressed(GLFW_KEY_S)) // To move backwards
	{
		m_velocity.x = -(m_camera.GetFrontDir().x / cos(glm::radians(m_camera.GetEulerPitch()))) * finalSpeed;
		m_velocity.z = -(m_camera.GetFrontDir().z / cos(glm::radians(m_camera.GetEulerPitch()))) * finalSpeed;
	}

	if (m_window->WasKeyPressed(GLFW_KEY_A)) // To move left
	{
		m_velocity.x = -glm::normalize(glm::cross(m_camera.GetFrontDir(), glm::vec3(0.0f, 1.0f, 0.0f))).x * finalSpeed;
		m_velocity.z = -glm::normalize(glm::cross(m_camera.GetFrontDir(), glm::vec3(0.0f, 1.0f, 0.0f))).z * finalSpeed;
	}
	else if (m_window->WasKeyPressed(GLFW_KEY_D)) // To move right
	{
		m_velocity.x = glm::normalize(glm::cross(m_camera.GetFrontDir(), glm::vec3(0.0f, 1.0f, 0.0f))).x * finalSpeed;
		m_velocity.z = glm::normalize(glm::cross(m_camera.GetFrontDir(), glm::vec3(0.0f, 1.0f, 0.0f))).z * finalSpeed;
	}

	if (m_flyingEnabled)
	{
		if (m_window->WasKeyPressed(GLFW_KEY_SPACE)) // To move up
			m_velocity.y = finalSpeed;
		else if (m_window->WasKeyPressed(GLFW_KEY_LEFT_SHIFT)) // To move down
			m_velocity.y = -finalSpeed;
	}
}

void Player::HandleEvents()
{
	static float prevTime = 0.0f;
	float currentTime = (float)glfwGetTime();

	// Press "F" to toggle flying
	if (m_window->WasKeyPressed(GLFW_KEY_F) && (currentTime - prevTime) > 0.5f)
	{
		m_flyingEnabled = !m_flyingEnabled;
		prevTime = currentTime;
	}
	
	// Hold "Right Shift" while moving forward to run
	if (m_window->WasKeyPressed(GLFW_KEY_RIGHT_SHIFT) && m_window->WasKeyPressed(GLFW_KEY_W))
		m_currentSpeed = m_baseSpeed * 1.75f;
	else
		m_currentSpeed = m_baseSpeed;
}

void Player::UpdateTick(const float& deltaTime)
{
	this->HandleMovement(deltaTime);
	this->HandleEvents();

	m_camera.SetPosition(m_camera.GetPosition() + m_velocity);
	m_camera.UpdateTick();
}

const PerspectiveCamera& Player::GetCamera() const
{
	return m_camera;
}