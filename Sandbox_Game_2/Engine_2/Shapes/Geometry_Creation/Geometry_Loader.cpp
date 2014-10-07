#include "Geometry_Loader.h"

#include <Shapes\Geometry.h>
#include <Shapes\Geometry_Creation\Shape_Generator.h>

#include <glm\glm.hpp>
using glm::vec3;

// for GL typedefs
#include <Utilities\include_GL_version.h>

#include <vector>
using std::vector;


namespace Shapes
{
   namespace Geometry_Creation
   {
      void Geometry_Loader::load_from_generator(GEOMETRY_LOADER_ENUMS load_type, Geometry &geo)
      {
         switch (load_type)
         {
         case Geometry_Loader::CUBE:
         {
            Shape_Generator::create_cube_data(&(geo.m_shape_data));
            break;
         }
         default:
         {
            // ??show error? throw exception?
            break;
         }
         }

         

      }
   }
   
}