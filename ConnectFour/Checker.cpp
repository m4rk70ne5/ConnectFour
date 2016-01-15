#include "Checker.h"

using namespace std;

Checker::Checker(CheckerColor col, string file_path) : m_color(col)
{
	SDL_Surface* loadedBitmap = IMG_Load(file_path.c_str());
	if (loadedBitmap != NULL)
	{
		m_bitmap = SDL_DisplayFormat(loadedBitmap);
		SDL_FreeSurface(loadedBitmap);

		//set the color key
		Uint32 colorKey = SDL_MapRGB(m_bitmap->format, 0, 0xFF, 0xFF);
		SDL_SetColorKey(m_bitmap, SDL_SRCCOLORKEY, colorKey);
	}
}

Checker::~Checker()
{
	SDL_FreeSurface(m_bitmap);
}

void Checker::Draw(SDL_Surface* dest, POINT loc)
{
	SDL_Rect destPos;
	destPos.x = (int)loc.x;
	destPos.y = (int)loc.y;
	SDL_BlitSurface(m_bitmap, NULL, dest, &destPos);
}

string Checker::GetBitmapPath(CheckerColor col)
{
	if (col == red)
		return REDCHECKER_FILEPATH;
	else if (col == black)
		return BLACKCHECKER_FILEPATH;
	else
		return EMPTYCHECKER_FILEPATH;
}