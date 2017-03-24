/*
 *
 *    Author:  Xinguo Liu, Zhejiang University
 *     Email:  xgliu@cad.zju.edu.cn
 *
 */

#include <fstream>
#include "TriMesh.h"
#include "MeshIO.h"

using std::string;
using std::vector;

namespace xglm {
	
	void MeshData::Reset()
	{
		position = NULL;      // position
		texCoord = NULL;      // texture coordinate
		normal = NULL;        // normal vector
		tangent = NULL;       // tagent vector
		triPosIndices = NULL;    // triangle indices
		triTexIndices = NULL;    // texture coord indices
		triNormalIndices = NULL; // normal vector indices
		polyPosIndices = NULL;// polygon indices
		polyTexIndices = NULL;// texture coord indices
		polyNormalIndices = NULL;// normal vector indices
	}
	

	template<>
	int MeshIO<TriMesh>::readOBJ(istream & inputstream, TriMesh & mesh)
	{
		MeshData data;
		data.position      = & mesh.mPosition;      // read vertex position
		data.triPosIndices = & mesh.mTriangles; // read triangles
		// start to read
		if( ! readOBJMeshData(inputstream, data) )
			return 0;
		// build the mesh
		mesh.needBBox();
		mesh.needFaceNormals();
		mesh.needVtxNormals();
		mesh.needVtxLinks();
		mesh.needTopoBoundary();
		mesh.needCotLaplacian();
		return 1;
	}

	template<>
	int MeshIO<TriMesh>::readMesh(const string & filename, TriMesh & mesh)
	{
 		int readResult = 0;
		const char * pext = filename.length() ? strrchr(filename.c_str(),'.') : NULL;
		if( ! pext ) {
			printf("file extension/type error\n"); 
			return 0;
		}
		ifstream ifs( filename.c_str());
		if( strcasecmp(pext,".obj")==0 )
            readResult = MeshIO<TriMesh>::readOBJ( ifs, mesh);
		return readResult;
		return 1;
	}


}
