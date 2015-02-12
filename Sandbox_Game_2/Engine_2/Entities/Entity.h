#ifndef ENGINE_Entity_H
#define ENGINE_Entity_H

#include <glm\vec3.hpp>
#include <Utilities\Typedefs.h>
#include <Math\F_Dual_Quat.h>
#include <string>


namespace Entities
{
   class Game_Component;

   class __declspec(dllexport) Entity
   {
   public:
      Entity(const std::string& new_ID);
      
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

      // use a dual quaternion for both orientation and position so that we only have to multiply a 
      // single item when transforming child entities
      Math::F_Dual_Quat m_where_and_which_way;

      // this setter is not an unecessary exposure of a private member because I only want to
      // expose the assignment operation
      void set_id(const std::string& new_id);
      const std::string& get_id();


   private:
      // hide the default constructor so that the user is forced to use the one with an ID 
      // specifier
      Entity() {}

      // this will need to be accessed during scene loading and saving, so
      // go ahead and make this public instead of bothering with friend classes or 
      // getters/setters
      // Note: This is not const because I want the user to be able to change the entity name.
      // Also, const-ness created disabled the creation of a default assignment operator and 
      // made writing my own impossible.
      std::string m_id;

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


