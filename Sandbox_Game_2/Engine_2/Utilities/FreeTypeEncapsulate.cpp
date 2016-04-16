#include "FreeTypeEncapsulate.h"

#include <Utilities/Include_Helpers/GL_Version.h>
#include <Utilities/ShaderManager.h>

// for making program from shader collection
#include <string>
#include <fstream>
#include <sstream>

namespace Utilities
{
    FreeTypeEncapsulate &FreeTypeEncapsulate::GetInstance()
    {
        static FreeTypeEncapsulate instance;
        return instance;
    }

    FreeTypeEncapsulate::FreeTypeEncapsulate()
        :
        _haveInitialized(0),
        _uniformTextSamplerLoc(0),
        _uniformTextColorLoc(0)
    {
    }

    FreeTypeEncapsulate::~FreeTypeEncapsulate()
    {
    }

    /*--------------------------------------------------------------------------------------------
    Description:
    Parameters: None
    Returns:    
        True if all went well, otherwise false (??use exceptions instead??)
    Exception:  
        Throws a std::exception if something goes wrong.
    Creator:
        John Cox (4-9-2016)
    --------------------------------------------------------------------------------------------*/
    bool FreeTypeEncapsulate::Init(const std::string &trueTypeFontFilePath, const int programId)
    {
        // FreeType needs to load itself into particular variables
        // Note: FT_Init_FreeType(...) returns something called an FT_Error, which VS can't find.
        // Based on the useage, it is assumed that 0 is returned if something went wrong, otherwise
        // non-zero is returned.  That is the only explanation for this kind of condition.
        if (FT_Init_FreeType(&_ftLib))
        {
            fprintf(stderr, "Could not init freetype library\n");
            return false;
        }

        // Note: FT_New_Face(...) also returns an FT_Error.
        if (FT_New_Face(_ftLib, trueTypeFontFilePath.c_str(), 0, &_ftFace))
        {
            fprintf(stderr, "Could not open font '%s'\n", trueTypeFontFilePath.c_str());
            return false;
        }

        // pick out the attributes and uniforms used in the FreeType GPU program

        char textTextureName[] = "textureSamplerId";
        _uniformTextSamplerLoc = glGetUniformLocation(programId, textTextureName);
        if (_uniformTextSamplerLoc == -1)
        {
            fprintf(stderr, "Could not bind uniform '%s'\n", textTextureName);
            return false;
        }

        //char textColorName[] = "color";
        char textColorName[] = "textureColor";
        _uniformTextColorLoc = glGetUniformLocation(programId, textColorName);
        if (_uniformTextColorLoc == -1)
        {
            fprintf(stderr, "Could not bind uniform '%s'\n", textColorName);
            return false;
        }

        // all went well
        _haveInitialized = true;

        return true;
    }

    const std::shared_ptr<FreeTypeAtlas> FreeTypeEncapsulate::GenerateAtlas(const int fontSize)
    {
        if (!_haveInitialized)
        {
            fprintf(stderr, "FreeTypeEncapsulate object has not been initialized\n");
            throw;
        }

        std::shared_ptr<FreeTypeAtlas> newAtlasPtr = std::make_shared<FreeTypeAtlas>(
            _uniformTextSamplerLoc, _uniformTextColorLoc);
        newAtlasPtr->Init(_ftFace, fontSize);

        return newAtlasPtr;
    }
}
