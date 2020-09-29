#version 330 core

struct DirectionalLight
{
    vec3 direction;
    vec3 ambient, diffuse, specular;
};

struct Material
{
    sampler2D diffuseTexture0, specularTexture0;
    vec3 ambient, diffuse, specular;
    float shininess;

    bool useTextures, useSpecularMap, renderingModel;
};

in VSH_OUT
{
    vec3 fragmentPos;
    vec3 normalPos;
    vec2 texturePos;

    vec4 lightMatrixFragmentPos;
} fshIn;

uniform vec3 skyColor;
uniform vec3 cameraPos;
uniform DirectionalLight dirLight;
uniform Material mat;

uniform sampler2D depthMap;

float GenerateFogValue(float density, float gradient);
float GenerateShadowValue(vec3 normal);

void main()
{
    vec3 diffuseTexture = texture(mat.diffuseTexture0, fshIn.texturePos).rgb;
    vec3 specularTexture = texture(mat.specularTexture0, fshIn.texturePos).rgb;
    vec3 normalDir = normalize(fshIn.normalPos);
    vec3 cameraDir = normalize(cameraPos - fshIn.fragmentPos);

    // Do lighting calculations
    vec3 ambientColor = vec3(0.0f), diffuseColor = vec3(0.0f), specularColor = vec3(0.0f);

    if(mat.useTextures)
    {
        ambientColor = dirLight.ambient * diffuseTexture;

        vec3 lightRay = normalize(-dirLight.direction);
        float diffuseStrength = max(dot(lightRay, normalDir), 0.0f);
        diffuseColor = diffuseStrength * dirLight.diffuse * diffuseTexture;

        if(mat.useSpecularMap || mat.renderingModel)
        {
            vec3 halfwayDir = normalize(cameraDir + lightRay);
            float specularStrength = pow(max(dot(halfwayDir, normalDir), 0.0f), mat.shininess);

            if(mat.useSpecularMap)
                specularColor = specularStrength * dirLight.specular * specularTexture;
            else if (mat.renderingModel)
                specularColor = specularStrength * dirLight.specular * mat.specular;
        }
    }
    else
    {
        ambientColor = dirLight.ambient * mat.ambient;

        vec3 lightRay = normalize(-dirLight.direction);
        float diffuseStrength = max(dot(lightRay, normalDir), 0.0f);
        diffuseColor = diffuseStrength * dirLight.diffuse * mat.diffuse;

        vec3 halfwayDir = normalize(cameraDir + lightRay);
        float specularStrength = pow(max(dot(halfwayDir, normalDir), 0.0f), mat.shininess);
        specularColor = specularStrength * dirLight.specular * mat.specular;
    }

    // Do final visibility calculations
    vec3 finalBlinnColor = ambientColor + (1.0f - GenerateShadowValue(normalDir)) * (diffuseColor + specularColor);

    float visibility = GenerateFogValue(0.04f, 2.5f);
    vec3 finalColor = clamp(mix(skyColor, finalBlinnColor, visibility), 0.0f, 1.0f);
    gl_FragColor = vec4(finalColor, 1.0f);
}

float GenerateFogValue(float density, float gradient)
{
    float fragDistance = length(fshIn.fragmentPos - cameraPos);
    return clamp(exp(-pow(fragDistance * density, gradient)), 0.0f, 1.0f);
}

float GenerateShadowValue(vec3 normal)
{
    vec3 projectedCoord = fshIn.lightMatrixFragmentPos.xyz / fshIn.lightMatrixFragmentPos.w;
    projectedCoord = projectedCoord * 0.5f + 0.5f;

    float currentDepth = projectedCoord.z;

    if(projectedCoord.z > 1.0f)
        return 0.0f;

    // Apply PCF to smoothen out the shadows
    vec2 texelSize = 1.0f / textureSize(depthMap, 0);
    float bias = 0.001f;

    float shadow = 0.0f;
    for(int y = -1; y <= 1; y++)
    {
        for(int x = -1; x <= 1; x++)
        {
            float closestDepth = texture(depthMap, projectedCoord.xy + vec2(x, y) * texelSize).r;
            shadow += currentDepth - bias > closestDepth ? 1.0f : 0.0f;
        }
    }

    return (shadow / 9.0f);
}
