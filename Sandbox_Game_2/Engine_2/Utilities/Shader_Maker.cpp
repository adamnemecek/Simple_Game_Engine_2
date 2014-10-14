#include "Shader_Maker.h"

// the following includes are for shader loading
#include <algorithm>
#include <fstream>
#include <sstream>

#include <glutil\glutil.h>

namespace Utilities
{
   GLuint Shader_Maker::create_shader_program(
      const std::string *shader_file_paths,
      GLenum *shader_types,
      uint num_shaders)
   {
      using std::vector;

      vector<GLuint> shader_list;
      for (size_t shader_count = 0; shader_count < num_shaders; shader_count++)
      {
         shader_list.push_back(LoadShader(shader_types[shader_count], shader_file_paths[shader_count]));
      }

      GLuint shader_program_ID = CreateProgram(shader_list);

      return shader_program_ID;
   }

   GLuint Shader_Maker::LoadShader(GLenum eShaderType, const std::string &strShaderFilename)
   {
      std::string strFilename = FindFileOrThrow(strShaderFilename);
      std::ifstream shaderFile(strFilename.c_str());
      std::stringstream shaderData;
      shaderData << shaderFile.rdbuf();
      shaderFile.close();

      try
      {
         return glutil::CompileShader(eShaderType, shaderData.str());
      }
      catch (std::exception &e)
      {
         fprintf(stderr, "%s\n", e.what());
         throw;
      }
   }


   GLuint Shader_Maker::CreateProgram(const std::vector<GLuint> &shaderList)
   {
      try
      {
         GLuint prog = glutil::LinkProgram(shaderList);
         std::for_each(shaderList.begin(), shaderList.end(), glDeleteShader);
         return prog;
      }
      catch (std::exception &e)
      {
         std::for_each(shaderList.begin(), shaderList.end(), glDeleteShader);
         fprintf(stderr, "%s\n", e.what());
         throw;
      }
   }

   std::string Shader_Maker::FindFileOrThrow(const std::string &strBasename)
   {
      // this commented code is Arcsynthesis' original code, so I think that I should leave it so that I don't pretend it's mine

      //std::string strFilename = LOCAL_FILE_DIR + strBasename;
      //std::ifstream testFile(strFilename.c_str());
      //if (testFile.is_open())
      //   return strFilename;


      //strFilename = GLOBAL_FILE_DIR + strBasename;
      //testFile.open(strFilename.c_str());
      //if (testFile.is_open())
      //   return strFilename;

      std::ifstream test_file(strBasename.c_str());
      if (test_file.is_open())
      {
         return strBasename;
      }

      throw std::runtime_error("Could not find the file " + strBasename);
   }
}
