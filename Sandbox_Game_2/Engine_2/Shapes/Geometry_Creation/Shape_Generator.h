#ifndef ENGINE_SHAPE_GENERATOR_H
#define ENGINE_SHAPE_GENERATOR_H

#include <glm\vec3.hpp>
#include <Utilities\Typedefs.h>
#include <memory>

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
         // Note: This class creates the data necessary for geometry, but only the geometry 
         // loader, which handles OpenGL VBOs and vertex attribues, can put data into the 
         // geometry class.  I am doing this by returning a pointer to a sructure with the 
         // necessary information.  It is the geometry loader's responsibility to extract data 
         // from this returned object and then let it terminate to free up memory.

         // 2D shapes

         // create a triangle in the X-Y plane (vertical)
         // Note: It is vertical because this was the original shape that I drew to make sure that 
         // my code could draw something to the screen.
         std::shared_ptr<Shape_Data> generate_triangle();

         // create a plane in the X-Z plane (horizontal) at Y = 0
         // Note: X = width, Z = length
         std::shared_ptr<Shape_Data> generate_plane(const float width, const float length, const uint width_segments, const uint length_segments);

         // create a rectangle wire-frame in the X-Z plane (horizontal) at Y = 0
         std::shared_ptr<Shape_Data> generate_box(const float width, const float length);

         // create a circle in the X-Z plane (horizontal) at Y = 0
         //std::shared_ptr<Shape_Data>
         std::shared_ptr<Shape_Data> generate_circle(const uint num_arc_segments, const float radius);

         // 3D shapes

         // create a cube centered on the origin
         std::shared_ptr<Shape_Data> generate_cube();

         // create a cylinder with top facing +Y, bottom facing -Y, and the bottom on Y = 0
         std::shared_ptr<Shape_Data> generate_cylinder(const uint num_arc_segments, const float radius, const uint num_vertical_segments, const float height);

         // create a sphere with the bottom on Y = 0
         std::shared_ptr<Shape_Data> generate_sphere(const uint num_arc_segments, const float radius, const uint num_vertical_segments);


      private:
         // enforce singlton-ness
         Shape_Generator() {}
         Shape_Generator(const Shape_Generator&);
         Shape_Generator &operator=(const Shape_Generator&);
      };
   }
}

#endif