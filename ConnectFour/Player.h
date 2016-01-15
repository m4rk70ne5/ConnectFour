#ifndef PLAYER_H
#define PLAYER_H

#include "Grid.h"
#include <vector>

using namespace std;

class Player
{
	friend class Referee;
	friend class GameMechanic;
	protected:
		int m_numPoints; //the number of games the player won
		int m_id; //unique player id
		CheckerColor m_color;
		Checker* m_checker; //its checker
	public:
		Player(CheckerColor, int);
		~Player();
		POINT DropChecker(Grid*, int); //drop a checker down a certain column
		inline void IncreaseScore(int amount) { m_numPoints += amount; };
};

#endif