#version 440

smooth in vec3 vert_pos_cs;
smooth in vec3 vert_norm_cs;
smooth in vec3 vert_base_color;

uniform vec3 unif_cam_pos_cs;
uniform vec3 unif_light_1_pos_cs;
uniform float unif_light_1_intensity;
uniform vec3 unif_light_2_pos_cs;
uniform float unif_light_2_intensity;

// The "specular" color models the color that a surface will reflect.  Subsurface 
// light absorption is usually, but not always, ignored with this model.  As a result, 
// the specular color is typically the same as the color of the incident light.  In 
// this case though, I am fudging the color.
const vec3 specular_color = vec3(0.25f, 0.25f, 0.25f);

out vec4 final_color;


// calculate the attenuation factor, and spit out the normalized light vector as part of an optimized normalization
float calculate_attenuation_factor(in vec3 light_pos_cs, in float light_intensity, out vec3 n_vert_to_light_vec)
{
   vec3 vert_to_light_vec = light_pos_cs - vert_pos_cs;

   // calculate the light's 'intensity falloff'
   // The basic equation is as follows: intensity / (1.0f + k * distance_factor).
   // The constant "k" is the "fudge factor" that you use to trim light intensity.
   // The distance factor might be a squared term, or linear, or a quadratic equation, or something else.
   float k = 1.0f;
   float light_vec_squared = dot(vert_to_light_vec, vert_to_light_vec);
   float attenuation_factor = light_intensity / (1.0f + (k * light_vec_squared));

   // normalize the light vector
   // Note: The "inverse square root" is a fast approximation that was discovered in the 1990s 
   // and is used to compute (1 / sqrt(X)), where X is a floating point argument.  If I pass 
   // in the square of a vector's magnitude by taking the dot product of a vector with itself, 
   // then I will have the inverse of the vector's magnitude.  Multiply the vector by this 
   // scalar for a cheap normalization.
   n_vert_to_light_vec = vert_to_light_vec * inversesqrt(light_vec_squared);

   return attenuation_factor;
}


float calculate_cosine_of_light_angle(in vec3 n_vert_to_light_vec)
{
   vec3 n_vert_normal_cs = vert_norm_cs * inversesqrt(dot(vert_norm_cs, vert_norm_cs));
   
   return dot(n_vert_to_light_vec, n_vert_normal_cs);
}


// this function calculates a basic specular highlight, which is traditionally referred to as the "phong" term
float calculate_phong_term(in vec3 n_vert_to_light_vec)
{
   // in camera space, the camera's position is implicitly at the origin <0,0,0>, and 
   // origin - any_vector = -any_vector
   vec3 n_vert_to_camera_vec = normalize(-vert_pos_cs);
   
   // The reflect(...) function requires that the light vector be incident to the point of reflection,
   // which is why I negated the vertex-to-light vector.  That vector is also already normalized (or should be 
   // because the argument's name commands normalization), so I don't need to re-normalize it.
   vec3 n_reflected_light_vec = reflect(-n_vert_to_light_vec, vert_norm_cs);

   // clamp the phong term so that it doesn't return a negative value
   //float phong_term = clamp(dot(n_vert_to_camera_vec, n_reflected_light_vec), 0, 1);
   float phong_term = dot(n_vert_to_camera_vec, n_reflected_light_vec);

   // incorporate the "shininess factor" power
   // Note: It's a magic number for now.
   phong_term = pow(phong_term, 50.0f);

   return phong_term;
}

void main()
{
   // calculate the attenuation factor first, which is common to both diffuse and
   // specular lighting computations
   vec3 n_vert_to_light_1_vec = vec3(0.0f);
   vec3 n_vert_to_light_2_vec = vec3(0.0f);
   float light_1_attenuation_factor = calculate_attenuation_factor(unif_light_1_pos_cs, unif_light_1_intensity, n_vert_to_light_1_vec);
   float light_2_attenuation_factor = calculate_attenuation_factor(unif_light_2_pos_cs, unif_light_2_intensity, n_vert_to_light_2_vec);
   
   // calculate each light's total diffuse light factor by multiplying each light's 
   // diffuse factor by its attenuation factor to determine the
   // fraction of light that is reflected in all directions given the angle of the
   // light relative to the fragment and the distance form the light to fragment
   
   float light_1_diffuse_factor = calculate_cosine_of_light_angle(n_vert_to_light_1_vec) * light_1_attenuation_factor;
   float light_1_specular_factor = calculate_phong_term(n_vert_to_light_1_vec) * light_1_attenuation_factor;

   float light_2_diffuse_factor = calculate_cosine_of_light_angle(n_vert_to_light_2_vec) * light_2_attenuation_factor;
   float light_2_specular_factor = calculate_phong_term(n_vert_to_light_2_vec) * light_2_attenuation_factor;

   // Note: Do not factor in the camera's distance to the light in a secondary attenuation 
   // factor.  Someone on reddit explained it to me this way: Imagine a 1 meter square plane
   // that is completely lit and is a displacement X from the camera.  If I displace the 
   // plane by another X, then it is twice as far away and four times smaller.  The effective
   // light that reaches me is therefore 1/4 of the plane's emitted light, which is what I
   // approximately wanted anyway.  Basically, the attenuation from the object to the camera
   // is already accounted for.  Unseen lights are not accounted for though because the 
   // displacement between them forms their own little vector, but I am now in camera space,
   // and the matrix transforms have already taken care of this.
   // This mostly makes sense to me.

   vec3 temp_vert_color = vec3(0.25f, 0.25f, 0.25f);

   vec3 temp_final_color = 
      clamp(temp_vert_color * light_1_diffuse_factor, 0, 1) +
      clamp(temp_vert_color * light_2_diffuse_factor, 0, 1) +
      clamp((specular_color * light_1_specular_factor), 0, 1) +
      clamp((specular_color * light_2_specular_factor), 0, 1);

   final_color = vec4(temp_final_color, 1.0f);
}

