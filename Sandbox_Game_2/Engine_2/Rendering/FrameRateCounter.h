#pragma once

#include <Utilities/FreeTypeAtlas.h>
#include <Utilities/Include_Helpers/GL_Version.h>
#include <memory>   // for the shared pointer

namespace Rendering
{
    // this is a simple class that uses FreeType and the game's clock to print the frame rate to
    // the bottom left corner of the screen
    class __declspec(dllexport) FrameRateRenderer
    {
    public:
        FrameRateRenderer();

        // calls upon the FreeType encapsulation class to give it an atlas
        // Note: The FreeType encapsulation class MUST be initialized first.
        bool Init(const GLuint programId);

        // increments frame counter, and that's it
        void NewFrame();

        // draw the text for the frame rate
        void Render();
    private:
        // this stores info on the texture and draws string data
        // TODO: ??Make atlas a struct and move drawing to this FrameRateRenderer function??
        std::shared_ptr<Utilities::FreeTypeAtlas> _pAtlas;

        GLuint _programId;  // bind this before telling the atlas to draw rendering
        unsigned int _framesSinceLastUpdate;
        double _secondsSinceLastUpdate;
        double _frameRate;
    };

}