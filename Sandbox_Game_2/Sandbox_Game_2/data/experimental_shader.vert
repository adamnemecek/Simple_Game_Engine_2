#version 440

layout (location = 0) in vec4 position;   // implicitly becomes (x, y, z, 1.0f)
layout (location = 1) in vec3 normal;     // keep as vec3 to explicitly set the forth value to 0.0f later (disable translation)
layout (location = 2) in vec4 color;      // implicitly becomes (r, g, b, 1.0f)

uniform mat4 unif_full_transform_matrix;
uniform mat4 unif_model_to_world_matrix;

// these outputs are for the lighting that is done in the fragment shader
smooth out vec4 vertex_position_world_space;
smooth out vec4 vertex_normal_world_space;
smooth out vec4 vertex_base_color;

void main()
{
   vertex_position_world_space = unif_model_to_world_matrix * position;
   vertex_base_color = color;

   // disable normal translation
   // Note: The model-to-world matrix might have a scaling factor that un-normalizes the normal, 
   // but the fragment shader's fragment interpolation could also un-normalize it, so let the 
   // fragment shader handler normalizing the normal vector.
   vertex_normal_world_space = normalize(unif_model_to_world_matrix * vec4(normal, 0.0f));

	gl_Position = unif_full_transform_matrix * position;
}

