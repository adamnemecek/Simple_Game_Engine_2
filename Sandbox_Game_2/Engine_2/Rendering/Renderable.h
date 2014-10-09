#ifndef ENGINE_RENDERABLE
#define ENGINE_RENDERABLE

#include <glm\mat4x4.hpp>

// for GL typedefs
#include <Utilities\include_GL_version.h>

namespace Shapes
{
   // forward declaration of Geometry class
   class Geometry;
}

namespace Rendering
{
   // a renderable is a simple data store and is a combination of geometry and 
   // location/orientation
   class __declspec(dllexport) Renderable
   {
   public:
      // these will be updated on every frame, so just make them public
      glm::mat4 m_model_to_world_mat;
      glm::mat4 m_orientation_only_mat;

   private:
      // let geometry loader, but no one else, have access to the geometry
      friend class Renderer;

      Shapes::Geometry *m_geometry_ptr;
   };
}

#endif
