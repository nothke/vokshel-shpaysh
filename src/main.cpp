#include <iostream>
#include "sdlw.h"
#include <vector>

SDLW sdlw;

struct Heightmap
{
	int width = 256;
	std::vector<int> heights;

	int Get(const int x, const int y) const
	{
		float freq = 0.134f;
		return (int)((sinf(x * freq) + sinf(y * freq)) * 20.0f);
		//return heights[y * width + x];
	}

	int GetColor(const int x, const int y) const
	{
		return fabsf(sinf(x * 0.01f) * 200.0f);
	}
};

Heightmap heightmap;

struct Point
{
	int x, y;
};

void DrawVerticalLine(int x, int y_top, int y_bottom, int color_r, int color_g)
{
	SDL_SetRenderDrawColor(sdlw.renderer, color_r, color_g, 0, 255);
	SDL_RenderDrawLine(sdlw.renderer, x, y_top, x, y_bottom);
}

void Render(Point p, int height, int horizon, int scale_height, int distance, int screen_width, int screen_height)
{
	// Draw from back to the front(high z coordinate to low z coordinate)
	for (int z = distance; z >= 20; z--)
	{
		// Find line on map. This calculation corresponds to a field of view of 90°
		Point pleft{ -z + p.x, -z + p.y };
		Point pright{ z + p.x, -z + p.y };

		// segment the line
		int dx = static_cast<int>((pright.x - pleft.x) / (float)screen_width);

		// Raster line and draw a vertical line for each segment
		for (int i = 0; i < screen_width; i++)
		{
			int height_on_screen = (height - heightmap.Get(pleft.x, pleft.y)) / (float)z * scale_height + horizon;

			SDL_Color color{ fabsf(sinf(pleft.x * 0.1f) * 200.0f), fabsf(sinf(pleft.y * 0.1f) * 200.0f), 0,255 };
			DrawVerticalLine(i, height_on_screen, screen_height, color.r, color.g);
			pleft.x += dx;
		}
	}
}

//for z in range(distance, 1, -1) :
//	# Find line on map.This calculation corresponds to a field of view of 90°
//	pleft = Point(-z + p.x, -z + p.y)
//	pright = Point(z + p.x, -z + p.y)
//	# segment the line
//	dx = (pright.x - pleft.x) / screen_width
//	# Raster line and draw a vertical line for each segment
//	for i in range(0, screen_width) :
//		height_on_screen = (height - heightmap[pleft.x, pleft.y]) / z * scale_height. + horizon
//		DrawVerticalLine(i, height_on_screen, screen_height, colormap[pleft.x, pleft.y])
//		pleft.x += dx

//		# Call the render function with the camera parameters :


int main(int argc, char* argv[]) {
	sdlw.Init("Vokshel Shpaysh", 400, 300);

	int height = 50;
	int horizon = 120;
	int distance = 100;

	while (sdlw.IsRunning())
	{
		while (sdlw.PopEvent())
		{
			if (sdlw.GetKeyDown(SDLK_ESCAPE))
				sdlw.Close();
		}

		if (sdlw.GetKey(SDL_SCANCODE_S)) height--;
		if (sdlw.GetKey(SDL_SCANCODE_W)) height++;

		if (sdlw.GetKey(SDL_SCANCODE_A)) horizon--;
		if (sdlw.GetKey(SDL_SCANCODE_D)) horizon++;

		sdlw.Clear();



		Render(Point(0, 0), height, horizon, 1000, 100, sdlw.getScreenWidth(), sdlw.getScreenHeight());

		sdlw.Render();
	}

	return EXIT_SUCCESS;
}