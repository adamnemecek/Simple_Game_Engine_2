#include "Printer_Helper.h"

#include <stdio.h>

namespace Utilities
{
   void Printer_Helper::print_vec(const glm::vec3 &vec)
   {
      printf("pos: <%.2f, %.2f, %.2f>\n", vec.x, vec.y, vec.z);
   }

   void Printer_Helper::print_mat(const glm::mat4 & mat)
   {
      printf("%.2f, %.2f, %.2f, %.2f\n%.2f, %.2f, %.2f, %.2f\n%.2f, %.2f, %.2f, %.2f\n%.2f, %.2f, %.2f, %.2f\n",
         mat[0][0], mat[1][0], mat[2][0], mat[3][0],
         mat[0][1], mat[1][1], mat[2][1], mat[3][1],
         mat[0][2], mat[1][2], mat[2][2], mat[3][2],
         mat[0][3], mat[1][3], mat[2][3], mat[3][3]);
   }
}