#ifndef ___my_glut_h___________________
#define ___my_glut_h___________________

#ifdef __APPLE__
#include <OpenGL/OpenGL.h>
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

// a base class of methods for handling GLUT window callback
class BaseGLView 
{
	public:
		virtual void cbReshape(int width, int height)             { } 
		virtual void cbDisplay(void)                              { }
		virtual void cbOverlayDisplay(void)                       { }
		virtual void cbKeyboard(unsigned char key, int x, int y)  { }
		virtual void cbKeyboardUp(unsigned char key, int x, int y){ }
		virtual void cbSpecial(int key, int x, int y)             { } 
		virtual void cbSpecialUp(int key, int x, int y)           { }
		virtual void cbMouse(int button, int state, int x, int y) { } 
		virtual void cbMotion(int x, int y)                       { }
		virtual void cbPassiveMotion(int x, int y)                { }
		virtual void cbEntry(int state)                           { }
		virtual void cbVisibility(int state)                      { }
		virtual void cbIdle(void)                                 { }
		virtual void cbTimer(int value)                           { } 
		virtual void cbMenuStatus(int status, int x, int y)       { }
		virtual void cbMenuState(int state)                       { } 
		virtual void cbWindowStatus(int state)                    { }
};

/* GLUT window callback functions. */
#ifndef ADD_GLUT_WINDOW_CALLBACKS
#define ADD_GLUT_WINDOW_CALLBACKS(view) \
	static void cbReshape(int width, int height)             { if (view) view->cbReshape(width,height); } \
	static void cbDisplay(void)                              { if (view) view->cbDisplay(); } \
	static void cbOverlayDisplay(void)                       { if (view) view->cbOverlayDisplay(); }  \
	static void cbKeyboard(unsigned char key, int x, int y)  { if (view) view->cbKeyboard(key,x,y); }  \
	static void cbKeyboardUp(unsigned char key, int x, int y){ if (view) view->cbKeyboardUp(key,x,y); }  \
	static void cbSpecial(int key, int x, int y)             { if (view) view->cbSpecial(key,x,y); }  \
	static void cbSpecialUp(int key, int x, int y)           { if (view) view->cbSpecialUp(key,x,y); }  \
	static void cbMouse(int button, int state, int x, int y) { if (view) view->cbMouse(button,state,x,y); }  \
	static void cbMotion(int x, int y)                       { if (view) view->cbMotion(x,y); }  \
	static void cbPassiveMotion(int x, int y)                { if (view) view->cbPassiveMotion(x,y); }  \
	static void cbEntry(int state)                           { if (view) view->cbEntry(state); }  \
	static void cbVisibility(int state)                      { if (view) view->cbVisibility(state); }  \
	static void cbIdle(void)                                 { if (view) view->cbIdle(); }  \
	static void cbTimer(int value)                           { if (view) view->cbTimer(value); }\
	static void cbMenuStatus(int status, int x, int y)       { if (view) view->cbMenuStatus(status,x,y); }  \
	static void cbMenuState(int state)                       { if (view) view->cbMenuState(state); }  \
	static void cbWindowStatus(int state)                    { if (view) view->cbWindowStatus(state); } 
#endif // #ifndef ADD_GLUT_WINDOW_CALLBACKS

#endif // #ifndef ___my_glut_h___________________
