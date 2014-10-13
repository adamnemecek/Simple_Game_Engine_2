#ifndef ENGINE_SHADER_MAKER_H
#define ENGINE_SHADER_MAKER_H

#include <Utilities\Include_GL_Version.h>
#include <Utilities\Typedefs.h>
#include <string>
#include <vector>

namespace Utilities
{
   class __declspec(dllexport) Shader_Maker
   {
   public:
      // returns the program ID
      static GLuint create_shader_program(
         const std::string *shader_file_paths,
         GLenum *shader_types,
         uint num_shaders);


   private:
      // enforce staticness
      Shader_Maker();
      Shader_Maker(const Shader_Maker &);
      Shader_Maker &operator=(const Shader_Maker &);

      // the following three helper functions were copied from the Arcsynthesis tutorial 0.3.8
      static GLuint LoadShader(GLenum eShaderType, const std::string &strShaderFilename);
      static GLuint CreateProgram(const std::vector<GLuint> &shaderList);
      static std::string FindFileOrThrow(const std::string &strBasename);


   };
}

#endif