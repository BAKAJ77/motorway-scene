#version 330 core

in vec2 textureCoord;
uniform sampler2DMS sceneTexture;
uniform float gammaValue;
uniform int numSamples;

vec3 GammaCorrectOutput(vec3 color, float gamma);

void main()
{
    // Do multisampling
    vec4 finalColor = vec4(0.0f);
    for(int i = 0; i < numSamples; i++)
        finalColor += texelFetch(sceneTexture, ivec2(gl_FragCoord.xy), i);

    finalColor /= float(numSamples);

    // Do final gamma-corrections on final color
    gl_FragColor = vec4(GammaCorrectOutput(finalColor.rgb, gammaValue), 1.0f);
}

vec3 GammaCorrectOutput(vec3 color, float gamma)
{
    return pow(color, vec3(1.0f / gamma));
}
