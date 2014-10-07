#include "Renderer.h"

#include <glm\glm.hpp>
using glm::mat4;

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
      return 0;
   }

   void Renderer::bind_shader_program(GLuint program_ID)
   {

   }

   void Renderer::unbind_current_shader_program()
   {

   }

   Geometry *Renderer::add_geometry(
      glm::vec3 *verts_ptr, glm::vec3 *normal_verts_ptr, uint num_verts,
      ushort *indices_ptr, uint num_indices,
      GLenum render_mode)
   {
      return 0;
   }

   Renderable *Renderer::add_renderable(Geometry *geometry_ptr)
   {
      return 0;
   }

   void Renderer::set_viewport(GLsizei width, GLsizei height)
   {

   }

   void Renderer::render_scene()
   {
      // for each geometry:
      // - bind VAO
      // - send "full transform" and "orientation only" matrices to GPU
      // - draw elements instanced
   }

   glm::mat4 Renderer::get_aspect_correction_matrix() const
   {
      return mat4();
   }
}


