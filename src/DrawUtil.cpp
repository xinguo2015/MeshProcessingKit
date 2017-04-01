#include "DrawUtil.h"

namespace xglm {
	
	// draw smoothed group
	void DrawMesh::Face(
		const vector<Vec3i>& triangle,
		const vector<Vec3i>& normalIndex,
		const vector<Vec3f>& position,
		const vector<Vec3f>& normal)
	{
		glBegin(GL_TRIANGLES);
		xglm_assert(triangle.size()==normalIndex.size(),"wrong input");
		for( std::size_t k=0; k<triangle.size(); k++ )
		{
			const int * v =    triangle[k].get_value();
			const int * n = normalIndex[k].get_value();
			glNormal3fv(  normal[n[0]].get_value());
			glVertex3fv(position[v[0]].get_value());
			glNormal3fv(  normal[n[1]].get_value());
			glVertex3fv(position[v[1]].get_value());
			glNormal3fv(  normal[n[2]].get_value());
			glVertex3fv(position[v[2]].get_value());
		}
		glEnd();
	}
	
	// 1. facet normal  - normal.size()  = triangle.size()
	// 2. vertex normal - normal.size() >= vertex.size()
	void DrawMesh::Face(
		const vector<Vec3i>& triangle, 
		const vector<Vec3f>& position, 
		const vector<Vec3f>& normal) 
	{
		xglm_assert(triangle.size()==normal.size() || position.size()<=normal.size(),
					"wrong input");
		glBegin(GL_TRIANGLES);
		for( std::size_t k=0; k<triangle.size(); k++ )
		{
			const int * v = triangle[k].get_value();
			if( normal.size()==triangle.size() ) 
			{	// normal per face
				glNormal3fv(  normal[ k  ].get_value());
				glVertex3fv(position[v[0]].get_value());
				glVertex3fv(position[v[1]].get_value());
				glVertex3fv(position[v[2]].get_value());
			}
			else// if( normal.size()==position.size() )
			{	// normal per vertex
				glNormal3fv(  normal[v[0]].get_value());
				glVertex3fv(position[v[0]].get_value());
				glNormal3fv(  normal[v[1]].get_value());
				glVertex3fv(position[v[1]].get_value());
				glNormal3fv(  normal[v[2]].get_value());
				glVertex3fv(position[v[2]].get_value());
			}
		}
		glEnd();
	}
	
	void DrawMesh::FaceID(
		const vector<Vec3i>& triangle, 
		const vector<Vec3f>& position)
	{
		PixelInfo p;
		glBegin(GL_TRIANGLES);
		for( std::size_t k=0; k<triangle.size(); k++ )
		{
			const int * t = triangle[k].get_value();
			p.pack(2,k);
			glColor4ubv((unsigned char*)p);
			glVertex3fv(position[t[0]].get_value());
			glVertex3fv(position[t[1]].get_value());
			glVertex3fv(position[t[2]].get_value());
		}
		glEnd();
	}
	
	void DrawMesh::PointID(
		const vector<Vec3f>& position)
	{
		PixelInfo p;
		glBegin(GL_POINTS);
		for( std::size_t k=0; k<position.size(); k++ )
		{
			p.pack(1,k);
			glColor4ubv((unsigned char*)p);
			glVertex3fv(position[k].get_value());
		}
		glEnd();
	}
	
////////////////////
} //namespace xglm {

