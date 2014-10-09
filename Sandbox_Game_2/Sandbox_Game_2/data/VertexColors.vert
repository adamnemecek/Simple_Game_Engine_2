#version 440

layout (location = 0) in vec4 position;
layout (location = 1) in vec4 normal;
layout (location = 2) in vec4 color;

uniform mat4 full_transform;
uniform mat4 orientation_only;

smooth out vec4 theColor;

void main()
{
	gl_Position = position;
   theColor = color;
}

