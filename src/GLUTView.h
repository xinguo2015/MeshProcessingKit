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

// glut callback functions
#define glut_callbacks(_PREFIX_)                                     \
	_PREFIX_ void cbReshape(int width, int height);                  \
	_PREFIX_ void cbDisplay(void);									 \
	_PREFIX_ void cbOverlayDisplay(void)							 \
	_PREFIX_ void cbKeyboard(unsigned char key, int x, int y);		 \
	_PREFIX_ void cbKeyboardUp(unsigned char key, int x, int y)		 \
	_PREFIX_ void cbSpecial(int key, int x, int y)              	 \
	_PREFIX_ void cbSpecialUp(int key, int x, int y)           		 \
	_PREFIX_ void cbMouse(int button, int state, int x, int y);		 \
	_PREFIX_ void cbMotion(int x, int y);							 \
	_PREFIX_ void cbPassiveMotion(int x, int y)                		 \
	_PREFIX_ void cbEntry(int state)                           		 \
	_PREFIX_ void cbVisibility(int state)                      		 \
	_PREFIX_ void cbIdle(void)                                 		 \
	_PREFIX_ void cbTimer(int value)                            	 \
	_PREFIX_ void cbMenuStatus(int status, int x, int y)      		 \
	_PREFIX_ void cbMenuState(int state)                      		 \
	_PREFIX_ void cbWindowStatus(int state)                         

namespace xglm {

using namespace std;

void drawString(const char *str,	// the string to draw
				int x, int y, 		// position to put
				float color[4],		// text color
				void *txtfont);		// text font

class GLCamera
{
public:
	GLfloat _fovy;   // field of view in Y direction
	GLfloat _aspect; // aspect ratio ( fovx / fovy )
	GLfloat _znear;  // near plane
	GLfloat _zfar;   // far plane
public:
	GLCamera();
	GLCamera(GLfloat fovy, GLfloat aspect, GLfloat znear, GLfloat zfar);
	void setProj() const;
};

template <typename TView>
class GLUTApp
{
public:
	GLUTApp(const char title[] = "OpenGL Application", int winWidth = 800, int winHeight = 600, int posX = 200, int posY = 200)
		: _winID(-1), _winPosX(posX), _winPosY(posY), 
		_winWidth(winWidth), _winHeight(winHeight)
	{	
		_winTitle = string(title);
	}

	int createWindow()
	{
		// initialize glut
		glutInitWindowPosition( _winPosX, _winPosY );
		glutInitWindowSize( _winWidth, _winHeight );
		// GLUT_ALPHA must be specified for using alpha
		glutInitDisplayMode(GLUT_DOUBLE | GLUT_DEPTH | GLUT_RGBA | GLUT_ALPHA); 
		_winID = glutCreateWindow(_winTitle.c_str());
		if( _winID < 0 ) 
			return 0;
		return 1;
	}

	static TView * getView() 
	{
		if( !_glview  && ! (_glview = createView()) )  {
			printf("Cannot create view\n");
			exit(-1);
		}
		return _glview; 
	}

public:
	int    _winID;
	int    _winPosX;
	int    _winPosY;
	int    _winWidth;
	int    _winHeight;
	string _winTitle;

protected:
	static TView * _glview;
	static TView * createView()
	{
		TView * view = new TView;
		if( view  ) view->initialize();
		return view;
	}
public:
	static void cbReshape(int width, int height)                { getView()->cbReshape(width, height); }
	static void cbDisplay(void)								    { getView()->cbDisplay(); }
	static void cbOverlayDisplay(void)							{ getView()->cbOverlayDisplay(); }
	static void cbKeyboard(unsigned char key, int x, int y)	    { getView()->cbKeyboard(key,x,y); }
	static void cbKeyboardUp(unsigned char key, int x, int y)	{ getView()->cbKeyboardUp(key,x,y); }
	static void cbSpecial(int key, int x, int y)              	{ getView()->cbSpecial(key,x,y); }
	static void cbSpecialUp(int key, int x, int y)           	{ getView()->cbSpecialUp(key,x,y); }
	static void cbMouse(int button, int state, int x, int y)	{ getView()->cbMouse(button,state,x,y); }
	static void cbMotion(int x, int y)							{ getView()->cbMotion(x,y); }
	static void cbPassiveMotion(int x, int y)                	{ getView()->cbPassiveMotion(x,y); }
	static void cbEntry(int state)                           	{ getView()->cbEntry(state); }
	static void cbVisibility(int state)                      	{ getView()->cbVisibility(state); }
	static void cbIdle(void)                                 	{ getView()->cbIdle(); }
	static void cbTimer(int value)                            	{ getView()->cbTimer(value); }
	static void cbMenuStatus(int status, int x, int y)      	{ getView()->cbMenuStatus(status,x,y); }
	static void cbMenuState(int state)                      	{ getView()->cbMenuState(state); }
	static void cbWindowStatus(int state)     					{ getView()->cbWindowStatus(state); }
};

template<typename TView> 
TView * GLUTApp<TView>::_glview;

class GLUTView
{
public:
	GLUTView();
	static GLUTView* create();
public:
	// glut callback functions
	virtual void cbReshape(int width, int height);            
	virtual void cbDisplay(void);                             
	virtual void cbOverlayDisplay(void)                        {}                 
	virtual void cbKeyboard(unsigned char key, int x, int y); 
	virtual void cbKeyboardUp(unsigned char key, int x, int y) {}
	virtual void cbSpecial(int key, int x, int y)              {}
	virtual void cbSpecialUp(int key, int x, int y)            {}          
	virtual void cbMouse(int button, int state, int x, int y);
	virtual void cbMotion(int x, int y);                      
	virtual void cbPassiveMotion(int x, int y )                {}           
	virtual void cbEntry(int state)                            {}
	virtual void cbVisibility(int state)                       {}                
	virtual void cbIdle(void)                                  {}                           
	virtual void cbTimer(int value)                            {}  
	virtual void cbMenuStatus(int status, int x, int y)        {}
	virtual void cbMenuState(int state)                        {}
	virtual void cbWindowStatus(int state)                     {}

public:
	virtual int  loadScene(string scene)                       = 0;
	virtual void getSceneBBox(Vec3f & bbmin, Vec3f& bbmax)     = 0;
	virtual void drawScene()                                   = 0;
	virtual void drawText()                                    {};
	virtual void initialize();							       
	virtual void applyProjectionAndModelview();			       
	virtual void displayText();							       
	virtual void setupLights();							       
	virtual void updateFrameCount();
	virtual float getFPS();

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
	//
	//---------------------------------------------------------------------------
	// timing fps
	Timer     _timer;
	double    _frameCount;
	double    _frameCountTotal;
	// misc variables
	VarSet    _variables;
};

} //namespace xglm {

#endif //#ifndef __glut_view_app_____
