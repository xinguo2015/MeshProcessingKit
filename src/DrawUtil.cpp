#include "XGLM.h"
#include "MyGLUT.h"
#include "DrawUtil.h"
#include "TriMesh.h"
#include "PickPixel.h"

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

template<>
void DrawPointID<TriMesh>::draw( TriMesh & mesh)
{
	PixelInfo p;
	glBegin(GL_POINTS);
	for( std::size_t k=0; k<mesh.mPosition.size(); k++ )
	{
		p.pack(1,k);
		glColor4ubv((unsigned char*)p);
		glVertex3fv(mesh.mPosition[k].get_value());
	}
	glEnd();
}

template<>
void DrawFaceID<TriMesh>::draw( TriMesh & mesh)
{
	PixelInfo p;
	glBegin(GL_TRIANGLES);
	for( std::size_t k=0; k<mesh.mTriangles.size(); k++ )
	{
		const int * t = mesh.mTriangles[k].get_value();
		p.pack(2,k);
		glColor4ubv((unsigned char*)p);
		glVertex3fv(mesh.mPosition[t[0]].get_value());
		glVertex3fv(mesh.mPosition[t[1]].get_value());
		glVertex3fv(mesh.mPosition[t[2]].get_value());
	}
	glEnd();
}

} //namespace xglm {

