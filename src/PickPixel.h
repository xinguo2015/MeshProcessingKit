#ifndef ____pick_pixel_h_____________
#define ____pick_pixel_h_____________

#include <vector>

using namespace std;

namespace xglm {

class PixelInfo
{
public:
	PixelInfo()											:idx(0){}
	PixelInfo(unsigned int index)						:idx(index){}
	operator unsigned int () const 						{ return idx; }
	operator unsigned int*()                            { return &idx; }
	operator unsigned char*()                           { return (unsigned char *)&idx; }
	void pack(unsigned int ptype, unsigned int index)   { idx = (index << MaskSize) | (ptype & MaskBits); }
	unsigned int getType() const						{ return idx & MaskBits; }
	unsigned int getIndex()	const						{ return idx >> MaskSize;   }
public:
	union {
		unsigned int idx;
		struct{
			unsigned char r,g,b,a;
		};
	};
private:
	enum { 
		MaskSize = 4, 
		MaskBits = ((1<<MaskSize)-1)
	};
};

class PickBuffer
{
public:
	PickBuffer() {
		mDirty = true;
		mSize[0] = mSize[1] = 0;
	}
	bool contain(int x, int y) const    { return (x>=0 && x<mSize[0] && y>=0 && y<mSize[1]); }
	bool isDirty() const				{ return mDirty; }
	void markDirty(int dirtyFlag)       { mDirty = dirtyFlag; }
	void resize(int width, int height)  { mSize[0] = width; mSize[1] = height; mBuffer.resize(width*height); }
	PixelInfo*getBuf()                  { return & mBuffer.front(); }
	PixelInfo getAt(int x, int y) 		{ return mBuffer[x+y*mSize[0]]; }
	
public:
	bool				mDirty;
	int					mSize[2];
	vector<PixelInfo>	mBuffer;
};

}
#endif // #ifndef ____pick_pixel_h_____________