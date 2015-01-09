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

      //X	Y	Z
      //   camera	0.436	3.126	15.997
      //   light 2 - 5	3	5
      //   cube 3 - 5.5	0	5.5


      glm::rotate(glm::mat4(), 0.5f, glm::vec3(0.0f, 1.0f, 0.0f));

      Math::F_Quat q_real(0.5f, glm::vec3(0.5f, 0.5f, -0.5f));
      Math::F_Quat q_real_c = q_real.conjugate();
      Math::F_Quat q_dual(-0.75f, glm::vec3(-1.25f, 0.25f, -1.75f));
      Math::F_Quat q_result = q_dual * q_real_c;

      Math::F_Dual_Quat dq_transform_1 = Math::F_Dual_Quat::generate_rotate_then_translate(glm::vec3(0.0f, +1.0f, 0.0f), 3.14159f / 2.0f, glm::vec3(0.0f, +2.0f, 0.0f));
      Math::F_Dual_Quat dq_transform_2 = Math::F_Dual_Quat::generate_rotate_then_translate(glm::vec3(0.0f, 0.0f, +1.0f), 3.14159f / 2.0f, glm::vec3(+5.0f, 0.0f, 0.0f));
      Math::F_Dual_Quat dq_transform_3 = Math::F_Dual_Quat::generate_translate_then_rotate(glm::vec3(0.0f, 0.0f, -1.0f), 3.14159f, glm::vec3(0.0f, -4.0f, 0.0f));
      Math::F_Dual_Quat dq_net = dq_transform_3 * dq_transform_2 * dq_transform_1;

      glm::vec3 point(1.0f, 1.0f, 0.0f);
      glm::vec3 result_1 = Math::F_Dual_Quat::transform(dq_transform_1, point);
      glm::vec3 result_2 = Math::F_Dual_Quat::transform(dq_transform_2, result_1);
      glm::vec3 result_3 = Math::F_Dual_Quat::transform(dq_transform_3, result_2);

      glm::vec3 result_net = Math::F_Dual_Quat::transform(dq_net, point);



      //glm::vec3 camera(1.0f, 1.0f, 0.0f);
      //glm::vec3 light(-3.0f, -1.0f, 0.0f);
      //glm::vec3 cube(0.0f, 0.0f, 0.0f);
      //glm::vec3 cube_surface_normal(0.0f, 1.0f, 0.0f);

      //glm::vec3 vertex_to_light_vector = glm::normalize(light - cube);
      //glm::vec3 vertex_to_camera_vector = glm::normalize(camera - cube);
      //glm::vec3 reflected_light_vector = glm::reflect(-vertex_to_light_vector, cube_surface_normal);
      
      // There is a corner case in which a light could be below the horizon of a surface,
      // but it is at a shallow angle (example, the sun just set, so there should be no 
      // direct illumination of ground) and therefore the reflected light vector will also 
      // be at a shallow angle.  The dot product between the reflected light vector and the 
      // vertex-to-camera vector will therefore be greater than zero, and therefore the phong
      // factor would be calculated as positive, thus providing specular lighting on a surface 
      // from a light that is below its horizon!  
      // This should not happen.
      // Compensate by checking the dot product between the reflected light vector and the
      // vertex normal.  If it is greater than 0, then the light is above the horizon, and
      // if it is less than 0, then it is below the horizon.  If the light is above the 
      // surface horizon, calculate the phong factor from the dot product of the 
      // vertex-to-camera vector and the reflected light vector.  However, if the light is
      // below the horizon, then hard-code the phong factor to 0.0f and be done.

      //float the_dot = glm::dot(reflected_light_vector, vertex_to_camera_vector);

      cout << "hello there" << endl;
   }
}
