#ifndef _____mesh_io_h_____________
#define _____mesh_io_h_____________

#include <vector>
#include <string>
#include <iostream>

namespace xglm 
{
using namespace std;

class MeshData
{
public:
	MeshData() { Reset(); }
	int parseOBJ(istream & infile);

public:
	void Reset();
	vector<Vec3f>    *position;         // position
	vector<Vec3f>    *texCoord;         // texture coordinate
	vector<Vec3f>    *normal;           // normal vector
	vector<Vec3f>    *tangent;          // tagent vector
	vector<Vec3i>    *triPosIndices;    // triangle indices
	vector<Vec3i>    *triTexIndices;    // triangle texture coord indices
	vector<Vec3i>    *triNormalIndices; // triangle normal vector indices
	vector<IntList>  *polyPosIndices;   // polygon indices
	vector<IntList>  *polyTexIndices;   // polygon texture coord indices
	vector<IntList>  *polyNormalIndices;// polygon normal vector indices
};

int readOBJMeshData(std::istream& sourceStream, MeshData& data );


template<typename TMesh>
class MeshIO 
{
public:
	static int readMesh(const string & filename, TMesh & mesh);
	static int readOBJ(istream & inputstream, TMesh & mesh);
};


}

#endif //#ifndef _____mesh_io_h_____________
