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
	
	// bilateral smooth mesh normal 
	int smoothNormal(
		float sigma, 
		std::vector<Vec3i>   & triangle,
		std::vector<Vec3f>   & triNormal,
		std::vector<VtxLink> & vtxLink,
		std::vector<Vec3f>   & newTriNormal )
	{
		//const int VN = vtxLink.size();
		const int FN = triangle.size();
		
		newTriNormal.resize( FN );
		// smooth face normal
		for( int f=0; f<FN; f++ )
		{
			Vec3f oldN ( triNormal[f] );
			Vec3f newN ( triNormal[f]*2.0f );
			//float wsum = 0;
			// find the adjacent faces
			for( int v=0; v<3; v++ )
			{
				const HEdgeList &edges = vtxLink[triangle[f][v]].edges();
				for( std::size_t e=0; e<edges.size(); e++ )
				{
					int lf = edges[e].leftFace();
					//pass boundary and face f
					if( lf==f || lf<0 ) continue;
					float dd, w;
					dd = 1 - oldN.dot( triNormal[lf] );
					w = dd>0.5 ? 0 : weight(sigma,dd); // wsum += w;
					newN += w * triNormal[lf];
				}
			}
			newTriNormal[f] = newN.normalize();
		}
		
		return 1;
		
	}
	
	// ask the edges to be perpendicular to the face normal
	int updateVertexBySmoothedFaceNormal(
		float updateStepSize,
		std::vector<Vec3i>   & triangle,
		std::vector<Vec3f>   & triNormal,
		std::vector<VtxLink> & vtxLink,
		std::vector<Vec3f>   & position )
	{
		const int VN = position.size();
		const int FN = triangle.size();
		// compute gradient for each vertex
		std::vector<Vec3f> g(VN,Vec3f(0,0,0));
		//std::for_each(g.begin(),g.end(),[](Vec3f &v){if(v.x!=0||v.y!=0||v.z!=0)printf("error");});
		for( int f = 0; f<FN; f++ )
		{
			for( int v=0; v<3; v++ )
			{
				int vi = triangle[f][v];
				int vj = triangle[f][v>1?0:v+1];
				float pij = triNormal[f].dot(position[vi]-position[vj]);
				g[vi] += triNormal[f]*pij;
				g[vj] -= triNormal[f]*pij;
			}
		}
		// v <== v + r * dv
		for( int v=0; v<VN; v++ )
			position[v] -= updateStepSize * g[v];
		
		return 1;
	}
	
	int Smooth(
		float normalSigma,
		int   normalIterNum,
		int   posIterNum,
		float posUpdateStepsize,
		std::vector<Vec3f>   & position,
		std::vector<Vec3i>   & triangle,
		std::vector<Vec3f>   & triNormal,
		std::vector<VtxLink> & vtxLink )
	{
		// smooth face normal
		for( int k = 0; k<normalIterNum; k++ )
		{
			std::vector<Vec3f> temp = triNormal;
			smoothNormal(normalSigma,triangle,temp,vtxLink,triNormal);
		}
		// return 0;
		for( int k = 0; k<posIterNum; k++ ) {
			updateVertexBySmoothedFaceNormal(posUpdateStepsize,
				triangle,triNormal,vtxLink,position);
		}
		calcFaceNormals(position,triangle,triNormal);
		//normalize the normal vector
		std::for_each(triNormal.begin(),triNormal.end(), [](Vec3f &n){ 
			n.normalize(); }
		);
		return 0;
	}

} // namespace xglm {
