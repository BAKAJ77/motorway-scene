#pragma once
#include <glm/gtc/matrix_transform.hpp>
#include <memory>

class WindowFrame;

class CameraObject
{
protected:
	glm::vec3 m_position;
	glm::mat4 m_view, m_projection;
	float m_FOV;

	std::shared_ptr<WindowFrame> m_window;
protected:
	CameraObject();
	CameraObject(std::shared_ptr<WindowFrame> window, const glm::vec3& pos, float fov);
	~CameraObject();
public:
	void SetPosition(const glm::vec3& pos);
	void SetFOV(const float& angle);

	virtual void UpdateTick() = 0;
public:
	const glm::vec3& GetPosition() const;
	const float& GetFOV() const;

	glm::mat4 GetMatrix() const; // Returns the product of the view and projection matrices
	const glm::mat4& GetView() const;
	const glm::mat4& GetProjection() const;
};