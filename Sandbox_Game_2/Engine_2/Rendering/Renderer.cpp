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

#include <glm\gtc\matrix_transform.hpp>
using glm::perspective;

#include <vector>
using std::vector;
#include <string>

using std::string;

#include <Utilities\include_GL_version.h>

#include <Shapes\Geometry.h>
using Shapes::Geometry;

#include <iostream>
using std::cout;
using std::endl;


#include <Shapes\My_Vertex.h>
using Shapes::My_Vertex;


My_Vertex g_verts[] =
{
   vec3(-1.0f, -1.0f, +0.0f),          // left bottom corner
   vec3(1.0f, 0.0f, 0.0f),             // all red
   vec3(+0.0f, +0.0f, +1.0f),          // normal points out of screen

   vec3(+1.0f, -1.0f, +0.0f),          // right bottom corner
   vec3(1.0f, 1.0f, 0.0f),             // red + green (apparently this makes yellow)
   vec3(+0.0f, +0.0f, +1.0f),          // normal points out of screen

   vec3(+0.0f, +1.0f, +0.0f),          // center top
   vec3(1.0f, 0.0f, 1.0f),             // red + blue (apparently this makes pink
   vec3(+0.0f, +0.0f, +1.0f),          // normal points out of screen
};

GLushort g_indices[] =
{
   0, 1, 2,
};

GLuint g_vao;

namespace Rendering
{
   bool Renderer::initialize()
   {
      glGenVertexArrays(1, &g_vao);

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
   void Renderer::bind_shader_program(GLuint program_ID)
   {
      glUseProgram(program_ID);

      glBindVertexArray(g_vao);

      GLuint vert_buffer_ID = 0;
      glGenBuffers(1, &vert_buffer_ID);
      glBindBuffer(GL_ARRAY_BUFFER, vert_buffer_ID);
      glEnableVertexAttribArray(0);
      glVertexAttribPointer(0, My_Vertex::FLOATS_PER_POSITION, GL_FLOAT, GL_FALSE, My_Vertex::BYTES_PER_VERTEX, (void *)0);
      glEnableVertexAttribArray(1);
      glVertexAttribPointer(1, My_Vertex::FLOATS_PER_NORMAL, GL_FLOAT, GL_FALSE, My_Vertex::BYTES_PER_VERTEX, (void *)(My_Vertex::BYTES_PER_VERTEX));
      glEnableVertexAttribArray(2);
      glVertexAttribPointer(2, My_Vertex::FLOATS_PER_COLOR, GL_FLOAT, GL_FALSE, My_Vertex::BYTES_PER_VERTEX, (void *)(My_Vertex::BYTES_PER_VERTEX * 2));
      glBufferData(GL_ARRAY_BUFFER, sizeof(g_verts), g_verts, GL_STATIC_DRAW);

      GLuint elem_buffer_ID = 0;
      glGenBuffers(1, &elem_buffer_ID);
      glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elem_buffer_ID);
      glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(g_indices), g_indices, GL_STATIC_DRAW);



      //string matrix_name = "full_transform";
      //try
      //{
      //   m_full_transform_uniform_location = find_uniform(program_ID, matrix_name);
      //}
      //catch (std::exception &e)
      //{
      //   fprintf(stderr, "%s\n", e.what());
      //   glUseProgram(0);
      //   return;
      //}

      //matrix_name = "orientation_only";
      //try
      //{
      //   m_orientation_only_uniform_location = find_uniform(program_ID, matrix_name);
      //}
      //catch (std::exception &e)
      //{
      //   fprintf(stderr, "%s\n", e.what());
      //   glUseProgram(0);
      //   return;
      //}
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
      r.m_model_to_world_mat = mat4(1.0f);
      r.m_orientation_only_mat = mat4(1.0f);

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

      glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_SHORT, 0);

      //for (uint renderable_count = 0; renderable_count < m_num_current_renderables; renderable_count++)
      //{
      //   //Renderable &r = m_renderables[renderable_count];
      //   //glBindVertexArray((r.m_geometry_ptr)->m_VAO_ID);



      //   //mat4 model_to_projection = m_perspective_mat * r.m_model_to_world_mat;
      //   mat4 model_to_projection = mat4(1);

      //   //glUniformMatrix4fv(m_full_transform_uniform_location, 1, GL_FALSE, value_ptr(model_to_projection));
      //   //glUniformMatrix4fv(m_orientation_only_uniform_location, 1, GL_FALSE, value_ptr(r.m_orientation_only_mat));

      //   glDrawElements(
      //      (r.m_geometry_ptr)->m_render_mode,
      //      (r.m_geometry_ptr)->m_shape_data.m_num_indices,
      //      GL_UNSIGNED_SHORT,
      //      (void *)0);
      //}
   }


   // Private functions

   // throws an "out of range" exception if the uniform's location is < 0
   // Note: I want the program to blow up if a uniform can't be found.  
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


