#include "Renderer.h"

#include <cassert>

#include <framework\framework.h>
using Framework::LoadShader;
using Framework::CreateProgram;

#include <glm\vec3.hpp>
using glm::vec3;

#include <glm\mat4x4.hpp>
using glm::mat4;

#include <glm\gtc\type_ptr.hpp>
using glm::value_ptr;

#include <Utilities\Include_GLM_Mat_Transform.h>

#include <vector>
using std::vector;
#include <string>

using std::string;

#include <Utilities\include_GL_version.h>

#include <Shapes\Geometry.h>
using Shapes::Geometry;

#include <Utilities\Printer_Helper.h>

#include <iostream>
using std::cout;
using std::endl;


namespace Rendering
{
   bool Renderer::initialize()
   {
      // do NOT let the uniform locations be initialized to 0, which is the first valid uniform location!
      m_full_transform_uniform_location = -1;
      m_model_to_world_uniform_location = -1;

      m_num_current_renderables = 0;

      return true;
   }

   bool Renderer::shutdown()
   {
      return true;
   }

   GLuint Renderer::create_shader_program(
      const std::string *shader_file_paths,
      GLenum *shader_types,
      uint num_shaders)
   {
      vector<GLuint> shader_list;
      for (size_t shader_count = 0; shader_count < num_shaders; shader_count++)
      {
         shader_list.push_back(LoadShader(shader_types[shader_count], shader_file_paths[shader_count]));
      }

      GLuint shader_program_ID = Framework::CreateProgram(shader_list);
      m_shader_programs.push_back(shader_program_ID);

      return shader_program_ID;
   }


   // if the uniforms can't be found, an error is reported to stderr and the program will not be bound
   // ??necessary??
   bool Renderer::bind_shader_program(GLuint program_ID)
   {
      glUseProgram(program_ID);

      string matrix_name = "full_transform_matrix";
      try
      {
         m_full_transform_uniform_location = find_uniform(program_ID, matrix_name);
      }
      catch (std::exception &e)
      {
         fprintf(stderr, "%s\n", e.what());
         glUseProgram(0);
         return false;
      }

      matrix_name = "model_to_world_matrix";
      try
      {
         m_model_to_world_uniform_location = find_uniform(program_ID, matrix_name);
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

   Renderable *Renderer::add_renderable(Geometry *geometry_ptr)
   {
      assert(m_num_current_renderables != m_MAX_RENDERABLES);
      Renderable &r = m_renderables[m_num_current_renderables++];
      r.m_geometry_ptr = geometry_ptr;
      r.m_model_to_world_mat = mat4(1.0f);

      return &r;
   }

   void Renderer::set_viewport(GLsizei width, GLsizei height)
   {
      glViewport(0, 0, width, height);

      float fov_radians = (1.0f / 2.0f) * 3.14159f;   // 1/2 pi
      float aspect_ratio = (float)width / height;
      float near_plane_dist = 0.1f;
      float far_plane_dist = 20.0f;
      mat4 perspective_mat = glm::perspective(fov_radians, aspect_ratio, near_plane_dist, far_plane_dist);
      //mat4 perspective_mat = glm::ortho(0.0f, (float)width, (float)height, 0.0f, near_plane_dist, far_plane_dist);
      mat4 camera_mat = m_cam.get_world_to_view_matrix();
      m_world_to_projection = perspective_mat * camera_mat;

      cout << "resizing to aspect ratio " << aspect_ratio << endl;
   }

   void Renderer::render_scene()
   {
      glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
      glClear(GL_COLOR_BUFFER_BIT);

      // for each renderable:
      // - bind the geometry VAO
      // - send "full transform" and "orientation only" matrices to GPU
      // - draw elements 

      for (uint renderable_count = 0; renderable_count < m_num_current_renderables; renderable_count++)
      {
         Renderable &r = m_renderables[renderable_count];
         glBindVertexArray((r.m_geometry_ptr)->m_VAO_ID);

         mat4 full_transform_matrix = m_world_to_projection * r.m_model_to_world_mat;

         glUniformMatrix4fv(m_full_transform_uniform_location, 1, GL_FALSE, value_ptr(full_transform_matrix));
         glUniformMatrix4fv(m_model_to_world_uniform_location, 1, GL_FALSE, value_ptr(r.m_model_to_world_mat));

         glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_SHORT, 0);
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
         _snprintf(err_str, 256, "find_uniform(...): could not find uniform '%s' in program '%u'; the requested uniform is either unused in the program or mispelled", uniform_name.c_str(), program_ID);
         throw std::out_of_range(err_str);
         return -1;
      }

      return uniform_location;
   }
}


