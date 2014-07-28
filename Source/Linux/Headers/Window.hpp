#ifndef __MINIFPS_WINDOW_HPP__
#define __MINIFPS_WINDOW_HPP__

#include <X11/Xlib.h>
#include <RenderTypes.hpp>
#include <GL/glx.h>

namespace MiniFPS
{
	class Window
	{
	public:
		Window( );
		~Window( );

		int Create( const RECT &p_Rect, const RENDER_SURFACE &p_Surface );

		void Destroy( );

		int GetWindowData( struct WINDOW_DATA *p_pWindowData );

	private:
		struct WINDOW_DATA	*m_pWindowData;
	};
}

#endif // __MINIFPS_WINDOW_HPP__

