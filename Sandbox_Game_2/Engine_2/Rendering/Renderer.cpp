#include "Renderer.h"

#include <cassert>

#include <framework\framework.h>
using Framework::LoadShader;
using Framework::CreateProgram;

#include <glm\glm.hpp>
using glm::mat4;
using glm::vec3;

#include <glm\gtc\type_ptr.hpp>
using glm::value_ptr;

#include <glm\gtc\matrix_transform.hpp>
using glm::perspective;

#include <vector>
using std::vector;

#include <Utilities\include_GL_version.h>

#include <Shapes\Geometry.h>
using Shapes::Geometry;

namespace Rendering
{
   bool Renderer::initialize()
   {
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

      //Framework::CreateShader(GL_VERTEX_SHADER, "bimble", "tooup");

      vector<GLuint> shader_list;
      for (size_t shader_count = 0; shader_count < num_shaders; shader_count++)
      {
         shader_list.push_back(LoadShader(shader_types[shader_count], shader_file_paths[shader_count]));
      }

      GLuint shader_program_ID = Framework::CreateProgram(shader_list);
      m_shader_programs.push_back(shader_program_ID);

      return shader_program_ID;
   }

   // ??necessary??
   void Renderer::bind_shader_program(GLuint program_ID)
   {
      glUseProgram(program_ID);
      m_full_transform_uniform_location = glGetUniformLocation(program_ID, "full_transform");
      m_orientation_only_uniform_location = glGetUniformLocation(program_ID, "orientation_only");
   }

   // ??necessary??
   void Renderer::unbind_current_shader_program()
   {
      glUseProgram(0);
   }

   Renderable *Renderer::add_renderable(Geometry *geometry_ptr)
   {
      assert(m_num_current_renderables != m_MAX_RENDERABLES);
      Renderable &r = m_renderables[m_num_current_renderables++];
      r.m_geometry_ptr = geometry_ptr;

      return &r;
   }

   void Renderer::set_viewport(GLsizei width, GLsizei height)
   {
      glViewport(0, 0, width, height);

      float fov_radians = (1.0f / 2.0f) * 3.14159f;   // 1/2 pi
      float aspect_ratio = (float)width / height;
      float near_plane_dist = 0.1f;
      float far_plane_dist = 20.0f;
      m_perspective_mat = perspective(fov_radians, aspect_ratio, near_plane_dist, far_plane_dist);
   }

   void Renderer::render_scene()
   {
      // for each renderable:
      // - bind the geometry VAO
      // - send "full transform" and "orientation only" matrices to GPU
      // - draw elements 

      for (uint renderable_count = 0; renderable_count < m_num_current_renderables; renderable_count++)
      {
         Renderable &r = m_renderables[m_num_current_renderables];
         glBindVertexArray((r.m_geometry_ptr)->m_VAO_ID);

         mat4 model_to_projection = m_perspective_mat * r.m_model_to_world_mat;

         glUniformMatrix4fv(m_full_transform_uniform_location, 1, GL_FALSE, value_ptr(model_to_projection));
         glUniformMatrix4fv(m_orientation_only_uniform_location, 1, GL_FALSE, value_ptr(r.m_orientation_only_mat));

         glDrawElements(
            (r.m_geometry_ptr)->m_render_mode,
            (r.m_geometry_ptr)->m_shape_data.m_num_indices,
            GL_UNSIGNED_SHORT,
            (r.m_geometry_ptr)->m_shape_data.m_indices);
      }
   }

}


