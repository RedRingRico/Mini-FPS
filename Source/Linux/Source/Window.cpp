#include <Window.hpp>
#include <GL/glx.h>
#include <GL/glxext.h>
#include <unistd.h>
#include <iostream>

namespace MiniFPS
{
	struct WINDOW_DATA
	{
		::Window	X11Window;
		Display		*X11Display;
		GLXContext	X11GLXContext;
	};

	Window::Window( ) :
		m_pWindowData( nullptr )
	{
	}

	Window::~Window( )
	{
		this->Destroy( );
	}

	int Window::Create( const RECT &p_Rect, const RENDER_SURFACE &p_Surface )
	{
		if( m_pWindowData )
		{
			delete m_pWindowData;
			m_pWindowData = nullptr;
		}

		m_pWindowData = new WINDOW_DATA( );

		// For now, just use the default window
		m_pWindowData->X11Display = XOpenDisplay( NULL );

		if( !m_pWindowData->X11Display )
		{
			return 1;
		}

		int GLXMajor, GLXMinor;

		if( !glXQueryVersion( m_pWindowData->X11Display, &GLXMajor, &GLXMinor ) ||
			( ( GLXMajor == 1 ) && GLXMinor < 3 ) || ( GLXMajor < 1 ) )
		{
			return 1;
		}

		int VisualAttributes[ ] =
		{
			GLX_X_RENDERABLE,	True,
			GLX_DRAWABLE_TYPE,	GLX_WINDOW_BIT,
			GLX_RENDER_TYPE,	GLX_RGBA_BIT,
			GLX_X_VISUAL_TYPE,	GLX_TRUE_COLOR,
			GLX_RED_SIZE,		p_Surface.RedSize,
			GLX_GREEN_SIZE,		p_Surface.GreenSize,
			GLX_BLUE_SIZE,		p_Surface.BlueSize,
			GLX_ALPHA_SIZE,		p_Surface.AlphaSize,
			GLX_DEPTH_SIZE,		p_Surface.DepthSize,
			GLX_STENCIL_SIZE,	p_Surface.StencilSize,
			GLX_DOUBLEBUFFER,	p_Surface.BufferCount > 1 ? True : False,
			None
		};

		int FBCount;
		GLXFBConfig *pFBConfigs = glXChooseFBConfig( m_pWindowData->X11Display,
			DefaultScreen( m_pWindowData->X11Display ), VisualAttributes, &FBCount );

		if( !pFBConfigs )
		{
			return 1;
		}

		// For now, use the first config found
		GLXFBConfig FBC = pFBConfigs[ 0 ];

		XFree( pFBConfigs );

		XVisualInfo *pVisualInfo = glXGetVisualFromFBConfig(
			m_pWindowData->X11Display, FBC );

		XSetWindowAttributes WindowAttribs;

		WindowAttribs.colormap = XCreateColormap( m_pWindowData->X11Display,
			RootWindow( m_pWindowData->X11Display, pVisualInfo->screen ),
			pVisualInfo->visual, AllocNone );
		WindowAttribs.background_pixmap = None;
		WindowAttribs.border_pixel = 0;
		WindowAttribs.event_mask = StructureNotifyMask;

		m_pWindowData->X11Window = XCreateWindow( m_pWindowData->X11Display,
			RootWindow( m_pWindowData->X11Display, pVisualInfo->screen ),
			p_Rect.X, p_Rect.Y, p_Rect.Width, p_Rect.Height, 0,
			pVisualInfo->depth, InputOutput, pVisualInfo->visual,
			CWBorderPixel | CWColormap | CWEventMask, &WindowAttribs );
		
		XFree( pVisualInfo );
		
		if( !m_pWindowData->X11Window )
		{
			return 1;
		}

		XMapWindow( m_pWindowData->X11Display, m_pWindowData->X11Window );

		// Having the function be right here isn't the best option, this should
		// be handled later via OpenGL extension handling
		PFNGLXCREATECONTEXTATTRIBSARBPROC glXCreateContextAttribsARB = nullptr;

		glXCreateContextAttribsARB = ( PFNGLXCREATECONTEXTATTRIBSARBPROC )
			glXGetProcAddressARB(
				( const GLubyte * )"glXCreateContextAttribsARB" );

		if( !glXCreateContextAttribsARB )
		{
			return 1;
		}

		// Only OpenGL 3.2 capable GPUs and above will be considered
		int ContextAttribs [ ] =
		{
			GLX_CONTEXT_MAJOR_VERSION_ARB,	3,
			GLX_CONTEXT_MINOR_VERSION_ARB,	2,
			None
		};

		m_pWindowData->X11GLXContext = glXCreateContextAttribsARB(
			m_pWindowData->X11Display, FBC, 0, True, ContextAttribs );

		if( !m_pWindowData->X11GLXContext )
		{
			return 1;
		}

		if( !glXIsDirect( m_pWindowData->X11Display,
			m_pWindowData->X11GLXContext ) )
		{
			return 1;
		}

		glXMakeCurrent( m_pWindowData->X11Display, m_pWindowData->X11Window,
			m_pWindowData->X11GLXContext );

		glClearColor( 1.0f, 1.0f, 0.0f, 1.0f );
		glClear( GL_COLOR_BUFFER_BIT );
		// On the test machine, the buffers had to be swapped three times to
		// show a difference in the render (unknown as to why)
		glXSwapBuffers( m_pWindowData->X11Display, m_pWindowData->X11Window );
		glXSwapBuffers( m_pWindowData->X11Display, m_pWindowData->X11Window );
		glXSwapBuffers( m_pWindowData->X11Display, m_pWindowData->X11Window );

		// There's no input loop, yet, so just sleep and exit
		sleep( 1 );

		return 0;
	}

	void Window::Destroy( )
	{
		if( m_pWindowData->X11Display && m_pWindowData->X11GLXContext )
		{
			glXMakeCurrent( m_pWindowData->X11Display, 0, 0 );
			glXDestroyContext( m_pWindowData->X11Display,
				m_pWindowData->X11GLXContext );
		}
		if( m_pWindowData->X11Display && m_pWindowData->X11Window )
		{
			XDestroyWindow( m_pWindowData->X11Display,
				m_pWindowData->X11Window );
			XCloseDisplay( m_pWindowData->X11Display );
		}

		if( m_pWindowData )
		{
			delete m_pWindowData;
			m_pWindowData = nullptr;
		}
	}

	int Window::GetWindowData( struct WINDOW_DATA *p_pWindowData )
	{
		return 0;
	}
}

