#ifndef ENGINE_GEOMETRY_LOADER_H
#define ENGINE_GEOMETRY_LOADER_H

#include <Utilities\Typedefs.h>
#include <glm\vec3.hpp>

namespace Shapes
{
   class Geometry;

   namespace Geometry_Creation
   {
      class __declspec(dllexport) Geometry_Loader
      {
      public:
         enum GEOMETRY_PRIMITIVE_ENUMS
         {
            CUBE = 0,
            PLANE,
            TRIANGLE,
            CIRCLE,
            RECTANGLE,
         };

         static void load_cube(Geometry *put_geometry_here);
         static void load_plane(Geometry *put_geometry_here, const uint num_unit_segments_on_a_side = 5);
         static void load_triangle(Geometry *put_geometry_here);
         static void load_circle(Geometry *put_geometry_here, const uint num_arc_segments = 16, const float radius = 1.0f);
         static void load_rectangle(Geometry *put_geometry_here, const float width = 1.0f, const float length = 1.0f);


      private:
         // enforce staticness
         Geometry_Loader();
         Geometry_Loader(const Geometry_Loader&);
         Geometry_Loader &operator=(const Geometry_Loader&);

         // helper function is private
         static void initialize_attributes(Geometry *geo);
      };
   }
}

#endif
