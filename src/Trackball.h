#ifndef __glut_manipulators_h_________________
#define __glut_manipulators_h_________________

#include "Math3D/math.h"
#include "MyGLUT.h" 

#ifdef _WIN
#pragma warning(disable:4244)
#endif

namespace xglm {

	inline void applyRotation(const Quatf & r)
	{
		float angle; Vec3f axis;
		r.get(axis, angle);
		glRotatef( angle/3.1415926f * 180.0f, axis[0], axis[1], axis[2]);
	}


	//////////////////////////////////////////////////////////////////////
	//  Base manipulator class

	class Manipulator
	{
		public:

			enum Button {
				ENoButton     = 0x0,
				ELeftButton   = 0x1,
				EMiddleButton = 0x2,
				ERightButton  = 0x4
			};

			enum Modifier {
				ENoModifier      = 0x0,
				EShiftModifier   = 0x1,
				EControlModifier = 0x2,
				EAltModifier     = 0x4
			};

			static Button convertButtonFromGlut( int button) 
			{
				switch (button) 
				{
					case GLUT_LEFT_BUTTON:		return ELeftButton;
					case GLUT_MIDDLE_BUTTON:	return EMiddleButton;
					case GLUT_RIGHT_BUTTON:		return ERightButton;
				}
				return ENoButton;
			}
			// modifier = glutGetModifiers()
			static Modifier convertModifierFromGlut( int modifier) 
			{
				int val = (int)ENoModifier;
				//need to use a bitwise or, because it could be multiple keys
				if (modifier & GLUT_ACTIVE_ALT)
					val |= EAltModifier;
				if (modifier & GLUT_ACTIVE_CTRL)
					val |= EControlModifier;
				if (modifier & GLUT_ACTIVE_SHIFT)
					val |= EShiftModifier;
				return (Modifier)val;
			}

		public:
			Manipulator()									: _width(0), _height(0) {}
			virtual ~Manipulator()							{}

		public:
			virtual void reshape( int width, int height)	{ _width = width; _height = height; }
			virtual void mouse( Button button, Modifier mod, bool pressDown, int x, int y) = 0;
			virtual void motion( int x, int y)	= 0;
			virtual void idle()					= 0;
			virtual void update()				= 0;
			virtual void applyTransform()		= 0;
			virtual Mat4f getTransform()		= 0;

		protected:
			int _width, _height;
	};

	//
	// Trackball Manipulator
	//
	//////////////////////////////////////////////////////////////////////
	class Trackball : public Manipulator 
	{
		public:
			Trackball()
			{
				_active = false;
				_activateButton = ELeftButton;
				_activateModifiers = ENoModifier;
				_qRot = Quatf(0.0f, 0.0f, 0.0f, 1.0f);
				_qDrag = Quatf(0.0f, 0.0f, 0.0f, 1.0f);
				_ballSize = 100;
				_ballCen = Vec2f(50,50);
			}
			virtual void reshape( int width, int height)
			{
				Manipulator::reshape(width,height);
				_ballSize = float ( width<height ? width : height ) /3;
				_ballCen = Vec2f(width/2, height/2);
				_active = false;
			}
			virtual void mouse( Button button, Modifier mod, bool pressDown, int x, int y) 
			{
				//check to see if this click changes the activation state
				if  ( pressDown  && button == _activateButton && 
						( mod == _activateModifiers || _activateModifiers == ENoModifier ) ) 
				{
					_active = true;
					_pNow = _pOld = Vec2f(x,y);
				}
				else if ( !pressDown && button == _activateButton ) 
				{
					_active = false;
				}
			}
			bool isActive() const { return _active; }
			virtual void motion( int x, int y) 
			{
				if ( !_active) return;
				_pNow = Vec2f(x,y);
				update();
			}
			virtual void idle() 
			{
				_qRot = _qDrag * _qRot;
			}

			static Vec3f makeBallPoint(const Vec2f &p)
			{
				float L2 = p.dot(p);
				Vec3f b = Vec3f(p, 0);
				if( L2<1 ) b.z = sqrtf(1-L2);
				else       b   = normalize(b);
				b.y = -b.y;
				return b;
			}

			virtual void update() 
			{
				if( _pNow==_pOld ) 
					return;
				Vec3f a = makeBallPoint( (_pOld-_ballCen)/_ballSize );
				Vec3f b = makeBallPoint( (_pNow-_ballCen)/_ballSize );
				_qDrag.set(a, b);
				_qRot = _qDrag*_qRot;
				_pOld = _pNow;
			}
			void applyTransform() 
			{
				glTranslatef( _rotCen[0], _rotCen[1], _rotCen[2] );
				applyRotation( _qRot );
				glTranslatef( -_rotCen[0], -_rotCen[1], -_rotCen[2] );
			}
			Mat4f getTransform()
			{
				Mat4f r;
				return r;
			}
			void setTrackballActivate( Button b, Modifier m = ENoModifier) 
			{
				_activateButton = b;
				_activateModifiers = m;
			}
			Vec3f   getCenterOfRotation( ) const			{ return _rotCen; }
			void    setCenterOfRotation( const Vec3f& c)	{ _rotCen = c; }
			Quatf & getRotation()							{ return _qRot; }
			Quatf   getRotation() const						{ return _qRot; }

		protected:
			// UI commands that this trackball responds to
			// defaults to left mouse button with no modifier key
			Button   _activateButton;
			Modifier _activateModifiers;
			// if the manipulator is presently tracking the mouse
			bool     _active; 
			// mouse data
			Vec2f _pNow;	// current mouse point
			Vec2f _pOld;	// previous mouse point
			Quatf _qDrag;   // rotation of last drag
			// track ball
			float _ballSize;// size of the ball
			Vec2f _ballCen; // center of the ball
			Vec3f _rotCen;  // center for rotation
			Quatf _qRot;    // total rotation
	};

}
#endif
