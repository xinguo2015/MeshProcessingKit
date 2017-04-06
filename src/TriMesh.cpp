#include <stdio.h>
#include <memory.h>
#include <math.h>
#include "TriMesh.h"

namespace xglm {
//--------------------------------------------------------------------------------

void BBox::SetPoints(const std::vector<Vec3f>& v)
{
	if( v.size()<1 ) return;
	this->Reset( v[0] );
	for( unsigned int i=1; i<v.size(); i++ )
		this->AddPoint( v[i] );
	this->UpdateSize();
}

void MeshData::Reset()
{
// 	position = NULL;
// 	texCoord = NULL;
// 	normal = NULL;
// 	tangent = NULL;
// 	triPosIndices = NULL;
// 	triTexIndices = NULL;
// 	triNormalIndices = NULL;
// 	polyPosIndices = NULL;
// 	polyTexIndices = NULL;
// 	polyNormalIndices = NULL;
	memset((void*)this,0,sizeof(MeshData));
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
	mVtxLinks.resize(mPosition.size());
	return genVtxLinks(mTriangles,mVtxLinks);
}

int TriMesh::needBoundaryEdges()
{
	return genBoundaryEdges(mTriangles, mVtxLinks);
}


int TriMesh::needFaceNormals( bool needFaceAreas )
{
	calcFaceNormals(mPosition,mTriangles,mFaceNormal);
	mFaceArea.resize( needFaceAreas?mTriangles.size():0 );
	normalizeNormal(mFaceNormal,mFaceArea);
	return 1;
}

int TriMesh::needFaceAreas()
{
	const int FN = mTriangles.size();
	if( FN<1 ) return 0;
	mFaceArea.resize( FN );
	for( int k=0; k<FN; k++ ) 
	{	
		mFaceArea[k] = calcFaceNormal(k).length() * 0.5f;
	}
	return 1;
}

int TriMesh::needVtxNormals()
{
	const int FN = mTriangles.size();
	const int VN = mPosition.size();
	if( mFaceNormal.size()!=mTriangles.size() )
		needFaceNormals(false);
	mNormal.assign( VN, Vec3f(0,0,0) );
	for( int f=0; f<FN; f++ ) //visit all triangles
		for( int k=0; k<3; k++ ) // add face normal into vertex
		{
			xglm_assert(mTriangles[f][k]>=0 && mTriangles[f][k]<VN, "vertex index is out of range in TriMesh::needVtxNormals()\n");
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
	const int *v = mTriangles[f].get_value();
	return cross(
			mPosition[v[1]]-mPosition[v[0]],
			mPosition[v[2]]-mPosition[v[0]]);
}

//--------------------------------------------------
//                                                //
//             v2                                 //
//             /\                                 //
//         e0 /a \ e1                             //
//           /    \                               //
//         v0------*v1                            //
//                                                //
//                           cos(a)*(|e1||e2|)
// cot(a) = cos(a)/sin(a) = -------------------
//                           sin(a)*(|e1||e2|)
//
//                           <e1, e2>
//        = -------------------------------------
//             sqrt(1-cos(a)*cos(a))*(|e1||e2|)
//
//                      <e1, e2>
//        = --------------------------------
//             sqrt((|e1||e2|)^2-<e1,e2>^2
//
//--------------------------------------------------


int TriMesh::needCotLaplacian()
{
	// clear old weight if any
	for( vector<VtxLink>::iterator k = mVtxLinks.begin(); k<mVtxLinks.end(); k++) {
		std::for_each(k->edges().begin(), k->edges().end(), [](HalfEdge &e){
			e.setWeight(0);
		});
	}

	// compute cot form laplacian weight
	for( std::size_t k = 0; k<mTriangles.size(); k++ ) {
		const int * t = mTriangles[k].get_value();
		for( int j=0; j<3; j++ ) {
			int v0 = t[j];
			int v1 = t[(j+1)%3];
			int v2 = t[(j+2)%3];
			Vec3f e0 = mPosition[v0]-mPosition[v2];
			Vec3f e1 = mPosition[v1]-mPosition[v2];
			float d = e0.dot(e1);
            float w = d / sqrtf(e0.dot(e0)*e1.dot(e1) - d*d);
			HalfEdge* e = NULL;
			e = findHalfEdge(v0,v1); xglm_assert(e!=NULL,"edge(%d,%d) not found",v0,v1);
			e->weight() += w;
			e = findHalfEdge(v1,v0); xglm_assert(e!=NULL,"edge(%d,%d) not found",v1,v0);
			e->weight() += w;
		}
	}
	// diagonal weight
	for( std::size_t k = 0; k<mVtxLinks.size(); k++ ) {
        HEdgeList &eL = mVtxLinks[k].edges();
        float sum = 0;
        if( eL.size()<1 ) continue;
		std::for_each(eL.begin(),eL.end(),[&sum](HalfEdge& e) { sum += e.weight(); });
		std::for_each(eL.begin(),eL.end(),[ sum](HalfEdge& e) { e.weight() /= sum; });
		mVtxLinks[k].setWeight(-sum);
	}
	// normalize weight
	return 1;
}

int TriMesh::needSimpleLaplacian()
{
	// clear old weight if any
	for( vector<VtxLink>::iterator k = mVtxLinks.begin(); k<mVtxLinks.end(); k++) {
		std::for_each(k->edges().begin(), k->edges().end(), [](HalfEdge &e){
			e.setWeight(1.f);
		});
	}
	return 1;
}

int TriMesh::needCrease(float angle) 
{
	const float cosa = cos(angle/180*M_PI);
	const size_t FN = mTriangles.size();
	const size_t VN = mPosition.size();
	if( mNormal.size()<=VN )
		needFaceNormals();
	else
		mNormal.resize(VN);
	
	mTriNormalIndices.resize(FN);
	for( size_t k = 0; k<FN; k++ )
	{
		Vec3i ndx = mTriangles[k];
		int * kv = (int*)ndx;
		int flag = 0;
		for( int j = 0; j<3; j++ )
		{
			if( mFaceNormal[k].dot(mNormal[kv[j]])<cosa ) {
				if( !flag ) { // push the face to the vertex normal
					flag = 1; mNormal.push_back(mFaceNormal[k]);
				}
				kv[j] = mNormal.size()-1;
			}
		}
		mTriNormalIndices[k] = ndx;
	}
	return 1; 
}

HalfEdge* TriMesh::findHalfEdge(int vStart, int vEnd)
{
	xglm_assert(vStart>=0 && vStart<(int)mVtxLinks.size(), "vStart is out of range");
	HEdgeList & eL = mVtxLinks[vStart].edges();
	vector<HalfEdge>::iterator e = std::find_if(eL.begin(), eL.end(), [vEnd](const HalfEdge& e) {
		return e.endVtx()==vEnd;
		});
	return (e==eL.end() ? NULL : &(*e));
}

//--------------------------------------------------------------------------------
} //namespace xglm {
