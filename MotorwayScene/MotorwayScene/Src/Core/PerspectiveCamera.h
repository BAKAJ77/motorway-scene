#pragma once
#include "CameraObject.h"

class WindowFrame;

class PerspectiveCamera : public CameraObject
{
public:
	PerspectiveCamera();
	PerspectiveCamera(std::shared_ptr<WindowFrame> window, const glm::vec3& pos, float fov);
	~PerspectiveCamera();

	void UpdateTick() override;
public:
	const glm::vec3& GetFrontDir() const;
	const float& GetEulerPitch() const;
	const float& GetEulerYaw() const;
};

namespace Camera
{
	PerspectiveCamera GeneratePerspective(std::shared_ptr<WindowFrame> window, const glm::vec3& pos, float fov);
}