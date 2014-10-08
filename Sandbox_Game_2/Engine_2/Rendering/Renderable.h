#ifndef ENGINE_RENDERABLE
#define ENGINE_RENDERABLE

// for vec
#include <glm\vec3.hpp>

// for GL typedefs
#include <Utilities\include_GL_version.h>

namespace Rendering
{
   // forward declaration of Geometry class
   class Geometry;

   // a renderable is a simple data store and is a combination of geometry and 
   // location/orientation
   class __declspec(dllexport) Renderable
   {
   public:
      // this will be updated on every frame, so just make it public
      glm::vec3 where;

   private:
      // let geometry loader, but no one else, have access to the geometry
      friend class Renderer;

      Geometry *what;
   };
}

#endif
