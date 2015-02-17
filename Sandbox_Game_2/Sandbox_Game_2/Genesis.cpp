//Copyright (C) 2010-2012 by Jason L. McKesson
//This file is licensed under the MIT License.


/***********************************************************************
This file contains all of the functions you need to implement, as well as
when/why they are called. You can copy this file and simply implement
these function.
***********************************************************************/


#include <Engine_2\Shapes\Geometry.h>
#include <Engine_2\Shapes\Geometry_Creation\Shape_Generator.h>
#include <Engine_2\Utilities\Shader_Maker.h>
#include <Engine_2\Rendering\Renderer.h>
#include <Engine_2\Rendering\Renderable.h>
#include <Rendering\Camera.h>
#include <Engine_2\Utilities\Include_Helpers\GL_Version.h>

#include <Engine_2\Entities\Entity.h>
#include <Engine_2\Entities\Components\Controller_Component.h>
#include <Engine_2\Entities\Components\Physics_Component.h>
#include <Engine_2\Entities\Components\AABB_Component.h>
#include <Engine_2\Collision_Detection\Collision_Handler.h>
#include <Engine_2\Input\Supported_Bindings.h>
#include <Engine_2\Timing\Game_Clock.h>

#include <Engine_2\Scene\Scene_Data.h>
#include <Engine_2\Scene\Scene_Loader.h>

#include <Engine_2\Utilities\Printer_Helper.h>
#include <Engine_2\Utilities\My_Assert.h>

#include <Engine_2\Math\F_Dual_Quat.h>

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

Shapes::Geometry *g_plane_geometry_ptr;
Shapes::Geometry *g_cube_geometry_ptr;
Shapes::Geometry *g_circle_geometry_ptr;
Shapes::Geometry *g_rectangle_box_geometry_ptr;
Shapes::Geometry *g_experimental_geometry_ptr;

Entities::Entity g_cube_1_entity("cube_1");
//Rendering::Renderable *g_cube_1_renderable_ptr;
Entities::Physics_Component g_cube_1_physics;
Entities::AABB_Component g_cube_1_bounding_box;

Entities::Entity g_cube_2_entity("cube_2");
//Rendering::Renderable *g_cube_2_renderable_ptr;
Entities::Physics_Component g_cube_2_physics;
Entities::AABB_Component g_cube_2_bounding_box;

Entities::Entity g_cube_3_entity("cube_3");
//Rendering::Renderable *g_cube_3_renderable_ptr;

Entities::Entity g_cube_4_entity("cube_4");
//Rendering::Renderable *g_cube_4_renderable_ptr;

Entities::Entity g_plane_entity("plane");
//Rendering::Renderable *g_plane_renderable_ptr;

Entities::Entity g_circle_entity("circle");
//Rendering::Renderable * g_circle_renderable_ptr;

Entities::Entity g_rectangle_box_entity("rectangle");
//Rendering::Renderable * g_rectangle_box_renderable_ptr;
Entities::Physics_Component g_rectangle_box_physics;

Entities::Entity g_experimental_entity("experiment");

Entities::Entity g_camera_entity("camera");
Entities::Controller_Component g_controller_component;


#include "experiment.h"

//Called after the window and OpenGL are initialized. Called exactly once, before the main loop.
void init()
{
   Scene::Scene_Data local_scene;
   local_scene.load("C:/Users/John/Documents/GitHub/Simple_Game_Engine_2/scene_save_exp.xml");

   //Scene::Scene_Loader& SL = Scene::Scene_Loader::get_instance();
   //SL.load_scene(&g_renderer, &local_scene);

   //Experiment::do_something();

   glEnable(GL_CULL_FACE);
   glCullFace(GL_BACK);
   glFrontFace(GL_CCW);

   glEnable(GL_DEPTH_TEST);
   glDepthMask(GL_TRUE);
   glDepthFunc(GL_LEQUAL);
   glDepthRange(0.0f, 1.0f);


   MY_ASSERT(g_renderer.initialize());

   std::string file_paths[] =
   {
      "data/experimental_shader.vert",
      "data/experimental_shader.frag",
   };
   GLenum shader_types[] =
   {
      GL_VERTEX_SHADER,
      GL_FRAGMENT_SHADER,
   };

   GLuint program_ID = Utilities::Shader_Maker::create_shader_program(file_paths, shader_types, 2);
   MY_ASSERT(g_renderer.add_shader_program(program_ID));
   MY_ASSERT(g_renderer.bind_shader_program(program_ID));

   cout << "Program ID: " << program_ID << endl;

   g_renderer.set_camera_to_render(&g_camera);

   // initialize the collision detector, which causes it to forget any bounding boxes that had been added
   Collision_Detection::Collision_Handler::get_instance().initialize();
   
   using Shapes::Geometry_Creation::Shape_Generator;
   Shape_Generator& shape_generator_ref = Shape_Generator::get_instance();

   Shapes::Shape_Data *cube_shape_ptr;
   cube_shape_ptr = shape_generator_ref.generate_cube();
   g_cube_geometry_ptr = new Shapes::Geometry(cube_shape_ptr, "my_cube");

   Shapes::Shape_Data *plane_shape_ptr;
   plane_shape_ptr = shape_generator_ref.generate_plane(20, 50, 11, 12);
   g_plane_geometry_ptr = new Shapes::Geometry(plane_shape_ptr, "my_plane");

   Shapes::Shape_Data *circle_shape_ptr;
   circle_shape_ptr = shape_generator_ref.generate_circle(15, 1.4);
   g_circle_geometry_ptr = new Shapes::Geometry(circle_shape_ptr, "my_circle");

   Shapes::Shape_Data *rectangle_shape_ptr;
   rectangle_shape_ptr = shape_generator_ref.generate_box(3, 2);
   g_rectangle_box_geometry_ptr = new Shapes::Geometry(rectangle_shape_ptr, "my_rectangle");

   Shapes::Shape_Data *experiment_shape_ptr;
   experiment_shape_ptr = shape_generator_ref.generate_sphere(12, 1.7f, 11);
   g_experimental_geometry_ptr = new Shapes::Geometry(experiment_shape_ptr, "my_experiment");










   // entity initialization notes:
   // - add the physics component before the renderable updater and the bounding box because
   // the physics component updates the entity's dual quat representing location and orientation
   // - give the physics initial force values AFTER initialization because initialization 
   // resets the physics values
   // - the entity's dual quat can be set any time during initialization because entity 
   // initialization initializes the components and does not set the dual quat values

   // add physics first so that the entity's transform is updated for the frame
   g_renderer.configure_new_renderable(g_cube_geometry_ptr, &g_cube_1_entity);
   g_cube_1_bounding_box.set_geometry(g_cube_geometry_ptr);
   g_cube_1_entity.add_component(&g_cube_1_physics);
   g_cube_1_entity.add_component(&g_cube_1_bounding_box);
   Collision_Detection::Collision_Handler::get_instance().add_collision_data(&g_cube_1_bounding_box, &g_cube_1_physics);
   MY_ASSERT(g_cube_1_entity.initialize());
   g_cube_1_entity.m_where_and_which_way = Math::F_Dual_Quat::generate_rotate_then_translate(glm::vec3(+1.0f, 0.0f, +1.0f), 0.3f, glm::vec3(0.0f, +2.0f, +4.0f));
   g_cube_1_physics.add_sustained_force_vector(glm::vec3(+0.0f, 0.0f, -0.0f));
   g_cube_1_physics.add_mass(5.0f);

   g_renderer.configure_new_renderable(g_cube_geometry_ptr, &g_cube_2_entity);
   g_cube_2_bounding_box.set_geometry(g_cube_geometry_ptr);
   g_cube_2_entity.add_component(&g_cube_2_physics);
   g_cube_2_entity.add_component(&g_cube_2_bounding_box);
   Collision_Detection::Collision_Handler::get_instance().add_collision_data(&g_cube_2_bounding_box, &g_cube_2_physics);
   MY_ASSERT(g_cube_2_entity.initialize());
   g_cube_2_entity.m_where_and_which_way = Math::F_Dual_Quat::generate_rotate_then_translate(glm::vec3(+1.0f, 0.0f, -1.0f), 0.6f, glm::vec3(+4.0f, +3.0f, 0.0f));
   g_cube_2_physics.add_sustained_force_vector(glm::vec3(0.0f, 0.0f, +0.0f));
   g_cube_2_physics.add_mass(10.0f);

   g_renderer.configure_new_renderable(g_cube_geometry_ptr, &g_cube_3_entity);
   MY_ASSERT(g_cube_3_entity.initialize());
   g_cube_3_entity.m_where_and_which_way = Math::F_Dual_Quat::generate_rotate_then_translate(glm::vec3(-1.0f, 0.0f, -1.0f), 0.9f, glm::vec3(0.0f, +4.0f, -4.0f));

   g_renderer.configure_new_renderable(g_cube_geometry_ptr, &g_cube_4_entity);
   MY_ASSERT(g_cube_4_entity.initialize());
   g_cube_4_entity.m_where_and_which_way = Math::F_Dual_Quat::generate_rotate_then_translate(glm::vec3(-1.0f, 0.0f, +1.0f), 1.2f, glm::vec3(-4.0f, +5.0f, 0.0f));


   g_renderer.configure_new_renderable(g_experimental_geometry_ptr, &g_experimental_entity);
   MY_ASSERT(g_experimental_entity.initialize());
   g_experimental_entity.m_where_and_which_way = Math::F_Dual_Quat::generate_rotate_then_translate(glm::vec3(1.0f, 0.0f, 0.0f), 0.0f, glm::vec3(0.0f, +4.0f, 0.0f));

   // set up the camera entity
   // Note: Set it to be off to the side of and above the scene and looking into it.
   // Note: Also, steal the 4th cube's renderable so that the camera's entity can sit out in front of it and I can see it.
   g_camera.set_entity_to_follow(&g_camera_entity);
   MY_ASSERT(g_controller_component.set_key_binding(Input::SUPPORTED_BINDINGS::KEYBOARD));
   g_camera_entity.add_component(&g_controller_component);

   g_renderer.configure_new_renderable(g_experimental_geometry_ptr, &g_camera_entity);
   MY_ASSERT(g_camera_entity.initialize());

   // I got these numbers by having the camera print out its dual quat, and then I 
   // copied the numbers when the camera was at the desired starting point.
   Math::F_Quat temp_real(0.92f, glm::vec3(-0.17f, 0.32f, 0.09f));
   Math::F_Quat temp_dual(-0.44f, glm::vec3(0.47f, 0.42f, 3.41f));
   Math::F_Dual_Quat entity_camera_offset(temp_real, temp_dual);
   g_camera_entity.m_where_and_which_way = entity_camera_offset;


   // and the plane
   // Note: Let the plane be it's default size and remain at the origin.
   g_renderer.configure_new_renderable(g_plane_geometry_ptr, &g_plane_entity);
   MY_ASSERT(g_plane_entity.initialize());

   // and the circle
   g_renderer.configure_new_renderable(g_circle_geometry_ptr, &g_circle_entity);
   Math::F_Dual_Quat entity_circle_offset = Math::F_Dual_Quat::generate_translate_only(glm::vec3(0.0f, +3.0f, 0.0f));
   g_circle_entity.m_where_and_which_way = entity_circle_offset;
   MY_ASSERT(g_circle_entity.initialize());

   // and the box
   g_rectangle_box_entity.add_component(&g_rectangle_box_physics);
   g_renderer.configure_new_renderable(g_experimental_geometry_ptr, &g_rectangle_box_entity);
   Math::F_Dual_Quat entity_rectangle_offset = Math::F_Dual_Quat::generate_translate_only(glm::vec3(0.0f, +3.0f, 0.0f));
   g_rectangle_box_entity.m_where_and_which_way = entity_rectangle_offset;
   MY_ASSERT(g_rectangle_box_entity.initialize());
   g_rectangle_box_physics.add_sustained_force_vector(glm::vec3(0.0f, 0.0f, 1.0f));

   Utilities::Printer_Helper &printer_helper_ref = Utilities::Printer_Helper::get_instance();
   printer_helper_ref.print_my_dual_quat("cube 1", g_cube_1_entity.m_where_and_which_way);
   printer_helper_ref.print_my_dual_quat("cube 2", g_cube_2_entity.m_where_and_which_way);
   printer_helper_ref.print_my_dual_quat("cube 3", g_cube_3_entity.m_where_and_which_way);
   printer_helper_ref.print_my_dual_quat("cube 4", g_cube_4_entity.m_where_and_which_way);
   printer_helper_ref.print_my_dual_quat("camera", g_camera_entity.m_where_and_which_way);
   printer_helper_ref.print_my_dual_quat("circle", g_circle_entity.m_where_and_which_way);
   printer_helper_ref.print_my_dual_quat("rectangle", g_rectangle_box_entity.m_where_and_which_way);
   printer_helper_ref.print_my_dual_quat("plane", g_plane_entity.m_where_and_which_way);
   printer_helper_ref.print_my_dual_quat("experiment", g_experimental_entity.m_where_and_which_way);


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

   g_cube_1_entity.update();
   g_cube_2_entity.update();
   g_cube_3_entity.update();
   g_cube_4_entity.update();
   g_plane_entity.update();
   g_circle_entity.update();
   g_rectangle_box_entity.update();

   Collision_Detection::Collision_Handler::get_instance().update();

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
      g_rectangle_box_physics.add_immediate_force_vector(glm::vec3(0.0f, 0.0f, -20.0f), glm::vec3());
      break;
   }
   case 'a':
   {
      g_rectangle_box_physics.add_immediate_force_vector(glm::vec3(-20.0f, 0.0f, 0.0f), glm::vec3());
      break;
   }
   case 's': 
   {
      g_rectangle_box_physics.add_immediate_force_vector(glm::vec3(0.0f, 0.0f, +20.0f), glm::vec3());
      break;
   }
   case 'd':
   {
      g_rectangle_box_physics.add_immediate_force_vector(glm::vec3(+20.0f, 0.0f, 0.0f), glm::vec3());
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

