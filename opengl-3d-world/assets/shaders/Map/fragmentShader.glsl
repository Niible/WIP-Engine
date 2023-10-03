#version 460 core

in vec2 FragTexCoord;
in float Height;
in vec3 FragPos;

out vec4 FragColor;

uniform sampler2D snowTexture;
uniform sampler2D grassTexture;
uniform sampler2D rockTexture;
uniform sampler2D sandTexture;

uniform float sandThreshold;
uniform float grassThreshold;
uniform float rockThreshold;

const float transitionWidth = 1.5;

void main()
{
    vec4 snowColor = texture(snowTexture, FragTexCoord);
    vec4 grassColor = texture(grassTexture, FragTexCoord);
    vec4 rockColor = texture(rockTexture, FragTexCoord);
    vec4 sandColor = texture(sandTexture, FragTexCoord);

    vec4 rockColorMix = rockColor * 2;
    rockColorMix.a = 1.0;

    float sandWeight = smoothstep(sandThreshold - transitionWidth, sandThreshold + transitionWidth, Height);
    float grassWeight = smoothstep(grassThreshold - transitionWidth, grassThreshold + transitionWidth, Height);
    float rockWeight = smoothstep(rockThreshold - transitionWidth, rockThreshold + transitionWidth, Height);

    vec4 finalColor = mix(sandColor, grassColor, sandWeight);
    finalColor = mix(finalColor, rockColor, grassWeight);
    finalColor = mix(finalColor, snowColor, rockWeight);

    FragColor = vec4(finalColor.rgb, finalColor.a);
}