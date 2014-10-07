#ifndef ENGINE_GEOMETRY
#define ENGINE_GEOMETRY

#include <Utilities\Typedefs.h>

// for storing the geometry data
#include <Shapes\Shape_Data.h>

// for cleaning up openGL objects that are unique for each geometry object
#include <Utilities\include_GL_version.h>


namespace Rendering
{
   class Renderer;
}

namespace Shapes
{
   namespace Geometry_Creation
   {
      // forward declaration from different namespace
      class Geometry_Loader;
   }

   // this is a data storage class for the renderer
   class __declspec(dllexport) Geometry
   {
      // for cleaning up when this object is destroyed
      ~Geometry()
      {
         glDeleteVertexArrays(1, &m_VAO_ID);
         glDeleteBuffers(1, &m_buffer_ID);
      }

      // let everything be private, but let the geometry loader and the renderer access 
      // everything (it's easier that way)
      // Note: In this design, the renderer could set things, but be nice to the program 
      // and don't exploit this permission loophole.
   private:
      friend class Geometry_Creation::Geometry_Loader;
      friend class Rendering::Renderer;

      GLuint m_VAO_ID;
      GLuint m_buffer_ID;
      GLenum m_render_mode;

      // it makes sense to contain the vertex and index data in the geometry class itself
      // Note: This member will clean itself up upon destruction.
      Shape_Data m_shape_data;
   };
}

#endif
