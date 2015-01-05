#version 440

smooth in vec4 vertex_position_world_space;
smooth in vec4 vertex_normal_world_space;
smooth in vec4 vertex_base_color;

uniform vec4 unif_camera_position_world_space;
uniform vec4 unif_light_1_position_world_space;
uniform float unif_light_1_intensity;
uniform vec4 unif_light_2_position_world_space;
uniform float unif_light_2_intensity;

// The "diffuse" color models the color that a material will reflect after subsurface 
// light absorption by this particular kind of surface (in this case, it is a constant 
// value but can be turned into an input or a uniform).
const vec4 diffuse_color = vec4(0.5f, 0.0f, 0.0f, 1.0f);

// The "specular" color models the color that a surface will reflect.  Subsurface 
// light absorption is usually, but not always, ignored with this model.  As a result, 
// the specular color is typically the same as the color of the incident light.  In 
// this case though, I am fudging the color.
const vec4 specular_color = vec4(0.25f, 0.25f, 0.25f, 1.0f);

out vec4 final_color;


// calculate the attenuation factor, and spit out the normalized light vector as part of an optimized normalization
float calculate_attenuation_factor(in vec4 light_position, in float light_intensity, out vec4 normalized_vertex_to_light_vector)
{
   vec4 vertex_to_light_vector = light_position - vertex_position_world_space;

   // calculate the light's 'intensity falloff'
   // The basic equation is as follows: intensity / (1.0f + k * distance_factor).
   // The constant "k" is the "fudge factor" that you use to trim light intensity.
   // The distance factor might be a squared term, or linear, or a quadratic equation, or something else.
   float k = 1.0f;
   float light_vector_distance_squared = dot(vertex_to_light_vector, vertex_to_light_vector);
   float attenuation_factor = light_intensity / (1.0f + (k * light_vector_distance_squared));

   // normalize the light vector
   // Note: The "inverse square root" is a fast approximation that was discovered in the 1990s 
   // and is used to compute (1 / sqrt(X)), where X is a floating point argument.  If I pass 
   // in the square of a vector's magnitude by taking the dot product of a vector with itself, 
   // then I will have the inverse of the vector's magnitude.  Multiply the vector by this 
   // scalar for a cheap normalization.
   normalized_vertex_to_light_vector = vertex_to_light_vector * inversesqrt(light_vector_distance_squared);

   // return the attenuation factor
   return attenuation_factor;
}


float calculate_diffuse_factor(in vec4 normalized_vertex_to_light_vector)
{
   vec4 normalized_vertex_normal = vertex_normal_world_space * inversesqrt(dot(vertex_normal_world_space, vertex_normal_world_space));
   
   return dot(normalized_vertex_to_light_vector, normalized_vertex_normal);
}


// this function calculates a basic specular highlight, which is traditionally referred to as the "phong" term
float calculate_phong_term(in vec4 normalized_vertex_to_light_vector)
{
   vec4 normalized_vertex_to_camera_vector = normalize(unif_camera_position_world_space - vertex_position_world_space);
   
   // The reflect(...) function requires that the light vector be incident to the point of reflection,
   // which is why I negated the vertex-to-light vector.  That vector is also already normalized (or should be 
   // because the argument's name commands normalization), so I don't need to re-normalize it.
   vec4 normalized_reflected_light_vector = reflect(-normalized_vertex_to_light_vector, vertex_normal_world_space);

   float phong_term = dot(normalized_vertex_to_camera_vector, normalized_reflected_light_vector);
   return phong_term;
}

void main()
{
   vec4 normalized_vertex_to_light_1_vector = vec4(0.0f);
   float light_1_attenuation_factor = calculate_attenuation_factor(unif_light_1_position_world_space, unif_light_1_intensity, normalized_vertex_to_light_1_vector);
   float light_1_diffuse_factor = calculate_diffuse_factor(normalized_vertex_to_light_1_vector);
   float light_1_phong = calculate_phong_term(normalized_vertex_to_light_1_vector);

   vec4 normalized_vertex_to_light_2_vector = vec4(0.0f);
   float light_2_attenuation_factor = calculate_attenuation_factor(unif_light_2_position_world_space, unif_light_2_intensity, normalized_vertex_to_light_2_vector);
   float light_2_diffuse_factor = calculate_diffuse_factor(normalized_vertex_to_light_2_vector);
   float light_2_phong = calculate_phong_term(normalized_vertex_to_light_2_vector);

   // Note: Do not factor in the camera's distance to the light.
   //??why??

   // multiply the base vertex color by the lighting factors to get the final color
   float light_1_factor = clamp(light_1_diffuse_factor * light_1_attenuation_factor, 0, 1);
   float light_2_factor = clamp(light_2_diffuse_factor * light_2_attenuation_factor, 0, 1);
   final_color = 
      clamp((vertex_base_color * (light_1_factor + light_2_factor)), 0, 0) +
      (specular_color * (light_1_phong + light_2_phong));
}

