#ifndef __variables_h_________________________
#define __variables_h_________________________

#include "XGLM.h"

#define __define_get_method__(TypeName)  \
    TypeName  get##TypeName() const { return *(const TypeName*)(const void*)v; } \
	TypeName& get##TypeName()       { return *(TypeName*)(void*)v; }


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
	__define_get_method__(char)
	__define_get_method__(int)
	__define_get_method__(unsigned)
	__define_get_method__(float)
	__define_get_method__(double)
	__define_get_method__(Vec2ui)
	__define_get_method__(Vec2i)
	__define_get_method__(Vec2f)
	__define_get_method__(Vec2d)
	__define_get_method__(Vec3ui)
	__define_get_method__(Vec3i)
	__define_get_method__(Vec3f)
	__define_get_method__(Vec3d)
	__define_get_method__(Vec4ui)
	__define_get_method__(Vec4i)
	__define_get_method__(Vec4f)
	__define_get_method__(Vec4d)
	__define_get_method__(Quatf)
	__define_get_method__(Quatd)
	__define_get_method__(Mat4f)
	__define_get_method__(Mat4d)
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
	VarValue & operator [] (const std::string & key)    { return _str_vs[key]; }
	VarValue & operator [] (int   key)                  { return _int_vs[key]; }
	const VarValue &    get(const std::string & key)    { return _str_vs.at(key); }
	const VarValue &    get(int   key)                  { return _int_vs.at(key); }
	// check variable existence
	bool find(const std::string & key) const { return _str_vs.find(key)!=_str_vs.end(); }
	bool find(int                 key) const { return _int_vs.find(key)!=_int_vs.end(); }
protected:
	std::map<std::string,VarValue>   _str_vs;
	std::map<int,        VarValue>   _int_vs;
};

} //namespace xglm {

#endif //#ifndef __variables_h_________________________