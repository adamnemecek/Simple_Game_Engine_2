#version 440

smooth in vec3 particleColor;

out vec4 finalColor;

void main()
{
    // just shove the color out the door
    finalColor = vec4(particleColor, 1.0f);
}