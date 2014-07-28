#ifndef __MINIFPS_RENDERTYPES_HPP__
#define __MINIFPS_RENDERTYPES_HPP__

namespace MiniFPS
{
	typedef struct __tagRENDER_SURFACE
	{
		int	Width;
		int	Height;
		int BufferCount;
		int RedSize;
		int BlueSize;
		int GreenSize;
		int AlphaSize;
		int DepthSize;
		int StencilSize;
	}RENDER_SURFACE;

	typedef struct __tagRECT
	{
		int X;
		int Y;
		int Width;
		int Height;
	}RECT;
}

#endif // __MINIFPS_RENDERTYPES_HPP__

