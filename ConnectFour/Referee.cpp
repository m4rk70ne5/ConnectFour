#include "Referee.h"

using namespace std;

Referee::Referee()
{
	for (int i = -1; i <= 1; i++)
	{
		for (int j = -1; j <= 1; j++)
		{
			POINT inc;
			inc.x = i;
			inc.y = j;
			if (j != 0 || i != 0)
				testPoints.push_back(inc);
		}
	}
}

Referee::~Referee()
{
	testPoints.clear();
}

bool Referee::CheckChecker(Grid* pgrid, int column)
{
	//if there is a checker in the topmost square of the grid
	//return true
	return (pgrid->m_vs[column]->GetColor() != white);
}

void Referee::GivePoints(int amount, Player* pPlayer)
{
	pPlayer->IncreaseScore(amount);
}

bool Referee::CheckWinner(Grid* pGrid, POINT droppedChecker, Player* pPlayer)
{
	//to check to see if there are four checkers in a row
	//check all the squares surrounding the recently
	//dropped checker

	//first, fill up the bitsets with values
	//create four bitsets, each a size of 8
	bitset<7> bitGrid[4];
	//establish the center
	int bitsetCenter = 3;
	for (int i = 0; i < 4; i++)
	{
		bitGrid[i].reset();
		bitGrid[i].set(bitsetCenter);
	}

	//go through all the neighbors
	for (int i = 1; i <= 3; i++)
	{
		vector<POINT>::iterator pit;
		for (pit = testPoints.begin(); pit < testPoints.end(); pit++)
		{
			int testX = pit->x;
			int testY = pit->y;
			testX *= i;
			testY *= i;

			POINT newPoint;
			newPoint.x = droppedChecker.x + testX;
			newPoint.y = droppedChecker.y + testY;

			if (!pGrid->CheckBounds(newPoint))
				continue;  // if the checker is out of bounds, go on to the next point

			//now, decide where this new point should go
			int index = 0; //default is north
			int testValue = pit->x * pit->y;
			if (testValue < 0)
				//if negative, it is northeast
				index = 1;
			else if (testValue == 0 && pit->y == 0)
				index = 2; //east
			else if (testValue > 0)
				index = 3; //northwest
			
			int gridIndex = pGrid->coordToIndex(newPoint, pGrid->m_cutoff);
			if (pGrid->m_vs[gridIndex]->GetColor() == pPlayer->m_color)
			{
				//we need to set the appropriate bit
				//based off of test point, we can figure this out easily
				int indexOffset = (pit->x == 0) ? testY : testX;
				bitGrid[index].set(bitsetCenter + indexOffset);
			}
		}
	}
	//now that we have the bitsets filled, let's decide if the player won
	bool found = false;
	int testIndex = 0;
	while (!found && testIndex < 4)
	{
		int sumOfBits = 0;
		for (int i = 0; i < 7 && sumOfBits < 4; i++)
		{
			bool set = bitGrid[testIndex].test(i);
			sumOfBits += set;
			sumOfBits *= set;
		}
		if (sumOfBits >= 4)
			//we have a winner!
			found = true;
		else
			testIndex++;
	}
	return found; 
}

void Referee::EndMatch(SDL_Surface* pSurface, Player* pPlayer)
{
	//award the player an extra point
	GivePoints(1, pPlayer);	
}

void Referee::EndGame(vector<Player*> vPlayer)
{
	Player* winner;
	int highScore = 0;
	stringstream ss;
	string score = "";
	//char buffer[17];
	int playerNum, playerScore;
	bool tie = false;
	ss.clear();
	//go through the Player* vector and find the highest score
	for (size_t i = 0; i < vPlayer.size(); i++)
	{
		//string formation code
		playerNum = vPlayer[i]->m_id;
		playerScore = vPlayer[i]->m_numPoints;
		ss << "Player " << playerNum << ": " << playerScore << endl;
		//string temp = "";
		//score += "Player ";
		//_itoa(playerNum, buffer, 10);
		//temp.assign(buffer);
		//score += temp;
		//score += ": ";
		
		//_itoa(playerScore, buffer, 10);
		//temp.assign(buffer);
		//score += temp;
		//score += "\n";
		//find the winner
		if (playerScore > highScore)
		{
			winner = vPlayer[i];
			highScore = playerScore;
			tie = false;
		}
		else if (playerScore == highScore)
			tie = true;
	}
	if (!tie)
	{
		//string temp = "";
		//score += "\nPlayer ";
		//_itoa(winner->m_id,buffer,10);
		//temp.assign(buffer);
		//score += temp;
		//score += " wins!";
		ss << "\nPlayer " << winner->m_id << " wins!";
	}
	else
		ss << "\nA tie!";

	score = ss.str();
	MessageBox(NULL,score.c_str(),TEXT("Final Score"),MB_OK); 
}