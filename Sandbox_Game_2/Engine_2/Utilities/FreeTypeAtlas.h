#pragma once

// there doesn't seem to be any way out of including all of FreeType without some shady forward 
// declarations of FreeType internal structures
// Note: FreeType has a single header for everything.  This is evil.
#include <ft2build.h>
#include FT_FREETYPE_H  // also defined relative to "freetype-2.6.1/include/"

#include <string>

namespace Utilities
{
    // this can only be properly created by the FreeTypeEncapsulation class
    class __declspec(dllexport) FreeTypeAtlas
    {
    public:
        // the FT_Face type is a pointer, so don't use a reference or pointer
        FreeTypeAtlas(const int uniformTextSamplerLoc, const int uniformTextColorLoc);

        bool Init(const FT_Face face, const int fontPixelHeightSize);

        ~FreeTypeAtlas();

        // position is in screen coordinates of the OpenGL display, which on the range 
        // [-0.999f, +0.999f], and note that the range is technically [-1,+1] but in practice an X
        // of -1 will not render

        // render a single char (demonstrates most simple drawing of a single char)
        void RenderChar(const char c, const float posScreenCoord[2], const float userScale[2],
            const float color[4]) const;

        // render a string (demonstrates use of glyph "advance" value between characters)
        void RenderText(const std::string &str, const float posScreenCoord[2],
            const float userScale[2], const float color[4]) const;
    private:
        // stores all the vertex buffer and attribute points for easy use after initialization
        unsigned int _vaoId;

        // have to reference it on every draw call, so keep it around
        // Note: It is actually a GLuint, which is a typedef of "unsigned int", but I don't want to 
        // include all of the OpenGL declarations in a header file, so just use the original type.
        // It is a tad shady, but keep an eye on build warnings about incompatible types and you
        // should be fine.
        unsigned int _textureId;

        // the atlas needs access to a vertex buffer
        // Note: Let the atlas make one.  The FreeType encapsulation could do this, but I am 
        // hesitant to make a buffer object that would be used by potentially multiple atlases, so 
        // let the atlas make its own.
        unsigned int _vboId;

        // which sampler to use (0 - GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS (??you sure??)
        // Note: Technically it is a GLint, but is simple int here for the same reason 
        // "texture ID" is an unsigned int instead of GLuint.
        int _textureSamplerId;

        // this is directly related to how much GPU memory is allocated for the quads' vertices
        // Note: Alternatively, I could have made the RenderChar(...) and RenderText(...) 
        // methods non-const and then resized this value as necessary to accomodate possibly 
        // larger strings, but I didn't want to make them non-const, so this value will be the 
        // end of discussion about how big the string can be.  ??bad idea??)
        const int _MAX_STR_LENGTH = 32;

        // the FreeType glyph set provides the basic printable ASCII character set, and even though 
        // the first 31 are not visible and will therefore not be loaded, the useless bytes are an 
        // acceptable tradeoff for rapid ASCII character lookup
        struct FreeTypeGlyphCharInfo {
            // advance X and Y for screen coordinate calculations
            float ax;
            float ay;

            // bitmap left and top for screen coordinate calculations
            float bl;
            float bt;

            // glyph bitmap width and height for screen coordinate calculations
            float bw;
            float bh;

            // glyph bitmap width and height normalized to [0.0,1.0] for texture coordinate 
            // calculations
            // Note: It is better for performance to do the normalization (a division) at startup.
            float nbw;
            float nbh;

            // TODO: change to "texture S" and "texture T" to be clear
            float tx;	// x offset of glyph in texture coordinates (S on range [0.0,1.0])
            float ty;	// y offset of glyph in texture coordinates (T on range [0.0,1.0])
        } _glyphCharInfo[128];

        // the atlas needs to tell the fragment shader which texture sampler and texture color 
        // (FreeType only provides alpha channel) to use, it does that via uniform, and to use 
        // it the atlas should store the uniform's location
        // Note: Actually a GLint.
        // Also Note: The FreeType encapsulation is responsible for the texture atlas' shader 
        // program, and it will provide these values.
        int _uniformTextSamplerLoc;
        int _uniformTextColorLoc;
    };
}
