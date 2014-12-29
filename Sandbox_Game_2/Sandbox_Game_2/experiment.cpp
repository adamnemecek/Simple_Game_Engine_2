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
      glm::vec3 v1(2, -3, 2);
      glm::vec3 v2(2, 3, -2);
      float mag_v1 = glm::length(v1);
      float mag_v2 = glm::length(v2);
      float mag_v1_times_mag_v2 = mag_v1 * mag_v2;

      glm::vec3 cross1 = glm::cross(v1, v2);
      float mag_cross1 = glm::length(cross1);


      cout << "hello there" << endl;
   }
}
