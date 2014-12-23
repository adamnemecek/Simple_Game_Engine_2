#include "experiment.h"

#include <Windows.h>
#include <GL\GL.h>
#include <math.h>

#include <iostream>
using std::cout;
using std::endl;


#include <Utilities\Include_Helpers\GLM_Mat_Transform.h>

#include <Engine_2\Utilities\Quaternion_Helper.h>
#include <Engine_2\Math\F_Dual_Quat.h>
#include <Engine_2\Utilities\Include_Helpers\Default_Vectors.h>

namespace Experiment
{
   void DrawCircle(const glm::vec3 &center, float r, int num_segments)
   {
      float theta = 2 * 3.1415926 / float(num_segments);
      float tangetial_factor = tanf(theta);//calculate the tangential factor 

      float radial_factor = cosf(theta);//calculate the radial factor 

      float x = r;//we start at angle = 0 

      float z = 0;

      glBegin(GL_LINE_LOOP);
      for (int ii = 0; ii < num_segments; ii++)
      {
         glVertex3f(x + center.x, center.y, z + center.z);//output vertex 

         //calculate the tangential vector 
         //remember, the radial vector is (x, y) 
         //to get the tangential vector we flip those coordinates and negate one of them 

         float tx = -z;
         float tz = x;

         //add the tangential vector 

         x += tx * tangetial_factor;
         z += tz * tangetial_factor;

         //correct using the radial factor 

         x *= radial_factor;
         z *= radial_factor;
      }
      glEnd();
   }


   void do_something()
   {
      //glm::vec3 point(1.0f, 0.0f, 0.0f);
      //glm::mat4 old_transform = glm::translate(glm::mat4(), glm::vec3(0.0f, 2.2f, 0.0f));
      //glm::mat4 new_rotate = glm::rotate(glm::mat4(), 3.1415926f / 2.0f, glm::vec3(0.0f, 1.0f, 0.0f));
      //glm::mat4 new_transform = glm::translate(new_rotate, glm::vec3(0.0f, 2.2f, 0.0f));
      //glm::mat4 net_transform = new_transform * old_transform;

      //glm::vec3 result = glm::vec3(net_transform * glm::vec4(point, 1.0f));


      //Math::F_Dual_Quat dq = Math::F_Dual_Quat::generate_rotator_only(Utilities::Default_Vectors::WORLD_UP_VECTOR, 3.14159f / 4);
      //glm::vec3 point(1.0f, 0.0f, 0.0f);
      //glm::vec3 result = Math::F_Dual_Quat::transform(dq, point);

      //Math::F_Quat q1 = Math::F_Quat::generate_rotator_for_pure_quat(glm::vec3(1.0f, 0.0f, 0.0f), 3.14159f / 4);
      //Math::F_Quat p = Math::F_Quat::generate_pure_quat(glm::vec3(1.0f, 1.0f, 0.0f));
      //Math::F_Quat result_quat1 = q1 * p;
      //Math::F_Quat result_quat2 = q1 * q1 * p;
      //Math::F_Quat result_quat3 = q1 * q1 * q1 * q1 * p;

      //glm::fquat q;
      //Utilities::Quaternion_Helper::orientation_offset(glm::vec3(0.0f, 1.0f, 0.0f), 3.14159f / 2, q);
      //glm::fquat p(0.0f, glm::vec3(1.0f, 1.0f, 0.0f));
      //glm::fquat result_quat1 = q * p * glm::conjugate(glm::normalize(q));

      //glm::mat4 mat = glm::mat4_cast(q);
      //glm::vec4 point(glm::vec3(1.0f, 1.0f, 0.0f), 0.0f);
      //glm::vec4 result = mat * point;

      //glm::mat3 mat3 = glm::rotate(glm::mat3(), 3.14159f / 2)

      //Math::F_Dual_Quat dq = Math::F_Dual_Quat::generate_rotate_then_translate(glm::vec3(0.0f, 1.0f, 0.0f), 3.14159f / 2.0f, glm::vec3(+2.0f, +3.0f, 0.0f));
      //glm::vec3 result = Math::F_Dual_Quat::transform(dq, glm::vec3(1.0f, 1.0f, 0.0f));


      glm::vec3 point_original = glm::vec3(1.0f, 1.0f, 0.0f);
      Math::F_Dual_Quat dq_1 = Math::F_Dual_Quat::generate_rotate_then_translate(glm::vec3(0.0f, 1.0f, 0.0f), (3.14159f) / 2.0f, glm::vec3(0.0f, 2.0f, 0.0f));
      Math::F_Dual_Quat dq_2 = Math::F_Dual_Quat::generate_rotate_then_translate(glm::vec3(0.0f, 0.0f, 1.0f), (3.14159f) / 2.0f, glm::vec3(5.0f, 0.0f, 0.0f));
      Math::F_Dual_Quat dq_3 = Math::F_Dual_Quat::generate_translate_then_rotate(glm::vec3(0.0f, 0.0f, -1.0f), 3.14159f, glm::vec3(0.0f, -4.0f, 0.0f));
      Math::F_Dual_Quat dq_temp = dq_2 * dq_1;
      Math::F_Dual_Quat dq_net = dq_3 * dq_temp;
      glm::vec3 result_point = Math::F_Dual_Quat::transform(dq_net, point_original);


      float sqrt_2 = sqrtf(2.0f);
      glm::vec3 v1(-5.0f / 4.0f, 1.0f / 4.0f, -7.0f / 4.0f);
      glm::vec3 v2(-1.0f / 2.0f, -1.0f / 2.0f, 1.0f / 2.0f);
      glm::vec3 v3 = (-3.0f / 4.0f) * v2;
      glm::vec3 v4 = (1.0f / 2.0f) * v1;

      glm::vec3 cross1 = glm::cross(v1, v2);
      glm::vec3 result = v3 + v4 + cross1;
      float f1 = glm::dot(v1, v2);
      float f2 = v1.x * v2.z;
      //float f2 = (-92.0f) * (1.0f) - f1;

      cout << "hello there" << endl;
   }
}
