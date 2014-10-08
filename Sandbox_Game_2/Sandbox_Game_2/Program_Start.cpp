//Copyright (C) 2010-2012 by Jason L. McKesson
//This file is licensed under the MIT License.


/***********************************************************************
This file contains all of the functions you need to implement, as well as
when/why they are called. You can copy this file and simply implement
these function.
***********************************************************************/


#include <glload/gl_3_3_comp.h>
#include <GL/freeglut.h>
#include <framework.h>

#include <string>
#include <vector>


GLuint g_program_ID;

void initialize_program(const std::string &str_vertex_shader, const std::string &str_fragment_shader)
{
   std::vector<GLuint> shader_list;
   shader_list.push_back(Framework::LoadShader(GL_VERTEX_SHADER, str_vertex_shader));
   shader_list.push_back(Framework::LoadShader(GL_FRAGMENT_SHADER, str_fragment_shader));

   g_program_ID = Framework::CreateProgram(shader_list);
}

const float vertexData[] = {
   1.0f, 0.0f, 0.0f, 1.0f,
   0.0f, 1.0f, 0.0f, 1.0f,
   0.0f, 0.0f, 1.0f, 1.0f,
   0.0f, 0.5f, 0.0f, 1.0f,
   0.5f, -0.366f, 0.0f, 1.0f,
   -0.5f, -0.366f, 0.0f, 1.0f,
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


//Called after the window and OpenGL are initialized. Called exactly once, before the main loop.
void init()
{
   //initialize_program("VertexColors.vert", "VertexColors.frag");
   //initialize_vertex_buffer();
   //glGenVertexArrays(1, &vao);
   //glBindVertexArray(vao);

   // load level

}

//Called to update the display.
//You should call glutSwapBuffers after all of your rendering to display what you rendered.
//If you need continuous updates of the screen, call glutPostRedisplay() at the end of the function.
void display()
{
   glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
   glClear(GL_COLOR_BUFFER_BIT);

   glUseProgram(g_program_ID);

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

   glutSwapBuffers();
   glutPostRedisplay();
}

//Called whenever the window is resized. The new window size is given, in pixels.
//This is an opportunity to call glViewport or glScissor to keep up with the change in size.
void reshape (int w, int h)
{
   glViewport(0, 0, (GLsizei)w, (GLsizei)h);
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
