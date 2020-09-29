#version 330 core
layout (location = 0) in vec3 vertexPos;
layout (location = 3) in mat4 instancedModel;

uniform mat4 model, lightMatrixVP;
uniform bool usingInstancing;

void main()
{
    mat4 modelMatrix = mat4(1.0f);
    if(usingInstancing)
        modelMatrix = instancedModel;
    else
        modelMatrix = model;

    gl_Position = lightMatrixVP * modelMatrix * vec4(vertexPos, 1.0f);
}
