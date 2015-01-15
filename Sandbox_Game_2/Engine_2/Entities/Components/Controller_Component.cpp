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


namespace Entities
{
   bool Controller_Component::initialize()
   {
      m_easy_physics_ptr = m_parent_entity_ptr->get_component_ptr<Physics_Component>();
      //return (m_easy_physics_ptr != 0);
      return true;
   }

   void Controller_Component::update()
   {
      using Input::ACTION_LIST;
      MY_ASSERT(m_key_binder_ptr != 0);

      static const float LINEAR_SPEED = 0.3f;
      static const float ROTATION_SPEED = 0.02f;

      // position adjustment will be additive, so start the relative new position at all zeros
      // Note: Use a vec3 rather than a dual quaternion because this is much cheaper to 
      // constructor and change in the following condition statements.  I'll construct the
      // dual part of the full transformation AFTER the conditionals are all finished.
      glm::vec3 new_position;

      // orientation adjustment will be multiplicitive with a quaternion, so start it at one
      // so that the first orientation adjustment will simply be itself
      // Note: Remember that the usual multiplication order is NEWEST * NEW * OLD * OLDER.
      Math::F_Quat new_orientation(1.0f, glm::vec3());


      // define these to avoid lots of repetious dereferencing
      glm::vec3 forward_vector = Utilities::Default_Vectors::WORLD_FORWARD_VECTOR;
      glm::vec3 left_vector = Utilities::Default_Vectors::WORLD_LEFT_VECTOR;
      glm::vec3 relative_up = Utilities::Default_Vectors::WORLD_UP_VECTOR;

         // get the list of actions that are active right now
      uint active_actions = m_key_binder_ptr->get_active_actions();

      if (active_actions & ACTION_LIST::FORWARD)
      {
         new_position += forward_vector * LINEAR_SPEED;
      }

      if (active_actions & ACTION_LIST::BACK)
      {
         new_position -= forward_vector * LINEAR_SPEED;
      }

      if (active_actions & ACTION_LIST::STRAFE_LEFT)
      {
         new_position += left_vector * LINEAR_SPEED;
      }

      if (active_actions & ACTION_LIST::STRAFE_RIGHT)
      {
         new_position -= left_vector * LINEAR_SPEED;
      }

      if (active_actions & ACTION_LIST::GO_UP)
      {
         new_position += relative_up * LINEAR_SPEED;
      }

      if (active_actions & ACTION_LIST::GO_DOWN)
      {
         new_position -= relative_up * LINEAR_SPEED;
      }

      if (active_actions & ACTION_LIST::YAW_LEFT)
      {
         new_orientation *= Math::F_Quat::generate_rotator(relative_up, +ROTATION_SPEED);
      }

      if (active_actions & ACTION_LIST::YAW_RIGHT)
      {
         new_orientation *= Math::F_Quat::generate_rotator(relative_up, -ROTATION_SPEED);
      }

      if (active_actions & ACTION_LIST::PITCH_FORWARD)
      {
         new_orientation *= Math::F_Quat::generate_rotator(left_vector, +ROTATION_SPEED);
      }

      if (active_actions & ACTION_LIST::PITCH_BACK)
      {
         new_orientation *= Math::F_Quat::generate_rotator(left_vector, -ROTATION_SPEED);
      }

      if (active_actions & ACTION_LIST::ROLL_LEFT)
      {
         new_orientation *= Math::F_Quat::generate_rotator(forward_vector, -ROTATION_SPEED);
      }

      if (active_actions & ACTION_LIST::ROLL_RIGHT)
      {
         new_orientation *= Math::F_Quat::generate_rotator(forward_vector, +ROTATION_SPEED);
      }

      // construct the translational part of the transformation
      // Note: The orientation quaternion was already constructed, 
      Math::F_Quat new_dual = 0.5f * Math::F_Quat::generate_pure_quat(new_position) * new_orientation;
      
      Math::F_Dual_Quat prev_state(m_parent_entity_ptr->m_where_and_which_way);
      Math::F_Dual_Quat delta_state(new_orientation, new_dual);

      // apply the change in transformation 
      // Note: I am multiplying the transformations with OLD * NEW instead of the usual order.  
      // Why?
      //
      // This is because, in the current program, the camera follows the controlled entity
      // verbatim.  The camera matrix needs to be constructed with reversed transformation
      // order because the camera is mathematically modeled to remain stationary while the 
      // world is transformed around it.  Recall that all renderables have a "full transform"
      // applied to them, which uses the renderable's model-to-world matrix, the world-to-camera
      // matrix, and the perspective matrix (camera-to-window).  ALL renderables have this
      // applied to them.  So to give the illusion that the camera rotates and then translates,
      // the whole world must be translated first, then rotated.  It's like the whole is on
      // the end of a stick that the camera pushes around and rotates at will while the camera
      // remains stationary.
      //
      // TODO: When implementing camera controls, keep this transformation or, but when 
      // controlling something else, use the usual order of NEWER * OLDER.
      m_parent_entity_ptr->m_where_and_which_way = prev_state * delta_state;
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