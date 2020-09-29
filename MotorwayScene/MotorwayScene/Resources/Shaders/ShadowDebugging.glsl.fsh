#version 330 core

in vec2 textureCoord;
uniform sampler2D depthMap;

void main()
{
    float sampledDepth = texture(depthMap, textureCoord).r;
    gl_FragColor = vec4(vec3(sampledDepth), 1.0f);
}
