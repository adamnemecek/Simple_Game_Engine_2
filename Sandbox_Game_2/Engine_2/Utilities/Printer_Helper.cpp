#include "Printer_Helper.h"

#include <stdio.h>

namespace Utilities
{
   namespace Printer_Helper
   {
      void print_vec(const char *foreword, const glm::vec3 &vec)
      {
         printf("%s <%.2f, %.2f, %.2f>\n", foreword, vec.x, vec.y, vec.z);
      }

      void print_mat(const char *foreword, const glm::mat4 &mat)
      {
         printf("%s %.2f, %.2f, %.2f, %.2f\n%.2f, %.2f, %.2f, %.2f\n%.2f, %.2f, %.2f, %.2f\n%.2f, %.2f, %.2f, %.2f\n",
            foreword,
            mat[0][0], mat[1][0], mat[2][0], mat[3][0],
            mat[0][1], mat[1][1], mat[2][1], mat[3][1],
            mat[0][2], mat[1][2], mat[2][2], mat[3][2],
            mat[0][3], mat[1][3], mat[2][3], mat[3][3]);
      }

      void print_quat(const char *foreword, const glm::fquat &quat)
      {
         printf("%s <%.2f, %.2f, %.2f, %.2f>\n", foreword, quat.w, quat.x, quat.y, quat.z);
      }

      void print_dual_quat(const char *foreword, const glm::fdualquat &dq)
      {
         printf("%s real: <%.2f, %.2f, %.2f, %.2f>, dual: <%.2f, %.2f, %.2f, %.2f>\n",
            foreword,
            dq.real.w, dq.real.x, dq.real.y, dq.real.z,
            dq.dual.w, dq.dual.x, dq.dual.y, dq.dual.z, 5);
      }
   }
}