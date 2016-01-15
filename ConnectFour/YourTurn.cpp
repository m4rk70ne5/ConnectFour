#include "YourTurn.h"

PLNode::PLNode(Player* player)
{
	m_next = NULL;
	m_player = player;
}

PLNode::~PLNode()
{
	delete m_player;
}

YourTurn::YourTurn()
{
	m_ownerPlayer = NULL;
	m_head = NULL;
	m_tail = NULL;
}

YourTurn::~YourTurn()
{
	DeleteList();
}

void YourTurn::AddPlayer(Player* player)
{
	if (m_head == NULL)
	{
		// this is the first node added to the list
		PLNode* first = new PLNode(player);
		m_ownerPlayer = first;
		m_head = first; //this will always be the case, unless it is removed
		m_tail = first; //this will not always be true
		first->m_next = first;  //for now, it will just be itself
	}
	else
	{
		//this is not the first one
		PLNode* node = new PLNode(player);
		m_tail->m_next = node; //the old last node's next node is this node
		node->m_next = m_head; //make the last item point to the first item
		m_tail = node; //this node is the last item
	}
}

Player* YourTurn::DefaultNextPlayer()
{
	m_ownerPlayer = m_ownerPlayer->m_next;
	return m_ownerPlayer->m_player;
}

//only do this when you're not going to the use the list again
void YourTurn::DeleteList()
{
	if (m_head != NULL)
	{
		while (m_head != m_tail)
		{
			PLNode* temp = m_head;
			m_head = m_head->m_next; //make the new head the node after original head
			m_tail->m_next = m_head; //make the node after the tail the new head
			delete temp; //delete the original head
		}
		//now, just delete the head
		delete m_head;
		m_head = NULL;
		m_tail = NULL;
		m_ownerPlayer = NULL;
	}
}