#include "experiment.h"

#include <Windows.h>
#include <GL\GL.h>
#include <math.h>

#include <Math\Float_Quat.h>
#include <iostream>
using std::cout;
using std::endl;

//#include <glm\glm.hpp>
#include <Engine_2\Utilities\Quaternion_Helper.h>

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
      //Math::Float_Quat fq_1;
      //Math::Float_Quat fq_2;
      //fq_2 = fq_1 * 2.0f;

      //Math::Float_Quat rotator = Math::Float_Quat::generate_rotator(glm::vec3(0.0f, 1.0f, 0.0f), 3.14159f);
      //Math::Float_Quat point = Math::Float_Quat::generate_pure_quat(glm::vec3(1.0f, 0.0f, 0.0f));
      //point = point * rotator;
      //point *= rotator;

      //glm::vec3 V1(1.1f, 2.2f, 3.3f);
      //glm::vec3 V2(-3.3f, -2.2f, -1.1f);
      //glm::vec3 V3 = glm::cross(V1, V2);

      //glm::vec3 sum = (1.0f * V2) + (2.0f * V1) + glm::cross(V1, V2);
      //float f_sum = 2.0f - glm::dot(V1, V2);

      //float f = glm::dot(V1, V2);

      glm::fquat fq(1.0f, glm::vec3(0.0f, 1.0f, 0.0f));
      
      float angle_rad = 3.14159f / 2.0f;
      glm::vec3 rotation_axis = glm::vec3(+1.0f, 0.0f, 0.0f) * sinf(angle_rad / 2.0f);
      float scalar = cosf(angle_rad / 2.0f);
      glm::fquat rotator(scalar, rotation_axis);

      glm::fquat result = fq * rotator;

      cout << "hello there" << endl;
   }
}
