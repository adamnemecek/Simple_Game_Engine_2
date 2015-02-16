//#ifndef ENGINE_SCENE_LOADER_H
//#define ENGINE_SCENE_LOADER_H
//
//// forward declaration
//namespace Rendering
//{
//   class Renderer;
//}
//
//namespace Scene
//{
//   class Scene_Data;
//}
//
//namespace Scene
//{
//   // make it a singleton
//   class __declspec(dllexport) Scene_Loader
//   {
//   public:
//      static Scene_Loader& get_instance();
//
//      bool load_scene(Rendering::Renderer *renderer_ptr, Scene::Scene_Data *load_scene_data_here);
//
//   private:
//      Scene_Loader() {}
//      Scene_Loader(const Scene_Loader&);
//      Scene_Loader& operator=(const Scene_Loader&);
//   };
//}
//
//
//#endif