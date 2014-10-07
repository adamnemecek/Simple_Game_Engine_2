#ifndef ENGINE_GEOMETRY_LOADER_H
#define ENGINE_GEOMETRY_LOADER_H


namespace Rendering
{
   // forward declaration from different namespace
   class Geometry;
}

namespace Utility
{
   namespace Geometry_Creation
   {
      static class __declspec(dllexport) Geometry_Loader
      {
      public:
         enum GEOMETRY_LOADER_ENUMS
         {
            CUBE = 0,
         };

         static void load_from_generator(
            GEOMETRY_LOADER_ENUMS load_type,
            Rendering::Geometry &geo);
      };
   }
}

#endif
