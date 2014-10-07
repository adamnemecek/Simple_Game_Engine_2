#ifndef ENGINE_SHAPE_GENERATOR_H
#define ENGINE_SHAPE_GENERATOR_H

#include <vector>
#include <glm\glm.hpp>

// for GL typedefs
#include <Utilities\include_GL_version.h>

namespace Utility
{
   namespace Geometry_Creation
   {
      class Geometry;

      // this class can create geometry primitives or more complex shapes that are mathematically defined, like the teapot
      // Note: It is a static class because it never requires an instance
      static class __declspec(dllexport) Shape_Generator
      {
      public:
         // the order of the returned vertices is as follows:
         // - position (3 floats)
         // - normal (3 floats)
         // - color (3 floats)
         // Note: Only the geometry loader can put data into the geometry class.  This class can 
         // only create the geometry.  It is therefore necessary to create geometry data and send 
         // it to the geometry loader.  I am doing this by returning a vector with the necessary 
         // information.  There will be lots of copying, especially with the mathematically
         // generatoed shapes liek the torus and the teapot.  It is the geometry loader's 
         // responsibility to extract data from this returned object and then let it terminate to 
         // free up memory.


         static void create_cube_data(std::vector<glm::vec3> &put_vertex_data_here, std::vector<GLushort> &put_index_data_here);
      };
   }
}

#endif