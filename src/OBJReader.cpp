#include <vector>
#include <sstream>
#include <iostream>
#include <algorithm>

#include "TriMesh.h"
#include "MeshIO.h"

#ifndef V3_MAKE
#define V3_MAKE(V, x, y, z) \
        ((V)[0] = x,\
         (V)[1] = y,\
         (V)[2] = z)
#endif

#define MaxOBJLine 2048
#define OneOf2(c,a,b) ((c)==(a)||(c)==(b))

#define ErrReturn(a, b)  do { printf(a); return(b); } while(0)
#define CheckMsg(a,b)  do { if( !a ) printf(b); } while(0)

using namespace std;

namespace xglm {
	
static void readPosition( stringstream & ss, MeshData & data )
{
	float x, y, z;
	ss >> x >> y >> z;
	data.position->push_back( Vec3f( x, y, z ));
}
static void readTexCoord( stringstream & ss, MeshData & data )
{
	float u, v, w;
	ss >> u >> v >> w;
	data.texCoord->push_back( Vec3f( u, v, w ));
}
static void readNormal( stringstream & ss, MeshData & data )
{
	float x, y, z;
	ss >> x >> y >> z;
	data.normal->push_back( Vec3f( x, y, z ));
}
static void readTangent( stringstream & ss, MeshData & data )
{
	float x, y, z;
	ss >> x >> y >> z;
	data.tangent->push_back( Vec3f( x, y, z) );
}
   
static Vec3i parseFaceIndex( const string& token )
{
	// parse indices of the form
	//
	// p/[t]/[n]
	//
	// where p is an index into positions,
	//       t is an index into texcoords,
	//       n is an index into normals,
	//
	// and [.] indicates an optional index
	
	stringstream theInput( token );
	string indexstring;
	int indices[3] = { 0, 0, 0 }, k = 0;

	while( getline( theInput, indexstring, '/' ) && k<3 )
		indices[k++] = atoi( indexstring.c_str() );
	// decrement since indices in OBJ files are 1-based
	return Vec3i(indices[0]-1,
				 indices[1]-1,
				 indices[2]-1);
}

static int addTriangle(vector<Vec3i> & f, MeshData & data)
{
	data.triangles->push_back(Vec3i(f[0][0],f[1][0],f[2][0]));
	if( data.triTexIndices )
		data.triTexIndices->push_back(Vec3i(f[0][1],f[1][1],f[2][2]));
	if( data.triNormalIndices )
		data.triNormalIndices->push_back(Vec3i(f[0][2],f[1][2],f[2][2]));	
	return 1;
}

static int addPolygon(vector<Vec3i> & f, MeshData & data)
{
	IntList L;
	vector<Vec3i>::const_iterator k;
	// position indices
	for( k=f.begin(); k!=f.end(); k++ )
		L.push_back( (*k)[0] );
	data.polygons->push_back( L );
	// texture coordinate indices
	for( k=f.begin(); k!=f.end(); k++ )
		L.push_back( (*k)[1] );
	data.polyTexIndices->push_back( L );
	// normal indices
	for( k=f.begin(); k!=f.end(); k++ )
		L.push_back( (*k)[2] );
	data.polygons->push_back( L );
	return 1;
}

static int readFace( stringstream & ss, MeshData & data )
{
	string token;
	vector<Vec3i> f;
	if( ! data.polygons && ! data.triangles )
		return 1;
	while( ss >> token )
	{
		Vec3i idx = parseFaceIndex( token );
		if ( idx[0]<0 ) return 0;
		f.push_back( idx );		
	}
	if( f.size()<3 ) 
	{
		xglm_debug("face with < 3 vertices");
		return 0;
	}
	if( f.size()==3 && data.triangles )
		return addTriangle(f, data);
	else if( data.polygons )
		return addPolygon(f, data);
	return 1;
}

int readOBJMeshData(std::istream& sourceStream, MeshData& data )
{
	int VN=0, FN=0, TN=0, NN=0;
	string line;

	while( getline( sourceStream, line ))
	{
		string token;
		stringstream ss( line );
 
		ss >> token;

		if( token == "f" && (data.triangles || data.polygons ) ) { FN++;
			if ( ! readFace ( ss, data ) ) {
				xglm_debug("Wrong line for face\n");
				return 0;
			}
			continue;
		}
		if( token == "v"  && data.position ) {     
			readPosition( ss, data ); VN++; continue; }
		if( token == "vt" && data.texCoord ) {    
			readTexCoord( ss, data ); TN++; continue; }
		if( token == "vn" && data.normal ) {      
			readNormal  ( ss, data ); NN++; continue; } 
		if( token == "vf" && data.texCoord ){ 
			readTangent ( ss, data ); continue; }
		if( token[0] == '#' )    // comment
			continue;  
		if( token == "o" )       // object name		
			continue;
		if( token == "g" )       // group name
			continue;
		if( token == "s" )       // smoothing group
			continue;
		if( token == "mtllib" )  // material library
			continue; 
		if( token == "usemtl" )  // material
			continue; 
		if( token == "" )        // empty string
			continue; 
		// fail with error
		xglm_debug("Seems invalid Wavefront OBJ file! (Offending line:\n%s\n",line.c_str());
		return 0;
	}
	// succeed
	return 1;
}

}; // namespace xglm {
