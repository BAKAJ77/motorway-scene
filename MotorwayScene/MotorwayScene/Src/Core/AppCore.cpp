#include "AppCore.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>

AppCore::AppCore() :
	m_window(Core::GenerateWindow("OpenGLScene 3D", 1600, 900))
{
	this->SetupScripts();
	this->MainLoop();
}

AppCore::~AppCore() {}

void AppCore::SetupScripts()
{
	m_playerUser.InitScript(m_window, 1.5f);
	m_worldScene.InitScript(m_playerUser);
}

void AppCore::MainLoop()
{
	float prevTime = 0.0f;
	while (!m_window->WasRequestedClose())
	{
		// Calculate the delta time
		const float currentTime = (float)glfwGetTime();
		const float deltaTime = currentTime - prevTime;
		prevTime = currentTime;

		this->UpdateTick(deltaTime);
		this->Render();
	}
}

void AppCore::UpdateTick(const float& deltaTime)
{
	if (m_window->WasKeyPressed(GLFW_KEY_ESCAPE))
		m_window->RequestClose();

	m_window->UpdateTick();

	m_playerUser.UpdateTick(deltaTime);
	m_worldScene.UpdateTick(deltaTime);
}

void AppCore::Render() const
{
	m_worldScene.Render();
}