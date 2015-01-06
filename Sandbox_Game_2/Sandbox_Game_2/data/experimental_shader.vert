#version 440

layout (location = 0) in vec3 vert_pos_ms;   // vertex position in model space (MS)
layout (location = 1) in vec3 vert_norm_ms;  // vertex normal in model space (MS)
layout (location = 2) in vec3 vert_color;    

uniform mat4 unif_model_to_camera_matrix;
uniform mat4 unif_camera_to_clip_matrix;

// these outputs are for the lighting that is done in the fragment shader
smooth out vec3 vert_pos_cs;       // vertex position in camera space (CS)
smooth out vec3 vert_norm_cs;      // vertex normal in camera space (CS)
smooth out vec3 vert_base_color;

void main()
{
   // enable translation on the position
   vec4 temp_pos_cs = unif_model_to_camera_matrix * vec4(vert_pos_ms, 1.0f);
   vert_pos_cs = vec3(temp_pos_cs);

   // disable normal translation
   // Note: Do not normalize the normal vector here, even though the model-to-camera matrix 
   // may have introduced scaling that un-normalized it, because the linear interpolation from
   // the vert shader to the frag shader may also un-normalize it.  Let the fragment shader to
   // do the normalization.
   vert_norm_cs = vec3(unif_model_to_camera_matrix * vec4(vert_norm_ms, 0.0f));
   
   // pass the color straight through
   vert_base_color = vert_color;

	gl_Position = unif_camera_to_clip_matrix * temp_pos_cs;
}

