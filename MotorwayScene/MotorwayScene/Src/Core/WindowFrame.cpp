#include "WindowFrame.h"
#include "Utils/LoggingManager.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

WindowFrame::WindowFrame(const char* title, uint32_t width, uint32_t height) :
	m_width(width), m_height(height)
{
	this->InitGLFW();

	m_window = glfwCreateWindow(width, height, title, nullptr, nullptr);
	if (!m_window)
		OutputLog("Failed to create the window!", Logging::Severity::FATAL);

	glfwMakeContextCurrent(m_window);

	// Set the position of window to center of screen
	const GLFWvidmode* videoMode = glfwGetVideoMode(glfwGetPrimaryMonitor());
	const int& monitorWidth = videoMode->width;
	const int& monitorHeight = videoMode->height;

	glfwSetWindowPos(m_window, (monitorWidth / 2) - (width / 2), (monitorHeight / 2) - (height / 2));

	// Initialize GLAD library
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
		OutputLog("Failed to initialize GLAD!", Logging::Severity::FATAL);

	// Finish up setup of window
	glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glEnable(GL_DEPTH_TEST);
}

WindowFrame::~WindowFrame()
{
	glfwTerminate();
}

void WindowFrame::InitGLFW() const
{
	if (glfwInit() < 0)
		OutputLog("Failed to initialize GLFW!", Logging::Severity::FATAL);

	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, false);
}

void WindowFrame::RequestClose() const
{
	glfwSetWindowShouldClose(m_window, true);
}

void WindowFrame::UpdateTick() const
{
	glfwPollEvents();
	glfwSwapBuffers(m_window);
}

GLFWwindow* WindowFrame::GetPtr() const
{
	return m_window;
}

const uint32_t& WindowFrame::GetWidth() const
{
	return m_width;
}

const uint32_t& WindowFrame::GetHeight() const
{
	return m_height;
}

bool WindowFrame::WasRequestedClose() const
{
	return glfwWindowShouldClose(m_window);
}

bool WindowFrame::WasKeyPressed(int key) const
{
	return glfwGetKey(m_window, key) == GLFW_PRESS;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////

namespace Core
{
	std::shared_ptr<WindowFrame> GenerateWindow(const char* title, uint32_t width, uint32_t height)
	{
		return std::make_shared<WindowFrame>(title, width, height);
	}
}