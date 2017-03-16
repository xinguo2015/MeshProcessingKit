#ifndef __xglm_quat_h______________________
#define __xglm_quat_h______________________

#include <math.h>

namespace xglm {

	template <class T> class Vec2;
	template <class T> class Vec3;
	template <class T> class Vec4;
	template <class T> class Mat4;

	////////////////////////////////////////////////////////////////////////////////
	//
	//  Quaternion
	//
	////////////////////////////////////////////////////////////////////////////////

	template< class T>
	class Quat
	{
	public:

		Quat()														{ V4_MAKE(_v,T(0),T(0),T(0),T(0)); }
		Quat( const T v[4] )										{ V4_COPY(_v, v); }
		Quat( T x, T y, T z, T w  )									{ V4_MAKE(_v,x,y,z,w); }
		Quat( const Mat4<T> & m )									{ set( m );  }
		Quat( const Vec3<T> &axis, T radians)						{ set( axis, radians ); }
		Quat( const Vec3<T> &rotateFrom, const Vec3<T> &rotateTo )	{ set( rotateFrom, rotateTo ); }

		Quat & set_value( const T * qp )							{ V4_COPY(_v, qp); return *this; }
		Quat & set_value( T x, T y, T z, T w )						{ V4_MAKE(_v,x,y,z,w); return *this; }
		const T * get_value() const									{ return _v; }

		void get( Vec3<T> &axis, T &radians ) const
		{
			radians = T(2*acos(w));
			if ( radians == T(0.0) )
				axis = Vec3<T>( 0.0, 0.0, 1.0 );
			else
				axis = normalize( Vec3<T>(_v) );
		}

		void get( Mat4<T> & m ) const
		{
			T s, xs, ys, zs, wx, wy, wz, xx, xy, xz, yy, yz, zz;
			T norm = V4_DOT(_v, _v);

			s = ( norm == T(0.0)) ? T(0.0) : ( T(2.0) / norm );

			xs = _v[0] * s;
			ys = _v[1] * s;
			zs = _v[2] * s;

			wx = _v[3] * xs;
			wy = _v[3] * ys;
			wz = _v[3] * zs;

			xx = _v[0] * xs;
			xy = _v[0] * ys;
			xz = _v[0] * zs;

			yy = _v[1] * ys;
			yz = _v[1] * zs;
			zz = _v[2] * zs;

			m(0,0) = T( T(1.0) - ( yy + zz ));
			m(1,0) = T ( xy + wz );
			m(2,0) = T ( xz - wy );

			m(0,1) = T ( xy - wz );
			m(1,1) = T ( T(1.0) - ( xx + zz ));
			m(2,1) = T ( yz + wx );

			m(0,2) = T ( xz + wy );
			m(1,2) = T ( yz - wx );
			m(2,2) = T ( T(1.0) - ( xx + yy ));

			m(3,0) = m(3,1) = m(3,2) = m(0,3) = m(1,3) = m(2,3) = T(0.0);
			m(3,3) = T(1.0);
		}

		Quat & set( const Mat4<T> & m )
		{
			T s, tr = m(0,0) + m(1,1) + m(2,2);

			if ( tr > T(0) )
			{
				s = T(sqrt( tr + m(3,3) ));
				_v[3] = T ( s * 0.5 );
				s = T(0.5) / s;

				_v[0] = T ( ( m(1,2) - m(2,1) ) * s );
				_v[1] = T ( ( m(2,0) - m(0,2) ) * s );
				_v[2] = T ( ( m(0,1) - m(1,0) ) * s );
			}
			else
			{
				int k, j, i = 0;
				if ( m(1,1) > m(0,0) ) i = 1;
				if ( m(2,2) > m(i,i) ) i = 2;

				j = (i+1)%3;
				k = (j+1)%3;

				s = T(sqrt( ( m(i,j) - ( m(j,j) + m(k,k) )) + T(1.0) ));
				_v[i] = T ( s * 0.5 );

				s = T(0.5 / s);
				_v[j] = T ( ( m(i,j) + m(j,i) ) * s );
				_v[k] = T ( ( m(i,k) + m(k,i) ) * s );
				_v[3] = T ( ( m(j,k) - m(k,j) ) * s );
			}

			return *this;
		}

		Quat & set( const Vec3<T> &axis, T radian )
		{
			T len = axis.length();
			if( len == T(0.0) ) // axis too small.
				set_value(0,0,0,1);
			else {
				T s = T( sin(radian/2) / len );
				set_value(axis[0]*s, axis[1]*s, axis[2]*s, T( cos( radian/2 ) ) );
			}
			return *this;
		}

		// inputs are 2 unit vector
		Quat & set( const Vec3<T> & rotateFrom, const Vec3<T> & rotateTo )
		{
			T cc = rotateFrom.dot(rotateTo);
			Vec3<T> a;
			// parallel case
			if( cc>=1 ) set_value(0, 0, 0, 1);
			else if( cc<=-1 ) { // anti parallel
				a = normalize( rotateFrom.orthoVector() );
				set_value(a[0], a[1], a[2], 0);
			}
			else { // general case
				a = normalize( cross(rotateFrom, rotateTo) ) * T( sqrt((1-cc)/2) );
				set_value(a[0], a[1], a[2], T( sqrt((1+cc)/2) ) );
			}
			return *this;
		}
		
		Quat & operator *= ( const Quat<T> & qr ) 
		{
			return set_value(
				w * qr.x + x * qr.w + y * qr.z - z * qr.y,
				w * qr.y + y * qr.w + z * qr.x - x * qr.z,
				w * qr.z + z * qr.w + x * qr.y - y * qr.x,
				w * qr.w - x * qr.x - y * qr.y - z * qr.z);
		}

		//friend Quat<T> normalize( const Quat<T> &q);

		//friend Quat<T> conjugate( const Quat<T> & q);

		//friend Quat<T> inverse( const Quat<T> & q);

		//
		// Quaternion multiplication with cartesian vector
		// v' = q*v*q(star)
		//
		void mult_vec( const Vec3<T> &src, Vec3<T> &dst ) const
		{
			T v_coef = w * w - x * x - y * y - z * z;
			T u_coef = T(2.0) * (src[0] * x + src[1] * y + src[2] * z);
			T c_coef = T(2.0) * w;

			dst._v[0] = v_coef * src._v[0] + u_coef * x + c_coef * (y * src._v[2] - z * src._v[1]);
			dst._v[1] = v_coef * src._v[1] + u_coef * y + c_coef * (z * src._v[0] - x * src._v[2]);
			dst._v[2] = v_coef * src._v[2] + u_coef * z + c_coef * (x * src._v[1] - y * src._v[0]);
		}

		void mult_vec( Vec3<T> & src_and_dst) const
		{
			mult_vec(Vec3<T>(src_and_dst), src_and_dst);
		}

		void scale_angle( T scaleFactor ) {
			Vec3<T> axis;
			T radians;

			get_value(axis, radians);
			radians *= scaleFactor;
			set_value(axis, radians);
		}

		T & operator []( int i ) {
			return _v[i];
		}

		const T & operator []( int i ) const {
			return _v[i];
		}


		friend bool operator == ( const Quat<T> & lhs, const Quat<T> & rhs ) {
			bool r = true;
			for (int i = 0; i < 4; i++)
				r &= lhs._v[i] == rhs._v[i];
			return r;
		}

		friend bool operator != ( const Quat<T> & lhs, const Quat<T> & rhs ) {
			bool r = true;
			for (int i = 0; i < 4; i++)
				r &= lhs._v[i] == rhs._v[i];
			return r;
		}

		friend Quat<T> operator * ( const Quat<T> & lhs, const Quat<T> & rhs ) {
			Quat r(lhs);
			r *= rhs;
			return r;
		}


		union
		{
			struct { T x,y,z,w; };
			T _v[4];
		};
	};

	template<class T>
	inline Quat<T> normalize( const Quat<T> &q) {
		Quat<T> r(q);
		T rnorm = T(1.0) / T(sqrt( q.w * q.w + q.x * q.x + q.y * q.y + q.z * q.z));

		r.x *= rnorm;
		r.y *= rnorm;
		r.z *= rnorm;
		r.w *= rnorm;

		return r;
	}

	template<class T>
	Quat<T> conjugate( const Quat<T> & q) {
		Quat<T> r(q);
		r._v[0] *= T(-1.0);
		r._v[1] *= T(-1.0);
		r._v[2] *= T(-1.0);
		return r;
	}

	template<class T>
	Quat<T> inverse( const Quat<T> & q) {
		return conjugate(q);
	}

	template<class T>
	Quat<T> slerp( const Quat<T> & p, const Quat<T> & q, T alpha ) {
		Quat<T> r;

		T cos_omega = p.x * q.x + p.y * q.y + p.z * q.z + p.w * q.w;
		// if B is on opposite hemisphere from A, use -B instead

		int bflip;
		if ( ( bflip = (cos_omega < T(0))) )
			cos_omega = -cos_omega;

		// complementary interpolation parameter
		T beta = T(1) - alpha;

		if(cos_omega >= T(1))
			return p;

		T omega = T(acos(cos_omega));
		T one_over_sin_omega = T(1.0) / T(sin(omega));

		beta    = T(sin(omega*beta)  * one_over_sin_omega);
		alpha   = T(sin(omega*alpha) * one_over_sin_omega);

		if (bflip)
			alpha = -alpha;

		r.x = beta * p._v[0]+ alpha * q._v[0];
		r.y = beta * p._v[1]+ alpha * q._v[1];
		r.z = beta * p._v[2]+ alpha * q._v[2];
		r.w = beta * p._v[3]+ alpha * q._v[3];
		return r;
	}

};

#endif
