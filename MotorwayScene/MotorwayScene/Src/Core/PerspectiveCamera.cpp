#include "PerspectiveCamera.h"
#include "WindowFrame.h"

#include <GLFW/glfw3.h>

////////////////////////////////////////////////////////////////////////////////////////////////////////////

namespace
{
	glm::vec3 front = { 0.0f, 0.0f, -1.0f };
	glm::vec3 up = { 0.0f, 1.0f, 0.0f };

	float prevX = 0.0f, prevY = 0.0f;
	float pitch = 0.0f, yaw = front.z * 90.0f;
	float sensitivity = 0.14f; // You can adjust this value to your liking

	bool cursorFocused = false;
}

void UpdateCameraRotation(GLFWwindow* window, double xPos, double yPos)
{
	if (!cursorFocused)
	{
		prevX = (float)xPos;
		prevY = (float)yPos;
		cursorFocused = true;
	}

	float offsetX = (float)xPos - prevX;
	float offsetY = prevY - (float)yPos;
	prevX = (float)xPos;
	prevY = (float)yPos;

	offsetX *= sensitivity;
	offsetY *= sensitivity;
	pitch += offsetY;
	yaw += offsetX;

	// Add constraints to pitch value
	if (pitch > 89.0f)
		pitch = 89.0f;
	else if (pitch < -89.0f)
		pitch = -89.0f;

	glm::vec3 newFront;
	newFront.x = cos(glm::radians(pitch)) * cos(glm::radians(yaw));
	newFront.y = sin(glm::radians(pitch));
	newFront.z = cos(glm::radians(pitch)) * sin(glm::radians(yaw));
	
	front = glm::normalize(newFront);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////

PerspectiveCamera::PerspectiveCamera() {}

PerspectiveCamera::PerspectiveCamera(std::shared_ptr<WindowFrame> window, const glm::vec3& pos, float fov) :
	CameraObject(window, pos, fov)
{
	glfwSetCursorPosCallback(window->GetPtr(), UpdateCameraRotation);
}

PerspectiveCamera::~PerspectiveCamera() {}

void PerspectiveCamera::UpdateTick()
{
	// Generate the view and projection matrices
	m_view = glm::lookAt(m_position, m_position + front, up);
	m_projection = glm::perspective(glm::radians(m_FOV), (float)m_window->GetWidth() / (float)m_window->GetHeight(),
		0.1f, 1000.0f);
}

const glm::vec3& PerspectiveCamera::GetFrontDir() const
{
	return front;
}

const float& PerspectiveCamera::GetEulerPitch() const
{
	return pitch;
}

const float& PerspectiveCamera::GetEulerYaw() const
{
	return yaw;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////

namespace Camera
{
	PerspectiveCamera GeneratePerspective(std::shared_ptr<WindowFrame> window, const glm::vec3& pos, float fov)
	{
		return PerspectiveCamera(window, pos, fov);
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////