#ifndef ENGINE_RENDERABLE_H
#define ENGINE_RENDERABLE_H

#include <Utilities\include_GL_version.h>
#include <Utilities\Typedefs.h>
#include <string>
#include <vector>

// the renderer stores arrays of both of these, so we can't forward declare them
#include <Rendering\Geometry.h>
#include <Rendering\Renderable.h>

namespace Rendering
{
   class __declspec(dllexport) Renderer
   {
   public:
      bool initialize();
      bool shutdown();

      GLuint create_shader_program(
         const std::string *shader_file_paths,
         GLenum *shader_types,
         uint num_shaders);
      void bind_shader_program(GLuint program_ID);
      void unbind_current_shader_program();

      Geometry *add_geometry(
         glm::vec3 *verts_ptr, glm::vec3 *normal_verts_ptr, uint num_verts,
         ushort *indices_ptr, uint num_indices,
         GLenum render_mode = GL_TRIANGLES);

      Renderable *add_renderable(Geometry *geometry_ptr);

      void set_viewport(GLsizei width, GLsizei height);

      void render_scene();



   private:
      std::vector<GLuint> m_shader_programs;

      glm::mat4 get_aspect_correction_matrix() const;
      GLsizei m_viewport_width;
      GLsizei m_viewport_height;

      // declare a pool of geometries
      static const uint m_MAX_GEOMETRIES = 10;
      Geometry m_geometries[m_MAX_GEOMETRIES];
      uint m_num_current_geometries;

      // declare a pool of renderables
      static const uint m_MAX_RENDERABLES = 10;
      Renderable m_renderables[m_MAX_RENDERABLES];
      uint m_num_current_renderables;
   };
}


#endif
