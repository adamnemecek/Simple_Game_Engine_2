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
#include <Utilities\Include_Helpers\Default_Vectors.h>
#include <Utilities\My_Assert.h>
#include <Math\F_Dual_Quat.h>

#include <Utilities\Printer_Helper.h>
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
      MY_ASSERT(m_key_binder_ptr != 0);

      static const float LINEAR_SPEED = 0.3f;
      static const float ROTATION_SPEED = 0.02f;

      // position adjustment will be additive, so start the relative new position at all zeros
      glm::vec3 new_position;

      // orientation adjustment will be multiplicitive with a quaternion, so start it at one
      // so that the first orientation adjustment will simply be itself
      Math::F_Quat new_orientation(1.0f, glm::vec3());


      glm::vec3 position_adjustment(0.0f);
      Math::F_Dual_Quat temp_dq(new_orientation, Math::F_Quat());


      // define these to avoid lots of repetious dereferencing
      glm::vec3 forward_vector = Utilities::Default_Vectors::WORLD_FORWARD_VECTOR;
      glm::vec3 left_vector = Utilities::Default_Vectors::WORLD_LEFT_VECTOR;
      glm::vec3 relative_up = Utilities::Default_Vectors::WORLD_UP_VECTOR;

         // get the list of actions that are active right now
      uint active_actions = m_key_binder_ptr->get_active_actions();

      if (active_actions & ACTION_LIST::FORWARD)
      {
         new_position += forward_vector * LINEAR_SPEED;

         position_adjustment = forward_vector * LINEAR_SPEED;
         temp_dq *= Math::F_Dual_Quat::generate_translate_only(position_adjustment);

         //cout<< "forward";
      }

      if (active_actions & ACTION_LIST::BACK)
      {
         new_position -= forward_vector * LINEAR_SPEED;

         position_adjustment = -forward_vector * LINEAR_SPEED;
         temp_dq *= Math::F_Dual_Quat::generate_translate_only(position_adjustment);

         //cout<< "-back";
      }

      if (active_actions & ACTION_LIST::STRAFE_LEFT)
      {
         new_position += left_vector * LINEAR_SPEED;

         position_adjustment = left_vector * LINEAR_SPEED;
         temp_dq *= Math::F_Dual_Quat::generate_translate_only(position_adjustment);

         //cout<< "-left";
      }

      if (active_actions & ACTION_LIST::STRAFE_RIGHT)
      {
         new_position -= left_vector * LINEAR_SPEED;

         position_adjustment = -left_vector * LINEAR_SPEED;
         temp_dq *= Math::F_Dual_Quat::generate_translate_only(position_adjustment);

         //cout<< "-right";
      }

      if (active_actions & ACTION_LIST::GO_UP)
      {
         new_position += relative_up * LINEAR_SPEED;

         position_adjustment = relative_up * LINEAR_SPEED;
         temp_dq *= Math::F_Dual_Quat::generate_translate_only(position_adjustment);

         //cout<< "-up";
      }

      if (active_actions & ACTION_LIST::GO_DOWN)
      {
         new_position -= relative_up * LINEAR_SPEED;

         position_adjustment = -relative_up * LINEAR_SPEED;
         temp_dq *= Math::F_Dual_Quat::generate_translate_only(position_adjustment);

         //cout<< "-down";
      }

      if (active_actions & ACTION_LIST::YAW_LEFT)
      {
         //new_orientation = Math::F_Quat::generate_rotator(relative_up, -ROTATION_SPEED) * new_orientation;
         new_orientation *= Math::F_Quat::generate_rotator(relative_up, -ROTATION_SPEED);

         temp_dq *= Math::F_Dual_Quat::generate_rotator_only(relative_up, -ROTATION_SPEED);

         //cout<< "-yaw left";
      }

      if (active_actions & ACTION_LIST::YAW_RIGHT)
      {
         //new_orientation = Math::F_Quat::generate_rotator(relative_up, +ROTATION_SPEED) * new_orientation;
         new_orientation *= Math::F_Quat::generate_rotator(relative_up, +ROTATION_SPEED);

         temp_dq *= Math::F_Dual_Quat::generate_rotator_only(relative_up, +ROTATION_SPEED);

         //cout<< "-yaw right";
      }

      if (active_actions & ACTION_LIST::PITCH_FORWARD)
      {
         //new_orientation = Math::F_Quat::generate_rotator(left_vector, +ROTATION_SPEED) * new_orientation;
         new_orientation *= Math::F_Quat::generate_rotator(left_vector, -ROTATION_SPEED);

         temp_dq *= Math::F_Dual_Quat::generate_rotator_only(left_vector, -ROTATION_SPEED);

         //cout<< "-pitch forw";
      }

      if (active_actions & ACTION_LIST::PITCH_BACK)
      {
         //new_orientation = Math::F_Quat::generate_rotator(left_vector, -ROTATION_SPEED) * new_orientation;
         new_orientation *= Math::F_Quat::generate_rotator(left_vector, +ROTATION_SPEED);

         temp_dq *= Math::F_Dual_Quat::generate_rotator_only(left_vector, +ROTATION_SPEED);

         //cout<< "-pitch back";
      }

      if (active_actions & ACTION_LIST::ROLL_LEFT)
      {
         //new_orientation = Math::F_Quat::generate_rotator(forward_vector, +ROTATION_SPEED) * new_orientation;
         new_orientation *= Math::F_Quat::generate_rotator(forward_vector, +ROTATION_SPEED);

         temp_dq *= Math::F_Dual_Quat::generate_rotator_only(forward_vector, +ROTATION_SPEED);

         //cout<< "-roll left";
      }

      if (active_actions & ACTION_LIST::ROLL_RIGHT)
      {
         //new_orientation = Math::F_Quat::generate_rotator(forward_vector, -ROTATION_SPEED) * new_orientation;
         new_orientation *= Math::F_Quat::generate_rotator(forward_vector, -ROTATION_SPEED);

         temp_dq *= Math::F_Dual_Quat::generate_rotator_only(forward_vector, -ROTATION_SPEED);

         //cout<< "-roll right";
      }

      //cout<< endl;

      // construct the translational part of the transformation
      Math::F_Quat new_dual = 0.5f * Math::F_Quat::generate_pure_quat(new_position) * new_orientation;
      
      Math::F_Dual_Quat prev_state(m_parent_entity_ptr->m_where_and_which_way);
      Math::F_Dual_Quat new_state(new_orientation, new_dual);

      Math::F_Dual_Quat cumulative_state = new_state * prev_state;

      if (prev_state != cumulative_state)
      {
         glm::vec3 V;
         glm::vec3 transformed_V = Math::F_Dual_Quat::transform(cumulative_state, V);
         Utilities::Printer_Helper::print_vec("V: ", transformed_V);
      }


      //m_parent_entity_ptr->m_where_and_which_way = new_state * prev_state;
      
      //m_parent_entity_ptr->m_where_and_which_way = prev_state * temp_dq;
      m_parent_entity_ptr->m_where_and_which_way = temp_dq * prev_state;
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