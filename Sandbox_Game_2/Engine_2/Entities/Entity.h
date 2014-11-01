#ifndef ENGINE_Entity_H
#define ENGINE_Entity_H

#include <glm\vec3.hpp>
#include <glm\mat4x4.hpp>
//#include <Utilities\Include_Helpers\GLM_Quaternion.h>
#include <Utilities\Include_Helpers\GLM_Dual_Quaternion.h>
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

      // add a derivation of the Game_Component to this class
      // Note: The derived component will not be stored here.  Only a pointer is stored in this class.
      void add_component(Game_Component *component_ptr);

      // goes through all components and runs their update() function
      void update();

      // this is a template function so that a component can get at one of its siblings, such as
      // the border handler component needing to update the velocity, which is in the physics component
      // Note: This function is public 
      template<class T>
      T *get_component_ptr() const;

      // shared by all entities, so make it public to make it simple to manipulate
      //glm::vec3 m_position;

      // Note: I call it a "base" orientation because sub parts of the entity may be oriented differently.
      //glm::vec3 m_base_orientation;
      //glm::fquat m_base_orientation_quat;

      // use a dual quaternion for both orientation and position so that we only have to multiply a 
      // single item when transforming child entities
      glm::fdualquat m_where_and_which_way;

   protected:
      // these are protected instead of private so that the Controllable_Entity can also access them
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


