#include <stdio.h>
#include <stdlib.h>

#include <iostream>
#include <vector>
#include <numeric>
#include <algorithm>

#include "GLUTApp.h"
#include "TriMesh.h"
#include "MeshIO.h"
#include "DrawUtil.h"
#include "XGLM.h"


using namespace xglm;

class MyGLView : public GLView
{
public:
	TriMesh      _mesh;
public:
	virtual int loadScene(string scene)
	{
		XGLM_LOG("Reading mesh from %s\n", scene.c_str());
		if( ! MeshIO<TriMesh>::readMesh(scene, _mesh) )
		{
			XGLM_LOG("Failed reading mesh %s\n");
			return 0;
		}
		return 1;
	}
	virtual void getSceneBBox(Vec3f & bbmin, Vec3f& bbmax)
	{
		if( ! _mesh._bbox.IsValid() )
			_mesh.needBBox();
		bbmin = _mesh._bbox._min;
		bbmax = _mesh._bbox._max;
	}
	virtual void drawScene(void)
	{
		glColor4f(1.f, 1.f, 1.f, 1.f);
		DrawFace<TriMesh>::draw(_mesh);
	}
};


int main (int argc, char *argv[])
{
	GLUTApp theApp("QMesh", 1600, 1600, 800, 200);
	MyGLView theView;

	if( argc<2 ) {
		printf("Filename for the input model is required\n");
		return 1;
	}
	if( ! theApp.create (argc, argv) ) {
		printf("Failed to create the application\n");
		return 1;		
	}
	if( ! theView.loadScene (string(argv[1])) ) {
		printf("Failed to load the input model:\n\t%s\n", argv[1]);
		return 1;
	}
	theView.initialize ();
	theApp.setView (& theView);
	glutMainLoop();
    return 0;
}
