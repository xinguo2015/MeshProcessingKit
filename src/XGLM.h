/*****************************************************
	> File:   xglm.h
	> Author: Xinguo Liu
	> EMail:  xgliu@cad.zju.edu.cn 
	> Date:   2016年01月17日 星期日 12时58分44秒
 ***************************************************/

#include <vector>
#include "Math3D/math.h"

namespace xglm {
	
typedef std::vector<int>    IntArray;
typedef std::vector<float>  FloatArray;
typedef std::vector<double> DoubleArray;

typedef std::vector<Vec3i>  V3iArray;
typedef std::vector<Vec3f>  V3fArray;
typedef std::vector<Vec3d>  V3dArray;

#ifdef _DEBUG

#define XGLM_LOG(format,...) printf("%s() in %s, line - %d)\n\t",__FUNCTION__, __FILE__, __LINE__), printf(format, ##__VA_ARGS__)
#define XGLM_ASSERT(a,format,...) if ( !(a) ) XGLM_LOG(format,##__VA_ARGS__)

#else

#define XGLM_LOG(format,...)
#define XGLM_ASSERT(a,format,...)

#endif

} //namespace xglm {