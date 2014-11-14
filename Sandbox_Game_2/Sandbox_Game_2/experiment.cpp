#include "experiment.h"

#include <Windows.h>
#include <GL\GL.h>
#include <math.h>

#include <Math\F_Quat.h>
#include <iostream>
using std::cout;
using std::endl;

//#include <glm\glm.hpp>
#include <Engine_2\Utilities\Quaternion_Helper.h>

#include <Engine_2\Math\F_Quat.h>

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
      //Math::F_Quat fq_1;
      //Math::F_Quat fq_2;
      //fq_2 = fq_1 * 2.0f;

      //Math::F_Quat rotator = Math::F_Quat::generate_rotator(glm::vec3(0.0f, 1.0f, 0.0f), 3.14159f);
      //Math::F_Quat point = Math::F_Quat::generate_pure_quat(glm::vec3(1.0f, 0.0f, 0.0f));
      //point = point * rotator;
      //point *= rotator;

      //glm::vec3 V1(1.1f, 2.2f, 3.3f);
      //glm::vec3 V2(-3.3f, -2.2f, -1.1f);
      //glm::vec3 V3 = glm::cross(V1, V2);

      //glm::vec3 sum = (1.0f * V2) + (2.0f * V1) + glm::cross(V1, V2);
      //float f_sum = 2.0f - glm::dot(V1, V2);

      glm::vec3 A(2.0f, 3.0f, 4.0f);
      glm::vec3 B(6.0f, 7.0f, 8.0f);
      //float dot_1 = glm::dot(A, -1.0f * B);
      //float dot_2 = glm::dot(B, -1.0f * A);
      //float dot_3 = glm::dot(A, B);
      //glm::vec3 cross_1 = glm::cross(A, -1.0f * B);
      //glm::vec3 cross_2 = glm::cross(B, -1.0f * A);
      glm::vec3 cross_3 = glm::cross(A, B);
      //glm::vec3 cross_sum = cross_1 + cross_2;

      glm::fquat real;
      Utilities::Quaternion_Helper::orientation_offset(glm::vec3(0.0f, 1.0f, 0.0f), 3.14159f / 4, real);
      glm::fdualquat dq(real, glm::vec3(0.0f, 0.5f, 0.0f));
      glm::fdualquat p = Utilities::Quaternion_Helper::dual_quat_from_point(glm::vec3(1.0f, 0.0f, 0.0f));
      glm::fdualquat dq_conjugate = Utilities::Quaternion_Helper::dual_quat_conjugate(dq);

      glm::fdualquat result = dq * p;
      glm::fquat t = (result.dual * 2.0f * glm::conjugate(result.real));

      //glm::vec3 cross_1 = glm::cross(glm::vec3(1, 0, 0), glm::vec3(0, sqrt(2) / 2.0f, 0));
      //glm::vec3 cross_1 = glm::cross(glm::vec3(1, 0, 0), glm::vec3(0, 1, 0));
      float sqrt_2_over_2 = sqrtf(2) / 2.0f;
      float sqrt_2_over_4 = sqrtf(2) / 4.0f;
      glm::vec3 V1(sqrt_2_over_2, sqrt_2_over_4, -sqrt_2_over_2);
      glm::vec3 V2(0, -sqrt_2_over_2, 0);

      glm::vec3 V3(0.462f, 0.25f, -0.191);
      glm::vec3 V4(0, -0.383, 0);
      glm::vec3 cross_1 = glm::cross(V1, V2);

      Math::F_Quat my_q(1.0f, glm::vec3(2.0f, 3.0f, 4.0f));
      Math::F_Quat my_q_conjugate = my_q.conjugate();
      Math::F_Quat my_p(5.0f, glm::vec3(6.0f, 7.0f, 8.0f));
      Math::F_Quat my_result = my_q * my_p;
      my_result = my_result * my_q_conjugate;

      //glm::fquat rotator_1;
      //glm::fquat rotator_2;
      //Utilities::Quaternion_Helper::orientation_offset(glm::vec3(1.0f, 1.0f, 0.0f), 3.1415926f / 2.0f, rotator_1);
      //Utilities::Quaternion_Helper::orientation_offset(glm::vec3(0.0f, 1.0f, 1.0f), 3.1415926f / 4.0f, rotator_2);
      //glm::fquat rotator_1(1.0f, 2.0f, 3.0f, 4.0f);
      //glm::fquat rotator_2(4.0f, 3.0f, 2.0f, 1.0f);
      //glm::fquat q1 = rotator_1 * glm::conjugate(rotator_2);
      //glm::fquat q2 = rotator_2 * glm::conjugate(rotator_1);
      //glm::fquat result = q1 + q2;
      //glm::normalize(result);

      //glm::fquat fq(0.0f, glm::vec3(0.0f, 1.0f, 0.0f));

      //glm::fquat result = fq * rotator;

      //glm::fdualquat dq = glm::fdualquat(rotator, glm::vec3(0.0f, 1.0f, 0.0f));
      //glm::normalize(dq);

      cout << "hello there" << endl;
   }
}
