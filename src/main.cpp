#include <iostream>
#include "sdlw.h"

SDLW sdlw;

struct Heightmap
{

};

Heightmap heightmap;

struct Point
{
	float x, y;
};

void DrawVerticalLine(const int& x, const int& y_top, const int& y_bottom, const Uint8& color_r, const Uint8& color_g)
{
	SDL_SetRenderDrawColor(sdlw.renderer, color_r, color_g, 0, 255);
	SDL_RenderDrawLine(sdlw.renderer, x, y_top, x, y_bottom);
}

float GetHeight(const float& x, const float& y)
{
	//return 45;
	float freq = 0.0334f;
	return (sinf(x * freq) + sinf(y * freq)) * 2.0f;
}

int GetColor(const int x, const int y)
{
	return fabsf(sinf(x * 0.01f) * 200.0f);
}

void Render(Point p, int height, int horizon, int scale_height, int distance, int screen_width, int screen_height)
{
	// Draw from back to the front(high z coordinate to low z coordinate)
	for (int z = distance; z >= 20; z--)
	{
		// Find line on map. This calculation corresponds to a field of view of 90°
		Point pleft{ -z + p.x, -z + p.y };
		const Point pright{ z + p.x, -z + p.y };

		// segment the line
		const float dx = ((pright.x - pleft.x) / (float)screen_width);
		const float zInvByScale = (1.0 / z) * scale_height;

		// Raster line and draw a vertical line for each segment
		for (int i = 0; i < screen_width; i++)
		{
			const int height_on_screen = (height - GetHeight(pleft.x, pleft.y)) * zInvByScale + horizon;

			const Uint8 c1 = static_cast<Uint8>(fabsf(sinf(pleft.x * 0.1f) * 200.0f));
			const Uint8 c2 = static_cast<Uint8>(fabsf(cosf(pleft.y * 0.1f) * 200.0f));

			DrawVerticalLine(i, height_on_screen, screen_height, c1, c2);
			pleft.x += dx;
		}
	}
}

int main(int argc, char* argv[]) {
	const int pixelSize = 2;
	int wwidth = 800;
	int wheight = 600;

	int renderWidth = wwidth / pixelSize;
	int renderHeight = wheight / pixelSize;

	sdlw.Init("Vokshel Shpaysh", wwidth, wheight);

	SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, 0);
	SDL_RenderSetLogicalSize(sdlw.renderer, renderWidth, renderHeight);

	int height = 10;
	int horizon = 0;
	int distance = 200;

	while (sdlw.IsRunning())
	{
		while (sdlw.PopEvent())
		{
			if (sdlw.GetKeyDown(SDLK_ESCAPE))
				sdlw.Close();
		}

		if (sdlw.GetKey(SDL_SCANCODE_S)) height--;
		if (sdlw.GetKey(SDL_SCANCODE_W)) height++;

		if (sdlw.GetKey(SDL_SCANCODE_A)) horizon -= 10;
		if (sdlw.GetKey(SDL_SCANCODE_D)) horizon += 10;

		sdlw.Clear();



		Render(Point(0, 0), height, horizon, 1000, 100, renderWidth, renderHeight);

		sdlw.Render();
	}

	return EXIT_SUCCESS;
}