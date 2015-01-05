#version 440

smooth in vec4 vertex_position_world;
smooth in vec4 vertex_normal_world;
smooth in vec4 vertex_base_color;

uniform vec4 light_1_position_world;
uniform float light_1_intensity;
uniform vec4 light_2_position_world;
uniform float light_2_intensity;

out vec4 final_color;

// calculate the diffuse and attenuation factors
void calculate_light_modification_factors(in vec4 light_position, in float light_intensity, out float diffuse_factor, out float attenuation_factor)
{
   // vertex-to-light vector
   vec4 light_vector = light_position - vertex_position_world;


   // calculate the light's 'intensity falloff'
   // The basic equation is as follows: intensity / (1.0f + k * distance_factor).
   // The constant "k" is the "fudge factor" that you use to trim light intensity.
   // The distance factor might be a squared term, or linear, or a quadratic equation, or something else.
   float k = 1.0f;
   float light_vector_distance_squared = dot(light_vector, light_vector);
   attenuation_factor = light_intensity / (1.0f + (k * light_vector_distance_squared));

   
   // the diffuse factor is simply the cosine of the normalized vertex normal and light vectors
   // Note: The "inverse square root" is a fast approximation that was discovered in the 1990s 
   // and is used to compute (1 / sqrt(X)), where X is a floating point argument.  If I pass 
   // in the square of a vector's magnitude by taking the dot product of a vector with itself, 
   // then I will have the inverse of the vector's magnitude.  Multiply the vector by this 
   // scalar for a cheap normalization.
   vec4 normalized_light_vector = light_vector * inversesqrt(light_vector_distance_squared);
   vec4 normalized_vertex_normal = vertex_normal_world * inversesqrt(dot(vertex_normal_world, vertex_normal_world));
   diffuse_factor = dot(normalized_light_vector, normalized_vertex_normal);

   
   // done
}

void main()
{
   float light_1_diffuse_factor;
   float light_1_attenuation_factor;
   calculate_light_modification_factors(light_1_position_world, light_1_intensity, light_1_diffuse_factor, light_1_attenuation_factor);

   float light_2_diffuse_factor;
   float light_2_attenuation_factor;
   calculate_light_modification_factors(light_2_position_world, light_2_intensity, light_2_diffuse_factor, light_2_attenuation_factor);
   
   // Note: Do not factor in the camera's distance to the light.
   //??why??

   // multiply the base vertex color by the lighting factors to get the final color
   vec4 temp_final_color;
   if (dot(normalize(light_1_position_world), normalize(vertex_normal_world)) < 0.0f)
   {
      // red-shift it
      temp_final_color = clamp(vertex_base_color + vec4(0.5f, 0.0f, 0.0f, 0.0f), 0, 1);
   }
   else
   {
      // green-shift it
      temp_final_color = clamp(vertex_base_color + vec4(0.0f, 0.5f, 0.0f, 0.0f), 0, 1);
   }

   float diffuse_factor = clamp(clamp(light_1_diffuse_factor, 0, 1) + clamp(light_2_diffuse_factor, 0, 0), 0.0f, 1.0f);
   float attenuation_factor = clamp(clamp(light_1_attenuation_factor, 0, 1) + clamp(light_2_attenuation_factor, 0, 0), 0.0f, 1.0f);
   temp_final_color = 
      clamp(vertex_base_color, 0, 1) *
      diffuse_factor *
      attenuation_factor;
   float color_intensity = sqrt(dot(vec3(temp_final_color), vec3(temp_final_color)));

   if (attenuation_factor < 0.01f)
   {
      // red-shift it
      final_color = clamp(temp_final_color + vec4(0.5f, 0.0f, 0.0f, 0.0f), 0, 1);
   }
   else
   {
      // green-shift it
      final_color = clamp(temp_final_color + vec4(0.0f, 0.5f, 0.0f, 0.0f), 0, 1);
   }

   float light_1_factor = clamp(light_1_diffuse_factor * light_1_attenuation_factor, 0, 1);
   float light_2_factor = clamp(light_2_diffuse_factor * light_2_attenuation_factor, 0, 1);
   final_color = clamp(normalize(vertex_base_color), 0, 1) * (light_1_factor + light_2_factor);
}

