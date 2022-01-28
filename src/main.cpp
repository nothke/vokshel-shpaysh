#include <iostream>
#include "sdlw.h"

SDLW sdlw;

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
	float freq = 0.0334f;
	return (sinf(x * freq) + sinf(y * freq)) * 2.0f;
}

int GetColor(const int x, const int y)
{
	return fabsf(sinf(x * 0.01f) * 200.0f);
}

void Render(Point p, int height, int horizon, int scale_height, int distance, int screen_width, int screen_height)
{
	const float dInv = 1.f / distance;

	// Draw from back to the front(high z coordinate to low z coordinate)
	for (int z = distance; z >= 20; z--)
	{
		// Find line on map. This calculation corresponds to a field of view of 90°
		Point pleft{ -z + p.x, -z + p.y };
		const Point pright{ z + p.x, -z + p.y };

		// segment the line
		const float dx = ((pright.x - pleft.x) / (float)screen_width);
		const float zInvByScale = (1.0 / z) * scale_height;
		const float fogFactor = (distance - z) * dInv;

		// Raster line and draw a vertical line for each segment
		for (int i = 0; i < screen_width; i++)
		{
			const int height_on_screen = (height - GetHeight(pleft.x, pleft.y)) * zInvByScale + horizon;

			const Uint8 c1 = static_cast<Uint8>(fabsf(sinf(pleft.x * 0.1f) * 255.0f) * fogFactor);
			const Uint8 c2 = static_cast<Uint8>(fabsf(cosf(pleft.y * 0.1f) * 255.0f) * fogFactor);

			DrawVerticalLine(i, height_on_screen, screen_height, c1, c2);
			pleft.x += dx;
		}
	}
}

int main(int argc, char* argv[]) {

	const int pixelSize = 2;
	const int windowWidth = 800;
	const int windowHeight = 600;

	const int renderWidth = windowWidth / pixelSize;
	const int renderHeight = windowHeight / pixelSize;

	sdlw.Init("Vokshel Shpaysh", windowWidth, windowHeight);

	SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, 0);
	SDL_RenderSetLogicalSize(sdlw.renderer, renderWidth, renderHeight);

	int height = 10;
	int horizon = 0;
	int distance = 200;

	Point p{ 0, 0 };

	while (sdlw.IsRunning())
	{
		while (sdlw.PopEvent())
		{
			if (sdlw.GetKeyDown(SDLK_ESCAPE))
				sdlw.Close();
		}

		if (sdlw.GetKey(SDL_SCANCODE_S)) p.y++;
		if (sdlw.GetKey(SDL_SCANCODE_W)) p.y--;

		if (sdlw.GetKey(SDL_SCANCODE_A)) p.x -= 1;
		if (sdlw.GetKey(SDL_SCANCODE_D)) p.x += 1;

		if (sdlw.GetKey(SDL_SCANCODE_LCTRL)) height--;
		if (sdlw.GetKey(SDL_SCANCODE_SPACE)) height++;

		if (sdlw.GetKey(SDL_SCANCODE_Q)) horizon -= 10;
		if (sdlw.GetKey(SDL_SCANCODE_E)) horizon += 10;

		sdlw.Clear();

		Render(p, height, horizon, 1000, distance, renderWidth, renderHeight);

		sdlw.Render();
	}

	return EXIT_SUCCESS;
}