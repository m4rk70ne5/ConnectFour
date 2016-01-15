#ifndef HOTSPOT_H
#define HOTSPOT_H

#include <sdl/SDL.h>
#include <Windows.h>
typedef void (*voidFunc)();

class Hotspot
{
	friend class GameMechanic;
	protected:
		SDL_Rect m_area;
		bool m_inArea;
		voidFunc onEnter;
		voidFunc onExit;
		POINT findMax();
	public:
		Hotspot(POINT, POINT, voidFunc, voidFunc); //min, max, onEnter, onExit
		void Update(int, int); //mouse coordinates
};

#endif