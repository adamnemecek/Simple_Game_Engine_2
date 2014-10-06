#ifndef ENGINE_GEOMETRY
#define ENGINE_GEOMETRY

#include <Misc\Typedefs.h>
#include <glm\glm.hpp>

namespace Rendering
{
   // this is a helper data storage class for the renderer
   class Geometry
   {
      // let everything be private, but let the renderer access things
      friend class Renderer;

      const glm::vec3 *verts_ptr;
      uint num_verts;
      const glm::vec3 *normal_verts_ptr;
      uint num_normal_verts;
      const ushort *indices;
      uint num_indices;

      GLenum render_mode;
   };
}

#endif
