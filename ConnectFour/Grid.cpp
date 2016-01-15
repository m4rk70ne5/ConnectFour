#include "Grid.h"

Grid::Grid(int columns, int rows, int squareSize) : m_squareSize(squareSize)
{
	m_cutoff = columns;
	m_vs.resize(columns * rows, NULL);

	if (m_empty != NULL)
		m_empty = new Checker(white, EMPTYCHECKER_FILEPATH);

	Reset();
}

Grid::~Grid()
{
	delete m_empty;
}

void Grid::Reset()
{
	for (size_t i = 0; i < m_vs.size(); i++)
		m_vs[i] = m_empty;
}

void Grid::Draw(SDL_Surface* screen)
{
	//draw a series of rectangles based off of size Checker grid and cutoff
	for (size_t i = 0; i < m_vs.size(); i++)
	{
		POINT gridCoord = indexToCoord(i, m_cutoff); //convert the index to an actual grid coordinate
		POINT pixCoord = coordToPix(gridCoord); //get pixel coordinate of grid coordinate
		//draw each square's checker
		//a "white" checker (color of the background) if there is no checker
		//if (m_vs[i] == NULL)
			m_vs[i]->Draw(screen, pixCoord);
		//else
			//m_empty->Draw(screen, pixCoord);
	}
}

int Grid::CalculateColumn(int coord)
{
	//using the width of a square, find the column
	
	int relative_x = coord - m_position.x;
	return (int)(relative_x / m_squareSize);
}

int Grid::coordToIndex(POINT pt, int cutoff)
{
	int colOffset = pt.x;
	int rowOffset = pt.y * cutoff;
	return (colOffset + rowOffset);
}

POINT Grid::indexToCoord(int index, int cutoff)
{
	POINT coordinate;
	int column = index % cutoff;
	int row = (int)(index / cutoff);
	
	//x,y will be column,row
	coordinate.x = column;
	coordinate.y = row;

	return coordinate;
}

POINT Grid::coordToPix(POINT gridCoord)
{
	POINT screenCoord;
	screenCoord.x = m_position.x + (gridCoord.x * m_squareSize);
	screenCoord.y = m_position.y + (gridCoord.y * m_squareSize);

	return screenCoord;
}

bool Grid::CheckBounds(POINT testCoord)
{
	if ((testCoord.x >= 0) && (testCoord.x < m_cutoff) && (testCoord.y >= 0) && (testCoord.y < (m_vs.size() / m_cutoff)))
			return true;
	else
		return false;
}