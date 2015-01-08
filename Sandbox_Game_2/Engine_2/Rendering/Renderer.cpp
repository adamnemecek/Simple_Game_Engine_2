#include "Renderer.h"

#include <Utilities\My_Assert.h>

#include <glm\vec3.hpp>
#include <glm\mat4x4.hpp>
#include <glm\gtc\type_ptr.hpp>

// for glm::perspective
#include <Utilities\Include_Helpers\GLM_Mat_Transform.h>
#include <Utilities\Include_Helpers\GL_Version.h>
#include <Shapes\Geometry.h>
#include <Rendering\Camera.h>

#include <Utilities\Printer_Helper.h>
#include <iostream>
using std::cout;
using std::endl;


namespace Rendering
{
   bool Renderer::initialize()
   {
      //my_function();

      // do NOT let the uniform locations be initialized to 0, which is the first valid uniform location!
      m_unif_loc_model_to_camera_matrix = -1;
      m_unif_loc_camera_to_clip_matrix = -1;
      m_unif_loc_cam_pos_cs = -1;
      m_unif_loc_light_1_pos_cs = -1;
      m_unif_loc_light_1_intensity = -1;
      m_unif_loc_light_2_pos_cs = -1;
      m_unif_loc_light_2_intensity = -1;

      m_num_current_renderables = 0;

      return true;
   }

   bool Renderer::shutdown()
   {
      return true;
   }

   bool Renderer::add_shader_program(GLuint program_ID)
   {
      for (uint program_count = 0; program_count < m_shader_programs.size(); program_count++)
      {
         if (m_shader_programs[program_count] == program_ID)
         {
            // already have this shader program
            return false;
         }
      }

      // don't have this shader program yet, so add it
      m_shader_programs.push_back(program_ID);

      return true;
   }

   // if the uniforms can't be found, an error is reported to stderr and the program will not be bound
   // ??necessary??
   bool Renderer::bind_shader_program(GLuint program_ID)
   {
      glUseProgram(program_ID);

      std::string uniform_name_str = "unif_model_to_camera_matrix";
      if (!find_uniform(program_ID, uniform_name_str, &m_unif_loc_model_to_camera_matrix)) { glUseProgram(0); return false; }

      uniform_name_str = "unif_camera_to_clip_matrix";
      if (!find_uniform(program_ID, uniform_name_str, &m_unif_loc_camera_to_clip_matrix)) { glUseProgram(0); return false; }

      uniform_name_str = "unif_cam_pos_cs";
      //if (!find_uniform(program_ID, uniform_name_str, &m_unif_loc_cam_pos_cs)) { glUseProgram(0); return false; }

      uniform_name_str = "unif_light_1_pos_cs";
      if (!find_uniform(program_ID, uniform_name_str, &m_unif_loc_light_1_pos_cs)) { glUseProgram(0); return false; }

      uniform_name_str = "unif_light_1_intensity";
      if (!find_uniform(program_ID, uniform_name_str, &m_unif_loc_light_1_intensity)) { glUseProgram(0); return false; }

      uniform_name_str = "unif_light_2_pos_cs";
      if (!find_uniform(program_ID, uniform_name_str, &m_unif_loc_light_2_pos_cs)) { glUseProgram(0); return false; }

      uniform_name_str = "unif_light_2_intensity";
      if (!find_uniform(program_ID, uniform_name_str, &m_unif_loc_light_2_intensity)) { glUseProgram(0); return false; }

      return true;
   }

   // ??necessary??
   bool Renderer::unbind_current_shader_program()
   {
      glUseProgram(0);
      return false;
   }

   Renderable *Renderer::add_renderable(Shapes::Geometry *geometry_ptr)
   {
      MY_ASSERT(m_num_current_renderables != m_MAX_RENDERABLES);
      Renderable &r = m_renderables[m_num_current_renderables++];
      r.m_geometry_ptr = geometry_ptr;
      r.m_model_to_world_mat = glm::mat4(1.0f);

      return &r;
   }

   void Renderer::set_viewport(GLsizei width, GLsizei height)
   {
      glViewport(0, 0, width, height);

      float fov_radians = (1.0f / 4.0f) * 3.14159f;   // 1/2 pi
      float aspect_ratio = (float)width / height;
      float near_plane_dist = 0.1f;
      float far_plane_dist = 100.0f;
      m_perspective_mat = glm::perspective(fov_radians, aspect_ratio, near_plane_dist, far_plane_dist);

      cout << "resizing to aspect ratio " << aspect_ratio << endl;
   }

   void Renderer::set_camera_to_render(Camera *camera_ptr)
   {
      MY_ASSERT(camera_ptr != 0);
      m_camera_ptr = camera_ptr;
   }

   void Renderer::render_scene()
   {
      // we can't render without a camera
      MY_ASSERT(m_camera_ptr != 0);

      glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
      glClearDepth(1.0f);
      glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

      // for each renderable:
      // - bind the geometry VAO
      // - send "full transform" and "orientation only" matrices to GPU
      // - draw elements 

      //// the camera position is independent of each renderable
      //glUniform4fv(m_, 1, glm::value_ptr(m_camera_ptr->get_position()));

      glm::mat4 camera_mat = m_camera_ptr->get_world_to_view_matrix();

      // the lights are independent of each renderable
      // Note: The light locations that are hard-coded here are in world space, so I only need
      // to transform them with the camera matrix to get them into camera space.
      glm::vec3 light_1_location = glm::vec3(camera_mat * glm::vec4(+5.0f, +6.0f, -7.0f, 1.0f));
      float light_1_intensity = 90.0f;
      glm::vec3 light_2_location = glm::vec3(camera_mat * glm::vec4(-5.0f, +3.0f, +5.0f, 1.0f));
      float light_2_intensity = 30.0f;
      glUniform3fv(m_unif_loc_light_1_pos_cs, 1, glm::value_ptr(light_1_location));
      glUniform1f(m_unif_loc_light_1_intensity, light_1_intensity);
      glUniform3fv(m_unif_loc_light_2_pos_cs, 1, glm::value_ptr(light_2_location));
      glUniform1f(m_unif_loc_light_2_intensity, light_2_intensity);

      // the camera-to-clip matrix is independent of each renderable
      glUniformMatrix4fv(m_unif_loc_camera_to_clip_matrix, 1, GL_FALSE, glm::value_ptr(m_perspective_mat));

      for (uint renderable_count = 0; renderable_count < m_num_current_renderables; renderable_count++)
      {
         Renderable &r = m_renderables[renderable_count];
         glBindVertexArray((r.m_geometry_ptr)->m_VAO_ID);

         glm::mat4 model_to_camera = camera_mat * r.m_model_to_world_mat;

         glUniformMatrix4fv(m_unif_loc_model_to_camera_matrix, 1, GL_FALSE, glm::value_ptr(model_to_camera));

         glDrawElements(
            (r.m_geometry_ptr)->m_render_mode, 
            (r.m_geometry_ptr)->m_shape_data.m_num_indices, 
            GL_UNSIGNED_SHORT, 0);
      }
   }


   // Private functions

   // throws an "out of range" exception if the uniform's location is < 0
   bool Renderer::find_uniform(const GLuint program_ID, const std::string &uniform_name, GLint *put_uniform_location_here)
   {
      GLint temp_uniform_location = 0;
      temp_uniform_location = glGetUniformLocation(program_ID, uniform_name.c_str());
      if (temp_uniform_location < 0)
      {
         printf("find_uniform(...): could not find uniform '%s' in program '%u'; the requested uniform is either mispelled or unused in the program", uniform_name.c_str(), program_ID);
         *put_uniform_location_here = -1;
         return false;
      }

      *put_uniform_location_here = temp_uniform_location;

      return true;
   }
}


