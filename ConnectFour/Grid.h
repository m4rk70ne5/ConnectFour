#ifndef GRID_H
#define GRID_H

#include <Windows.h>
#include <math.h>
#include <vector>
#include <algorithm>
#include "Checker.h"

class Grid
{
	friend class Referee;
	friend class Player;
	protected:
		std::vector<Checker*> m_vs; //a grid of checkers (or no checkers)
		int m_cutoff; //number of columns
		int m_squareSize; //dimension (in pixels) of the square
		POINT m_position; //position on the screen of top left corner
		static int coordToIndex(POINT, int); //returns an index from a grid coordinate
		static POINT indexToCoord(int, int); //returns a grid coordinate from index
		POINT coordToPix(POINT); //returns a pixel coordinate from a grid coordinate
		Checker* m_empty; //the empty circle bitmap
	public:
		Grid(int, int, int); //creates a arg[0] x arg[1] array, with square size and border width
		~Grid();
		void Draw(SDL_Surface*); //draws itself on the sdl surface
		void Reset(); //clears the grid of checkers
		int CalculateColumn(int); //returns a column number based off of x coordinate
		bool CheckBounds(POINT); //check if the point is within bounds
		void SetPosition(POINT position) { m_position = position; };
};

#endif