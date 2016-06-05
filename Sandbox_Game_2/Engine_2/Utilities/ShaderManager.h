#pragma once

#include <Utilities/Include_Helpers/GL_Version.h>
#include <Utilities/Typedefs.h>
#include <map>
#include <vector>
#include <string>

namespace Utilities
{

    //TODO: some kind of structure for a program that can be populated with all values in the shader, like the answer to this post might allow: http://stackoverflow.com/questions/440144/in-opengl-is-there-a-way-to-get-a-list-of-all-uniforms-attribs-used-by-a-shade


    class __declspec(dllexport) ShaderManager
    {
    public:
        static ShaderManager &GetInstance();

        int LoadProgram(const std::string &theKey, const std::string *shaderFilePaths,
            const GLenum *shaderTypes, const uint numShaders);
        int GetProgramId(const std::string &theKey);
        int FindUniform(const int programId, const std::string &findThis);
        int FindAttribute(const int programId, const std::string &findThis);
    private:
        // hide these to enforce singleton-ness
        ShaderManager() {}
        ShaderManager(const ShaderManager &);
        ShaderManager &operator=(const ShaderManager &rhs);

        // the following three helper functions were copied from the Arcsynthesis tutorial 0.3.8
        GLuint LoadShader(GLenum eShaderType, const std::string &strShaderFilename);
        GLuint CreateProgram(const std::vector<GLuint> &shaderList);
        std::string FindFileOrThrow(const std::string &strBasename);

        // this typedef can save me some typing later
        typedef std::map<std::string, GLuint> KEY_VAL_MAP_TYPE;
        KEY_VAL_MAP_TYPE _programs;
    };
}