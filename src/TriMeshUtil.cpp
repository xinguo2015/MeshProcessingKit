#include <stdio.h>
#include <memory.h>
#include <math.h>
#include "TriMesh.h"

namespace xglm {
//--------------------------------------------------------------------------------

int genVtxLinks(
	const std::vector<Vec3i> & triangle,
	std::vector<VtxLink>     & vlink)
{
	const std::size_t FN = triangle.size();
	const std::size_t VN = vlink.size();
	std::for_each(vlink.begin(),vlink.end(),
		[](VtxLink& vL){ vL.edges().resize(0); });
	// generate half edges of each triangle
	for( std::size_t k = 0; k<FN; k++ )
	{
		const Vec3i & f = triangle[k];
		for( int j=0; j<3; j++ )
		{
			xglm_assert(f[j]>=0 && f[j]<(int)VN, "vertex index out of range\n");
			vlink[f[j]].edges().push_back(HalfEdge(f[(j+1)%3],k));
		}
	}
	return 1;
}

int genBoundaryEdges(
	const std::vector<Vec3i> & triangle,
	std::vector<VtxLink>     & vlink)
{
	int isManifold = 1;
	const std::size_t FN = triangle.size();
	const std::size_t VN = vlink.size();
	for( std::size_t k = 0; k<FN; k++ )
	{
		const Vec3i &f = triangle[k];
		for( int j=0; j<3; j++ )
		{
			int v1 = f[j];       // NOTE: inverse order
			int v0 = f[(j+1)%3]; // NOTE: inverse order
			xglm_assert(v0>=0&&v0<(int)VN&&v1>=0&&v1<(int)VN,"Index check error");
			// find half edge: v0->v1
			HEdgeList & es = vlink[v0].edges();
			HEdgeList::iterator eFound = std::find_if(es.begin(),es.end(),
				[v1](const HalfEdge &h) { return h.endVtx()==v1; });
			// if not found, then add a boundary edge with leftFace = NullFace(-1)
			if( eFound==es.end() ) 
			{
				xglm_debug("Add a boundary edge(%d->%d | %d)\n", v0, v1, (int)k);
				vlink[v0].edges().push_back(HalfEdge(v1,NullFace));
			}
			else if( eFound->leftFace()<0 )
			{
				// another face produce the same boundary
				xglm_debug("Multiple face generate the same boundary(%d->%d | %d)\n", v0, v1, (int)k);
				xglm_debug("Indicates non-manifold mesh");
				eFound->setLeftFace(eFound->leftFace()-1);
				isManifold = 0;
			}
		}
	}
	return isManifold;
}

void normalizeNormal(
	std::vector<Vec3f> & faceNormal,
	std::vector<float> & faceArea)
{
	const int FN = faceNormal.size();
	const int keepArea = (faceArea.size()==faceNormal.size());
	for( int k=0; k<FN; k++ ) 
	{
		float len = faceNormal[k].length();
		if( len>1.0e-6 )
			faceNormal[k] /= len;
		if( keepArea )
			faceArea[k] = len/2.0;
	}
}

int calcFaceNormals(
	const std::vector<Vec3f> & position,
	const std::vector<Vec3i> & triangle,
	std::vector<Vec3f> & faceNormal )
{
	const int FN = triangle.size();
	if( FN<1 ) return 0;
	faceNormal.resize( FN );
	for( int k=0; k<FN; k++ ) 
	{	
		const int *v = triangle[k].get_value();
		faceNormal[k] = (position[v[1]]-position[v[0]]).cross(
			(position[v[2]]-position[v[0]]));
	}
	return 1;
}

//--------------------------------------------------------------------------------
} //namespace xglm {
