/*****************************************************
	> File:   xglm.h
	> Author: Xinguo Liu
	> EMail:  xgliu@cad.zju.edu.cn 
	> Date:   2016年01月17日 星期日 12时58分44秒
 ***************************************************/

#ifndef __xglm_h_________________________
#define __xglm_h_________________________

#include <stdio.h>
#include <vector>
#include <map>
#include "Math3D/math.h"

// print some log information

#ifdef _DEBUG
#define xglm_debug(format,...) printf("%s() in %s, line - %d)\n\t",__FUNCTION__, __FILE__, __LINE__), printf(format, ##__VA_ARGS__)
#define xglm_assert(a,format,...) if ( !(a) ) xglm_debug(format,##__VA_ARGS__)
#else
#define xglm_debug(format,...) 
#define xglm_assert(a,format,...)
#endif


namespace xglm {
	
typedef std::vector<int>    IntArray;
typedef std::vector<float>  FloatArray;
typedef std::vector<double> DoubleArray;

typedef std::vector<Vec3i>  V3iArray;
typedef std::vector<Vec3f>  V3fArray;
typedef std::vector<Vec3d>  V3dArray;

} //namespace xglm {

#endif //#ifndef __xglm_h_________________________