#include <stdio.h>
#include <stdlib.h>

#include <iostream>
#include <sstream>
#include <iomanip>
#include <vector>
#include <numeric>
#include <algorithm>
#include <functional>

#include "GLView.h"
#include "TriMesh.h"
#include "MeshIO.h"
#include "DrawUtil.h"
#include "PickPixel.h"
#include "XGLM.h"


using namespace xglm;

//void *gTextFont = GLUT_BITMAP_9_BY_15;
//void *gTextFont = GLUT_BITMAP_HELVETICA_18;
void *gTextFont = GLUT_BITMAP_TIMES_ROMAN_24;
const int TEXT_WIDTH = 13;
const int TEXT_HEIGHT = 24;
class MyGLView : public GLView
{
public:
	TriMesh			mMesh;
public:
	PickBuffer      mPickBuffer;
	PixelInfo       mObjUnderMouse;
	vector<int>		mPickedPoints;
	vector<int>		mPickedFaces;
	bool            mInPicking;
	
public:
	MyGLView()
	{
		mInPicking = true;
	}
	virtual void cbReshape(int width, int height)
	{
		GLView::cbReshape(width,height);
		mPickBuffer.markDirty(true);
	}
	virtual void cbKeyboard(unsigned char key, int x, int y)
	{
		switch (key)
		{
			case 'v':
				mInPicking = !mInPicking; break;
				glutPostRedisplay();
			case 's': case 'S':
				GLView::cbKeyboard(key,x,y);
				mPickBuffer.markDirty(true); break;
			case 'n':// add noise
				addNoise();
				break;
			case 'x':
				smoothOnce(); 
				glutPostRedisplay();
				break;
			default:
				GLView::cbKeyboard(key,x,y);
		}
		
	}
	virtual void cbMouse(int button, int state, int x, int y)
	{
		GLView::cbMouse(button,state,x,y);
		if( state==GLUT_DOWN )
		{
			if( mInPicking && ! _trackball.isActive() && ! mPickBuffer.isDirty() )
			{
			}
		}
	}
	virtual void cbMotion(int x, int y)
	{
		GLView::cbMotion(x,y);
		if( _trackball.isActive() )
			mPickBuffer.markDirty(true);
	}
	virtual void cbPassiveMotion(int x, int y)
	{
		if( mInPicking )
			mObjUnderMouse = getObjectAt(x,_viewport[3]-y);
		else
			mObjUnderMouse = PixelInfo(0);
	}
	// callbacks 
	virtual void cbDisplay ()
	{
		if( mInPicking && ! _trackball.isActive() && mPickBuffer.isDirty() )
		{
			updatePickBuffer();
		}
		GLView::cbDisplay();
	}
	virtual int loadScene(string scene)
	{
		xglm_debug("Reading mesh from %s\n", scene.c_str());
		if( ! MeshIO<TriMesh>::readMesh(scene, mMesh) )
		{
			xglm_debug("Failed reading mesh %s\n", scene.c_str());
			return 0;
		}
		return 1;
	}
	virtual void getSceneBBox(Vec3f & bbmin, Vec3f& bbmax)
	{
		if( ! mMesh._bbox.IsValid() )
			mMesh.needBBox();
		bbmin = mMesh._bbox._min;
		bbmax = mMesh._bbox._max;
	}
	virtual void drawScene(void)
	{
		glColor4f(1.f, 1.f, 1.f, 1.f);
		// fill or wire
		if( _variables["GL_POLYGON_MODE"].getint()==GL_LINE )
			glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
		else
			glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
		//const char *shademodel = _variables["MyShadeModel"].getstr();
		switch( _variables["MyShadeModel"].getint() )
		{
			case DrawMesh::ShadeModelFacet:  DrawMesh::Face(mMesh.mTriangles, mMesh.mPosition, mMesh.mFaceNormal); break;
			case DrawMesh::ShadeModelSmooth: DrawMesh::Face(mMesh.mTriangles, mMesh.mPosition, mMesh.mNormal); break;
			case DrawMesh::ShadeModelGroup:
				if( mMesh.mTriNormalIndices.size()!=mMesh.mTriangles.size() )
					mMesh.needCrease(30);
				DrawMesh::Face(mMesh.mTriangles, mMesh.mTriNormalIndices, mMesh.mPosition, mMesh.mNormal);
		}
		if( (unsigned int)mObjUnderMouse )
			drawObjUnderMouse();
	}
	
	virtual void drawText()
	{
		int ycoord = 10;
		const int *vport = _viewport;
		const int screenWidth = vport[2];
		const int screenHeight = vport[3];
		float color[] = {1.0f,.0f,1.0f,1.0f};
		stringstream ss;
		// frame per second
        //ss << std::fixed << std::setprecision(1);
        ss << getFPS() << " FPS" << ends;
        //ss << std::resetiosflags(std::ios_base::fixed | std::ios_base::floatfield);
		string fps = ss.str();
		drawString(fps.c_str(), screenWidth-(int)fps.size() * TEXT_WIDTH, screenHeight-TEXT_HEIGHT,color,gTextFont);
		ss.str(""); // clear buffer
		int sm = _variables["MyShadeModel"].getint();
		ss << "Shade Model: " << (sm==DrawMesh::ShadeModelFacet?"Facet Normal":(sm==DrawMesh::ShadeModelGroup?"Cornal Normal":"Vertex Normal")) << ends;
		drawString(ss.str().c_str(), 1, ycoord, color, gTextFont);
		ss.str(""); // clear buffer
		ss << "Press v to toggle visual model." << ends;
		drawString(ss.str().c_str(), 1, ycoord+=TEXT_HEIGHT*1.2, color, gTextFont);
	}
	void drawObjUnderMouse()
	{
		unsigned int typ = mObjUnderMouse.getType();
		unsigned int idx = mObjUnderMouse.getIndex();
		glPushAttrib(GL_LIGHTING_BIT);
		glDisable(GL_LIGHTING);
		if( typ==1 && idx<mMesh.mPosition.size() ) { // vertex
			glBegin(GL_POINTS);
			glColor4ub(255,0,0,0);
			glVertex3fv(mMesh.mPosition[idx].get_value());
			glEnd();
			//printf("cursor points\n");
		}
		else if( typ==2 && idx<mMesh.mTriangles.size() ) {  // face
			const int * t = mMesh.mTriangles[idx].get_value();
			glPointSize(15);
			glBegin(GL_TRIANGLES);
			glColor4ub(255,0,0,0);
			for( int k = 0; k<3; k++ )
				glVertex3fv(mMesh.mPosition[t[k]].get_value());
			glEnd();
			//printf("cursor triangle\n");
		}
		glPopAttrib();
	}
protected:
	PixelInfo getObjectAt(int x, int y)
	{
		if( mPickBuffer.isDirty() || ! mPickBuffer.contain(x,y) )
			return PixelInfo(0);
		return mPickBuffer.getAt(x,y);
	}
	
	void updatePickBuffer()
	{
		//printf("update pick buffer\n");
		int viewport[4];
		glPixelStorei(GL_UNPACK_ALIGNMENT, 4);	
		glGetIntegerv(GL_VIEWPORT,viewport);
		mPickBuffer.resize(viewport[2],viewport[3]);
		// prepare to draw IDs
		glPushAttrib(GL_COLOR_BUFFER_BIT|GL_LIGHTING_BIT|GL_POLYGON_BIT|GL_POINT_BIT);
		glClearColor(0.f, 0.f, 0.f, 0.f);
		glDisable(GL_LIGHTING);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		GLView::applyProjectionAndModelview();
		// drawing....
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		DrawMesh::FaceID(mMesh.mTriangles, mMesh.mPosition);
		glPointSize(10.0f);
		DrawMesh::PointID(mMesh.mPosition);
		glFlush();
		// read out the color buffer 
		glReadBuffer(GL_BACK);
		glReadPixels(0,0,viewport[2],viewport[3], GL_RGBA, GL_UNSIGNED_BYTE, mPickBuffer.getBuf());
		glPopAttrib();
		mPickBuffer.markDirty(false);
	}
	void addNoise()
	{
		srand((int)time(0));
		float mag = mMesh._bbox.Size().length()*0.001f/RAND_MAX;
		for_each(mMesh.mPosition.begin(), mMesh.mPosition.end(), [ mag ](Vec3f &p) 
		{
			p.x += rand()*mag;
			p.y += rand()*mag;
			p.z += rand()*mag;
		});
		calcFaceNormals(mMesh.mPosition,mMesh.mTriangles,mMesh.mFaceNormal);
		//normalize the normal vector
		std::for_each(mMesh.mFaceNormal.begin(),mMesh.mFaceNormal.end(), [](Vec3f &n){ 
			n.normalize(); }
		);
	}
	void smoothOnce()
	{
		Smooth(
			0.1f, //float normalSigma,
			2,    //int   normalIterNum,
			10,    //int   posUpdateNum,
			0.01f, //float posUpdateStepsize,
			this->mMesh.mPosition,
			this->mMesh.mTriangles,
			this->mMesh.mFaceNormal,
			this->mMesh.mVtxLinks);
	}
	
};

GLUTApp  theApp("Mesh Processing", 1800, 1600, 1200, 200);
MyGLView theView;
//GlutCallbacks<MyGLView> glutCBS(theView);

int main (int argc, char *argv[])
{
	setbuf(stdout, NULL); // to seed ouput immediately for debugging

	glutInit(&argc, argv);
	if( ! theApp.createWindow() ) {
		printf("Failed creating the window\n");
		return 1;
	}
	if( argc<2 || ! theView.loadScene( argv[1] ) ) {
		printf("Need a valid path to the input model\n");
		return 1;
	}
	theView.initialize ();
	// glut callback functions // using c++ 11's lambda
	glutReshapeFunc(	[](int w, int h)						->void { theView.cbReshape(w,h); });
	glutDisplayFunc(	[]()									->void { theView.cbDisplay(); });
	glutKeyboardFunc(	[](unsigned char key, int x, int y)		->void { theView.cbKeyboard(key,x,y); });
	glutMouseFunc(		[](int button, int state, int x, int y)	->void { theView.cbMouse(button,state,x,y); });
	glutMotionFunc(		[](int x, int y)						->void { theView.cbMotion(x,y); });
	glutPassiveMotionFunc([](int x, int y)						->void { theView.cbPassiveMotion(x,y); });
	// running the application
	glutMainLoop();
    return 0;
}
