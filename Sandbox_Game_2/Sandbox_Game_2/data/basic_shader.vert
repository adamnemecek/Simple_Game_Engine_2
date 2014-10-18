#version 440

layout (location = 0) in vec4 position;   // implicitly becomes (x, y, z, 1.0f)
layout (location = 1) in vec3 normal;     // keep as vec3 to disable translation later
layout (location = 2) in vec4 color;      // implicitly becomes (r, g, b, 1.0f)

uniform mat4 full_transform_matrix;
uniform mat4 model_to_world_matrix;

// these outputs are for the lighting that is done in the fragment shader
smooth out vec4 vertex_position_world;
smooth out vec4 vertex_normal_world;
smooth out vec4 vertex_base_color;

void main()
{
   vec4 position_world = model_to_world_matrix * position;
   vec4 normal_world = model_to_world_matrix * vec4(normal, 0.0f);      // disable translation and keep rotation only

	gl_Position = full_transform_matrix * position;

   vertex_position_world = (position_world);
   vertex_normal_world = (normal_world);
   vertex_base_color = color;
}

