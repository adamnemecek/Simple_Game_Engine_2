#ifndef ENGINE_GEOMETRY_LOADER_H
#define ENGINE_GEOMETRY_LOADER_H



namespace Shapes
{
   class Geometry;

   namespace Geometry_Creation
   {
      class __declspec(dllexport) Geometry_Loader
      {
      public:
         enum GEOMETRY_LOADER_ENUMS
         {
            CUBE = 0,
            PLANE,
         };

         static void load_from_generator(GEOMETRY_LOADER_ENUMS load_type, Geometry &geo);

      private:
         // enforce staticness
         Geometry_Loader();
         Geometry_Loader(const Geometry_Loader&);
         Geometry_Loader &operator=(const Geometry_Loader&);
      };
   }
}

#endif
