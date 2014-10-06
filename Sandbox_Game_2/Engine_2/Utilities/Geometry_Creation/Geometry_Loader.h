#ifndef ENGINE_GEOMETRY_LOADER_H
#define ENGINE_GEOMETRY_LOADER_H


namespace Utility
{
   class __declspec(dllexport) Geometry_Loader
   {
   public:
      static Geometry_Loader &get_instance();


   private:
      Geometry_Loader();
      Geometry_Loader(const Geometry_Loader &);
      Geometry_Loader &operator=(const Geometry_Loader &);
   };
}

#endif
