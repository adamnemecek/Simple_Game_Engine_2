#include "Printer_Helper.h"

#include <stdio.h>

namespace Utilities
{
   Printer_Helper& Printer_Helper::get_instance()
   {
      static Printer_Helper instance;

      return instance;
   }

   void Printer_Helper::print_vec(const char *foreword, const glm::vec3 &vec)
   {
      printf("%s <%.2f, %.2f, %.2f>\n", foreword, vec.x, vec.y, vec.z);
   }

   void Printer_Helper::print_dual_vec(
      const char *foreword_v1, const glm::vec3 &v1,
      const char *foreword_v2, const glm::vec3 &v2)
   {
      printf("%s <%.2f, %.2f, %.2f>, %s <%.2f, %.2f, %.2f>\n",
         foreword_v1, v1.x, v1.y, v1.z,
         foreword_v2, v2.x, v2.y, v2.z);
   }

   void Printer_Helper::print_mat(const char *foreword, const glm::mat4 &mat)
   {
      printf("%s %.2f, %.2f, %.2f, %.2f\n%.2f, %.2f, %.2f, %.2f\n%.2f, %.2f, %.2f, %.2f\n%.2f, %.2f, %.2f, %.2f\n",
         foreword,
         mat[0][0], mat[1][0], mat[2][0], mat[3][0],
         mat[0][1], mat[1][1], mat[2][1], mat[3][1],
         mat[0][2], mat[1][2], mat[2][2], mat[3][2],
         mat[0][3], mat[1][3], mat[2][3], mat[3][3]);
   }

   void Printer_Helper::print_glm_quat(const char *foreword, const glm::fquat &quat)
   {
      printf("%s <%.2f, %.2f, %.2f, %.2f>\n", foreword, quat.w, quat.x, quat.y, quat.z);
   }

   void Printer_Helper::print_glm_dual_quat(const char *foreword, const glm::fdualquat &dq)
   {
      printf("%s real: <%.2f, %.2f, %.2f, %.2f>, dual: <%.2f, %.2f, %.2f, %.2f>\n",
         foreword,
         dq.real.w, dq.real.x, dq.real.y, dq.real.z,
         dq.dual.w, dq.dual.x, dq.dual.y, dq.dual.z);
   }

   void Printer_Helper::print_my_quat(const char *foreword, const Math::F_Quat &quat)
   {
      printf("%s <%.2f, %.2f, %.2f, %.2f>\n", foreword, quat.m_scalar, quat.m_vector.x, quat.m_vector.y, quat.m_vector.z);
   }

   void Printer_Helper::print_my_dual_quat(const char *foreword, const Math::F_Dual_Quat &dq)
   {
      printf("%s real: <%.2f, %.2f, %.2f, %.2f>, dual: <%.2f, %.2f, %.2f, %.2f>\n",
         foreword,
         dq.m_real.m_scalar, dq.m_real.m_vector.x, dq.m_real.m_vector.y, dq.m_real.m_vector.z,
         dq.m_dual.m_scalar, dq.m_dual.m_vector.x, dq.m_dual.m_vector.y, dq.m_dual.m_vector.z);
   }

}