#ifndef _____mesh_io_h_____________
#define _____mesh_io_h_____________

#include <vector>
#include <string>
#include <iostream>
#include "TriMesh.h"

namespace xglm 
{
using namespace std;


int readOBJMeshData(std::istream& sourceStream, MeshData& data );


template<typename TMesh>
class MeshIO 
{
public:
	static int readMesh(const string & filename, TMesh & mesh);
public:
	static int readOBJ(istream & inputstream, TMesh & mesh);
};


}

#endif //#ifndef _____mesh_io_h_____________
