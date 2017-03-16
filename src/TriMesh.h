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

class TriFace : public Vec3i
{
public:
	TriFace()							{}
	TriFace( int *v ) 					{ V3_COPY(_v,v); }
	TriFace( int v0, int v1, int v2 )	{ V3_MAKE(_v,v0,v1,v2); }
public:
	int indexOf(int v) const			{ return ( v==x ? 0 : ( v==y ? 1 : ( v==z? 2 : -1 ) ) ); };
	int nextOf(int v) const				{ return ( v==x ? y : ( v==y ? z : ( v==z? x : -1 ) ) ); };
	int prevOf(int v) const				{ return ( v==x ? z : ( v==y ? x : ( v==z? y : -1 ) ) ); };
};

class Polygon : public IntList
{
public:
	Polygon()							{}
	Polygon( int *v, int n )			: IntList(n) { iterator k=begin(); while( k!=end() ) *k++ = *v++; }
	Polygon( int v0, int v1, int v2 )	: IntList(3) { iterator k=begin(); *k++ = v0; *k++ = v1; *k++=v2; }
public:
	int indexOf(int v) const			{ IntList::const_iterator k = std::find(begin(), end(), v); return k==end() ? -1 : k-begin(); }
	int nextOf(int v) const				{ IntList::const_iterator k = std::find(begin(), end(), v); if( k==end() ) return -1; k++; return( k==end()? front() : *k ); }
	int prevOf(int v) const				{ IntList::const_iterator k = std::find(begin(), end(), v); if( k==end() ) return -1; return( k==begin()? back() : *--k ); }
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
	HEdgeList & edges()   { return mEdges; }
	
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
	int needVtxNormals();
	int needEdgeWeight(char whichkind); // 'U' - uniform wight, 'L' - cotangent form laplacian
	// Delete everything
	void clear();
public:
	Vec3f calcFaceNormal(int f) const;
};

} //namespace xglm {
