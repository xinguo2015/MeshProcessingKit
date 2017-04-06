#pragma once

#include <memory.h>
#include <float.h>
#include <vector>
#include <string>
#include <algorithm>
#include "XGLM.h"

#define NullFace   (-1)
#define NullVertex (-1)

namespace xglm {

using std::vector;

#ifndef SWAP3
#define SWAP3(a,b,temp) ((temp)=(a),(a)=(b),(b)=(temp))
#endif // ifndef SWAP3

typedef vector<int> IntList;

// bounding sphere
// axis aligned bounding box
class BBox
{
	public:
		Vec3f _min;
		Vec3f _max;
		Vec3f _size;
	public:
		BBox()                       { Reset(); }
		bool IsValid() const		 { return (_size[0]!=-FLT_MAX); }
		void Reset()		         { _size[0] = -FLT_MAX; }
		void Reset(const Vec3f &p)   { _min = _max = p; _size=Vec3f(0,0,0); }
		void AddPoint(const Vec3f &p){ _min = min(_min,p); _max = max(_max,p); }
		void UpdateSize()            { _size = _max - _min; }
		const Vec3f& Min() const     { return _min; }
		const Vec3f& Max() const     { return _max; }
		const Vec3f& Size() const    { return _size;}
		void SetPoints(const vector<Vec3f>& v); // calc bounding box
};


class HalfEdge
{
public:
	HalfEdge()                {}
	HalfEdge(int v, int f)    : mEndVtx(v),mLeftFace(f) {}
public:
	int   endVtx() const	  { return mEndVtx; }
	int   leftFace() const	  { return mLeftFace; }
	float weight() const      { return mWeight; }
    float&weight()            { return mWeight; }
	void  setEndVtx(int v)	  { mEndVtx = v; }
	void  setLeftFace(int f)  { mLeftFace = f; }
	void  setWeight( float w) { mWeight = w; }
public:
	float mWeight;
protected:
	// NOTE: the starting vertex is the owner of this half edge, but not recorded here
	int mEndVtx;   // ending vertex, 
	int mLeftFace; // face at left side
};

class HEdgeList : public vector<HalfEdge> 
{
public:
	HEdgeList()        {}
	HEdgeList(int n)   :vector<HalfEdge>(n) {}
	int edgeOf(int ev) const
	{	
		vector<HalfEdge>::const_iterator f = std::find_if(
			begin(), end(), [ev](const HalfEdge &a){ return a.endVtx()==ev; } );
		return ( f == end() ? (-1) : f-begin() );
	}
	int nextEdgeOf(int ev) const
	{
		vector<HalfEdge>::const_iterator f = std::find_if(
			begin(), end(), [ev](const HalfEdge &a){ return a.endVtx()==ev; } );
		if( f == end() ) return (-1);
		return ( ++f == end() ? 0 : f-begin() );
	}
	int prevEdgeOf(int ev) const
	{
		vector<HalfEdge>::const_iterator f = std::find_if(
			begin(), end(), [ev](const HalfEdge &a){ return a.endVtx()==ev; } );
		if( f == end() ) return (-1);
		return ( f == begin() ? size()-1 : f-begin()-1 );
	}
};

class VtxTopology
{
public:
	VtxTopology() : mManifold(0), mBoundary(0) {}
	short getManifold() const { return mManifold; }
	short getBoundary() const { return mBoundary; }
	void  setManifold( short m ) { mManifold = m; }
	void  setBoundary( short b ) { mBoundary = b; } 
protected:
	short mManifold;
	short mBoundary;
};

class VtxLink : public VtxTopology
{
public:
	VtxLink()                           :mWeight(0) {}
	const HEdgeList & edges() const     { return mEdges; }
	      HEdgeList & edges()           { return mEdges; }
	void setWeight(float w)             { mWeight = w; }
	float weight()                      { return mWeight; }
	
public:
	float mWeight;
protected:
	HEdgeList mEdges; // list of half edges originating from this


};

class TriMesh 
{
public:
	// vertices
	vector<Vec3f>    mPosition;         // position
	vector<Vec3f>    mTexCoord;         // texture coordinate
	vector<Vec3f>    mNormal;           // normal vector
	vector<Vec3f>    mTangent;          // tagent vector
	// triangles
	vector<Vec3i>    mTriangles;        // triangle indices
	vector<Vec3i>    mTriTexIndices;    // triangle texture coord indices
	vector<Vec3i>    mTriNormalIndices; // triangle normal vector indices
	// polygons
	vector<IntList>  mPolygons;         // polygon indices
	vector<IntList>  mPolyTexIndices;   // polygon texture coord indices
	vector<IntList>  mPolyNormalIndices;// polygon normal vector indices
	// face properties
	vector<Vec3f>    mFaceNormal;
	vector<float>    mFaceArea;
	// augmented vertices
	vector<VtxLink>  mVtxLinks;
	// global geometry
	BBox _bbox; // bounding box
public:
	// if not presented, create them
	int needBBox();
	int needEdges();
	int needBoundaryEdges();
	int needVtxLinks();
	int needTopoBoundary();
	int needTopoManifold();
	int needFaceNormals( bool needFaceAreas = true );
	int needFaceAreas();
	int needVtxNormals();
	int needCotLaplacian();
	int needSimpleLaplacian();
	int needCrease(float angle);
	// Delete everything
	void clear();
public:
	Vec3f calcFaceNormal(int f) const;
	HalfEdge* findHalfEdge(int vStart, int vEnd);
};

// for mesh io and processing algorithms;
class MeshData
{
public:
	MeshData() { Reset(); }
	void Reset();

public:
	vector<Vec3f>    *position;         // position
	vector<Vec3f>    *texCoord;         // texture coordinate
	vector<Vec3f>    *normal;           // normal vector
	vector<Vec3f>    *tangent;          // tagent vector
	vector<Vec3i>    *triangles;        // triangle vertex indices
	vector<Vec3i>    *triTexIndices;    // triangle texture coord indices
	vector<Vec3i>    *triNormalIndices; // triangle normal vector indices
	vector<IntList>  *polygons;         // polygon vertex indices
	vector<IntList>  *polyTexIndices;   // polygon texture coord indices
	vector<IntList>  *polyNormalIndices;// polygon normal vector indices
	vector<Vec3f>    *faceNormal;       // face normal vector
	vector<VtxLink>  *vtxLinks;
};

// generate the edges linked to each vertex
int genVtxLinks(
	const std::vector<Vec3i> & triangle,
	std::vector<VtxLink>     & vlink);

// insert a virtual edge at each boundary edge
// return value: 1 - manifold mesh, 0 - otherwise
int genBoundaryEdges(
	const std::vector<Vec3i> & triangle,
	std::vector<VtxLink>     & vlink);

int calcFaceNormals(
	const std::vector<Vec3f> & position,
	const std::vector<Vec3i> & triangle,
	std::vector<Vec3f> & faceNormal );

void normalizeNormal(
	std::vector<Vec3f> & faceNormal,
	std::vector<float> & faceArea);

int Smooth(
	float normalSigma,
	int   normalIterNum,
	int   posIterNum,
	float posUpdateStepsize,
	std::vector<Vec3f>   & position,
	std::vector<Vec3i>   & triangle,
	std::vector<Vec3f>   & triNormal,
	std::vector<VtxLink> & vtxLink );
} //namespace xglm {
