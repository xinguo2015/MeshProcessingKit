
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "GLUTApp.h"
#include "XGLM.h"

#ifdef _WIN
#pragma warning(disable:4996)
#endif

namespace xglm {

GLCamera::GLCamera()
{
	_fovy   = 45;
	_aspect = 1;
	_znear  = 0.01f;
	_zfar   = 100;
}

GLCamera::GLCamera( GLfloat fovy, GLfloat aspect, GLfloat znear,  GLfloat zfar)
{
	_fovy   = fovy;
	_aspect = aspect;
	_znear  = znear;
	_zfar   = zfar;
}

void GLCamera::setProj() const
{
	gluPerspective(_fovy, _aspect, _znear, _zfar);
}

GLView::GLView ()
{
}

void GLView::initialize()
{
	// store some scene parameters
	Vec3f bbx[2];
	getSceneBBox(bbx[0], bbx[1]);
	float sceneSize = (bbx[1]-bbx[0]).length()/2;
	// center and scaling
	_sceneCenter = Vec4f((bbx[0]+bbx[1])*0.5f, 1.0f);
	_sceneScaling = 1.0f;
	// model view
	Vec3f eye = Vec3f(_sceneCenter) + sceneSize*2.f*Vec3f(0,0,1);
	_modelview.translation(-eye);
	// adjust near/far plane
	_pproj._znear = sceneSize/10;
	_pproj._zfar = sceneSize*10;
	// setup track ball
	Vec3f coRot ( _modelview*_sceneCenter );
	_trackball.setCenterOfRotation(coRot);
	// glut callback functions
    glutReshapeFunc(GLUTApp::cbReshape);
    glutDisplayFunc(GLUTApp::cbDisplay);
    glutKeyboardFunc(GLUTApp::cbKeyboard);
    glutMouseFunc(GLUTApp::cbMouse);
    glutMotionFunc(GLUTApp::cbMotion);
}

void GLView::cbDisplay ()
{
	GLint viewport[4];
	// clears requested bits (color and depth) in glut window
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glGetIntegerv(GL_VIEWPORT, viewport);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	_pproj.setProj();
	// p*[-c]*S[c]*M*[-(c*M)]A[(c*M)]
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix(); {
		glLoadIdentity();
		_trackball.applyTransform();
		// model view matrix
		glMultMatrixf( (float*)_modelview );
		// scene scaling with center fixed
		glTranslatef(_sceneCenter[0], _sceneCenter[1], _sceneCenter[2]);
		glScalef(_sceneScaling[0], _sceneScaling[1], _sceneScaling[2]);
		glTranslatef(-_sceneCenter[0], -_sceneCenter[1], -_sceneCenter[2]);
		// drawing....
		drawScene();
	} glPopMatrix();
	glFlush();
	glutSwapBuffers();
	glutPostRedisplay();
}

void GLView::cbReshape (int width, int height)
{
	glViewport(0, 0, width, height);
	_pproj._aspect = width/float(height);
	_trackball.reshape(width, height);
}

void GLView::cbKeyboard (unsigned char key, int x, int y)
{
  GLint params[2];

  switch (key)
  {
    case 'f':
      glGetIntegerv(GL_SHADE_MODEL, params);
      if (params[0] == GL_FLAT)
        glShadeModel(GL_SMOOTH);
      else
        glShadeModel(GL_FLAT);
      break;

    case 'w':
      glGetIntegerv(GL_POLYGON_MODE, params);
      if (params[0] == GL_FILL)
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
      else
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
      break;

	case 's':
		_sceneScaling *= 0.95;
		break;
	case 'S':
		_sceneScaling *= 1.05;
		break;

	case 'q': case 'Q': case 27:
		exit(0);
  }
}

void GLView::cbMotion (int x, int y)
{
	_trackball.motion(x, y);
}

void GLView::cbMouse(int button, int state, int x, int y)
{
	_trackball.mouse(
		Manipulator::convertButtonFromGlut(button),
		Manipulator::convertModifierFromGlut(glutGetModifiers()),
		state == GLUT_DOWN, x, y);
}

GLView * GLUTApp::_pView = NULL;

GLView* GLUTApp::getView() const
{
	return _pView;
}

GLUTApp::GLUTApp(const char title[], int winWidth, int winHeight, int posX, int posY)
{
	_winID = -1;
	_winTitle = string(title);
	_winPosX = posX;
	_winPosY = posY;
	_winWidth = winWidth;
	_winHeight = winHeight;
}

int GLUTApp::create(int argc, char *argv[])
{
	// initialize glut
	glutInit(&argc, argv);
	glutInitWindowPosition(_winPosX, _winPosY);
	glutInitWindowSize(_winWidth, _winHeight);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_DEPTH | GLUT_RGBA);
	_winID = glutCreateWindow(_winTitle.c_str());
	if( _winID < 0 ) 
		return 0;
	// setup opengls
	glClearColor(0, 0, 0, 0.0);
	glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_COLOR_MATERIAL);
	return 1;
}

int GLUTApp::setView(GLView *pview)
{
	_pView = pview;
	return 1;
}

} //namespace xglm {