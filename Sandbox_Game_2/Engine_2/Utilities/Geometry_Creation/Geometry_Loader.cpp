#include "Geometry_Loader.h"

#include <Rendering\Geometry.h>
using Rendering::Geometry;

#include <Utilities\Geometry_Creation\Shape_Generator.h>

#include <glm\glm.hpp>
using glm::vec3;

// for GL typedefs
#include <Utilities\include_GL_version.h>

#include <vector>
using std::vector;


namespace Utility
{
   namespace Geometry_Creation
   {
      void Geometry_Loader::load_from_generator(GEOMETRY_LOADER_ENUMS load_type, Rendering::Geometry &geo)
      {
         vector<vec3> vertex_data;
         vector<GLushort> index_data;

         switch (load_type)
         {
         case Geometry_Loader::CUBE:
         {
            Shape_Generator::create_cube_data(vertex_data, index_data);
            break;
         }
         default:
         {
            // ??show error? throw exception?
            break;
         }
         }

         // look at the shape generator's header to see how the vectors are organized into position, normal, and color
         for (size_t index = 0; index < vertex_data.size(); index += 3)
         {
            geo.m_verts.push_back(vertex_data[index + 0]);
            geo.m_normals.push_back(vertex_data[index + 1]);
            geo.m_colors.push_back(vertex_data[index + 2]);
         }

         for (size_t index = 0; index < index_data.size(); index++)
         {
            geo.m_indices.push_back(index_data[index]);
         }


      }
   }
   
}