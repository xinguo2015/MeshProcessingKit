#ifndef ___xglm_vec_h_________________
#define ___xglm_vec_h_________________

#include <math.h>

#include "vmacros.h"

namespace xglm {

	template <class T> class Vec2;
	template <class T> class Vec3;
	template <class T> class Vec4;

	/// Template class for 2-tuple vector.
	template <class T>
	class Vec2 {

	public:
		typedef T value_type;
		static int dim()					{ return 2;}

		/// Default/scalar constructor
		Vec2(const T & t = T())				{ V2_MAKE(_v, t, t); }
		/// Construct from array
		Vec2(const T * tp)					{ V2_COPY(_v, tp); }
		/// Construct from explicit values
		Vec2( const T vx, const T vy)		{ V2_MAKE(_v, vx, vy); }
		/// "Cropping" explicit constructor from Vec3 to Vec2
		explicit Vec2( const Vec3<T> & u)	{ V2_COPY(_v, u._v); }
		/// "Cropping" explicit constructor from Vec4 to Vec2
		explicit Vec2( const Vec4<T> &u)	{ V2_COPY(_v, u._v); }

		const T * get_value() const			{ return _v; }
		Vec2<T> & set_value( const T * rhs ){ V2_COPY(_v, rhs); }

		// indexing operators
		T & operator [] ( int i )				{ return _v[i]; }
		const T & operator [] ( int i ) const	{ return _v[i]; }
		// type-cast operators
		operator T * ()						{ return _v; }
		operator const T * () const			{ return _v; }

		////////////////////////////////////////////////////////
		// Math
		////////////////////////////////////////////////////////
		// dot product
		T dot(const Vec2<T> & b) const										{  return V2_DOT(_v,b._v); }
		// length
		T length() const													{  return T(sqrt(this->dot(*this))); }
		T square_length() const												{  return this->dot(*this); }
		// scalar multiply assign
		friend Vec2<T> & operator *= ( Vec2<T> &a, T s )					{ V2_MULs(a,a,s); return a; }
		// component-wise vector multiply assign
		friend Vec2<T> & operator *= ( Vec2<T> &a, const Vec2<T> &b )		{ V2_MUL(a,a,b); return a; }
		// scalar divide assign
		friend Vec2<T> & operator /= ( Vec2<T> &a, T d )					{ if(d != 0 ) V2_DIVs(a,a,d); return a; }
		// component-wise vector add assign
		friend Vec2<T> & operator += ( Vec2<T> &a, const Vec2<T> & b )		{ V2_ADD(a,a,b); return a; }
		// component-wise vector sub assign
		friend Vec2<T> & operator -= ( Vec2<T> &a, const Vec2<T> & b )		{ V2_SUB(a,a,b); return a; }
		// unary negate
		friend Vec2<T> operator - ( const Vec2<T> &a)						{ return Vec2<T>(-a[0], -a[1]); }
		// vector add
		friend Vec2<T> operator + ( const Vec2<T> & a, const Vec2<T> & b)	{ Vec2<T> r; V2_ADD(r,a,b); return r; }
		// vector subtract
		friend Vec2<T> operator - ( const Vec2<T> & a, const Vec2<T> & b)	{ Vec2<T> r; V2_SUB(r,a,b); return r; }
		// scalar multiply
		friend Vec2<T> operator * ( const Vec2<T> & a, T s)					{  Vec2<T> r; V2_MULs(r,a,s); return r; }
		friend Vec2<T> operator * ( T s, const Vec2<T> & a)					{  Vec2<T> r; V2_MULs(r,a,s); return r; }
		// vector component-wise multiply
		friend Vec2<T> operator * ( const Vec2<T> & a, const Vec2<T> & b)	{  Vec2<T> r; V2_MUL(r,a,b); return r; }
		// scalar division
		friend Vec2<T> operator / ( const Vec2<T> & a, T s)					{  Vec2<T> r; V2_DIVs(r,a,s); return r; }
		// vector component-wise division
		friend Vec2<T> operator / ( const Vec2<T> & a, const Vec2<T> & b)	{  Vec2<T> r; V2_DIV(r,a,b); return r; }
		// normalize
		friend Vec2<T> normalize ( const Vec2<T> & a )						{  T len = a.length(); return( len==0 ? a : a/len ); }
		// min/max
		friend Vec2<T> min( const Vec2<T> & a, const Vec2<T> & b)			{ Vec2<T>r; V2_MIN(r,a,b); return r; }
		friend Vec2<T> max( const Vec2<T> & a, const Vec2<T> & b)			{ Vec2<T>r; V2_MAX(r,a,b); return r; }
		// equality
		friend bool operator == ( const Vec2<T> &a, const Vec2<T> &b )		{ return V2_SAME(a,b); }
		// inequality
		friend bool operator != ( const Vec2<T> &a, const Vec2<T> &b )		{ return ! V2_SAME(a,b); }

	public:
		//data intentionally left public to allow Vec2.x
		union {
			struct { T x,y; };  // standard names for components
			struct { T s,t; };  // standard names for components
			T _v[2];			// array access
		};
	};

	//////////////////////////////////////////////////////////////////////
	//
	// Vec3 - template class for 3-tuple vector
	//
	//////////////////////////////////////////////////////////////////////
	template <class T>
	class Vec3 {

	public:
		typedef T value_type;
		static int dim()							{ return 3;}

		// Default/scalar constructor
		Vec3(const T & t = T())						{ V3_MAKE(_v,t,t,t); }
		// Construct from array
		Vec3(const T * tp)							{ V3_COPY(_v,tp); }
		// Construct from explicit values
		Vec3( const T vx, const T vy , const T vz)	{ V3_MAKE(_v, vx, vy, vz); }
		explicit Vec3( const Vec4<T> &u)			{ V3_COPY(_v, u._v); }
		explicit Vec3( const Vec2<T> &u, T vz)      { V3_MAKE(_v, u.x, u.y, vz); }

		const T * get_value() const					{ return _v; }
		Vec3<T> & set_value( const T * a )			{ V3_COPY(_v, a); return (*this); }

		// indexing operators
		T & operator [] ( int i )					{ return _v[i]; }
		const T & operator [] ( int i ) const		{ return _v[i]; }

		// type-cast operators
		operator T * ()								{ return _v; }
		operator const T * () const					{ return _v; }

		////////////////////////////////////////////////////////
		// Math
		////////////////////////////////////////////////////////
		// dot product
		T dot(const Vec3<T> & b) const										{  return V3_DOT(_v,b._v); }
		// length
		T length() const													{  return T(sqrt(this->dot(*this))); }
		T square_length() const												{  return this->dot(*this); }
		// cross product
		Vec3<T> cross(const Vec3<T>& a) const
		{
			return Vec3<T>(
				((_v[1] * a[2]) - (_v[2] * a[1])),
				((_v[2] * a[0]) - (_v[0] * a[2])),
				((_v[0] * a[1]) - (_v[1] * a[0])));
		}
		// axis rotate
		void rotate(const Vec3<T>& axis, float angle)
		{
			Vec3<T> a, u, v, p;
			a = normalize(axis);
			p = this->dot(a) * a;
			u = (*this) - p;
			v = a.cross(u);
			*this = p + (float)cos((double)angle)*u + (float)sin((double)angle)*v;
		}
		// an orthogonal vector
		Vec3<T> orthoVector() const
		{
			if( ABS(x)<ABS(y) && ABS(x)<ABS(z) )
				return Vec3<T>(0,z,-y);
			if( ABS(y) <ABS(z) )
				return Vec3<T>(z,0,-x);
			return Vec3<T>(y,-x,0);
		}
		Vec3<T>& min(const Vec3<T>& p)										{ V3_MIN(_v,_v,p._v); }
		Vec3<T>& max(const Vec3<T>& p)										{ V3_MAX(_v,_v,p._v); }

		// scalar multiply assign
		friend Vec3<T> & operator *= ( Vec3<T> &a, T s )					{ V3_MULs(a,a,s); return a; }
		// component-wise vector multiply assign
		friend Vec3<T> & operator *= ( Vec3<T> &a, const Vec3<T> &b )		{ V3_MUL(a,a,b); return a; }
		// scalar divide assign
		friend Vec3<T> & operator /= ( Vec3<T> &a, T d )					{ if(d != 0 ) V3_DIVs(a,a,d); return a; }
		// component-wise vector add assign
		friend Vec3<T> & operator += ( Vec3<T> &a, const Vec3<T> & b )		{ V3_ADD(a,a,b); return a; }
		// component-wise vector sub assign
		friend Vec3<T> & operator -= ( Vec3<T> &a, const Vec3<T> & b )		{ V3_SUB(a,a,b); return a; }
		// unary negate
		friend Vec3<T> operator - ( const Vec3<T> &a)						{ return Vec3<T>(-a[0], -a[1], -a[2]); }
		// vector add
		friend Vec3<T> operator + ( const Vec3<T> & a, const Vec3<T> & b)	{ Vec3<T> r; V3_ADD(r,a,b); return r; }
		// vector subtract
		friend Vec3<T> operator - ( const Vec3<T> & a, const Vec3<T> & b)	{ Vec3<T> r; V3_SUB(r,a,b); return r; }
		// scalar multiply
		friend Vec3<T> operator * ( const Vec3<T> & a, T s)					{  Vec3<T> r; V3_MULs(r,a,s); return r; }
		friend Vec3<T> operator * ( T s, const Vec3<T> & a)					{  Vec3<T> r; V3_MULs(r,a,s); return r; }
		// vector component-wise multiply
		friend Vec3<T> operator * ( const Vec3<T> & a, const Vec3<T> & b)	{  Vec3<T> r; V3_MUL(r,a,b); return r; }
		// scalar division
		friend Vec3<T> operator / ( const Vec3<T> & a, T s)					{  Vec3<T> r; V3_DIVs(r,a,s); return r; }
		// vector component-wise division
		friend Vec3<T> operator / ( const Vec3<T> & a, const Vec3<T> & b)	{  Vec3<T> r; V3_DIV(r,a,b); return r; }
		// cross product
		friend Vec3<T> cross( const Vec3<T> & a, const Vec3<T> & b)			{ return a.cross(b); }
		// normalize
		friend Vec3<T>  normalize ( const Vec3<T> & a )						{  T len = a.length(); return( len==0 ? a : a/len ); }
		Vec3<T>&        normalize ()										{  T len = length(); if(len>0) *this /= len; return(*this); }
		// min/max
		friend Vec3<T>  min( const Vec3<T> & a, const Vec3<T> & b)			{ Vec3<T>r; V3_MIN(r,a,b); return r; }
		friend Vec3<T>  max( const Vec3<T> & a, const Vec3<T> & b)			{ Vec3<T>r; V3_MAX(r,a,b); return r; }
		// equality
		friend bool operator == ( const Vec3<T> &a, const Vec3<T> &b )		{ return V3_SAME(a,b); }
		// inequality
		friend bool operator != ( const Vec3<T> &a, const Vec3<T> &b )		{ return ! V3_SAME(a,b); }

	public:
		//data intentionally left public to allow Vec2.x
		union {
			struct { T x,y,z; };  // standard names for components
			struct { T s,t,r; };  // standard names for components
			T _v[3];			  // array access
		};
	};

	//////////////////////////////////////////////////////////////////////
	//
	// Vec4 - template class for 4-tuple vector
	//
	//////////////////////////////////////////////////////////////////////
	template <class T>
	class Vec4 {

	public:
		typedef T value_type;
		static int dim()										{ return 4;}

		// Default/scalar constructor
		Vec4(const T & t = T())									{ V4_MAKE(_v,t,t,t,t); }
		// Construct from array
		Vec4(const T * tp)										{ V4_COPY(_v,tp); }
		// Construct from explicit values
		Vec4( const T vx, const T vy , const T vz, const T vw)	{ V4_MAKE(_v, vx, vy, vz, vw); }
		explicit Vec4( const Vec3<T> &u, T vw)					{ V4_MAKE(_v, u.x, u.y, u.z, vw); }
		// array access
		const T * get_value() const								{ return _v; }
		Vec4<T> & set_value( const T * a )						{ V4_COPY(_v, a); return (*this); }
		// indexing operators
		T & operator [] ( int i )								{ return _v[i]; }
		const T & operator [] ( int i ) const					{ return _v[i]; }
		// type-cast operators
		operator T * ()											{ return _v; }
		operator const T * () const								{ return _v; }

		////////////////////////////////////////////////////////
		// algebra
		////////////////////////////////////////////////////////
		// dot product
		T dot(const Vec4<T> & b) const										{  return V4_DOT(_v,b._v); }
		// length
		T length() const													{  return T(sqrt(this->dot(*this))); }
		T square_length() const												{  return this->dot(*this); }
		// scalar multiply assign
		friend Vec4<T> & operator *= ( Vec4<T> &a, T s )					{ V4_MULs(a,a,s); return a; }
		// component-wise vector multiply assign
		friend Vec4<T> & operator *= ( Vec4<T> &a, const Vec4<T> &b )		{ V4_MUL(a,a,b); return a; }
		// scalar divide assign
		friend Vec4<T> & operator /= ( Vec4<T> &a, T d )					{ if(d != 0 ) V4_DIVs(a,a,d); return a; }
		// component-wise vector add assign
		friend Vec4<T> & operator += ( Vec4<T> &a, const Vec4<T> & b )		{ V4_ADD(a,a,b); return a; }
		// component-wise vector sub assign
		friend Vec4<T> & operator -= ( Vec4<T> &a, const Vec4<T> & b )		{ V4_SUB(a,a,b); return a; }
		// unary negate
		friend Vec4<T> operator - ( const Vec4<T> &a)						{ return Vec4<T>(-a[0], -a[1], -a[2], -a[3]); }
		// vector add
		friend Vec4<T> operator + ( const Vec4<T> & a, const Vec4<T> & b)	{ Vec4<T> r; V4_ADD(r,a,b); return r; }
		// vector subtract
		friend Vec4<T> operator - ( const Vec4<T> & a, const Vec4<T> & b)	{ Vec4<T> r; V4_SUB(r,a,b); return r; }
		// scalar multiply
		friend Vec4<T> operator * ( const Vec4<T> & a, T s)					{  Vec4<T> r; V4_MULs(r,a,s); return r; }
		friend Vec4<T> operator * ( T s, const Vec4<T> & a)					{  Vec4<T> r; V4_MULs(r,a,s); return r; }
		// vector component-wise multiply
		friend Vec4<T> operator * ( const Vec4<T> & a, const Vec4<T> & b)	{  Vec4<T> r; V4_MUL(r,a,b); return r; }
		// scalar division
		friend Vec4<T> operator / ( const Vec4<T> & a, T s)					{  Vec4<T> r; V4_DIVs(r,a,s); return r; }
		// vector component-wise division
		friend Vec4<T> operator / ( const Vec4<T> & a, const Vec4<T> & b)	{  Vec4<T> r; V4_DIV(r,a,b); return r; }
		// normalize
		friend Vec4<T> normalize ( const Vec4<T> & a )						{  T len = a.length(); return( len==0 ? a : a/len ); }
		// min/max
		friend Vec4<T> min( const Vec4<T> & a, const Vec4<T> & b)			{ Vec4<T>r; V4_MIN(r,a,b); return r; }
		friend Vec4<T> max( const Vec4<T> & a, const Vec4<T> & b)			{ Vec4<T>r; V4_MAX(r,a,b); return r; }
		// equality
		friend bool operator == ( const Vec4<T> &a, const Vec4<T> &b )		{ return V4_SAME(a,b); }
		// inequality
		friend bool operator != ( const Vec4<T> &a, const Vec4<T> &b )		{ return ! V4_SAME(a,b); }

		//data intentionally left public to allow Vec2.x
		union {
			struct { T x,y,z,w; };  // standard names for components
			struct { T s,t,r,q; };  // standard names for components
			T _v[4];				// array access
		};
	};
};

#endif
