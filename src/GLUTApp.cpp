
#include <stdio.h>
#include <stdlib.h>
#include <functional>
#include <iostream>
#include <sstream>
#include <string>
#include <iomanip>
//#include <cstdlib>
#include "GLUTApp.h"
#include "XGLM.h"

#ifdef _WIN
#pragma warning(disable:4996)
#endif

namespace xglm {
	
	void drawString(const char *str, int x, int y, float color[4], void *txtfont)
	{
		glColor4fv(color);          // set text color
		glRasterPos2i(x, y);        // place text position
		// loop all characters in the string
		while(*str)
		{
			glutBitmapCharacter(txtfont, *str);
			++str;
		}
	}
	
	
	///////////////////////////////////////////////////////////////////////////////
	// display frame rates
	///////////////////////////////////////////////////////////////////////////////
	float GLView::getFPS()
	{ 
		return _frameCount/_timer.getElapsedTime();
	}
	
	void GLView::updateFrameCount()
	{
		double elapsedTime = _timer.getElapsedTime();
		// reset timer every second
		if(elapsedTime > 1.0) {
			_frameCount = 0;   // reset counter
			_timer.start();    // restart timer
		}
		else
			_frameCount ++;
		_frameCountTotal++;
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
	
	int GLUTApp::createWindow(int argc, char *argv[])
	{
		// initialize glut
		glutInit(&argc, argv);
		glutInitWindowPosition(_winPosX, _winPosY);
		glutInitWindowSize(_winWidth, _winHeight);
		glutInitDisplayMode(GLUT_DOUBLE | GLUT_DEPTH 
			| GLUT_RGBA | GLUT_ALPHA); // GLUT_ALPHA must be specified for using alpha
		_winID = glutCreateWindow(_winTitle.c_str());
		if( _winID < 0 ) 
			return 0;
// 		int bn;
// 		glGetIntegerv(GL_RED_BITS, &bn); printf("r bits=%d\n",bn);
// 		glGetIntegerv(GL_GREEN_BITS, &bn);printf("g bits=%d\n",bn);
// 		glGetIntegerv(GL_BLUE_BITS, &bn);printf("b bits=%d\n",bn);
// 		glGetIntegerv(GL_ALPHA_BITS, &bn);printf("a bits=%d\n",bn);
		return 1;
	}
	
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
		_frameCount = 0;
		_frameCountTotal = 0;
	}
	
	using namespace std::placeholders;
	void keyf(GLView *v, unsigned char k, int x, int y)
	{
		v->cbKeyboard(k,x,y);
	}
	void gggKeyboardFunc( void (* callback)( unsigned char, int, int ) );
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
		// setup OpenGL
		glClearStencil(0);                          // clear stencil buffer
		glClearColor(0, 0, 0, 0);
		glClearDepth(1.0f);                         // 0 is near, 1 is far
		glDepthFunc(GL_LEQUAL);
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_LIGHTING);
		glEnable(GL_LIGHT0);
		glEnable(GL_COLOR_MATERIAL);
		glPixelStorei(GL_UNPACK_ALIGNMENT, 4);      // 4-byte pixel alignment
		glLineWidth(2.0f);
	}
	
	void GLView::applyProjectionAndModelview()
	{
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		_pproj.setProj();
		// p*[-c]*S[c]*M*[-(c*M)]A[(c*M)]
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		_trackball.applyTransform();
		// model view matrix
		glMultMatrixf( (float*)_modelview );
		// scene scaling with center fixed
		glTranslatef(_sceneCenter[0], _sceneCenter[1], _sceneCenter[2]);
		glScalef(_sceneScaling[0], _sceneScaling[1], _sceneScaling[2]);
		glTranslatef(-_sceneCenter[0], -_sceneCenter[1], -_sceneCenter[2]);
		// now ready for drawScene()
	}
	void GLView::displayText()
	{
		// backup current projection
		glMatrixMode(GL_PROJECTION);        // switch to projection matrix
		glPushMatrix();                     // save current projection matrix
		// set orthogonal projection
		glLoadIdentity();                   // reset projection matrix
		gluOrtho2D(0, _viewport[2], 0, _viewport[3]); 
		// backup current model-view matrix
		glMatrixMode(GL_MODELVIEW);         // switch to modelview matrix
		glPushMatrix();                     // save current modelview matrix
		glLoadIdentity();                   // reset modelview matrix
		// lighting, enalbe, rasterization position
		glPushAttrib(GL_LIGHTING_BIT | GL_ENABLE_BIT | GL_CURRENT_BIT);
		glDisable(GL_LIGHTING); //disable lighting for proper text color
		glDisable(GL_TEXTURE_2D);
		// draw the infomation
		drawText();
		glPopAttrib();
		// restore modelview matrix
		glPopMatrix();             
		// restore projection matrix
		glMatrixMode(GL_PROJECTION);
		glPopMatrix();             
	}
	void GLView::cbDisplay ()
	{
		// clears requested bits (color and depth) in glut window
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		applyProjectionAndModelview();
		drawScene();
		updateFrameCount();
		displayText();
		glFlush();
		glutSwapBuffers();
		glutPostRedisplay();
	}
	
	void GLView::cbReshape (int width, int height)
	{
		glViewport(0, 0, width, height);
		glGetIntegerv(GL_VIEWPORT,_viewport);
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
				
			case 'S':
				_sceneScaling *= 0.95;
				break;
			case 's':
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

} //namespace xglm {