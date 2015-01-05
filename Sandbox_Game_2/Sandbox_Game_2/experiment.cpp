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

   glm::vec3 my_function_1(const glm::vec3 &v_a_i, const glm::vec3 &v_b_i, const glm::vec3 &v_b_f,
      const float m_a, const float m_b)
   {
      glm::vec3 base = v_a_i + (m_b / m_a) * (v_b_i - v_b_f);

      // square it
      return base * base;
   }

   glm::vec3 my_function_2(const glm::vec3 &v_a_i, const glm::vec3 &v_b_i, const glm::vec3 &v_b_f,
      const float m_a, const float m_b)
   {
      glm::vec3 a_squared_term = v_a_i * v_a_i;
      glm::vec3 two_a_b_c_term = 2.0f * (v_a_i)* (m_b / m_a) * (v_b_i - v_b_f);

      float mass_squared_fraction = (m_b * m_b) / (m_a * m_a);
      glm::vec3 bc_squared_term = mass_squared_fraction * (v_b_i * v_b_i) - 2 * (mass_squared_fraction)* (v_b_i)* (v_b_f)+mass_squared_fraction * (v_b_f * v_b_f);

      return a_squared_term + two_a_b_c_term + bc_squared_term;
   }

   glm::vec3 my_b_squared(
      const glm::vec3 &v_a_i, const glm::vec3 &v_b_i,
      const float m_a, const float m_b)
   {
      glm::vec3 b = ((-1.0f) * v_a_i * m_b - (m_b * m_b / m_a) * v_b_i);
      glm::vec3 b_squared = b * b;

      glm::vec3 v_a_i_squared_m_b_squared = (v_a_i * v_a_i) * (m_b * m_b);
      glm::vec3 two_v_a_i_m_b_cubed_over_m_a_v_b_i = 2.0f * v_a_i * (m_b * m_b * m_b / m_a) * v_b_i;
      glm::vec3 m_b_quad_over_m_a_squared_v_b_i_squared = (m_b * m_b * m_b * m_b / (m_a * m_a)) * (v_b_i * v_b_i);
      glm::vec3 result = v_a_i_squared_m_b_squared + two_v_a_i_m_b_cubed_over_m_a_v_b_i + m_b_quad_over_m_a_squared_v_b_i_squared;

      return result;
   }

   glm::vec3 my_4ac(
      const glm::vec3 &v_a_i, const glm::vec3 &v_b_i,
      const float m_a, const float m_b)
   {
      glm::vec3 a = (v_a_i * m_b * v_b_i) + (0.5f * v_b_i * v_b_i * ((m_b * m_b / m_a) - m_b));
      float c = (0.5f * (m_b * m_b / m_a) + (0.5f * m_b));
      glm::vec3 four_a_c = 4.0f * a * c;

      glm::vec3 two_v_a_i_v_b_i_m_b_cubed_over_m_a = 2.0f * v_a_i * v_b_i * (m_b * m_b * m_b / m_a);
      glm::vec3 two_v_a_i_m_b_squared_v_b_i = 2.0f * v_a_i * (m_b * m_b) * v_b_i;
      glm::vec3 v_b_i_squared_m_b_quad_over_m_a_squared = v_b_i * v_b_i * (m_b * m_b * m_b * m_b / (m_a * m_a));
      glm::vec3 v_b_i_squared_m_b_squared = v_b_i * v_b_i * (m_b * m_b);
      glm::vec3 result = two_v_a_i_v_b_i_m_b_cubed_over_m_a + two_v_a_i_m_b_squared_v_b_i + v_b_i_squared_m_b_quad_over_m_a_squared - v_b_i_squared_m_b_squared;

      return result;
   }

   glm::vec3 my_b_squared_minus_4ac(
      const glm::vec3 &v_a_i, const glm::vec3 &v_b_i,
      const float m_a, const float m_b)
   {
      //glm::vec3 v_a_i_squared_m_b_squared = (v_a_i * v_a_i) * (m_b * m_b);
      //glm::vec3 two_v_a_i_m_b_cubed_over_m_a_v_b_i = 2.0f * v_a_i * (m_b * m_b * m_b / m_a) * v_b_i;
      //glm::vec3 m_b_quad_over_m_a_squared_v_b_i_squared = (m_b * m_b * m_b * m_b / (m_a * m_a)) * (v_b_i * v_b_i);
      //glm::vec3 result1 = v_a_i_squared_m_b_squared + two_v_a_i_m_b_cubed_over_m_a_v_b_i + m_b_quad_over_m_a_squared_v_b_i_squared;

      //glm::vec3 two_v_a_i_v_b_i_m_b_cubed_over_m_a = 2.0f * v_a_i * v_b_i * (m_b * m_b * m_b / m_a);
      //glm::vec3 two_v_a_i_m_b_squared_v_b_i = 2.0f * v_a_i * (m_b * m_b) * v_b_i;
      //glm::vec3 v_b_i_squared_m_b_quad_over_m_a_squared = v_b_i * v_b_i * (m_b * m_b * m_b * m_b / (m_a * m_a));
      //glm::vec3 v_b_i_squared_m_b_squared = v_b_i * v_b_i * (m_b * m_b);
      //glm::vec3 result2 = two_v_a_i_v_b_i_m_b_cubed_over_m_a + two_v_a_i_m_b_squared_v_b_i + v_b_i_squared_m_b_quad_over_m_a_squared - v_b_i_squared_m_b_squared;

      //glm::vec3 final_result1 = result1 - result2;
      //glm::vec3 final_result2 = (v_a_i_squared_m_b_squared + two_v_a_i_m_b_cubed_over_m_a_v_b_i + m_b_quad_over_m_a_squared_v_b_i_squared) -
      //   (two_v_a_i_v_b_i_m_b_cubed_over_m_a + two_v_a_i_m_b_squared_v_b_i + v_b_i_squared_m_b_quad_over_m_a_squared - v_b_i_squared_m_b_squared);

      //glm::vec3 simplified1 = (m_b * m_b) * ((v_a_i * v_a_i) - (2.0f * v_a_i * v_b_i) + (v_b_i * v_b_i));
      //glm::vec3 simplified2 = v_a_i_squared_m_b_squared - two_v_a_i_m_b_squared_v_b_i + v_b_i_squared_m_b_squared;

      glm::vec3 b_squared = my_b_squared(v_a_i, v_b_i, m_a, m_b);
      glm::vec3 four_a_c = my_4ac(v_a_i, v_b_i, m_a, m_b);
      glm::vec3 result = b_squared - four_a_c;
      
      glm::vec3 simplified = (m_b * m_b) * (v_a_i - v_b_i) * (v_a_i - v_b_i);

      return simplified;
   }

   glm::vec3 my_square_root_b_squared_minus_4ac(
      const glm::vec3 &v_a_i, const glm::vec3 &v_b_i,
      const float m_a, const float m_b)
   {
      glm::vec3 base = my_b_squared_minus_4ac(v_a_i, v_b_i, m_a, m_b);
      glm::vec3 sqrt_base(sqrtf(base.x), sqrtf(base.y), sqrtf(base.z));

      glm::vec3 simplified = (m_b * v_a_i) - (m_b * v_b_i);

      return simplified;
   }

   glm::vec3 my_negative_b_plus_square_root_b_squared_minus_4_a_c(
      const glm::vec3 &v_a_i, const glm::vec3 &v_b_i,
      const float m_a, const float m_b)
   {
      glm::vec3 b = ((-1.0f) * v_a_i * m_b) - (m_b * m_b / m_a) * v_b_i;
      glm::vec3 negative_b = (-1.0f) * b;
      glm::vec3 result_of_root = my_square_root_b_squared_minus_4ac(v_a_i, v_b_i, m_a, m_b);
      glm::vec3 result = negative_b + result_of_root;

      glm::vec3 simplified = (2.0f * v_a_i * m_b) + ((m_b * m_b / m_a) * v_b_i) - (m_b * v_b_i);

      return simplified;
   }

   glm::vec3 my_negative_b_minus_square_root_b_squared_minus_4_a_c(
      const glm::vec3 &v_a_i, const glm::vec3 &v_b_i,
      const float m_a, const float m_b)
   {
      glm::vec3 b = ((-1.0f) * v_a_i * m_b) - (m_b * m_b / m_a) * v_b_i;
      glm::vec3 negative_b = (-1.0f) * b;
      glm::vec3 result_of_root = my_square_root_b_squared_minus_4ac(v_a_i, v_b_i, m_a, m_b);
      glm::vec3 result = negative_b - result_of_root;

      glm::vec3 simplified = ((m_b * m_b / m_a) * v_b_i) + (m_b * v_b_i);

      return simplified;
   }

   glm::vec3 my_negative_b_plus_square_root_b_squared_minus_4_a_c_all_over_2a(
      const glm::vec3 &v_a_i, const glm::vec3 &v_b_i,
      const float m_a, const float m_b)
   {
      float a = (0.5f * (m_b * m_b / m_a)) + (0.5f * m_b);
      float two_a = 2.0f * a;
      glm::vec3 numerator = my_negative_b_plus_square_root_b_squared_minus_4_a_c(v_a_i, v_b_i, m_a, m_b);
      glm::vec3 result = numerator / two_a;

      // "simplified" now being applied loosely
      glm::vec3 simplified = ((2.0f * v_a_i * m_b) + ((m_b * m_b / m_a) * v_b_i) - (m_b * v_b_i)) /
         ((m_b * m_b / m_a) + m_b);

      return simplified;
   }

   glm::vec3 my_negative_b_minus_square_root_b_squared_minus_4_a_c_all_over_2a(
      const glm::vec3 &v_a_i, const glm::vec3 &v_b_i,
      const float m_a, const float m_b)
   {
      float a = (0.5f * (m_b * m_b / m_a)) + (0.5f * m_b);
      float two_a = 2.0f * a;
      glm::vec3 numerator = my_negative_b_minus_square_root_b_squared_minus_4_a_c(v_a_i, v_b_i, m_a, m_b);
      glm::vec3 result = numerator / two_a;

      // "simplified" now being applied loosely
      glm::vec3 simplified = v_b_i;

      return simplified;
   }

   glm::vec3 solve_for_zero(
      const glm::vec3 &v_a_i, const glm::vec3 &v_b_i, const glm::vec3 &v_b_f,
      const float m_a, const float m_b)
   {
      glm::vec3 a_x_squared = ((0.5f * (m_b * m_b / m_a)) + (0.5f * m_b)) * (v_b_f * v_b_f);
      glm::vec3 b_x = (((-1.0f) * v_a_i * m_b) - ((m_b * m_b / m_a) * v_b_i)) * v_b_f;
      glm::vec3 c = (v_a_i * m_b * v_b_i) + (0.5f * (v_b_i * v_b_i) * ((m_b * m_b / m_a) - m_b));

      glm::vec3 result = a_x_squared + b_x + c;

      return result;
   }

   void solve_for_and_verify_v_b_f(
      const glm::vec3 &v_a_i, const glm::vec3 &v_b_i,
      const float m_a, const float m_b)
   {
      glm::vec3 v_b_f_plus = my_negative_b_plus_square_root_b_squared_minus_4_a_c_all_over_2a(v_a_i, v_b_i, m_a, m_b);
      glm::vec3 result1 = solve_for_zero(v_a_i, v_b_i, v_b_f_plus, m_a, m_b);

      glm::vec3 v_b_f_minus = my_negative_b_minus_square_root_b_squared_minus_4_a_c_all_over_2a(v_a_i, v_b_i, m_a, m_b);
      glm::vec3 result2 = solve_for_zero(v_a_i, v_b_i, v_b_f_minus, m_a, m_b);
   }

   void do_something()
   {
      //glm::vec3 v_a_i(1.0f, 2.0f, 3.0f);
      //glm::vec3 v_b_i(-3.0f, -2.0f, -1.0f);
      //glm::vec3 v_b_f(+2.0f, +5.0f, -1.0f);
      //float mass_a = 5.0f;
      //float mass_b = 10.0f;

      //solve_for_and_verify_v_b_f(v_a_i, v_b_i, mass_a, mass_b);

      glm::vec3 v3(2.0f, 3.0f, 4.0f);
      glm::vec4 v4(2.0f, 3.0f, 4.0f, 1.0f);

      float f1 = glm::dot(glm::normalize(v3), glm::normalize(v3));
      float f2 = glm::dot(glm::normalize(v4), glm::normalize(v4));

      cout << "hello there" << endl;
   }
}
