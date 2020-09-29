#pragma once
#include "Core/WindowFrame.h"
#include "Scripts/Player.h"
#include "Scripts/WorldScene.h"

#include <memory>

class AppCore
{
private:
	const std::shared_ptr<WindowFrame> m_window;

	// Scripts
	Player m_playerUser;
	WorldScene m_worldScene;
private:
	void SetupScripts();

	void MainLoop();
	void UpdateTick(const float& deltaTime);
	void Render() const;
public:
	AppCore();
	~AppCore();
};