#ifndef ENGINE_PRINTER_HELPER_H
#define ENGINE_PRINTER_HELPER_H

#include <glm\vec3.hpp>
#include <glm\mat4x4.hpp>
#include <Utilities\Include_Helpers\GLM_Quaternion.h>
#include <Utilities\Include_Helpers\GLM_Dual_Quaternion.h>
#include <Math\F_Quat.h>
#include <Math\F_Dual_Quat.h>

namespace Utilities
{
   class Printer_Helper
   {
   public:
      static Printer_Helper& get_instance();

      void print_vec(const char *foreword, const glm::vec3 &vec);
      void print_dual_vec(
         const char *foreword_v1, const glm::vec3 &v1,
         const char *foreword_v2, const glm::vec3 &v2);
      void print_mat(const char *foreword, const glm::mat4 &mat);
      void print_glm_quat(const char *foreword, const glm::fquat &quat);
      void print_glm_dual_quat(const char *foreword, const glm::fdualquat &dq);
      void print_my_quat(const char *foreword, const Math::F_Quat &quat);
      void print_my_dual_quat(const char *foreword, const Math::F_Dual_Quat &dq);

   private:
      // enforce singletonness
      Printer_Helper() {}
      Printer_Helper(const Printer_Helper&);
      Printer_Helper &operator=(const Printer_Helper&);
   };
}

#endif