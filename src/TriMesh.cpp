#include <stdio.h>
#include "MyGLUT.h"
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
				XGLM_LOG("Add a boundary edge(%d->%d | %d)\n", v0, v1, (int)k);
				mVtxLinks[v0].edges().push_back(HalfEdge(v1,NullFace));
			}
			else if( eFound->leftFace()<0 )
			{
				// another face produce the same boundary
				XGLM_LOG("Multiple face generate the same boundary(%d->%d | %d)\n", v0, v1, (int)k);
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
//                                  <e1, e2>/(|e1|*|e2|)
//        cot(a) = cos(a)/sin(a) = ----------------------
//                                   2*Area /(|e1|*|e2|)
//--------------------------------------------------

#ifndef CLAMP
#define CLAMP(x, a, b) ((x)<(a) ? (a) : ((x)>(b) ? (b) : (x)))
#endif

int TriMesh::needCotLaplacian()
{
	// clear old weight if any
	for( vector<VtxLink>::iterator k = mVtxLinks.begin(); k<mVtxLinks.end(); k++) {
		std::for_each(k->edges().begin(), k->edges().end(), [](HalfEdge &e){
			e.setWeight(0);
		});
	}
	// compute triangle area
	needFaceAreas();
	// compute new weight
	for( std::size_t k = 0; k<mTriangles.size(); k++ ) {
		const int * t = mTriangles[k].get_value();
		for( int j=0; j<3; j++ ) {
			int v0 = t[j];
			int v1 = t[(j+1)%3];
			int v2 = t[(j+2)%3];
			//Vec3f e0 = mPosition[v0]-mPosition[v2];
			//Vec3f e1 = mPosition[v1]-mPosition[v2];
			float d = (mPosition[v0]-mPosition[v2]).dot(mPosition[v1]-mPosition[v2]);
			float area2 = 2*mFaceArea[k];
			area2 = MAX(area2, 1.0e-6);
			float cota = d/area2;
			cota = CLAMP(cota,-100,100);
			HalfEdge* e = NULL;
			e = findHalfEdge(v0,v1); XGLM_ASSERT(e!=NULL,"edge(v0,v1) not found");
			e->setWeight( e->weight()+cota );
			e = findHalfEdge(v1,v0); XGLM_ASSERT(e!=NULL,"edge(v1,v0) not found");
			e->setWeight( e->weight()+cota );
		}
	}
	// diagonal weight
	for( std::size_t k = 0; k<mVtxLinks.size(); k++ ) {
		HEdgeList &eL = mVtxLinks[k].edges();
		float sum = 0;
		std::for_each(eL.begin(),eL.end(),[&sum](HalfEdge& e) { sum += e.weight(); });
		std::for_each(eL.begin(),eL.end(),[ sum](HalfEdge& e) { e.setWeight(e.weight()/sum); });
		mVtxLinks[k].setWeight(sum);
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


HalfEdge* TriMesh::findHalfEdge(int vStart, int vEnd)
{
	XGLM_ASSERT(vStart>=0 && vStart<(int)mVtxLinks.size(), "vStart is out of range");
	HEdgeList & eL = mVtxLinks[vStart].edges();
	vector<HalfEdge>::iterator e = std::find_if(eL.begin(), eL.end(), [vEnd](const HalfEdge& e) {
		return e.endVtx()==vEnd;
		});
	return (e==eL.end() ? NULL : &(*e));
}

//--------------------------------------------------------------------------------
} //namespace xglm {
