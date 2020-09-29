#version 330 core
layout (location = 0) in vec3 vertexPos;
layout (location = 1) in vec3 normalPos;
layout (location = 2) in vec2 texturePos;

layout (location = 3) in mat4 instancedModel; // Only to be used for instancing

uniform mat4 model, vpMatrix, lightMatrixVP; // vpMatrix is basically the product of the projection and view matrices
uniform bool usingInstancing;

out VSH_OUT
{
    vec3 fragmentPos;
    vec3 normalPos;
    vec2 texturePos;

    vec4 lightMatrixFragmentPos;
} vshOut;

void main()
{
    mat4 modelMatrix = mat4(1.0f);
    if(usingInstancing)
        modelMatrix = instancedModel;
    else
        modelMatrix = model;

    vshOut.fragmentPos = vec3(modelMatrix * vec4(vertexPos, 1.0f));
    vshOut.lightMatrixFragmentPos = lightMatrixVP * vec4(vshOut.fragmentPos, 1.0f);
    vshOut.normalPos = mat3(transpose(inverse(modelMatrix))) * normalPos;
    vshOut.texturePos = texturePos;

    gl_Position = vpMatrix * modelMatrix * vec4(vertexPos, 1.0f);
}
