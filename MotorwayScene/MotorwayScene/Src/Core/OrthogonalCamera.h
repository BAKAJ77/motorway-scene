#pragma once
#include "CameraObject.h"

class OrthogonalCamera : public CameraObject
{
public:
	OrthogonalCamera();
	OrthogonalCamera(std::shared_ptr<WindowFrame> window, const glm::vec3& pos, float fov);
	~OrthogonalCamera();

	void UpdateTick() override;
};