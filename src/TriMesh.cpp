#include <stdio.h>
#include "MyGLUT.h"
#include "TriMesh.h"

namespace xglm {


void BBox::SetPoints(const std::vector<Vec3f>& v)
{
	if( v.size()<1 ) return;
	this->Reset( v[0] );
	for( unsigned int i=1; i<v.size(); i++ )
		this->AddPoint( v[i] );
	this->UpdateSize();
}

void TriMesh::clear()
{
	mPosition.clear();         
	mTexCoord.clear();         
	mNormal.clear();           
	mTangent.clear();          
	mTriangles.clear();    
	mTriTexIndices.clear();    
	mTriNormalIndices.clear(); 
	mPolygons.clear();   
	mPolyTexIndices.clear();   
	mPolyNormalIndices.clear();
	mFaceNormal.clear();
	mFaceArea.clear();
	mVtxLinks.clear();
}

int TriMesh::needBBox()
{
	if( !_bbox.IsValid() )
		_bbox.SetPoints(mPosition);
	return 1;
}

int TriMesh::needEdges()
{
	const std::size_t FN = mTriangles.size();
	const std::size_t VN = mPosition.size();
	mVtxLinks.resize( VN );
	std::for_each(mVtxLinks.begin(),mVtxLinks.end(),[](VtxLink& v){
		v.edges().resize(0);
	});
	// generate half edges of each triangle
	for( std::size_t k = 0; k<FN; k++ )
	{
		const Vec3i & f = mTriangles[k];
		for( int j=0; j<3; j++ )
		{
			XGLM_ASSERT(f[j]>=0 && f[j]<(int)VN, "vertex index out of range\n");
			mVtxLinks[f[j]].edges().push_back(HalfEdge(f[(j+1)%3],k));
		}
	}
	return 1;
}

int TriMesh::needBoundaryEdges()
{
	const std::size_t FN = mTriangles.size();
	const std::size_t VN = mPosition.size();
	if( mVtxLinks.size()!=VN ) needEdges();
	// detecting and adding boundary edges
	for( std::size_t k = 0; k<FN; k++ )
	{
		const Vec3i &f = mTriangles[k];
		for( int j=0; j<3; j++ )
		{
			int v1 = f[j];       // NOTE: inverse order
			int v0 = f[(j+1)%3]; // NOTE: inverse order
			// find half edge: v0->v1
			HEdgeList & es = mVtxLinks[v0].edges();
			HEdgeList::iterator eFound = std::find_if(es.begin(),es.end(),[v1](const HalfEdge &h) {
				return h.endVtx()==v1;  });
			// if not found, then add a boundary edge with leftFace = NullFace(-1)
			if( eFound==es.end() ) 
			{
				XGLM_LOG("Add a boundary edge(%d->%d | %d)\n", v0, v1, k);
				mVtxLinks[v0].edges().push_back(HalfEdge(v1,NullFace));
			}
			else if( eFound->leftFace()<0 )
			{
				// another face produce the same boundary
				XGLM_LOG("Multiple face generate the same boundary(%d->%d | %d)\n", v0, v1, k);
				XGLM_LOG("Indicates non-manifold mesh");
				eFound->setLeftFace(eFound->leftFace()-1);
			}
		}
	}
	return 1;
}


int TriMesh::needFaceNormals( bool needFaceAreas )
{
	const int FN = mTriangles.size();
	if( FN<1 ) return 0;
	mFaceNormal.resize( FN );
	if( needFaceAreas ) mFaceArea.resize( FN );
	for( int k=0; k<FN; k++ ) 
	{	
		mFaceNormal[k] = calcFaceNormal(k);
		float len = mFaceNormal[k].length();
		if( needFaceAreas )
			mFaceArea[k] = len*0.5f;
		if( len>1.0e-6 )
			mFaceNormal[k] /= len;
	}
	return 1;
}

int TriMesh::needVtxNormals()
{
	const int FN = mTriangles.size();
	const int VN = mPosition.size();
	needFaceNormals(true);
	mNormal.assign( VN, Vec3f(0,0,0) );
	for( int f=0; f<FN; f++ ) //visit all triangles
		for( int k=0; k<3; k++ ) // add face normal into vertex
		{
			XGLM_ASSERT(mTriangles[f][k]>=0 && mTriangles[f][k]<VN, "vertex index is out of range in TriMesh::needVtxNormals()\n");
			mNormal[mTriangles[f][k]] += mFaceNormal[f];
		}
	// normalize vertex normal
	for( int k=0; k<VN; k++ )
		mNormal[k].normalize();
	return 1;
}

int TriMesh::needVtxLinks()
{
	const std::size_t VN = mPosition.size();
	if( mVtxLinks.size()<VN )
	{
		needEdges();
		needBoundaryEdges();
	}
	return 1;
}

// manifold: (1) at most 1 simple boundary edge,
//           (2) no over lapped edge
int TriMesh::needTopoManifold()
{
	const std::size_t VN = mPosition.size();
	if( mVtxLinks.size()<VN )
		needTopoBoundary();
	// identify boundary edges
	for( vector<VtxLink>::iterator k = mVtxLinks.begin(); k<mVtxLinks.end(); k++) {
		k->setManifold(0); // initialized as manifold
		if( k->getBoundary()>1 )
			k->setManifold(1); // not manifold
		else {
			const HEdgeList & eL = k->edges();
			for( HEdgeList::const_iterator e = eL.begin(); e<eL.end(); e++ )
			{
				int v1 = e->endVtx();
				// find overlapped edges
				HEdgeList::const_iterator f = std::find_if(++e,eL.end(),[v1](const HalfEdge &h) {
					return h.endVtx()==v1;
				});
				if( f<eL.end() ) { // found
					k->setManifold(1);  // not manifold
					break;
				}
			}
		}
	};
	
	return 1;
}

int TriMesh::needTopoBoundary()
{
	const std::size_t VN = mPosition.size();
	if( mVtxLinks.size()<VN )
		needVtxLinks();
	// identify boundary edges
	for( vector<VtxLink>::iterator k = mVtxLinks.begin(); k<mVtxLinks.end(); k++) {
		HEdgeList & eL = k->edges();
		int bn = 0;
		std::for_each(eL.begin(), eL.end(), [& bn](const HalfEdge &e){ 
			if( e.leftFace()<0 ) 
				bn += e.leftFace(); 
		});
		k->setBoundary(-bn);
	};
	
	return 1;
}

// Normal of face f
Vec3f TriMesh::calcFaceNormal(int f) const
{	
	const Vec3i & v = mTriangles[f];
	return cross(
			mPosition[v[1]]-mPosition[v[0]],
			mPosition[v[2]]-mPosition[v[0]]);
}

int TriMesh::needEdgeWeight(char whichkind)
{
	// clear old weight if any
	for( vector<VtxLink>::iterator k = mVtxLinks.begin(); k<mVtxLinks.end(); k++) {
		std::for_each(k->edges().begin(), k->edges().end(), [](HalfEdge &e){
			e.setWeight(0);
		});
	}
}

} //namespace xglm {
