#ifndef ENGINE_GEOMETRY
#define ENGINE_GEOMETRY

#include <Utilities\Typedefs.h>

// for storing the geometry data and meta data
#include <Shapes\Shape_Data.h>
#include <Shapes\Shape_Meta_Data.h>

// for cleaning up openGL objects that are unique for each geometry object
#include <Utilities\Include_Helpers\GL_Version.h>


// forward declarations
namespace Rendering
{
   class Renderable;
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
      // these will be cleaned up by the destructor
      GLuint m_VAO_ID;
      GLuint m_vertex_buffer_ID;
      GLuint m_element_buffer_ID;

      // the vertex and index data of the shape
      Shape_Data m_shape_data;

      // shape-specific meta data, like min/max on X, Y, and Z
      Shape_Meta_Data m_meta_data;

      // The AABB needs access to the geometry's meta data, which includes min/max values 
      // on all axes
      friend class Entities::AABB_Component;

      // The geometry loader needs access to fill out these private fields
      friend class Geometry_Creation::Geometry_Loader;

      // the Renderable needs access to these to selectively hand them off to the Renderer 
      // for VAO binding and index collections and draw commands
      friend class Rendering::Renderable;
   };
}

#endif
