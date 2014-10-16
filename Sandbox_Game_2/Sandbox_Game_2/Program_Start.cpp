//Copyright (C) 2010-2012 by Jason L. McKesson
//This file is licensed under the MIT License.


/***********************************************************************
This file contains all of the functions you need to implement, as well as
when/why they are called. You can copy this file and simply implement
these function.
***********************************************************************/

#define MY_ENGINE

#ifdef MY_ENGINE
#include <Engine_2\Shapes\Geometry.h>
#include <Engine_2\Shapes\Geometry_Creation\Geometry_Loader.h>
#include <Engine_2\Utilities\Shader_Maker.h>
#include <Engine_2\Utilities\Include_GLM_Mat_Transform.h>
#include <Engine_2\Rendering\Renderer.h>
#include <Engine_2\Rendering\Renderable.h>
#include <Engine_2\Utilities\include_GL_version.h>

#include <Engine_2\Entities\Entity.h>
#include <Engine_2\Entities\Components\Controller_Component.h>
#include <Engine_2\Entities\Components\Renderable_Updater_Component.h>
#include <Engine_2\Input\Supported_Bindings.h>
#include <Engine_2\Entities\Components\Physics_Component.h>

#include <cassert>

#include <string>
#include <iostream>
using std::cout;
using std::endl;

#include <glm\vec3.hpp>
#include <glm\mat4x4.hpp>

// include this last to avoid errors when this includes gl.h 
// (apparently this needs to be included after the glm and glload and other libraries)
#include <GL/freeglut.h>


GLuint g_program_ID;

Entities::Entity g_shape_entity;
Entities::Controller_Component g_controller_component;
Entities::Renderable_Updater_Component g_renderable_updater_component;
Entities::Physics_Component g_physics_component;
Rendering::Renderer g_renderer;
Rendering::Renderable *g_renderable_ptr;
Shapes::Geometry g_geometry;


#else
#include <glload/gl_3_3_comp.h>
#include <GL/freeglut.h>
#include <framework.h>
#include <vector>

GLuint theProgram;
#endif






#ifndef MY_ENGINE
void initialize_program(const std::string &str_vertex_shader, const std::string &str_fragment_shader)
{
   std::vector<GLuint> shader_list;
   shader_list.push_back(Framework::LoadShader(GL_VERTEX_SHADER, str_vertex_shader));
   shader_list.push_back(Framework::LoadShader(GL_FRAGMENT_SHADER, str_fragment_shader));

   theProgram = Framework::CreateProgram(shader_list);
}

const float vertexData[] = {
   1.0f, 0.0f, 0.0f, 1.0f,
   0.0f, 1.0f, 0.0f, 1.0f,
   0.0f, 0.0f, 1.0f, 1.0f,
   //0.0f, 0.5f, 0.0f, 1.0f,
   //0.5f, -0.366f, 0.0f, 1.0f,
   //-0.5f, -0.366f, 0.0f, 1.0f,
   0.0f, 0.5f, +3.0f, 1.0f,
   0.5f, -0.366f, -3.0f, 1.0f,
   -0.5f, -0.366f, -3.0f, 1.0f,
};

GLuint vertexBufferObject;
GLuint vao;

void initialize_vertex_buffer()
{
   glGenBuffers(1, &vertexBufferObject);
   glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObject);
   glBufferData(GL_ARRAY_BUFFER, sizeof(vertexData), vertexData, GL_STATIC_DRAW);
   glBindBuffer(GL_ARRAY_BUFFER, 0);
}

#endif


//Called after the window and OpenGL are initialized. Called exactly once, before the main loop.
void init()
{
#ifdef MY_ENGINE
   //glDisable(GL_CULL_FACE);

   bool initialize_success = false;
   initialize_success = g_renderer.initialize();
   assert(initialize_success);

   std::string file_paths[] =
   {
      "data/basic_shader.vert",
      "data/basic_shader.frag",
   };
   GLenum shader_types[] =
   {
      GL_VERTEX_SHADER,
      GL_FRAGMENT_SHADER,
   };

   GLuint program_ID = Utilities::Shader_Maker::create_shader_program(file_paths, shader_types, 2);
   initialize_success = g_renderer.add_shader_program(program_ID);
   assert(initialize_success);
   initialize_success = g_renderer.bind_shader_program(program_ID);
   assert(initialize_success);

   cout << "Program ID: " << program_ID << endl;

   using Shapes::Geometry_Creation::Geometry_Loader;
   Geometry_Loader::load_from_generator(Geometry_Loader::CUBE, g_geometry);
   g_renderable_ptr = g_renderer.add_renderable(&g_geometry);


   // Entity creation and order of component attachment:
   // - Controller - read inputs
   // - Physics - respond to inputs
   // - Renderable Updater - update model_to_world matrix

   initialize_success = g_controller_component.set_key_binding(Input::SUPPORTED_BINDINGS::KEYBOARD);
   assert(initialize_success);

   g_renderable_updater_component.set_renderable(g_renderable_ptr);

   g_shape_entity.add_component(&g_controller_component);
   g_shape_entity.add_component(&g_physics_component);
   g_shape_entity.add_component(&g_renderable_updater_component);

   initialize_success = g_shape_entity.initialize();
   assert(initialize_success);


   float translation = -6.001f;
   float rotation_rad = -(3.14159f * 1.0f);
   glm::mat4 model_to_world = 
      glm::translate(glm::mat4(), glm::vec3(0.0f, 0.0f, translation)) * 
      glm::rotate(glm::mat4(), rotation_rad, glm::vec3(1.0f, 1.0f, 1.0f));
   g_renderable_ptr->m_model_to_world_mat = model_to_world;
#else
   initialize_program("VertexColors.vert", "VertexColors.frag");
   initialize_vertex_buffer();
   glGenVertexArrays(1, &vao);
   glBindVertexArray(vao);

#endif
}

//Called to update the display.
//You should call glutSwapBuffers after all of your rendering to display what you rendered.
//If you need continuous updates of the screen, call glutPostRedisplay() at the end of the function.
void display()
{
#ifdef MY_ENGINE
   g_shape_entity.update();
   g_renderer.render_scene();
#else
   glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
   glClear(GL_COLOR_BUFFER_BIT);

   glUseProgram(theProgram);

   glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObject);
   glEnableVertexAttribArray(0);
   glEnableVertexAttribArray(15);
   //glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, 0);
   //glVertexAttribPointer(15, 4, GL_FLOAT, GL_FALSE, 0, (void*)48);
   glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, (void*)48);
   glVertexAttribPointer(15, 4, GL_FLOAT, GL_FALSE, 0, 0);

   glDrawArrays(GL_TRIANGLES, 0, 3);

   glDisableVertexAttribArray(0);
   glDisableVertexAttribArray(15);
   glUseProgram(0);
#endif


   glutSwapBuffers();
   glutPostRedisplay();
}

//Called whenever the window is resized. The new window size is given, in pixels.
//This is an opportunity to call glViewport or glScissor to keep up with the change in size.
void reshape (int w, int h)
{
#ifdef MY_ENGINE
   g_renderer.set_viewport(w, h);
#else
   glViewport(0, 0, w, h);
#endif
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
   //case 'w':
   //{
   //   g_renderer.manipulate_active_camera(Renderer::CAMERA_MOVE_FORWARD);
   //   break;
   //}
   //case 'a':
   //{
   //   g_renderer.manipulate_active_camera(Renderer::CAMERA_MOVE_LEFT);
   //   break;
   //}
   //case 's':
   //{
   //   g_renderer.manipulate_active_camera(Renderer::CAMERA_MOVE_BACK);
   //   break;
   //}
   //case 'd':
   //{
   //   g_renderer.manipulate_active_camera(Renderer::CAMERA_MOVE_RIGHT);
   //   break;
   //}
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
