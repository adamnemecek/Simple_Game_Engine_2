#version 440

smooth in vec4 vertex_position_world;
smooth in vec4 vertex_normal_world;
smooth in vec4 vertex_base_color;

uniform vec4 light_1_position_world;
uniform float light_1_intensity;
uniform vec4 light_2_position_world;
uniform float light_2_intensity;

out vec4 final_color;

// calculate the light's 'intensity falloff'
float calculate_attenuation_factor(in vec4 light_vector, in float light_intensity)
{
   // The basic equation is as follows: intensity / (1.0f + k * distance_factor).
   // The distance factor might be a squared term, or linear, or a quadratic equation, or something else.

   // the "fudge" factor to crudly trim the light intensity
   float k = 1.0f;

   // use a "square of the distance" for the distance factor
   float distance_factor = dot(light_vector, light_vector);

   return (light_intensity / (1.0f + (k * distance_factor)));
}

void main()
{
   // make a vector pointing from the position vertex to the light
   vec4 light_1_vector_world = light_1_position_world - vertex_position_world;
   vec4 light_2_vector_world = light_2_position_world - vertex_position_world;

   // diffuse brightness modification
   // Note: Normalize the light vector, and normalize the normal vector in case 
   // model-to-world and fragment interpolation un-normalized it.  Do this so 
   // that the dot product is just the angle.
   vec4 normalized_vertex_normal_world = normalize(vertex_normal_world);
   float light_1_diffuse_factor = dot(normalize(light_1_vector_world), normalized_vertex_normal_world);
   float light_2_diffuse_factor = dot(normalize(light_2_vector_world), normalized_vertex_normal_world);

   // calculate the attenuation factors
   float light_1_attenuation_factor = calculate_attenuation_factor(light_1_vector_world, light_1_intensity);
   float light_2_attenuation_factor = calculate_attenuation_factor(light_2_vector_world, light_2_intensity);

   // Note: Do not factor in the camera's distance to the light.
   //??why??

   // multiply the base vertex color by the lighting factors to get the final color
   final_color = 
      clamp(vertex_base_color, 0, 1) *
      clamp(light_1_diffuse_factor + light_2_diffuse_factor, 0, 1) *
      clamp(light_1_attenuation_factor + light_2_attenuation_factor, 0, 1);
      //attenuation_factor;
}

