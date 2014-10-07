#ifndef ENGINE_GEOMETRY_LOADER_H
#define ENGINE_GEOMETRY_LOADER_H



namespace Shapes
{
   class Geometry;

   namespace Geometry_Creation
   {
      static class __declspec(dllexport) Geometry_Loader
      {
      public:
         enum GEOMETRY_LOADER_ENUMS
         {
            CUBE = 0,
         };

         static void load_from_generator(GEOMETRY_LOADER_ENUMS load_type, Geometry &geo);
      };
   }
}

#endif
