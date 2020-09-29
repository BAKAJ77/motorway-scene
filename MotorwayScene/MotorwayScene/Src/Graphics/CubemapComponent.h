#pragma once
#include <string>
#include <array>

typedef unsigned int uint32_t;

class CubemapComponent
{
private:
	uint32_t m_ID;
public:
	CubemapComponent(const std::array<std::string, 6>& paths);
	~CubemapComponent();

	void BindCubemap(const std::string& samplerName, uint32_t samplerUnit) const;
public:
	const uint32_t& GetID() const;
};