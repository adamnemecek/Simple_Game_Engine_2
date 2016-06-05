#include <Engine_2/Rendering/Renderer.h>
#include <Engine_2/Rendering/ParticleRenderer.h>
#include <Engine_2/Rendering/FrameRateCounter.h>
#include <Engine_2/Timing/Game_Clock.h>
#include <Engine_2/Scene/Scene_Data.h>
#include <Engine_2/Utilities/My_Assert.h>
#include <Engine_2/Utilities/ShaderManager.h>
#include <Engine_2/Utilities/FreeTypeEncapsulate.h>

#include <string>
#include <iostream>
using std::cout;
using std::endl;

#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>

// include this last to avoid errors when this includes gl.h 
// (apparently this needs to be included after the glm and glload and other libraries)
#include <GL/freeglut.h>

Rendering::Renderer gGeometryRenderer;
Rendering::ParticleRenderer gParticleRenderer;
Rendering::FrameRateRenderer gFrameRateRenderer;
Scene::Scene_Data gScene;

#include "experiment.h"

//Called after the window and OpenGL are initialized. Called exactly once, before the main loop.
void init()
{
   glEnable(GL_CULL_FACE);
   glCullFace(GL_BACK);
   glFrontFace(GL_CCW);

   glEnable(GL_DEPTH_TEST);
   glDepthMask(GL_TRUE);
   glDepthFunc(GL_LEQUAL);
   glDepthRange(0.0f, 1.0f);

   Utilities::ShaderManager &shaderManagerRef = Utilities::ShaderManager::GetInstance();

   // load the geometry rendering program
   std::string filePaths[] =
   {
       "data/experimental_shader.vert",
       "data/experimental_shader.frag",
   };
   GLenum shaderTypes[] =
   {
       GL_VERTEX_SHADER,
       GL_FRAGMENT_SHADER,
   };
   int mainRendererProgram = shaderManagerRef.LoadProgram("main", filePaths, shaderTypes, 2);
   MY_ASSERT(gGeometryRenderer.Init(mainRendererProgram));

   // load the particle rendering program
   filePaths[0] = "data/particles.vert";
   filePaths[1] = "data/particles.frag";
   int particleProgram = shaderManagerRef.LoadProgram("particle", filePaths, shaderTypes, 2);
   MY_ASSERT(gParticleRenderer.Init(particleProgram));

   // now load the program for FreeType
   filePaths[0] = "data/freeTypeText.vert";
   filePaths[1] = "data/freeTypeText.frag";
   // the shader type array is still ok
   int freeTypeProgram = shaderManagerRef.LoadProgram("freetype", filePaths, shaderTypes, 2);
   MY_ASSERT(Utilities::FreeTypeEncapsulate::GetInstance().
       Init("data/FreeSans.ttf", freeTypeProgram));
   MY_ASSERT(gFrameRateRenderer.Init(freeTypeProgram));

   // now the scene itself
   gScene.Init("C:/Users/John/Documents/GitHub/Simple_Game_Engine_2/scene_save_exp.xml");
   gScene.load_from_blender_obj("C:/Users/John/Documents/untitled.obj");
   gScene.ConfigureGeometryRenderer(&gGeometryRenderer);
   gScene.ConfigureParticleRenderer(&gParticleRenderer);

   // start the game clock
   MY_ASSERT(Timing::Game_Clock::get_instance().initialize());
}

//Called to update the display.
//You should call glutSwapBuffers after all of your rendering to display what you rendered.
//If you need continuous updates of the screen, call glutPostRedisplay() at the end of the function.
void display()
{
   Timing::Game_Clock::get_instance().new_frame();
   //float delta_time = Timing::Game_Clock::get_instance().get_delta_time_last_frame();
   //if (0 != delta_time)
   //{
   //   printf("frame rate: '%.2f'\n", 1.0f / delta_time);
   //}

   //Collision_Detection::Collision_Handler::get_instance().update();

   gScene.update();
   gGeometryRenderer.Render();
   gParticleRenderer.Render();
   gFrameRateRenderer.NewFrame();
   gFrameRateRenderer.Render();

   glutSwapBuffers();
   glutPostRedisplay();
}

//Called whenever the window is resized. The new window size is given, in pixels.
//This is an opportunity to call glViewport or glScissor to keep up with the change in size.
void reshape (int w, int h)
{
   gGeometryRenderer.SetViewport(w, h);
   gParticleRenderer.SetViewport(w, h);
}

//Called whenever a key on the keyboard was pressed.
//The key is given by the ''key'' parameter, which is in ASCII.
//It's often a good idea to have the escape key (ASCII value 27) call glutLeaveMainLoop() to 
//exit the program.
void keyboard(unsigned char key, int x, int y)
{
   using Rendering::Renderer;

   switch (key)
   {
      case 32:
      {
         // space bar
         break;
      }

      case 27:
      {
         glutLeaveMainLoop();
         return;
      }
      default:
         break;
   }
}

//Called before FreeGLUT is initialized. It should return the FreeGLUT
//display mode flags that you want to use. The initial value are the standard ones
//used by the framework. You can modify it or just return you own set.
//This function can also set the width/height of the window. The initial
//value of these variables is the default, but you can change it.
unsigned int defaults(unsigned int displayMode, int &width, int &height) {return displayMode;}

