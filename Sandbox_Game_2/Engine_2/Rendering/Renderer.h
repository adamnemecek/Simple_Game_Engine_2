#ifndef ENGINE_RENDERABLE_H
#define ENGINE_RENDERABLE_H

#include <glm\mat4x4.hpp>
#include <glm\vec3.hpp>
#include <Utilities\include_GL_version.h>
#include <Utilities\Typedefs.h>
#include <string>
#include <vector>
#include <Rendering\Camera.h>

// the renderer stores arrays of both of these, so we can't forward declare them
#include <Rendering\Renderable.h>

namespace Shapes
{
   class Geometry;
}

namespace Rendering
{
   class __declspec(dllexport) Renderer
   {
   public:
      bool initialize();
      bool shutdown();

      bool add_shader_program(GLuint program_ID);
      bool bind_shader_program(GLuint program_ID);
      bool unbind_current_shader_program();

      // returns a pointer to a renderable (??why??)
      Renderable *add_renderable(Shapes::Geometry *geometry_ptr);

      void set_viewport(GLsizei width, GLsizei height);

      void render_scene();

      static const uint CAMERA_MOVE_FORWARD =   0x01;
      static const uint CAMERA_MOVE_BACK =      0x02;
      static const uint CAMERA_MOVE_LEFT =      0x04;
      static const uint CAMERA_MOVE_RIGHT =     0x08;
      static const uint CAMERA_MOVE_UP =        0x10;
      static const uint CAMERA_MOVE_DOWN =      0x20;
      void manipulate_active_camera(uint flags);

   private:
      // helper function(s)
      GLint find_uniform(const GLuint program_ID, const std::string &uniform_name);
      
      Camera m_cam;

      std::vector<GLuint> m_shader_programs;
      GLint m_full_transform_uniform_location;
      GLint m_model_to_world_uniform_location;

      glm::mat4 m_perspective_mat;

      // declare a pool of renderables
      static const uint m_MAX_RENDERABLES = 10;
      Renderable m_renderables[m_MAX_RENDERABLES];
      uint m_num_current_renderables;
   };
}


#endif
