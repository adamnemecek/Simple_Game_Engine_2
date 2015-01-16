#ifndef ENGINE_GEOMETRY
#define ENGINE_GEOMETRY

#include <Utilities\Typedefs.h>

// for storing the geometry data and meta data
#include <Shapes\Shape_Data.h>
#include <Shapes\Geometry_Meta_Data.h>

// for cleaning up openGL objects that are unique for each geometry object
#include <Utilities\Include_Helpers\GL_Version.h>


// forward declarations
namespace Rendering
{
   class Renderer;
}

namespace Entities
{
   class AABB_Component;
   class Physics_Component;
}

namespace Shapes
{
   namespace Geometry_Creation
   {
      class Geometry_Loader;
   }
}

namespace Shapes
{
   // this is a data storage class for the renderer
   class __declspec(dllexport) Geometry
   {
   public:
      // public destructor for cleaning up when this object is destroyed
      ~Geometry()
      {
         glDeleteVertexArrays(1, &m_VAO_ID);
         glDeleteBuffers(1, &m_vertex_buffer_ID);
         glDeleteBuffers(1, &m_element_buffer_ID);
      }

   private:
      // let everything else be private, but let a select few classes access the private data 
      // Ex: Geometry_Loader needs to set the vertices, the bounding box needs to read the 
      // meta data, and the Renderer needs to get at VAO and buffer IDs.
      // Note: In this design, these classes could set things, but be nice to the program 
      // and don't exploit this permission loophole.
      friend class Entities::AABB_Component;
      friend class Entities::Physics_Component;
      friend class Geometry_Creation::Geometry_Loader;
      friend class Rendering::Renderer;

      // these will be cleaned up by the destructor
      GLuint m_VAO_ID;
      GLuint m_vertex_buffer_ID;
      GLuint m_element_buffer_ID;

      // it makes sense to contain the vertex and index data in the geometry class itself
      // Note: This member will clean itself up upon destruction.
      Shape_Data m_shape_data;

      // it also makes sense to store the geometry meta data in the geometry class itself,
      // but the geometry loader will need to call it's analyze(...) function
      Geometry_Meta_Data m_meta_data;

      //GLenum m_render_mode;
   };
}

#endif
