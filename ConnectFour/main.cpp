#include "GameMechanic.h"

GameMechanic* pGM;

int main( int argc, char* args[] )
{
	//start SDL
	SDL_Init(SDL_INIT_EVERYTHING);
	pGM = new GameMechanic();
	pGM->BuildGame();

	//main game loop
	bool done = false;
	SDL_Event sdlEvent;
	while (!done)
	{
		while (SDL_PollEvent(&sdlEvent))
		{
			switch (sdlEvent.type)
			{
				case SDL_MOUSEBUTTONDOWN:
				{
					if (!((pGM->GetFreeze()).frozen))
					{
						if (sdlEvent.button.button == SDL_BUTTON_LEFT)
							pGM->HandleClick(sdlEvent.button.x, sdlEvent.button.y);
					}
					else
					{
						switch ((pGM->GetFreeze()).freeze_id)
						{
							case FREEZE_ID_NEWGAME:
								pGM->NewGame();
							break;
						}
					}
				}
				break;
				case SDL_MOUSEMOTION:
				{
					if (!((pGM->GetFreeze()).frozen))
						pGM->HandleMovement(sdlEvent.motion.x, sdlEvent.motion.y);
				}
				break;
				case SDL_KEYDOWN:
				{
					switch(sdlEvent.key.keysym.sym)
					{
						case SDLK_ESCAPE:
						{
							SDL_Event sdlEventTwo;
							sdlEventTwo.type = SDL_QUIT;
							SDL_PushEvent(&sdlEventTwo); //send a quit message
						}
						break;
					}
				}
				break;
				case SDL_QUIT:
					pGM->ExitGame(done);
				break;
			}
		}
		//update

		//render
		pGM->PaintGame();
	}
	delete pGM;
	//quit SDL
	SDL_Quit();
	return 0;
}