#include "OrthogonalCamera.h"
#include "WindowFrame.h"

#include <GLFW/glfw3.h>

OrthogonalCamera::OrthogonalCamera() {}

OrthogonalCamera::OrthogonalCamera(std::shared_ptr<WindowFrame> window, const glm::vec3& pos, float fov) :
	CameraObject(window, pos, fov)
{}

OrthogonalCamera::~OrthogonalCamera() {}

void OrthogonalCamera::UpdateTick()
{
	// Generate the view and projection matrices
	m_view = glm::lookAt(m_position, m_position + glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	m_projection = glm::orthoLH(0.0f, (float)m_window->GetWidth(), 0.0f, (float)m_window->GetHeight(), 0.1f, 1000.0f);
}