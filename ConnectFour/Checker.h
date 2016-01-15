#ifndef CHECKER_H
#define CHECKER_H

#include <Windows.h>
#include <sdl/SDL.h>
#include <SDL_image.h>
#include <string>

#define REDCHECKER_FILEPATH "images/red_checker.png"
#define BLACKCHECKER_FILEPATH "images/black_checker.png"
#define EMPTYCHECKER_FILEPATH "images/empty_checker.png"

using namespace std;

enum CheckerColor {white, red, black};

class Checker
{
	friend class Player;
	friend class Grid;
	protected:
		CheckerColor m_color;
		SDL_Surface* m_bitmap;
	public:
		Checker(CheckerColor, string); //string is the file path of our bitmap
		virtual ~Checker();
		void Draw(SDL_Surface*, POINT); //draw a checker onto the SDL Surface
		CheckerColor GetColor() { return m_color; };
		static string GetBitmapPath(CheckerColor);
};

#endif