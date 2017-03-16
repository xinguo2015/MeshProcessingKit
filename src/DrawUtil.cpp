#include "XGLM.h"
#include "MyGLUT.h"
#include "DrawUtil.h"
#include "TriMesh.h"

namespace xglm {

template<>
void DrawFace<TriMesh>::draw( TriMesh & mesh)
{
	// vertices
	glEnableClientState(GL_VERTEX_ARRAY);
	glVertexPointer(3, GL_FLOAT, sizeof(GL_FLOAT)*3, &mesh.mPosition[0][0]);
	// normals
	if( mesh.mNormal.size()>0 )
	{
		glEnableClientState(GL_NORMAL_ARRAY);
		glNormalPointer(GL_FLOAT, sizeof(GL_FLOAT)*3, &mesh.mNormal[0][0]);
	}
	// draw triangles
	if( mesh.mTriangles.size()>0 )
		glDrawElements(GL_TRIANGLES, mesh.mTriangles.size()*3, GL_UNSIGNED_INT, &mesh.mTriangles[0][0]);
}

} //namespace xglm {



//		// draw points
//		if( ops.getOption(RenderOption::Points) )
//		{
//			// draw as points
//			glPointSize( ops.getPointSize() );
//			glDrawArrays(GL_POINTS, 0, mesh._vertices.size());
//		}
//		// bounding box
//		if( ops.getOption(RenderOption::BBox) )
//		{
//			Vec3f c = (mesh._bbox._min + mesh._bbox._max)*.5f;
//			Vec3f s = mesh._bbox._size;
//			glColor4f(1.0f, 0.0f, 1.0f, 0.3f);
//			glTranslatef(c[0], c[1], c[2]);
//			glScalef(s[0], s[1], s[2]);
//			glutSolidCube(1.0);
//		}
