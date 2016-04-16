#include "FrameRateCounter.h"

#include <Utilities/FreeTypeEncapsulate.h>
#include <Timing/Game_Clock.h>

namespace Rendering
{
    FrameRateRenderer::FrameRateRenderer()
        :
        _programId(0),
        _framesSinceLastUpdate(0),
        _secondsSinceLastUpdate(0.0),
        _frameRate(0.0)
    {
    }

    // calls upon the FreeType encapsulation class to give it an atlas
    // Note: The FreeType encapsulation class MUST be initialized first.
    bool FrameRateRenderer::Init(const GLuint programId)
    {
        // ??is this going to be a problem with smart pointers if Init(...) is called multiple times??
        _pAtlas = Utilities::FreeTypeEncapsulate::GetInstance().GenerateAtlas(48);
        if (0 == _pAtlas)
        {
            fprintf(stderr, "FreeType atlas could not be initialized\n");
            return false;
        }
        _programId = programId;

        // all went well
        return true;
    }

    // increments frame counter, and that's it
    void FrameRateRenderer::NewFrame()
    {
        _framesSinceLastUpdate++;
        _secondsSinceLastUpdate += Timing::Game_Clock::get_instance().
            get_delta_time_last_frame();
        
        if (_secondsSinceLastUpdate > 1.0)
        {
            _frameRate = _framesSinceLastUpdate / _secondsSinceLastUpdate;
            _framesSinceLastUpdate = 0;
            _secondsSinceLastUpdate = 0.0;
        }
    }

    void FrameRateRenderer::Render()
    {
        glUseProgram(_programId);

        // the shader for this program uses a vec4 (implicit content type is float) for color, so 
        // specify text color as a 4-float array and give it to shader 
        // Note: Even though color only needs RGB, use an alpha value as well in case some text
        // transparency is desired.
        GLfloat color[4] = { 0.5f, 0.5f, 0.0f, 1.0f };
        char str[8];
        sprintf(str, "%.2lf", _frameRate);
        float xy[2] = { -0.99f, -0.99f };
        float scaleXY[2] = { 1.0f, 1.0f };
        _pAtlas->RenderText(str, xy, scaleXY, color);

        // courtesy cleanup
        glUseProgram(0);
    }
}
