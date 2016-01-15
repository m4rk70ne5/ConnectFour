#ifndef YOURTURN_H
#define YOURTURN_H

#include "Player.h"

class PLNode
{
	friend class GameMechanic;
	friend class YourTurn;
	protected:
		PLNode* m_next;
		Player* m_player;
	public:
		PLNode(Player*);
		virtual ~PLNode();
};

class YourTurn
{
	friend class GameMechanic;
	protected:
		PLNode* m_ownerPlayer; //the current node
		PLNode* m_head; //linked list helpers
		PLNode* m_tail; //linked list helpers
	public:
		YourTurn();
		~YourTurn();
		Player* DefaultNextPlayer(); //returns the player whose turn should be next
		void AddPlayer(Player*); //add a player to the linked list
		void DeleteList(); //just get rid of the list
};

#endif