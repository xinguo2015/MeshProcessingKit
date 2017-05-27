#ifndef __glut_view_app_____
#define __glut_view_app_____

#include <string>
#include <map>
#include "XGLM.h"
#include "Variables.h"
#include "MyGLUT.h"
#include "Math3D/math.h"
#include "Trackball.h"
#include "Timer.h"

namespace xglm {

using namespace std;

void drawString(const char *str,	// the string to draw
				int x, int y, 		// position to put
				float color[4],		// text color
				void *txtfont);		// text font

class GLUTApp
{
public:
	GLUTApp(const char title[], int sizeX, int sizeY, int posX, int posY);
	int    createWindow();
public:
	int    _winID;
	int    _winPosX;
	int    _winPosY;
	int    _winWidth;
	int    _winHeight;
	string _winTitle;
};

class GlutCallbackInterface
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

class GLCamera
{
public:
	GLfloat _fovy;   // field of view in Y direction
	GLfloat _aspect; // aspect ratio ( fovx / fovy )
	GLfloat _znear;  // near plane
	GLfloat _zfar;   // far plane
public:
	GLCamera();
	GLCamera( GLfloat fovy, GLfloat aspect, GLfloat znear,  GLfloat zfar);
	void setProj() const;
};

class GLView : public GlutCallbackInterface
{
public:
	GLView();
	void  updateFrameCount();
	float getFPS();
public:
	virtual int  loadScene(string scene) = 0;
	virtual void getSceneBBox(Vec3f & bbmin, Vec3f& bbmax) = 0;
	virtual void drawScene() = 0;
	virtual void drawText() = 0;
	virtual void initialize();
	virtual void applyProjectionAndModelview();
	virtual void displayText();
	virtual void setupLights();
public:
	virtual void cbReshape(int width, int height);
	virtual void cbDisplay(void);
	virtual void cbKeyboard(unsigned char key, int x, int y);
	virtual void cbMouse(int button, int state, int x, int y);
	virtual void cbMotion(int x, int y);
public:
	int 	 _viewport[4];	 // viewport 
	GLCamera _pproj;		 // perspective projection
	Vec4f    _sceneCenter;   // c: center of the scene (in world)
	float    _sceneSize;     // size of the scene
	Vec4f    _sceneScaling;  // S: scaling imposed on the scene
	Mat4f    _projection;    // the projection matrix
	Mat4f    _modelview;     // M: model view matrix
	Mat4f    _userRotate;    // A: arcball rotation, rotation center is: c*M
	//
	// The total transformation for a point p on the scene is:
	//
	//           p*[-c]*S[c]*M*[-(c*M)]A[(c*M)]
	//
	//  where [t] denotes a translation by vector t
	//
	//
	Trackball _trackball;
	// timing fps
	Timer     _timer;
	double    _frameCount;
	double    _frameCountTotal;
	// misc variables
	VarSet    _variables;

};



} //namespace xglm {

#endif //#ifndef __glut_view_app_____
