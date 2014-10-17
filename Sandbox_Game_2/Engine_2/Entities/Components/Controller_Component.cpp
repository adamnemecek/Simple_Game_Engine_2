#include "Controller_Component.h"

// for setting a particular key binding
#include <Input\Supported_Bindings.h>
#include <Input\Key_Bindings\Keyboard_Bindings.h>

// for getting at the physics component
#include <Entities\Entity.h>
#include <Entities\Components\Physics_Component.h>

// for figuring out which actions are active
#include <Input\Action_List.h>

#include <Utilities\Typedefs.h>
#include <glm\vec3.hpp>
#include <Utilities\Include_Helper_WORLD_UP_VECTOR.h>
#include <Utilities\Include_Helper_GLM_Mat_Transform.h>
#include <glm\gtx\transform.hpp>

#include <cassert>

#include <iostream>
using std::cout;
using std::endl;

namespace Entities
{
   bool Controller_Component::initialize()
   {
      m_easy_physics_ptr = m_parent_entity_ptr->get_component_ptr<Physics_Component>();
      return (m_easy_physics_ptr != 0);
   }

   void Controller_Component::update()
   {
      using Input::ACTION_LIST;
      assert(m_key_binder_ptr != 0);
      //assert(m_easy_physics_ptr != 0);

      static const float LINEAR_SPEED = 0.3f;
      static const float ROTATION_SPEED = 0.02f;

      // define these to avoid lots of repetious dereferencing
      glm::vec3 position = m_parent_entity_ptr->m_position;
      glm::vec3 orientation = m_parent_entity_ptr->m_base_orientation;
      glm::vec3 strafe_vector = glm::cross(orientation, Utilities::WORLD_UP_VECTOR);
      glm::vec3 relative_up_vector = glm::cross(orientation, strafe_vector);

      // get the list of actions that are active right now
      uint active_actions = m_key_binder_ptr->get_active_actions();

      if (active_actions & ACTION_LIST::FORWARD)
      {
         //cout << "accellerating";
         position += orientation * LINEAR_SPEED;
      }

      if (active_actions & ACTION_LIST::BACK)
      {
         //cout << ", back";
         position -= orientation * LINEAR_SPEED;
      }

      if (active_actions & ACTION_LIST::STRAFE_LEFT)
      {
         //cout << ", strafing left";
         position -= strafe_vector * LINEAR_SPEED;
      }

      if (active_actions & ACTION_LIST::STRAFE_RIGHT)
      {
         //cout << ", strafing right";
         position += strafe_vector * LINEAR_SPEED;
      }

      if (active_actions & ACTION_LIST::ROTATE_LEFT)
      {
         //cout << ", rotating left";
         orientation = glm::mat3(glm::rotate((-1.0f) * ROTATION_SPEED, relative_up_vector)) * orientation;
      }

      if (active_actions & ACTION_LIST::ROTATE_RIGHT)
      {
         //cout << ", rotating right";
         orientation = glm::mat3(glm::rotate(ROTATION_SPEED, relative_up_vector)) * orientation;
      }

      if (active_actions & ACTION_LIST::TILT_FORWARD)
      {
         //cout << ", tilting forward";
         orientation = glm::mat3(glm::rotate((-1.0f) * ROTATION_SPEED, strafe_vector)) * orientation;
      }

      if (active_actions & ACTION_LIST::TILT_BACK)
      {
         //cout << ", tilting back";
         orientation = glm::mat3(glm::rotate(ROTATION_SPEED, strafe_vector)) * orientation;
      }

      if (active_actions != 0)
      {
         cout << endl;
      }

      m_parent_entity_ptr->m_position = position;
      m_parent_entity_ptr->m_base_orientation = glm::normalize(orientation);
   }

   bool Controller_Component::set_key_binding(const Input::SUPPORTED_BINDINGS binding)
   {
      using Input::SUPPORTED_BINDINGS;

      switch (binding)
      {
      case SUPPORTED_BINDINGS::KEYBOARD:
      {
         m_key_binder_ptr = Input::Keyboard_Bindings::get_instance_ptr();
         break;
      }
      default:
      {
         // unknown binding
         return false;
      }
      }

      return true;
   }
}