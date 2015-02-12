#ifndef ENGINE_SCENE_LOADER_H
#define ENGINE_SCENE_LOADER_H


namespace Scene
{
   // make it a singleton
   class __declspec(dllexport) Scene_Loader
   {
   public:
      static Scene_Loader& get_instance();

      bool load_scene();

   private:
      Scene_Loader() {}
      Scene_Loader(const Scene_Loader&);
      Scene_Loader& operator=(const Scene_Loader&);
   };
}


#endif