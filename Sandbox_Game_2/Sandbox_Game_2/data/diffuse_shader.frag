#version 440

smooth in vec4 vertex_position_world;
smooth in vec4 vertex_normal_world;
smooth in vec4 vertex_base_color;

uniform vec4 light_1_position_world;
uniform float light_1_intensity;

out vec4 final_color;


void main()
{
   // make a vector pointing from the position vertex to the light
   vec4 light_1_vector_world = light_1_position_world - vertex_position_world;

   // diffuse brightness modification
   // Note: Normalize the light vector, and normal vector in case model-to-world and fragment 
   // interpolation un-normalized it.  Do this so that the dot product is just the angle.
   float diffuse_brightness_angle_only = dot(normalize(light_1_vector_world), normalize(vertex_normal_world));

   // "light intensity falloff" factor
   float k = 1.0f;
   float distance_factor = dot(light_1_vector_world, light_1_vector_world);
   float attenuation_factor = light_1_intensity / (1.0f + (k * distance_factor));

   // Note: Do not factor in the camera's distance to the light.
   //??why??

   // multiply the base vertex color by the lighting factors to get the final color
   final_color = 
      clamp(vertex_base_color, 0, 1) *
      clamp(diffuse_brightness_angle_only, 0, 1) *
      clamp(attenuation_factor, 0, 1);
      //attenuation_factor;
}

