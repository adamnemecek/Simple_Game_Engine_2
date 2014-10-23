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
#include <Utilities\Quaternion_Helper.h>

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
      glm::vec3 new_position = m_parent_entity_ptr->m_position;
      glm::fquat final_quat = m_parent_entity_ptr->m_base_orientation;
      
      glm::vec3 forward_vector = glm::mat3(glm::mat4_cast(m_parent_entity_ptr->m_base_orientation)) * glm::vec3(0.0f, 0.0f, 1.0f);
      glm::vec3 strafe_vector = glm::cross(forward_vector, Utilities::WORLD_UP_VECTOR);
      printf("forward: <%.2f, %.2f, %.2f>, strafe: <%.2f, %.2f, %.2f>\n",
         forward_vector.x, forward_vector.y, forward_vector.z,
         strafe_vector.x, strafe_vector.y, strafe_vector.z);

      glm::vec3 relative_up_vector = glm::cross(forward_vector, strafe_vector);

      // get the list of actions that are active right now
      uint active_actions = m_key_binder_ptr->get_active_actions();

      if (active_actions & ACTION_LIST::FORWARD)
      {
         //cout << "accellerating";
         new_position += forward_vector * LINEAR_SPEED;
      }

      if (active_actions & ACTION_LIST::BACK)
      {
         //cout << ", back";
         new_position -= forward_vector * LINEAR_SPEED;
      }

      if (active_actions & ACTION_LIST::STRAFE_LEFT)
      {
         //cout << ", strafing left";
         new_position -= strafe_vector * LINEAR_SPEED;
      }

      if (active_actions & ACTION_LIST::STRAFE_RIGHT)
      {
         //cout << ", strafing right";
         new_position += strafe_vector * LINEAR_SPEED;
      }

      if (active_actions & ACTION_LIST::GO_UP)
      {
         //cout << ", going up";
         new_position += Utilities::WORLD_UP_VECTOR * LINEAR_SPEED;
      }

      if (active_actions & ACTION_LIST::GO_DOWN)
      {
         //cout << ", going up";
         new_position -= Utilities::WORLD_UP_VECTOR * LINEAR_SPEED;
      }

      if (active_actions & ACTION_LIST::ROTATE_LEFT)
      {
         //cout << ", rotating left";
         //forward_vector = glm::mat3(glm::rotate((-1.0f) * ROTATION_SPEED, relative_up_vector)) * forward_vector;
         Utilities::offset_orientation(
            Utilities::WORLD_UP_VECTOR, 
            (-1.0f) * ROTATION_SPEED, 
            final_quat);
      }

      if (active_actions & ACTION_LIST::ROTATE_RIGHT)
      {
         //cout << ", rotating right";
         //forward_vector = glm::mat3(glm::rotate(ROTATION_SPEED, relative_up_vector)) * forward_vector;
         Utilities::offset_orientation(
            Utilities::WORLD_UP_VECTOR,
            ROTATION_SPEED,
            final_quat);
      }

      if (active_actions & ACTION_LIST::TILT_FORWARD)
      {
         //cout << ", tilting forward";
         //forward_vector = glm::mat3(glm::rotate((-1.0f) * ROTATION_SPEED, strafe_vector)) * forward_vector;
         Utilities::offset_orientation(
            strafe_vector,
            (-1.0f) * ROTATION_SPEED,
            final_quat);
      }

      if (active_actions & ACTION_LIST::TILT_BACK)
      {
         //cout << ", tilting back";
         //forward_vector = glm::mat3(glm::rotate(ROTATION_SPEED, strafe_vector)) * forward_vector;
         Utilities::offset_orientation(
            strafe_vector,
            ROTATION_SPEED,
            final_quat);
      }

      if (active_actions != 0)
      {
         //cout << endl;
      }

      m_parent_entity_ptr->m_position = new_position;
      //m_parent_entity_ptr->m_base_orientation = glm::fquat(glm::normalize(forward_vector));
      m_parent_entity_ptr->m_base_orientation = glm::normalize(final_quat);
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