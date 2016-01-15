#include "Player.h"

Player::Player(CheckerColor col, int id) : m_color(col), m_id(id) 
{ 
	string file_path = Checker::GetBitmapPath(m_color);
	m_checker = new Checker(col, file_path);
	m_numPoints = 0;
}

Player::~Player()
{
	delete m_checker;
}

POINT Player::DropChecker(Grid* pgrid, int column)
{
	//vector<Checker*>::iterator it;
	bool found = false;
	int returnedIndex = 0;
	//search the list (skipping every cutoff amount of checkers) for an 
	//empty spot
	for (int index = column; index < pgrid->m_vs.size() && !found; index+=pgrid->m_cutoff)
	{
		if ((pgrid->m_vs[index]->GetColor() != white)) 
		{
			found = true;
			//once you find an occupied spot, place the checker at the spot above it
			returnedIndex = index -= pgrid->m_cutoff;
			pgrid->m_vs[index] = m_checker;
		}
		else if ((index + pgrid->m_cutoff) >= pgrid->m_vs.size())
		{
			//if it is the bottom most checker space
			pgrid->m_vs[index] = m_checker;
			returnedIndex = index;
		}
	}
	return pgrid->indexToCoord(returnedIndex, pgrid->m_cutoff); //return the index to the Checker* vector
}

