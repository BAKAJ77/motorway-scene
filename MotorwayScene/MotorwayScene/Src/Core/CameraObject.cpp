#include "CameraObject.h"

CameraObject::CameraObject() :
	m_FOV(45.0f), m_window(nullptr)
{}

CameraObject::CameraObject(std::shared_ptr<WindowFrame> window, const glm::vec3& pos, float fov) :
	m_window(window), m_position(pos), m_FOV(fov)
{}

CameraObject::~CameraObject() {}

void CameraObject::SetPosition(const glm::vec3& pos)
{
	m_position = pos;
}

void CameraObject::SetFOV(const float& angle)
{
	m_FOV = angle;
}

const glm::vec3& CameraObject::GetPosition() const
{
	return m_position;
}

const float& CameraObject::GetFOV() const
{
	return m_FOV;
}

glm::mat4 CameraObject::GetMatrix() const
{
	return m_projection * m_view;
}

const glm::mat4& CameraObject::GetView() const
{
	return m_view;
}

const glm::mat4& CameraObject::GetProjection() const
{
	return m_projection;
}