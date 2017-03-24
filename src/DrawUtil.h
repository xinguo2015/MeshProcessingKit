#ifndef ____draw_utilities_____________
#define ____draw_utilities_____________

namespace xglm {

template<typename TMesh>
class DrawFace
{
	public:
		static void draw(TMesh& mesh);
};

template<typename TMesh>
class DrawPointID
{
	public:
		static void draw(TMesh& mesh);
};


template<typename TMesh>
class DrawFaceID
{
	public:
		static void draw(TMesh& mesh);
};

} //namespace xglm {

#endif //#ifndef ____draw_utilities_____________
