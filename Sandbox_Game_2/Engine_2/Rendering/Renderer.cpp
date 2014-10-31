#include "Renderer.h"

#include <Utilities\My_Assert.h>

#include <glm\vec3.hpp>
#include <glm\mat4x4.hpp>
#include <glm\gtc\type_ptr.hpp>
#include <Utilities\Include_Helpers\GLM_Mat_Transform.h>
#include <vector>
#include <string>
#include <Utilities\Include_Helpers\GL_Version.h>
#include <Shapes\Geometry.h>
#include <Utilities\Printer_Helper.h>
#include <Rendering\Camera.h>
#include <iostream>
using std::cout;
using std::endl;

#include <glm\gtx\dual_quaternion.hpp>
#include <Utilities\Quaternion_Helper.h>

#include <Math\Dual_Quat.h>

glm::fdualquat dual_quat_conjugate(glm::fdualquat dq)
{
   return glm::fdualquat(glm::conjugate(dq.real), (-1.0f) * glm::conjugate(dq.dual));
}
void my_function()
{
   glm::fquat point(1.0f, 3.0f, 4.0f, 5.0f);
   glm::fdualquat point_dq(glm::fquat(), point);

   glm::vec3 translate(4.0f, 2.0f, 6.0f);
   glm::fdualquat translate_dq(glm::fquat(), translate);

   glm::fquat rotation;
   Utilities::Quaternion_Helper::orientation_offset(glm::vec3(0.0f, 1.0f, 0.0f), 3.14159f, rotation);
   glm::fdualquat rotation_dq(rotation, glm::fquat(0.0, 0.0f, 0.0f, 0.0f));

   // pure rotation
   //glm::vec4 v = glm::vec4(3.0f, 4.0f, 5.0f, 1.0f);
   //glm::vec4 result = glm::mat4_cast(rotation) * v;
   //glm::fdualquat result_1 = rotation_dq * point_dq * rotation_dq;
   //glm::fdualquat result_2 = glm::normalize(rotation_dq) * point_dq * glm::normalize(rotation_dq);

   // pure translation
   //glm::fdualquat result_1 = translate_dq * point_dq * dual_quat_conjugate(translate_dq);
   //glm::fdualquat result_2 = glm::normalize(translate_dq) * point_dq * glm::normalize(dual_quat_conjugate(translate_dq));

   // rotate, then translate
   glm::fdualquat transform(rotation, translate);
   glm::fdualquat result_1 = transform * point_dq * dual_quat_conjugate(transform);
   glm::fdualquat result_2 = glm::normalize(transform) * point_dq * glm::normalize(dual_quat_conjugate(transform));

   //glm::fdualquat dq(glm::fquat(), glm::fquat(0.0f, 0.0f, 5.0f, 0.0f));
   //dq = glm::normalize(dq);




   //Math::Dual_Quaternion point = Math::Dual_Quaternion::point(glm::vec3(0.0f, 1.0f, 0.0f));
   //Math::Dual_Quaternion translate = Math::Dual_Quaternion::translate(glm::vec3(0.0f, 2.0f, 0.0f));
   //Math::Dual_Quaternion result = translate * Math::Dual_Quaternion::conjugate(point);

   printf("hello\n");
}


namespace Rendering
{
   bool Renderer::initialize()
   {
      //my_function();

      // do NOT let the uniform locations be initialized to 0, which is the first valid uniform location!
      m_full_transform_uniform_location = -1;
      m_model_to_world_uniform_location = -1;
      m_light_position_world_uniform_location = -1;

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

      std::string uniform_name_str = "full_transform_matrix";
      try
      {
         m_full_transform_uniform_location = find_uniform(program_ID, uniform_name_str);
      }
      catch (std::exception &e)
      {
         fprintf(stderr, "%s\n", e.what());
         glUseProgram(0);
         return false;
      }

      uniform_name_str = "model_to_world_matrix";
      try
      {
         m_model_to_world_uniform_location = find_uniform(program_ID, uniform_name_str);
      }
      catch (std::exception &e)
      {
         fprintf(stderr, "%s\n", e.what());
         glUseProgram(0);
         return false;
      }

      uniform_name_str = "light_position_world";
      try
      {
         m_light_position_world_uniform_location = find_uniform(program_ID, uniform_name_str);
      }
      catch (std::exception &e)
      {
         fprintf(stderr, "%s\n", e.what());
         glUseProgram(0);
         return false;
      }

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

      float fov_radians = (1.0f / 2.0f) * 3.14159f;   // 1/2 pi
      float aspect_ratio = (float)width / height;
      float near_plane_dist = 0.1f;
      float far_plane_dist = 20.0f;
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

      // the light is independent of each renderable
      static glm::vec4 translation(0.0f, +0.5f, 0.0f, 1.0f);
      glUniform4fv(m_light_position_world_uniform_location, 1, glm::value_ptr(translation));

      glm::mat4 camera_mat = m_camera_ptr->get_world_to_view_matrix();
      glm::mat4 world_to_projection = m_perspective_mat * camera_mat;

      for (uint renderable_count = 0; renderable_count < m_num_current_renderables; renderable_count++)
      {
         Renderable &r = m_renderables[renderable_count];
         glBindVertexArray((r.m_geometry_ptr)->m_VAO_ID);

         glm::mat4 full_transform_matrix = world_to_projection * r.m_model_to_world_mat;

         glUniformMatrix4fv(m_full_transform_uniform_location, 1, GL_FALSE, glm::value_ptr(full_transform_matrix));
         glUniformMatrix4fv(m_model_to_world_uniform_location, 1, GL_FALSE, glm::value_ptr(r.m_model_to_world_mat));

         glDrawElements(
            (r.m_geometry_ptr)->m_render_mode, 
            (r.m_geometry_ptr)->m_shape_data.m_num_indices, 
            GL_UNSIGNED_SHORT, 0);
      }
   }


   // Private functions

   // throws an "out of range" exception if the uniform's location is < 0
   GLint Renderer::find_uniform(const GLuint program_ID, const std::string &uniform_name)
   {
      GLint uniform_location = 0;
      uniform_location = glGetUniformLocation(program_ID, uniform_name.c_str());
      if (uniform_location < 0)
      {
         char err_str[256];
         _snprintf(err_str, 256, "find_uniform(...): could not find uniform '%s' in program '%u'; the requested uniform is either mispelled or unused in the program", uniform_name.c_str(), program_ID);
         throw std::out_of_range(err_str);
         return -1;
      }

      return uniform_location;
   }
}


