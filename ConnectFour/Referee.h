#ifndef REFEREE_H
#define REFEREE_H

#include "Grid.h"
#include "Player.h"
#include "TextField.h"
#include "YourTurn.h"
#include <bitset>
using namespace std;

class Referee
{
	protected:
		void GivePoints(int, Player*); // gives a certain amount of points to the player
		vector<POINT> testPoints;
	public:
		TextField* m_bottomStatus;
		TextField* m_ScoreStatus;
		Referee();
		~Referee();
		bool CheckChecker(Grid*, int); // checks if the player can drop a checker into the column
		bool CheckWinner(Grid*,POINT,Player*); // checks if the player won with his last move (if there is one -- otherwise returns null)
		void EndGame(vector<Player*>); // Happens at the same time as above, except when the user tries the quit
		void EndMatch(SDL_Surface*, Player*); // Outputs a message (or image or whatever) onto the screen, resets the board
};

#endif