#include <iostream>
#include "sdlw.h"

SDLW sdlw;

int main(int argc, char* argv[]) {
	sdlw.Init("Vokshel Shpaysh", 800, 600);

	while (sdlw.IsRunning())
	{
		while (sdlw.PopEvent())
		{
			if (sdlw.GetKeyDown(SDLK_ESCAPE))
				sdlw.Close();
		}

		sdlw.Clear();

		sdlw.Render();
	}

	return EXIT_SUCCESS;
}