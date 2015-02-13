#ifndef ENGINE_SHAPE_GENERATOR_H
#define ENGINE_SHAPE_GENERATOR_H

#include <glm\vec3.hpp>
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
         static Shape_Generator& get_instance();

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

         // create a triangle in the X-Y plane (vertical)
         // Note: It is vertical because this was the original shape that I drew to make sure that 
         // my code could draw something to the screen.
         void generate_triangle(Shape_Data *put_data_here);

         // create a plane in the X-Z plane (horizontal) at Y = 0
         // Note: X = width, Z = length
         void generate_plane(const float width, const float length, const uint width_segments, const uint length_segments, Shape_Data *put_data_here);

         // create a rectangle wire-frame in the X-Z plane (horizontal) at Y = 0
         void generate_box(const float width, const float length, Shape_Data *put_data_here);

         // create a circle in the X-Z plane (horizontal) at Y = 0
         void generate_circle(const uint num_arc_segments, const float radius, Shape_Data *put_data_here);

         // 3D shapes

         // create a cube centered on the origin
         void generate_cube(Shape_Data *put_data_here);

         // create a cylinder with top facing +Y, bottom facing -Y, and the bottom on Y = 0
         void generate_cylinder(const uint num_arc_segments, const float radius, const uint num_vertical_segments, const float height, Shape_Data *put_data_here);

         // create a sphere with the bottom on Y = 0
         void generate_sphere(const uint num_arc_segments, const float radius, const uint num_vertical_segments, Shape_Data *put_data_here);


      private:
         // enforce singlton-ness
         Shape_Generator() {}
         Shape_Generator(const Shape_Generator&);
         Shape_Generator &operator=(const Shape_Generator&);
      };
   }
}

#endif