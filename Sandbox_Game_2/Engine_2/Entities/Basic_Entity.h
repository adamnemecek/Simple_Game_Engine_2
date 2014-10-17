#ifndef ENGINE_BASIC_ENTITY_H
#define ENGINE_BASIC_ENTITY_H

#include <glm\vec3.hpp>
#include <Utilities\Typedefs.h>

namespace Entities
{
   class Game_Component;

   class __declspec(dllexport) Basic_Entity
   {
   public:
      // for initializing non-static-const members
      Basic_Entity();

      bool initialize();
      bool shutdown();

      // this is vitual so that the derived Controllable_Entity can add controller and camera components
      virtual void add_component(Game_Component *component_ptr);

      // goes through all components and runs their update() function
      void update();

      // this is a template function so that a component can get at one of its siblings, such as
      // the border handler component needing to update the velocity, which is in the physics component
      // Note: This function is public 
      template<class T>
      T *get_component_ptr() const;

      // shared by all entities, so just make it public
      glm::vec3 m_position;

      // shared by all entities, so just make it public
      // Note: I call it a "base" orientation because sub parts of the entity may be oriented differently.
      glm::vec3 m_base_orientation;

   protected:
      // these are protected instead of private so that the Controllable_Entity can also access them
      static const uint m_MAX_COMPONENTS = 10;
      uint m_num_current_components;
      Game_Component *m_components[m_MAX_COMPONENTS];
   };

   // define the "get component" function in the header so that it is implicitly inline
   template<class T>
   T *Basic_Entity::get_component_ptr() const
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


