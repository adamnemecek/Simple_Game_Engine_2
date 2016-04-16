// Here's how this thing is going to go down:
// - In the engine project, there is file called Start_Engine.cpp that sets up a bunch of 
// freeglut stuff.
// - Some of the stuff is registering forward-declared functions like init(), display(), 
// reshape(...), and keyboard(...) with freeglut back-end functions that call them 
// automatically based on time or user-input.

// - Explanation: init() is called after freeglut's default GL contex is started, but before 
// anything else.  This function is where is I initialize GL objects and load them into GPU 
// memory.  
// - Explanation: display() is called automatically some time after initialization finishes.
// I call glutSwapBuffers() and glutPostRedisplay() manually at the end of in this function 
// to tell freeglut to call display() again as soon as possible, effectively causing freeglut 
// to render as fast as it can.
// - Explanation: reshape(...) is called automatically upon resizing the windowed render...
// window..that freeglut told Windows to create and that it now keeps track of.  I am happy 
// to let freeglut handle the window stuff.  The function arguments tell me the new 
// dimensions (in pixels) of the resized window.  I use this to calculate a new perspective
// matrix (I think that arcsynthesis calls this the "camera to clip" matrix).
// - Explanation: keyboard(...) is called when a key on the keyboard is pressed.  It will only
// register a single key at a time, so it can't handle multiple simultaneous key pressed.  
// The keys appear to be ASCII codes of the key pressed, which is fine for 0-9 and a-z and 
// A-Z, so I don't know off the top of my head how to handle the non-character keys here, like 
// the arrow or shift or control keys.  Fortunately for me, I have a controller component in 
// the engine that uses key bindings and Windows' virtual key codes for all keys, so I can 
// look up the key code of any key on MSDN and use it in my own key reader.  My own key reader 
// also handles simultaneous key presses, so I have little use of keyboard(...).  I only 
// intend to use this function for the ESC key, which causes freeglut to leave its inifite 
// loop and end the program, or for quick-and-dirty interface testing.

// - These functions are merely declared in Start_Engine.cpp, so the compiler is happy, but the 
// linker needs them to finish building.
// - These functions are defined in the Sandbox_Game_2 project's Genesis.cpp, which is named as
// such because my scene-loading code is primitive and turned into a god file with 
// initialization and entity-storage.  I may trim it up, or I may leave it as is and do better 
// in my next game engine.
//
// - Recap: My start_engine_loop(...) function is called from this project (Sandbox_Game_2), but 
// it is defined in the engine's Start_Engine.cpp.  Start_Engine.cpp contains some forward
// declarations of certain functions, and start_engine_loop(...) registers those functions with
// freeglut.  Those forward-declared functions are defined in Genesis.cpp (in Sandbox_Game_2).
// Circular dependency?  In a way, but from the linker's perspective it's just a two-way 
// dependency, not circular, so it works out.
//
// - Final Note on dynamic vs static libraries: I tried experimenting with making the engine 
// project into a dll, but I couldn't figure out how to link the forward-declarations of these 
// functions with their definitions in the Sandbox_Game_2 project.  I began to glimpse the 
// true meaning of the term "dll hell".  So I just went with the Engine as a static library.
#include <Engine_2\Start_Engine.h>

#include <Engine_2\EngineExperiments.h>

int main(int argc, char **argv)
{
	// if the program gets here, then the program has obviously finished compiling and linking
	// and my start_engine_loop(...) function can tell freeglut to register certain 
	// forward-declared functions that are linked to their definitions in the Sandbox_Game_2
	int retVal = start_engine_loop(argc, argv);
	//int retVal = ExperimentStart(argc, argv);

	return retVal;
}