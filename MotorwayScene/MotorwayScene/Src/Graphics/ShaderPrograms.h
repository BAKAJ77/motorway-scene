#pragma once
#include <glm/glm.hpp>
#include <unordered_map>

namespace Shader
{
	extern uint32_t currentBound;
}

enum class ProcessType
{
	COMPILATION,
	LINKING
};

class ShaderProgram
{
	friend class ShaderManager;
private:
	uint32_t m_ID;
	mutable std::unordered_map<std::string, uint32_t> m_uniformCache;

	const std::string m_vertexPath, m_fragmentPath, m_geometryPath;
private:
	void CheckProcessCompleted(const uint32_t& id, ProcessType type) const;

	std::string LoadShaderFile(const std::string& filePath) const;
	uint32_t GetUniformLocation(const std::string& uniform) const;
public:
	ShaderProgram(const std::string& vertexPath, const std::string& fragmentPath,
		const std::string& geometryPath = "");
	~ShaderProgram();

	void BindShader() const;
	void UnbindShader() const;
public:
	void SetUniform(const std::string& uniform, const int& value) const;
	void SetUniform(const std::string& uniform, const bool& value) const;
	void SetUniform(const std::string& uniform, const float& value) const;

	void SetUniform(const std::string& uniform, const glm::vec3& vector) const;
	void SetUniform(const std::string& uniform, const glm::mat4& matrix) const;
};