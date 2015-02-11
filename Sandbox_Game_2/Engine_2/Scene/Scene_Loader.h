#ifndef ENGINE_SCENE_LOADER_H
#define ENGINE_SCENE_LOADER_H


namespace Scene
{
   // make it a singleton
   class __declspec(dllexport) Scene_Loader
   {
   public:
      bool load_scene();

   private:
      Scene_Loader() {}

   };
}


#endif