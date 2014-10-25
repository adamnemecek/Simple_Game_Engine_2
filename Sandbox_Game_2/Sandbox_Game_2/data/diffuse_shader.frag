#version 440

smooth in vec4 vertex_position_world;
smooth in vec4 vertex_normal_world;
smooth in vec4 vertex_base_color;

uniform vec4 light_position_world;

out vec4 final_color;

void main()
{
   // make a vector pointing from a vertex to the light and normalize it
   vec4 light_vector_world = normalize(light_position_world - vertex_position_world);

   // diffuse brightness modification
   float diffuse_brightness = dot(light_vector_world, vertex_normal_world);
   vec4 diffuse_color = vec4(diffuse_brightness, diffuse_brightness, diffuse_brightness, 1.0f);


   // add colors together
   final_color = 
      clamp(vertex_base_color, 0, 1) +
      clamp(diffuse_color, 0, 0);
}

