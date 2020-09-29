#pragma once
#include <memory>

class FrameBuffer;
class ShaderProgram;

typedef unsigned int GLenum;

class PostProcess
{
private:
	std::shared_ptr<ShaderProgram> m_shader;
	std::shared_ptr<FrameBuffer> m_FBO;
private:
	PostProcess();
	~PostProcess();

	void InitScript();
public:
	static PostProcess* GetPtr();

	void RenderToFBO() const;
	void RenderPostProcess() const;
};