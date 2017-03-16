#ifndef ___xglm_matrix_h___________
#define ___xglm_matrix_h___________

#include <math.h>
#include <memory.h>

namespace xglm {

	template <class T> class Vec2;
	template <class T> class Vec3;
	template <class T> class Vec4;

	////////////////////////////////////////////////////////////////////////////////
	//
	//  Matrix
	//
	////////////////////////////////////////////////////////////////////////////////
	template<class T>
	class Mat4
	{
	public:
		typedef T value_type;
	public:

		Mat4()							{ make_identity(); }
		Mat4( T t )						{ set_value(t); }
		Mat4( const T * m )				{ set_value(m); }
		Mat4( const Mat4<T> & m )		{ set_value(m._array); }

		operator       T * ()			{ return _array; }
		operator const T * () const		{ return _array; }
		void set_value( const T t)		{ for(int i=0; i<16; i++ ) _array[i]=t; }
		void set_value( const T * mp)	{ memcpy(_array, mp, sizeof(T)*16); }
		void get_value( T * mp ) const	{ memcpy(mp, _array, sizeof(T)*16); }

		// set a uniform scale
		void set_scale(                T    s )			{ elem(0,0) = elem(1,1) = elem(2,2) = s; }
		void set_scale(     const Vec3<T> & s )			{ elem(0,0) = s[0]; elem(1,1) = s[1]; elem(2,2) = s[2]; }
		void set_translate( const Vec3<T> & t ) 		{ elem(0,3) = t[0]; elem(1,3) = t[1]; elem(2,3) = t[2]; }

		Vec4<T> &         col(int col)					{ return *(Vec4<T>*)&_array[col<<2]; }
		const Vec4<T> &   col(int col)          const	{ return *(Vec4<T>*)&_array[col<<2]; }
		T       &        elem(int row, int col)			{ return _array[row | (col<<2)]; }
		const T &        elem(int row, int col) const	{ return _array[row | (col<<2)]; }
		T       & operator ()(int row, int col)			{ return elem(row,col); }
		const T & operator ()(int row, int col) const	{ return elem(row,col); }
		
		void make_zero()								{ memset(_array, 0, sizeof(T)*16); }
		void make_identity()							{ make_zero(); elem(0,0) = elem(1,1) = elem(2,2) = elem(3,3) = 1.0; }
		Mat4<T>& rotationZ(const T radian )
		{
			T cosa = cos(radian);
			T sina = sin(radian); 
			make_identity();
			elem(0,0) =  cosa; elem(0,1) = -sina;
			elem(1,0) =  sina; elem(1,1) =  cosa;
			return (*this);
		}
		Mat4<T>& rotationX(const T radian )
		{
			T cosa = cos(radian);
			T sina = sin(radian);
			make_identity();
			elem(1,1) = cosa; elem(1,2) = -sina;
			elem(2,1) = sina; elem(2,2) =  cosa;
			return (*this);
		}
		Mat4<T>& rotationY( const T radian )
		{
			T cosa = cos(radian);
			T sina = sin(radian);
			make_identity();
			elem(2,2) = cosa; elem(2,0) = -sina; 
			elem(0,2) = sina; elem(0,0) =  cosa; 
			return (*this);
		}
		//
		// Rotation matrix creation
		// From euler radians
		//      1/ Yaw around Y axis in radians
		//      2/ Pitch around X axis in radians
		//      3/ Roll around Z axis in radians
		//
		//   result: [R] = [Roll].[Pitch].[Yaw]
		//
		Mat4<T>& rotationYawPitchRoll(const T yaw , const T pitch , const T roll )
		{
			Mat4<T> m;
			if (roll )	 rotationZ(roll);
			else	     make_identity();
			if ( pitch ) (*this) *= m.rotationX(pitch);
			if ( yaw )	 (*this) *= m.rotationY(yaw);
			return (*this);
		}
		Mat4<T>& translation( const T tx , const T ty , const T tz )
		{
			make_identity();
			elem(0,3) = tx;
			elem(1,3) = ty;
			elem(2,3) = tz;
			return (*this);
		}
		Mat4<T>& translation( const Vec3<T> & t )
		{
			make_identity();
			elem(0,3) = t[0];
			elem(1,3) = t[1];
			elem(2,3) = t[2];
			return (*this);
		}
		//
		// Look At matrix creation
		Mat4<T> & lookAt(const Vec3<T>& eye, const Vec3<T>& center, const Vec3<T>& up)
		{
			Vec3<T> z = normalize( eye - center );
			Vec3<T> x = normalize( cross(up,z) );
			Vec3<T> y = cross( z, x );
			// rotation part
			for( int c = 0; c<3; c++ )
				col(c) = Vec4<T>( x[c], y[c], z[c], 0);
			// translation part
			col(3) = Vec4<T>(-dot(x, eye), -dot(y, eye), -dot(z, eye),T(1));
			return (*this);
		}

		Mat4<T> &    transpose(Mat4<T> &m) const		{ for( int r=0; r<4; r++ ) for (int c=0; c<4; c++ ) m(r,c)=elem(c,r); return m; }
		Mat4<T>      transpose()           const		{ Mat4<T> m; return transpose(m); }
		Mat4<T> & operator *= ( const T & r )			{ for( int i = 0; i < 16; ++i) _array[i] *= r;				return *this; }
		Mat4<T> & operator += ( const Mat4<T> & mat )	{ for( int i = 0; i < 16; ++i) _array[i] += mat._array[i];	return *this; }
		Mat4<T> & operator *= ( const Mat4<T> & B ) 
		{
			for( int r=0; r<4; r++ ) {
				Vec4<T> row_r = Vec4<T>(elem(r,0),elem(r,1),elem(r,2),elem(r,3)); // row(r)
				for (int c=0; c<4; c++ ) 
					elem(r,c) = dot(row_r, B.col(c));
			}
			return *this;
		}

		// A * B // matrix multiplication
		friend Mat4<T> operator * ( const Mat4<T> & A, const Mat4<T> & B )  { Mat4<T> M = A; return A *= B; }
		// r = v * M
		friend Vec4<T> operator *( const Vec4<T> &v, const Mat4<T> &M) 
		{
			return Vec4<T>(dot(v,M.col(0)), dot(v,M.col(1)), dot(v,M.col(2)), dot(v,M.col(3)));
		}
		// r = M * v
		friend Vec4<T> operator * ( const Mat4<T> & M, const Vec4<T> &v) 
		{
			Vec4<T> r = M.col(0)*v[0]; r += M.col(1)*v[1]; r += M.col(2)*v[2]; r += M.col(3)*v[3];
			return r; // return v * M.transpose();
		}
		// equality
		friend bool operator == ( const Mat4 & lhs, const Mat4 & rhs )  { for (int i = 0; i < 16; i++) if( lhs._array[i] != rhs._array[i] ) return false; return true; }
		friend bool operator != ( const Mat4 & lhs, const Mat4 & rhs )  { return !(lhs==rhs); }

	protected:
		// column-wise array, OpenGL matrix format
		T _array[16]; // translation: [12,13,14,15]
	};

};

#endif
