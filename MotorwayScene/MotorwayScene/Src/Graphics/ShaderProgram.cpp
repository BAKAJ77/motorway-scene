#include "ShaderPrograms.h"
#include "Utils/LoggingManager.h"
#include "Utils/ResourceManager.h"

#include <glad/glad.h>
#include <fstream>
#include <sstream>
#include <memory>

namespace Shader
{
	uint32_t currentBound = 0;
}

ShaderProgram::ShaderProgram(const std::string& vertexPath, const std::string& fragmentPath,
	const std::string& geometryPath) :
	m_vertexPath(vertexPath), m_fragmentPath(fragmentPath), m_geometryPath(geometryPath)
{
	bool gshIncluded = (geometryPath != "");

	// Load the contents of the shader files
	std::string vertexStr, fragmentStr, geometryStr;
	const char* vertexSrc, *fragmentSrc, *geometrySrc;

	vertexStr = this->LoadShaderFile(vertexPath);
	fragmentStr = this->LoadShaderFile(fragmentPath);
	if (gshIncluded)
		geometryStr = this->LoadShaderFile(geometryPath);

	vertexSrc = vertexStr.c_str();
	fragmentSrc = fragmentStr.c_str();
	if(gshIncluded)
		geometrySrc = geometryStr.c_str();

	// Next compile the shaders
	uint32_t vertexID = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexID, 1, &vertexSrc, nullptr);
	glCompileShader(vertexID);

	this->CheckProcessCompleted(vertexID, ProcessType::COMPILATION);

	uint32_t fragmentID = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentID, 1, &fragmentSrc, nullptr);
	glCompileShader(fragmentID);

	this->CheckProcessCompleted(fragmentID, ProcessType::COMPILATION);

	uint32_t geometryID = 0;
	if (gshIncluded)
	{
		geometryID = glCreateShader(GL_GEOMETRY_SHADER);
		glShaderSource(geometryID, 1, &geometrySrc, nullptr);
		glCompileShader(geometryID);

		this->CheckProcessCompleted(geometryID, ProcessType::COMPILATION);
	}

	// Lastly, the shaders are attached and linked to the shader program
	m_ID = glCreateProgram();

	glAttachShader(m_ID, vertexID);
	glAttachShader(m_ID, fragmentID);
	if (gshIncluded)
		glAttachShader(m_ID, geometryID);

	glLinkProgram(m_ID);
	this->CheckProcessCompleted(m_ID, ProcessType::LINKING);

	glDeleteShader(vertexID);
	glDeleteShader(fragmentID);
	if (gshIncluded)
		glDeleteShader(geometryID);
}

ShaderProgram::~ShaderProgram()
{
	glDeleteProgram(m_ID);
}

void ShaderProgram::CheckProcessCompleted(const uint32_t& id, ProcessType type) const
{
	int logLength = 0;

	switch (type)
	{
	case ProcessType::COMPILATION:
		glGetShaderiv(id, GL_INFO_LOG_LENGTH, &logLength);
		if (logLength > 0)
		{
			auto outputLog = std::unique_ptr<char>(new char[logLength]);
			glGetShaderInfoLog(id, logLength, nullptr, outputLog.get());
			OutputLog(outputLog.get(), Logging::Severity::FATAL);
		}
		break;
	case ProcessType::LINKING:
		glGetProgramiv(id, GL_INFO_LOG_LENGTH, &logLength);
		if (logLength > 0)
		{
			auto outputLog = std::unique_ptr<char>(new char[logLength]);
			glGetProgramInfoLog(id, logLength, nullptr, outputLog.get());
			OutputLog(outputLog.get(), Logging::Severity::FATAL);
		}
		break;
	}
}

std::string ShaderProgram::LoadShaderFile(const std::string& filePath) const
{
	std::ifstream shaderFile(filePath);
	if (shaderFile.fail())
		OutputLog("Failed to open shader file: " + filePath, Logging::Severity::FATAL);

	std::stringstream ss;
	ss << shaderFile.rdbuf();
	shaderFile.close();

	return ss.str();
}

uint32_t ShaderProgram::GetUniformLocation(const std::string& uniform) const
{
	auto cacheIterator = m_uniformCache.find(uniform);
	if (cacheIterator != m_uniformCache.end())
		return cacheIterator->second;

	// If the uniform location isn't in cache, then query from GPU and store it
	const uint32_t location = glGetUniformLocation(m_ID, uniform.c_str());
	m_uniformCache[uniform] = location;

	return location;
}

void ShaderProgram::BindShader() const
{
	glUseProgram(m_ID);
	Shader::currentBound = m_ID;
}

void ShaderProgram::UnbindShader() const
{
	glUseProgram(0);
	Shader::currentBound = 0;
}

void ShaderProgram::SetUniform(const std::string& uniform, const int& value) const
{
	const uint32_t location = this->GetUniformLocation(uniform);
	glUniform1i(location, value);
}

void ShaderProgram::SetUniform(const std::string& uniform, const bool& value) const
{
	const uint32_t location = this->GetUniformLocation(uniform);
	glUniform1i(location, (int)value);
}

void ShaderProgram::SetUniform(const std::string& uniform, const float& value) const
{
	const uint32_t location = this->GetUniformLocation(uniform);
	glUniform1f(location, value);
}

void ShaderProgram::SetUniform(const std::string& uniform, const glm::vec3& vector) const
{
	const uint32_t location = this->GetUniformLocation(uniform);
	glUniform3fv(location, 1, &vector[0]);
}

void ShaderProgram::SetUniform(const std::string& uniform, const glm::mat4& matrix) const
{
	const uint32_t location = this->GetUniformLocation(uniform);
	glUniformMatrix4fv(location, 1, GL_FALSE, &matrix[0][0]);
}