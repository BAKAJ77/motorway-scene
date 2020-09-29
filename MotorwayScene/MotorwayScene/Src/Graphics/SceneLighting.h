#pragma once
#include <glm/glm.hpp>
#include <string>
#include <memory>

class TextureComponent;

namespace Lighting
{
	void SetMaterial(const std::string& structUniform, std::shared_ptr<TextureComponent> diffuseTexture,
		std::shared_ptr<TextureComponent> specularTexture, float shininess,
		const glm::vec3& ambient = glm::vec3(0.0f), const glm::vec3& diffuse = glm::vec3(0.0f),
		const glm::vec3& specular = glm::vec3(0.0f));

	void SetDirLight(const std::string& structUniform, const glm::vec3& dir, const glm::vec3& ambient, 
		const glm::vec3& diffuse, const glm::vec3& specular);
}