#pragma once
#include <memory>

struct GLFWwindow;
typedef unsigned int uint32_t;

class WindowFrame
{
private:
	GLFWwindow* m_window;
	uint32_t m_width, m_height;
private:
	void InitGLFW() const;
public:
	WindowFrame(const char* title, uint32_t width, uint32_t height);
	~WindowFrame();

	void RequestClose() const;
	void UpdateTick() const;
public:
	GLFWwindow* GetPtr() const;
	const uint32_t& GetWidth() const;
	const uint32_t& GetHeight() const;

	bool WasRequestedClose() const;
	bool WasKeyPressed(int key) const;
};

namespace Core
{
	std::shared_ptr<WindowFrame> GenerateWindow(const char* title, uint32_t width, uint32_t height);
}