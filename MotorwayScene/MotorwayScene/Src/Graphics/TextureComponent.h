#pragma once
#include <string>
#include <memory>

typedef unsigned int uint32_t;

class TextureComponent
{
private:
	uint32_t m_ID;
	std::string m_path;
	
	int m_width, m_height, m_numChannels;
private:
	void SetupTextureConfig() const;
public:
	TextureComponent(const std::string& path, bool srgb = false, bool flipVertical = false);
	~TextureComponent();

	void BindTexture(const std::string& samplerName, uint32_t samplerUnit) const;
	void UnbindTexture() const;
public:
	const uint32_t& GetID() const;
	const std::string& GetPath() const;

	const int& GetWidth() const;
	const int& GetHeight() const;
	const int& GetNumChannels() const;
};