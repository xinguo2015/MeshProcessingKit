#ifndef ____draw_utilities_____________
#define ____draw_utilities_____________

#include "XGLM.h"
#include "MyGLUT.h"
#include "PickPixel.h"

using namespace std;

namespace xglm {

class DrawMesh 
{
public:
	enum {
		ShadeModelSmooth = 0, //a normal per vertex
		ShadeModelFacet  = 1, //a normal per face
		ShadeModelGroup  = 2, //a normal per corner(
		ShadeModelNumber = 3
	};
public:
	// draw smoothed group
	static void Face( const vector<Vec3i>& triangle,
					  const vector<Vec3i>& normalIndex,
					  const vector<Vec3f>& position, 
				      const vector<Vec3f>& normal);
	// 1. facet normal  - normal.size() = triangle.size()
	// 2. vertex normal - normal.size() = vertex.size()
	static void Face( const vector<Vec3i>& triangle, 
					  const vector<Vec3f>& position,
					  const vector<Vec3f>& normal);
	static void FaceID( const vector<Vec3i>& triangle,
						const vector<Vec3f>& position );
	static void PointID(const vector<Vec3f>& position );
};
	
} //namespace xglm {

#endif //#ifndef ____draw_utilities_____________
