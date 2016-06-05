#version 440

layout (location = 0) in vec3 particleCoord;
layout (location = 1) in float magnitudeNetForce;    

uniform mat4 unifWorldToClipMatrix;

// to frag shader
smooth out vec3 particleColor;

void main()
{
    particleColor = magnitudeNetForce * vec3(1.0f, 1.0f, 1.0f);
    particleColor = vec3(1.0f, 1.0f, 1.0f);
    gl_Position = unifWorldToClipMatrix * vec4(particleCoord, 1.0f);
}