#ifndef __variables_h_________________________
#define __variables_h_________________________

#include "XGLM.h"

namespace xglm {
	
typedef std::vector<int>    IntArray;
typedef std::vector<float>  FloatArray;
typedef std::vector<double> DoubleArray;

typedef std::vector<Vec3i>  V3iArray;
typedef std::vector<Vec3f>  V3fArray;
typedef std::vector<Vec3d>  V3dArray;

                   
class VarValue
{
	char v[sizeof(double)*16];
public:
#define addmethod(a) a  get##a() const { return *(const a*)(const void*)v; } \
                     a& get##a()       { return *(a*)(void*)v; } 
	addmethod(char)
	addmethod(int)
	addmethod(unsigned)
	addmethod(float)
	addmethod(double)
	addmethod(Vec2ui)
	addmethod(Vec2i)
	addmethod(Vec2f)
	addmethod(Vec2d)
	addmethod(Vec3ui)
	addmethod(Vec3i)
	addmethod(Vec3f)
	addmethod(Vec3d)
	addmethod(Vec4ui)
	addmethod(Vec4i)
	addmethod(Vec4f)
	addmethod(Vec4d)
	addmethod(Quatf)
	addmethod(Quatd)
	addmethod(Mat4f)
	addmethod(Mat4d)
#undef addmethod
	const char* getstr() const  { return v; }
	const char* setstr(const char* s)
	{
		if( strlen(s)>sizeof(v)-1 ) {
			xglm_debug("The str is too long to store");
			return NULL;
		}
		strcpy(v,s);
		return v;
	}
};


class VarSet
{
public: 
	      VarValue & operator[](const std::string & key)    { return _str_vs[key]; }
	const VarValue &        get(const std::string & key)    { return _str_vs.at(key); }
	      VarValue & operator[](              int   key)    { return _int_vs[key]; }
	const VarValue &        get(              int   key)    { return _int_vs.at(key); }
	// check variable existence
	bool find(const std::string & key) const { return _str_vs.find(key)!=_str_vs.end(); }
	bool find(int                 key) const { return _int_vs.find(key)!=_int_vs.end(); }
protected:
	std::map<std::string,VarValue>   _str_vs;
	std::map<int,        VarValue>   _int_vs;
};

} //namespace xglm {

#endif //#ifndef __variables_h_________________________