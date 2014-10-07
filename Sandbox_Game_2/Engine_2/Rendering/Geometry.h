#ifndef ENGINE_GEOMETRY
#define ENGINE_GEOMETRY

#include <Utilities\Typedefs.h>

// for vec
#include <glm\glm.hpp>

// for GL typedefs
#include <Utilities\include_GL_version.h>

// for storing the geometry
#include <vector>


namespace Utility
{
   namespace Geometry_Creation
   {
      // forward declaration from different namespace
      class Geometry_Loader;
   }
}

namespace Rendering
{
   // this is a data storage class for the renderer
   class Geometry
   {
      // let everything be private, but let the geometry loader and the renderer access 
      // everything (it's easier that way)
      // Note: In this design, the renderer could set things, but be nice to the program 
      // and don't exploit this permission loophole.
   private:
      friend class Utility::Geometry_Creation::Geometry_Loader;
      friend class Renderer;

      GLuint m_VAO_ID;
      GLuint m_buffer_ID;

      // it makes sense to contain the vertex and index data in the geometry class itself,
      // and we want good resource management, so just use the std::vector
      std::vector<glm::vec3> m_verts;
      std::vector<glm::vec3> m_normals;
      std::vector<glm::vec3> m_colors;
      std::vector<GLushort> m_indices;

      GLenum m_render_mode;
   };
}

#endif
