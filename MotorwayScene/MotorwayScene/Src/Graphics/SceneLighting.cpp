#include "SceneLighting.h"
#include "Utils/ResourceManager.h"

namespace Lighting
{
	void SetMaterial(const std::string& structUniform, std::shared_ptr<TextureComponent> diffuseTexture,
		std::shared_ptr<TextureComponent> specularTexture, float shininess, const glm::vec3& ambient,
		const glm::vec3& diffuse, const glm::vec3& specular)
	{
		auto currentShader = ShaderManager::GetPtr()->GetBoundShader();
		currentShader->SetUniform(structUniform + ".shininess", shininess);
		currentShader->SetUniform(structUniform + ".useTextures", (diffuseTexture != nullptr));

		currentShader->SetUniform(structUniform + ".renderingModel", false);

		if (diffuseTexture)
		{
			diffuseTexture->BindTexture(structUniform + ".diffuseTexture0", 0);

			if (specularTexture)
			{
				currentShader->SetUniform(structUniform + ".useSpecularMap", true);
				specularTexture->BindTexture(structUniform + ".specularTexture0", 1);
			}
			else
				currentShader->SetUniform(structUniform + ".useSpecularMap", false);
		}
		else
		{
			currentShader->SetUniform(structUniform + ".ambient", ambient);
			currentShader->SetUniform(structUniform + ".diffuse", diffuse);
			currentShader->SetUniform(structUniform + ".specular", specular);
		}
	}

	void SetDirLight(const std::string& structUniform, const glm::vec3& dir, const glm::vec3& ambient, 
		const glm::vec3& diffuse, const glm::vec3& specular)
	{
		auto currentShader = ShaderManager::GetPtr()->GetBoundShader();

		currentShader->SetUniform(structUniform + ".direction", dir);

		currentShader->SetUniform(structUniform + ".ambient", ambient);
		currentShader->SetUniform(structUniform + ".diffuse", diffuse);
		currentShader->SetUniform(structUniform + ".specular", specular);
	}
}