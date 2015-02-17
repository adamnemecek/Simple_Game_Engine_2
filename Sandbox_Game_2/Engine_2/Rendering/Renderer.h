#ifndef ENGINE_RENDERER_H
#define ENGINE_RENDERER_H

#include <glm\mat4x4.hpp>
#include <glm\vec3.hpp>
#include <Utilities\Include_Helpers\GL_Version.h>
#include <Utilities\Typedefs.h>
#include <string>
#include <vector>

// the renderer stores arrays of both of these, so we can't forward declare them
#include <Rendering\Renderable.h>

// forward declarations
namespace Shapes
{
   class Geometry;
}
namespace Entities
{
   class Entity;
}
namespace Rendering
{
   class Camera;
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

      // fill out an unassigned renderable in the Renderer's collection of renderables
      // with the geometry that it is referencing, which includes information on the 
      // vertex array object binding, and the entity that it is for, which is how the
      // renderable gets the entity-specific model-to-world matrix
      void configure_new_renderable(const Entities::Entity *e_ptr, const Shapes::Geometry *g_ptr);

      void set_viewport(GLsizei width, GLsizei height);
      void set_camera_to_render(Camera *camera_ptr);

      void render_scene();

   private:
      // helper function(s)
      bool find_uniform(const GLuint program_ID, const std::string &uniform_name, GLint *put_uniform_location_here);
      
      Camera *m_camera_ptr;

      std::vector<GLuint> m_shader_programs;
      GLint m_unif_loc_model_to_camera_matrix;
      GLint m_unif_loc_camera_to_clip_matrix;
      GLint m_unif_loc_light_1_pos_cs;
      GLint m_unif_loc_light_1_intensity;
      GLint m_unif_loc_light_2_pos_cs;
      GLint m_unif_loc_light_2_intensity;


      glm::mat4 m_perspective_mat;

      // declare a pool of renderables
      static const uint m_MAX_RENDERABLES = 10;
      Renderable m_renderables[m_MAX_RENDERABLES];
      uint m_num_current_renderables;
   };
}


#endif
