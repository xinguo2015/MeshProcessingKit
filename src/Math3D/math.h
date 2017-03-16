// TODO: Mat4<T>

#ifndef __xglm_math_h____________________________
#define __xglm_math_h____________________________

#ifndef M_PI
#define M_PI   double(3.1415926535897932384626433832795)
#endif

#include "vmacros.h"
#include "vec.h"
#include "mat.h"
#include "quat.h"

namespace xglm {

	typedef unsigned int uint;

	typedef Vec2<double>		Vec2d; ///< float 2-Vectors
	typedef Vec2<float>			Vec2f; ///< float 2-Vectors
	typedef Vec2<int>			Vec2i; ///< signed integer 2-Vectors
	typedef Vec2<uint>			Vec2ui;///< unsigned integer 2-Vectors
	typedef Vec3<double>		Vec3d; ///< float 2-Vectors
	typedef Vec3<float>			Vec3f; ///< float 3-Vectors
	typedef Vec3<int>			Vec3i; ///< signed integer 3-Vectors
	typedef Vec3<uint>			Vec3ui;///< unsigned integer 4-Vectors
	typedef Vec4<double>		Vec4d; ///< float 2-Vectors
	typedef Vec4<float>			Vec4f; ///< float 4-Vectors
	typedef Vec4<int>			Vec4i; ///< signed integer 4-Vectors
	typedef Vec4<uint>			Vec4ui;///< unsigned integer 4-Vectors
	typedef Mat4<double>		Mat4d; ///< float 2-Vectors
	typedef Mat4<float>			Mat4f; ///< float 4x4 matrices
	typedef Quat<double>		Quatd; ///< float 2-Vectors
	typedef Quat<float>			Quatf; ///< float quaternions

};

#endif
