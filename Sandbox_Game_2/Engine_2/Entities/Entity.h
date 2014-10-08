#ifndef ENGINE_ENTITY_H
#define ENGINE_ENTITY_H

#include <glm\vec3.hpp>
#include <Utilities\Typedefs.h>

namespace Entities
{
   class Game_Component;

   class __declspec(dllexport) Entity
   {
   public:
      // for initializing non-static-const members
      Entity();

      bool initialize();
      bool shutdown();

      void add_component(Game_Component *component_ptr);

      // goes through all components and runs their update() function
      void update();

      // this is a template function so that a component can get at one of its siblings
      // Note: The border handler, for example, needs to update the velocity, which is in the physics component
      template<class T>
      T *get_component_ptr() const;

      // these components/values are shared by all entities, and they will likely need to be
      // updated by some other component, so just make them public
      glm::vec3 position;

   private:
      static const uint m_MAX_COMPONENTS = 10;
      uint m_num_current_components;
      Game_Component *m_components[m_MAX_COMPONENTS];
   };

   // define the "get component" function in the header so that it is implicitly inline
   template<class T>
   T *Entity::get_component_ptr() const
   {
      for (uint index = 0; index < m_num_current_components; index++)
      {
         // downcast the component to the requested class
         T *ret = dynamic_cast<T*>(m_components[index]);

         if (ret != 0)
         {
            return ret;
         }
      }

      // could not downcast any component to the requested type
      return 0;
   }
}

#endif


