#include "ShaderManager.h"

// for finding and loading shader files
#include <algorithm>
#include <fstream>
#include <sstream>

#include <glutil\glutil.h>

namespace Utilities
{
    ShaderManager &ShaderManager::GetInstance()
    {
        static ShaderManager theThing;
        return theThing;
    }

    int ShaderManager::LoadProgram(const std::string &theKey, const std::string *shaderFilePaths,
        const GLenum *shaderTypes, const uint numShaders)
    {
        using std::vector;

        // don't even try to catch any exceptions
        // Note: If something goes wrong here, the program will be unable to render, so it should 
        // stop right here
        vector<GLuint> shaderList;
        for (size_t shaderCount = 0; shaderCount < numShaders; shaderCount++)
        {
            shaderList.push_back(LoadShader(shaderTypes[shaderCount],
                shaderFilePaths[shaderCount]));
        }

        GLuint shaderProgramID = CreateProgram(shaderList);
        _programs.insert(std::make_pair(theKey, shaderProgramID));

        return shaderProgramID;
    }

    int ShaderManager::GetProgramId(const std::string &theKey)
    {
        KEY_VAL_MAP_TYPE::iterator itr = _programs.find(theKey);
        if (itr == _programs.end())
        {
            fprintf(stderr, "No program for key '%s'", theKey.c_str());
            throw;
        }

        // didn't throw, so must have found it
        return itr->second;
    }

    // PRIVATE
    GLuint ShaderManager::LoadShader(GLenum eShaderType, const std::string &strShaderFilename)
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
        catch (glutil::CompileLinkException &e)
        {
            fprintf(stderr, "%s\n", e.what());
            throw;
        }
    }

    GLuint ShaderManager::CreateProgram(const std::vector<GLuint> &shaderList)
    {
        GLuint prog = 0;
        try
        {
            prog = glutil::LinkProgram(shaderList);

            // the program contains binary, linked versions of the shaders, so clean up the 
            // compile objects
            // Note: Shader objects need to be un-linked before they can be deleted.  This is ok 
            // because the program safely contains the shaders in binary form.
            for (size_t shaderCount = 0; shaderCount < shaderList.size(); shaderCount++)
            {
                // it seems that glutil already detached the shaders when it linked the program,
                // so just delete the shaders now
                // Note: This was discovered when an OpenGL error was printed out that the 
                // specified shader was not attached to the program.
                glDeleteShader(shaderList[shaderCount]);
            }
            return prog;
        }
        catch (glutil::CompileLinkException &e)
        {
            // if a shader didn't link (something didn't in order to get here), then attempting to 
            // detach or delete a shader will have no ill effect
            for (size_t shaderCount = 0; shaderCount < shaderList.size(); shaderCount++)
            {
                GLuint shaderId = shaderList[shaderCount];
                glDetachShader(prog, shaderId);
                glDeleteShader(shaderId);
            }
            fprintf(stderr, "%s\n", e.what());
            throw;
        }
    }

    std::string ShaderManager::FindFileOrThrow(const std::string &strBasename)
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