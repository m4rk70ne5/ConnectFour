#ifndef GAMEMECHANIC_H
#define GAMEMECHANIC_H

#include "Hotspot.h"
#include "Referee.h"
#include "Grid.h"
#include <iostream>

#define ARROWBITMAP_PATH "images/arrow.png"
#define TF_HEADER 1
#define TF_SCORE 2
#define TF_TURN 3
#define TF_WINNER 4
#define TF_UNREFERRED 5

#define FREEZE_ID_NEWGAME 1

void ChangeCursorNormal();
void ChangeCursorArrow();

typedef struct _FREEZE_STRUCT
{
	int freeze_id;
	bool frozen;
} FREEZE_STRUCT;

//this class handles all the "technicalities" of the game
class GameMechanic
{
	friend void ChangeCursorNormal();
	friend void ChangeCursorArrow();
	protected:
		YourTurn* m_yt; //a your turn token
		Hotspot* m_hotspot; //a hotspot (in other games, there will be an array of hotspots)
		Referee* m_referee; // the referee
		Grid* m_grid; //the grid
		static bool m_arrow; //should the arrow cursor be drawn?
		SDL_Surface* m_arrowBitmap;
		SDL_Surface* m_screen; // contains the screen surface
		vector<TextField*> m_textPool;
		FREEZE_STRUCT m_freeze; //freezing input
		int m_currentPlayer; // for text output
	public:
		GameMechanic();
		~GameMechanic();
		void HandleClick(int, int); //depending on what was clicked where, a "message" will be sent to the clicked location
		void HandleMovement(int, int); // checks for movement within an object (useful for hotspots)
		void BuildGame(); //create all the objects
		void DestroyGame(); //destroy all the objects
		void PaintGame();  //paint the game on the screen, the parameters are mouse information
		TextField* GetTextField(int); //get a certain text field based on its ID
		void RemoveText(int); //remove a certain text field from the visible text vector
		vector<TextField*> m_visibleText; //just erase stuff from here, don't call "delete"
		inline FREEZE_STRUCT GetFreeze() {return m_freeze; };
		inline void SetFreeze(int id, bool freeze) { m_freeze.freeze_id = id; m_freeze.frozen = freeze; };
		void NewGame();
		bool QuitShield(); //this is for making sure the user wants to really quit
		void ExitGame(bool&);
};

#endif GAMEMECHANIC_H