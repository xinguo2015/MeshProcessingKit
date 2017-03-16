#ifndef __glut_view_app_____
#define __glut_view_app_____

#include <string>
#include "MyGLUT.h"
#include "Math3D/math.h"
#include "Trackball.h"
#include "XGLM.h"

namespace xglm {

using std::string; 

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

class GLView : public BaseGLView
{
	public:
		GLView();
	public:
		virtual int  loadScene(string scene) = 0;
		virtual void getSceneBBox(Vec3f & bbmin, Vec3f& bbmax) = 0;
		virtual void drawScene() = 0;
		virtual void initialize();
		// overloaded callback methods
		virtual void cbReshape(int width, int height);
		virtual void cbDisplay(void);
		virtual void cbKeyboard(unsigned char key, int x, int y);
		virtual void cbMouse(int button, int state, int x, int y);
		virtual void cbMotion(int x, int y);
	public:
		GLCamera _pproj;     // perspective projection
		Vec4f   _sceneCenter;   // c: center of the scene (in world)
		Vec4f   _sceneScaling;  // S: scaling imposed on the scene
		Mat4f   _projection;    // the projection matrix
		Mat4f   _modelview;     // M: model view matrix
		Mat4f   _userRotate;    // A: arcball rotation, rotation center is: c*M
		//
		// The total transformation for a point p on the scene is:
		//
		//           p*[-c]*S[c]*M*[-(c*M)]A[(c*M)]
		//
		//  where [t] denotes a translation by vector t
		//
		//
		Trackball _trackball;
};

class GLUTApp
{
	public:
		GLUTApp(const char title[], int sizeX, int sizeY, int posX, int posY);
		int     create(int argc, char *argv[]);
		int     setView(GLView *pView);
		GLView* getView() const;
	public:
		int    _winID;
		int    _winPosX;
		int    _winPosY;
		int    _winWidth;
		int	   _winHeight;
		string _winTitle;
	private:
		static GLView* _pView;
	public:
		// define the glut callback functions here
		ADD_GLUT_WINDOW_CALLBACKS(_pView)
};

} //namespace xglm {

#endif //#ifndef __glut_view_app_____
