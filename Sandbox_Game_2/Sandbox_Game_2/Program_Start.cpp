//Copyright (C) 2010-2012 by Jason L. McKesson
//This file is licensed under the MIT License.


/***********************************************************************
This file contains all of the functions you need to implement, as well as
when/why they are called. You can copy this file and simply implement
these function.
***********************************************************************/


#include <Engine_2\Shapes\Geometry.h>
#include <Engine_2\Shapes\Geometry_Creation\Geometry_Loader.h>
#include <Engine_2\Utilities\Shader_Maker.h>
#include <Engine_2\Rendering\Renderer.h>
#include <Engine_2\Rendering\Renderable.h>
#include <Rendering\Camera.h>
#include <Engine_2\Utilities\Include_Helpers\GL_Version.h>

#include <Engine_2\Entities\Entity.h>
#include <Engine_2\Entities\Components\Controller_Component.h>
#include <Engine_2\Entities\Components\Renderable_Updater_Component.h>
#include <Engine_2\Input\Supported_Bindings.h>
#include <Engine_2\Entities\Components\Physics_Component.h>
#include <Engine_2\Timing\Game_Clock.h>

#include <Engine_2\Utilities\Quaternion_Helper.h>
#include <Engine_2\Utilities\Printer_Helper.h>
#include <Engine_2\Utilities\My_Assert.h>

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

Rendering::Renderer g_renderer;
Rendering::Camera g_camera;

Shapes::Geometry g_cube_geometry;
Entities::Entity g_cube_1_entity;
Entities::Entity g_cube_2_entity;
Entities::Entity g_cube_3_entity;
Entities::Entity g_cube_4_entity;
Rendering::Renderable *g_cube_1_renderable_ptr;
Rendering::Renderable *g_cube_2_renderable_ptr;
Rendering::Renderable *g_cube_3_renderable_ptr;
Rendering::Renderable *g_cube_4_renderable_ptr;
Entities::Renderable_Updater_Component g_cube_1_renderable_updater_component;
Entities::Renderable_Updater_Component g_cube_2_renderable_updater_component;
Entities::Renderable_Updater_Component g_cube_3_renderable_updater_component;
Entities::Renderable_Updater_Component g_cube_4_renderable_updater_component;

Entities::Entity g_plane_entity;
Shapes::Geometry g_plane_geometry;
Rendering::Renderable *g_plane_renderable_ptr;
Entities::Renderable_Updater_Component g_plane_renderable_updater_component;

Entities::Entity g_circle_entity;
Shapes::Geometry g_circle_geometry;
Rendering::Renderable * g_circle_renderable_ptr;
Entities::Renderable_Updater_Component g_circle_renderable_updater_component;

Entities::Entity g_rectangle_box_entity;
Shapes::Geometry g_rectangle_box_geometry;
Rendering::Renderable * g_rectangle_box_renderable_ptr;
Entities::Renderable_Updater_Component g_rectangle_box_renderable_updater_component;
Entities::Physics_Component g_rectangle_box_physics;

Entities::Entity g_camera_entity;
Entities::Controller_Component g_controller_component;


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


   bool initialize_success = false;
   initialize_success = g_renderer.initialize();
   MY_ASSERT(initialize_success);

   std::string file_paths[] =
   {
      "data/diffuse_shader.vert",
      "data/diffuse_shader.frag",
   };
   GLenum shader_types[] =
   {
      GL_VERTEX_SHADER,
      GL_FRAGMENT_SHADER,
   };

   GLuint program_ID = Utilities::Shader_Maker::create_shader_program(file_paths, shader_types, 2);
   initialize_success = g_renderer.add_shader_program(program_ID);
   MY_ASSERT(initialize_success);
   initialize_success = g_renderer.bind_shader_program(program_ID);
   MY_ASSERT(initialize_success);

   cout << "Program ID: " << program_ID << endl;

   g_renderer.set_camera_to_render(&g_camera);

   using Shapes::Geometry_Creation::Geometry_Loader;
   Geometry_Loader::load_cube(&g_cube_geometry);

   g_cube_1_renderable_ptr = g_renderer.add_renderable(&g_cube_geometry);
   g_cube_2_renderable_ptr = g_renderer.add_renderable(&g_cube_geometry);
   g_cube_3_renderable_ptr = g_renderer.add_renderable(&g_cube_geometry);
   g_cube_4_renderable_ptr = g_renderer.add_renderable(&g_cube_geometry);

   g_cube_1_renderable_updater_component.set_renderable(g_cube_1_renderable_ptr);
   g_cube_2_renderable_updater_component.set_renderable(g_cube_2_renderable_ptr);
   g_cube_3_renderable_updater_component.set_renderable(g_cube_3_renderable_ptr);
   g_cube_4_renderable_updater_component.set_renderable(g_cube_4_renderable_ptr);

   g_cube_1_entity.add_component(&g_cube_1_renderable_updater_component);
   g_cube_2_entity.add_component(&g_cube_2_renderable_updater_component);
   g_cube_3_entity.add_component(&g_cube_3_renderable_updater_component);
   g_cube_4_entity.add_component(&g_cube_4_renderable_updater_component);


   initialize_success = g_cube_1_entity.initialize();
   MY_ASSERT(initialize_success);
   initialize_success = g_cube_2_entity.initialize();
   MY_ASSERT(initialize_success);
   initialize_success = g_cube_3_entity.initialize();
   MY_ASSERT(initialize_success);
   initialize_success = g_cube_4_entity.initialize();
   MY_ASSERT(initialize_success);

   // set some initial positions and rotations for these cubes
   //g_cube_1_entity.m_position = glm::vec3(+3.0f, +3.0f, +3.0f);
   //g_cube_1_entity.m_base_orientation = glm::vec3(1.0f, 1.0f, 1.0f);

   glm::fquat quat;
   Utilities::Quaternion_Helper::orientation_offset(glm::vec3(+1.0f, 0.0f, +1.0f), 0.5f, quat);
   //g_cube_1_entity.m_where_and_which_way = Utilities::Quaternion_Helper::make_dual_quat(quat, glm::vec3(+0.0f, +3.0f, +0.0f));
   
   //g_cube_2_entity.m_position = glm::vec3(+3.0f, +3.0f, -3.0f);
   //g_cube_2_entity.m_base_orientation = glm::vec3(1.0f, 1.0f, -1.0f);

   quat = glm::fquat();
   Utilities::Quaternion_Helper::orientation_offset(glm::vec3(+1.0f, 0.0f, -1.0f), 0.5f, quat);
   //g_cube_2_entity.m_where_and_which_way = Utilities::Quaternion_Helper::make_dual_quat(quat, glm::vec3(+3.0f, +3.0f, -3.0f));
   
   //g_cube_3_entity.m_position = glm::vec3(-3.0f, +3.0f, -3.0f);
   //g_cube_3_entity.m_base_orientation = glm::vec3(-1.0f, 1.0f, -1.0f);

   quat = glm::fquat();
   Utilities::Quaternion_Helper::orientation_offset(glm::vec3(-1.0f, 0.0f, -1.0f), 0.5f, quat);
   //g_cube_3_entity.m_where_and_which_way = Utilities::Quaternion_Helper::make_dual_quat(quat, glm::vec3(-3.0f, +3.0f, -3.0f));
   
   //g_cube_4_entity.m_position = glm::vec3(-3.0f, +3.0f, +3.0f);
   //g_cube_4_entity.m_base_orientation = glm::vec3(-1.0f, 1.0f, 1.0f);

   quat = glm::fquat();
   Utilities::Quaternion_Helper::orientation_offset(glm::vec3(-1.0f, 0.0f, +1.0f), 0.5f, quat);
   //g_cube_4_entity.m_where_and_which_way = Utilities::Quaternion_Helper::make_dual_quat(quat, glm::vec3(-3.0f, +3.0f, +3.0f));


   // set up the camera entity
   // Note: Set it to be off to the side of and above the scene and looking into it.
   g_camera.set_entity_to_follow(&g_camera_entity);
   initialize_success = g_controller_component.set_key_binding(Input::SUPPORTED_BINDINGS::KEYBOARD);
   MY_ASSERT(initialize_success);
   g_camera_entity.add_component(&g_controller_component);
   //g_camera_entity.add_component(&g_cube_1_renderable_updater_component);
   g_camera_entity.initialize();
   //g_camera_entity.m_position = glm::vec3(-8.0f, +6.0f, -8.0f);
   //g_camera_entity.m_base_orientation = (-1.0f) * g_camera_entity.m_position;

   // start the camera above and looking down at the scene
   quat = glm::fquat();
   glm::fdualquat translation = Utilities::Quaternion_Helper::make_dual_quat_translation_only(glm::vec3(0.0f, 20.0f, 0.0f));
   Utilities::Quaternion_Helper::orientation_offset(glm::vec3(1.0f, 0.0f, 0.0f), 3.14159f / 2.0f, quat);
   //Utilities::Quaternion_Helper::orientation_offset(glm::vec3(1.0f, 0.0f, 0.0f), 1.0, quat);
   g_camera_entity.m_where_and_which_way = Utilities::Quaternion_Helper::make_dual_quat_rotation_only(quat) * translation;
   //g_camera_entity.m_where_and_which_way = Utilities::Quaternion_Helper::make_dual_quat(quat, glm::vec3(0.0f, 10.0f, 0.0f));


   // and the plane
   // Note: Let the plane be it's default size and remain at the origin.
   Geometry_Loader::load_plane(&g_plane_geometry, 40);
   g_plane_renderable_ptr = g_renderer.add_renderable(&g_plane_geometry);
   g_plane_renderable_updater_component.set_renderable(g_plane_renderable_ptr);
   g_plane_entity.add_component(&g_plane_renderable_updater_component);
   initialize_success = g_plane_entity.initialize();
   MY_ASSERT(initialize_success);

   // and the circle
   Geometry_Loader::load_circle(&g_circle_geometry);
   g_circle_renderable_ptr = g_renderer.add_renderable(&g_circle_geometry);
   g_circle_renderable_updater_component.set_renderable(g_circle_renderable_ptr);
   g_circle_entity.add_component(&g_circle_renderable_updater_component);
   //g_circle_entity.m_position = glm::vec3(0.0f, +3.0f, 0.0f);

   g_circle_entity.m_where_and_which_way = Utilities::Quaternion_Helper::make_dual_quat(glm::fquat(), glm::vec3(0.0f, +3.0f, 0.0f));

   initialize_success = g_circle_entity.initialize();
   MY_ASSERT(initialize_success);

   // and the box
   Geometry_Loader::load_box(&g_rectangle_box_geometry, 5.0f, 2.0f);
   g_rectangle_box_entity.add_component(&g_rectangle_box_physics);
   g_rectangle_box_renderable_ptr = g_renderer.add_renderable(&g_rectangle_box_geometry);
   g_rectangle_box_renderable_updater_component.set_renderable(g_rectangle_box_renderable_ptr);
   g_rectangle_box_entity.add_component(&g_rectangle_box_renderable_updater_component);
   //g_rectangle_box_entity.m_position = glm::vec3(0.0f, +3.0f, 0.0f);

   g_rectangle_box_entity.m_where_and_which_way = Utilities::Quaternion_Helper::make_dual_quat(glm::fquat(), glm::vec3(0.0f, +3.0f, 0.0f));

   initialize_success = g_rectangle_box_entity.initialize();
   MY_ASSERT(initialize_success);

   g_rectangle_box_physics.add_sustained_force_vector(glm::vec3(0.0f, 0.0f, 1.0f));

   // start the game clock
   initialize_success = Timing::Game_Clock::get_instance().initialize();
   MY_ASSERT(initialize_success);
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

   g_cube_1_entity.update();
   g_cube_2_entity.update();
   g_cube_3_entity.update();
   g_cube_4_entity.update();
   g_plane_entity.update();
   g_circle_entity.update();

   g_rectangle_box_entity.update();

   //Utilities::Printer_Helper::print_dual_quat("rectangle: ", g_rectangle_box_entity.m_where_and_which_way);

   g_camera_entity.update();
   g_camera.update();
   
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
   using Rendering::Renderer;

   switch (key)
   {
   case 'w':
   {
      g_rectangle_box_physics.add_immediate_force_vector(glm::vec3(0.0f, 0.0f, -20.0f));
      break;
   }
   case 'a':
   {
      g_rectangle_box_physics.add_immediate_force_vector(glm::vec3(-20.0f, 0.0f, 0.0f));
      break;
   }
   case 's': 
   {
      g_rectangle_box_physics.add_immediate_force_vector(glm::vec3(0.0f, 0.0f, +20.0f));
      break;
   }
   case 'd':
   {
      g_rectangle_box_physics.add_immediate_force_vector(glm::vec3(+20.0f, 0.0f, 0.0f));
      break;
   }
   case 32:
   {
      // space bar
      g_rectangle_box_physics.reflect_linear_velocity_around_vector(glm::vec3(0.0f, 0.0f, -1.0f));
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

