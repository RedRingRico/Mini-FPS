#include <iostream>
#include <Window.hpp>

int main( int p_Argc, char **p_ppArgv )
{
	std::cout << "Mini FPS" << std::endl;
	MiniFPS::Window TestWindow;
	MiniFPS::RECT Rect;
	MiniFPS::RENDER_SURFACE Surface;

	Surface.Width = 800;
	Surface.Height = 600;
	Surface.RedSize = 8;
	Surface.GreenSize = 8;
	Surface.BlueSize = 8;
	Surface.AlphaSize = 8;
	Surface.DepthSize = 24;
	Surface.StencilSize = 8;
	Surface.BufferCount = 2;

	Rect.X = 0;
	Rect.Y = 0;
	Rect.Width = 800;
	Rect.Height = 600;

	if( TestWindow.Create( Rect, Surface ) != 0 )
	{
		return 1;
	}

	return 0;
}

