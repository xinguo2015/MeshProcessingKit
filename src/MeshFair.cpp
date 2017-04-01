#include <math.h>
#include "XGLM.h"
#include "TriMesh.h"

using std::string;
using std::vector;

namespace xglm {
	
	static float weight(float sigma, float x)
	{
		return exp(-x*x/(2.0*sigma*sigma));
	}
	
	int Smooth(
		float sigma[2],
		int   iterNum[2],
		float stepsize,
		std::vector<Vec3f>   & position,
		std::vector<Vec3i>   & triangle,
		std::vector<Vec3f>   & triNormal,
		std::vector<VtxLink> & vtxLink );
	
	int Smooth(
		float sigma[2],
		int   iterNum[2],
		float stepsize,
		std::vector<Vec3f>   & position,
		std::vector<Vec3i>   & triangle,
		std::vector<Vec3f>   & triNormal,
		std::vector<VtxLink> & vtxLink )
	{
		const int VN = position.size();
		const int FN = triangle.size();
		
		// smooth face normal
		std::vector<Vec3f> newFaceNormal(FN);
		for( int k = 0; k<iterNum[0]; k++ )
		{
			for( int f=0; f<FN; f++ )
			{
				Vec3f oldN ( triNormal[f] );
				Vec3f newN ( triNormal[f]*2.0f );
				float wsum = 0;
				// find the adjacent faces
				for( int v=0; v<3; v++ )
				{
					const HEdgeList &edges = vtxLink[triangle[f][v]].edges();
					for( std::size_t e=0; e<edges.size(); e++ )
					{
						int lf = edges[e].leftFace();
						//pass boundary and face f
						if( lf==f || lf<0 ) continue;
						float dd = oldN.dot(oldN - triNormal[lf]);
						float w = weight(sigma[1],dd); wsum += w;
						newN += w * triNormal[lf];
					}
				}
				newFaceNormal[f] = newN.normalize();
			}
		}
		
		// update vertex position
		std::vector<Vec3f> newPos = position; 
		for( int k = 0; k<iterNum[0]; k++ )
		{
			// compute gradient for each vertex
			std::vector<Vec3f> g(VN,Vec3f(0,0,0));
			for( int f = 0; f<FN; f++ )
			{
				for( int v=0; v<3; v++ )
				{
					int vi = triangle[f][v];
					int vj = triangle[f][(v+1)%3];
					float proj = newFaceNormal[f].dot(newPos[vi]-newPos[vj]);
					g[vi] += newFaceNormal[f]*proj;
					g[vj] -= newFaceNormal[f]*proj;
				}
			}
			// v <== v + r * dv
			for( int v=0; v<VN; v++ )
				newPos[v] -= stepsize * g[v];
		}
		// write the result back
		position = newPos;
		return 0;
	}

}
