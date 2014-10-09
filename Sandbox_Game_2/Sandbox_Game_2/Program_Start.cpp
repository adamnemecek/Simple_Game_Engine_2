//Copyright (C) 2010-2012 by Jason L. McKesson
//This file is licensed under the MIT License.


/***********************************************************************
This file contains all of the functions you need to implement, as well as
when/why they are called. You can copy this file and simply implement
these function.
***********************************************************************/

#include <Engine_2\Rendering\Renderer.h>
#include <Engine_2\Rendering\Renderable.h>
#include <Engine_2\Utilities\include_GL_version.h>

#include <string>
using std::string;

#include <iostream>
using std::cout;
using std::endl;

#include <Engine_2\Shapes\Geometry.h>
#include <Engine_2\Shapes\Geometry_Creation\Geometry_Loader.h>

#define GLM_FORCE_RADIANS
#include <glm\gtc\matrix_transform.hpp>
using glm::translate;
using glm::rotate;

#include <glm\vec3.hpp>
using glm::vec3;
#include <glm\mat4x4.hpp>
using glm::mat4;

//#include <glload/gl_3_3_comp.h>

// include this last to avoid errors when this includes gl.h 
// (apparently this needs to be included after the glm and glload and other libraries)
#include <GL/freeglut.h>
//#include <framework.h>
//#include <vector>


Rendering::Renderer g_renderer;

Rendering::Renderable *g_renderable;
Shapes::Geometry g_geometry;

//GLuint g_program_ID;
//
//void initialize_program(const std::string &str_vertex_shader, const std::string &str_fragment_shader)
//{
//   std::vector<GLuint> shader_list;
//   shader_list.push_back(Framework::LoadShader(GL_VERTEX_SHADER, str_vertex_shader));
//   shader_list.push_back(Framework::LoadShader(GL_FRAGMENT_SHADER, str_fragment_shader));
//
//   g_program_ID = Framework::CreateProgram(shader_list);
//}
//
//const float vertexData[] = {
//   1.0f, 0.0f, 0.0f, 1.0f,
//   0.0f, 1.0f, 0.0f, 1.0f,
//   0.0f, 0.0f, 1.0f, 1.0f,
//   0.0f, 0.5f, 0.0f, 1.0f,
//   0.5f, -0.366f, 0.0f, 1.0f,
//   -0.5f, -0.366f, 0.0f, 1.0f,
//};
//
//GLuint vertexBufferObject;
//GLuint vao;
//
//void initialize_vertex_buffer()
//{
//   glGenBuffers(1, &vertexBufferObject);
//   glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObject);
//   glBufferData(GL_ARRAY_BUFFER, sizeof(vertexData), vertexData, GL_STATIC_DRAW);
//   glBindBuffer(GL_ARRAY_BUFFER, 0);
//}




//Called after the window and OpenGL are initialized. Called exactly once, before the main loop.
void init()
{
   //initialize_program("VertexColors.vert", "VertexColors.frag");
   //initialize_vertex_buffer();
   //glGenVertexArrays(1, &vao);
   //glBindVertexArray(vao);

   // load level

   if (!g_renderer.initialize()) { exit(1); }
   string file_paths[] = 
   {
      "VertexColors.vert",
      "VertexColors.frag",
   };
   GLenum shader_types[] = 
   {
      GL_VERTEX_SHADER,
      GL_FRAGMENT_SHADER,
   };

   GLuint program_ID = g_renderer.create_shader_program(file_paths, shader_types, 2);
   if (!g_renderer.bind_shader_program(program_ID)) { exit(1); }

   cout << "Program ID: " << program_ID << endl;

   using Shapes::Geometry_Creation::Geometry_Loader;
   Geometry_Loader::load_from_generator(Geometry_Loader::TRIANGLE_2D, g_geometry);

   g_renderable = g_renderer.add_renderable(&g_geometry);

   float translation = 2.0f;
   mat4 model_to_world = translate(mat4(), vec3(0.0f, 0.0f, translation)) * rotate(mat4(), 0.5f, vec3(0.0f, 0.0f, +1.0f));
   //mat4 orientation_only = rotate(mat4(), 0.0f, vec3(0.0f, 1.0f, 0.0f));
   g_renderable->m_model_to_world_mat = model_to_world;
   //g_renderable->m_orientation_only_mat = orientation_only;
}

//Called to update the display.
//You should call glutSwapBuffers after all of your rendering to display what you rendered.
//If you need continuous updates of the screen, call glutPostRedisplay() at the end of the function.
void display()
{
   g_renderer.render_scene();

   glutSwapBuffers();
   glutPostRedisplay();
}

//Called whenever the window is resized. The new window size is given, in pixels.
//This is an opportunity to call glViewport or glScissor to keep up with the change in size.
void reshape (int w, int h)
{
   g_renderer.set_viewport(w, h);
}

//Called whenever a key on the keyboard was pressed.
//The key is given by the ''key'' parameter, which is in ASCII.
//It's often a good idea to have the escape key (ASCII value 27) call glutLeaveMainLoop() to 
//exit the program.
void keyboard(unsigned char key, int x, int y)
{
   switch (key)
   {
   case 27:
      glutLeaveMainLoop();
      return;
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
