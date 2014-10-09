#ifndef ENGINE_SHAPE_GENERATOR_H
#define ENGINE_SHAPE_GENERATOR_H


#include <Utilities\Typedefs.h>

namespace Shapes
{
   struct Shape_Data;

   namespace Geometry_Creation
   {
      // this class can create geometry primitives or more complex shapes that are mathematically defined, like the teapot
      class __declspec(dllexport) Shape_Generator
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

         // 2D shapes
         static void create_triangle(Shape_Data *put_data_here);

         static void create_cube_data(Shape_Data *put_data_here);
         static void create_plane_data(uint side_length, Shape_Data *put_data_here);

      private:
         // enforce staticness
         Shape_Generator();
         Shape_Generator(const Shape_Generator&);
         Shape_Generator &operator=(const Shape_Generator&);
      };
   }
}

#endif