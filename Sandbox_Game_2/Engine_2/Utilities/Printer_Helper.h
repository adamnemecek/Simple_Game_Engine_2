#ifndef ENGINE_PRINTER_HELPER_H
#define ENGINE_PRINTER_HELPER_H

#include <glm\vec3.hpp>
#include <glm\mat4x4.hpp>
#include <Utilities\Include_Helpers\GLM_Quaternion.h>
#include <Utilities\Include_Helpers\GLM_Dual_Quaternion.h>

namespace Utilities
{
   namespace Printer_Helper
   {
      void print_vec(const char *foreword, const glm::vec3 &vec);
      void print_mat(const char *foreword, const glm::mat4 &mat);
      void print_quat(const char *foreword, const glm::fquat &quat);
      void print_dual_quat(const char *foreword, const glm::fdualquat &dq);
   }
}

#endif