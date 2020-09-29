#version 330 core
layout (location = 0) in vec3 vertexPos;
layout (location = 2) in vec2 texturePos;

out vec2 textureCoord;

void main()
{
    textureCoord = texturePos;
    gl_Position = vec4(2.0f * vertexPos, 1.0f);
}
