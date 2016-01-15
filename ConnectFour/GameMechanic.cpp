#include "GameMechanic.h"

bool GameMechanic::m_arrow = false;

void ChangeCursorNormal()
{
	GameMechanic::m_arrow = false;
}

void ChangeCursorArrow()
{
	GameMechanic::m_arrow = true;
}

GameMechanic::GameMechanic()
{
	//GameMechanic::m_arrow = false;
	//set the screen and video mode and all that
	m_screen = SDL_SetVideoMode(1024, 768, 32, SDL_DOUBLEBUF|SDL_HWSURFACE);
	//load the mouse arrow bitmap
	SDL_Surface* m_tempBitmap = IMG_Load(ARROWBITMAP_PATH);
	if (m_tempBitmap != NULL)
	{
		m_arrowBitmap = SDL_DisplayFormat(m_tempBitmap);
		SDL_FreeSurface(m_tempBitmap);
		SDL_SetColorKey(m_arrowBitmap, SDL_SRCCOLORKEY, SDL_MapRGB(m_arrowBitmap->format, 0xFF, 0x00, 0xFF));
	}
	else
		MessageBox(NULL,TEXT("Can't open arrow png"),TEXT("Error"),MB_OK);
	//load TTF
	if (TTF_Init() == -1)
		MessageBox(NULL,TEXT("Error initializing TTF"),TEXT("Error"),MB_OK);

	//start out unfrozen
	m_freeze.frozen = false;
}

GameMechanic::~GameMechanic()
{
	SDL_FreeSurface(m_arrowBitmap);
	SDL_FreeSurface(m_screen);
	TTF_Quit();
	DestroyGame();
}

void GameMechanic::BuildGame()
{
	//white color
	SDL_Color white = {0xFF, 0xFF, 0xFF};

	//players
	Player* pOne = new Player(red, 1);
	Player* pTwo = new Player(black, 2);

	//player token
	m_yt = new YourTurn();

	m_yt->AddPlayer(pOne);
	m_yt->AddPlayer(pTwo);

	m_currentPlayer = m_yt->m_ownerPlayer->m_player->m_id;

	//add hotspot and grid
	POINT hsMin;
	POINT hsMax;
	hsMin.x = 176;
	hsMin.y = 128;
	hsMax.x = 176 + 448;
	hsMax.y = 128 + 64;

	//hotspot
	m_hotspot = new Hotspot(hsMin, hsMax, ChangeCursorArrow, ChangeCursorNormal);

	//grid
	POINT gridPosition;
	gridPosition.x = 176;
	gridPosition.y = 192;
	m_grid = new Grid(7, 7, 64);
	m_grid->SetPosition(gridPosition);
	//add text to pool
	POINT headerPos = {176, 64};
	POINT scorePos = {656, 300};
	POINT bottomPos = {196, 688};
	POINT defaultPos = {0, 0};

	//referee
	m_referee = new Referee();

	//header
	TextField* headerField = new TextField(headerPos, "arial.ttf", 50, TF_HEADER);
	headerField->Add(new StringField("CONNECT      FOUR", true, white));

	//side statuses
    TextField* sideHeader = new TextField(defaultPos, "arial.ttf", 36, TF_UNREFERRED);
    sideHeader->Add(new StringField("SCORE", true, white));
    sideHeader->Add(new StringField("------------", true, white));

    StringField* playerString = new StringField("Player ", false, white);
    StringField* colonChar = new StringField(": ", false, white);

    TextField* sideLineOne = new TextField(defaultPos, "arial.ttf", 30, TF_UNREFERRED);
    sideLineOne->Add(playerString);
    sideLineOne->Add(new NumberField(&(pOne->m_id), false, white));
    sideLineOne->Add(colonChar);
    sideLineOne->Add(new NumberField(&(pOne->m_numPoints), true, white));
    TextField* sideLineTwo = new TextField(defaultPos, "arial.ttf", 30, TF_UNREFERRED);
    sideLineTwo->Add(playerString);
    sideLineTwo->Add(new NumberField(&(pTwo->m_id), false, white));
    sideLineTwo->Add(colonChar);
    sideLineTwo->Add(new NumberField(&(pTwo->m_numPoints), true, white));

    TextField* sideStatus = new TextField(scorePos, "arial.ttf", 30, TF_SCORE);
    sideStatus->Add(sideHeader);
    sideStatus->Add(sideLineOne);
    sideStatus->Add(sideLineTwo);

	//bottom status
	//player #'s turn
	TextField* turnStatus = new TextField(bottomPos, "arial.ttf", 24, TF_TURN);
    turnStatus->Add(playerString);
    turnStatus->Add(new NumberField(&m_currentPlayer, false, white));
    turnStatus->Add(new StringField("'s turn to move.", true, white));

	//player # wins
	TextField* winStatus = new TextField(bottomPos, "arial.ttf", 24, TF_WINNER);
    winStatus->Add(playerString);
    winStatus->Add(new NumberField(&m_currentPlayer, false, white));
    winStatus->Add(new StringField(" wins!", true, white));
	winStatus->Add(new StringField("[click to continue]", true, white));

	//the text pool
	m_textPool.push_back(headerField);
	m_textPool.push_back(sideStatus);
	m_textPool.push_back(turnStatus);
	m_textPool.push_back(winStatus);

	//the visible text
	m_visibleText.push_back(headerField);
	m_visibleText.push_back(sideStatus);
	m_visibleText.push_back(turnStatus);
}

void GameMechanic::DestroyGame()
{
	delete m_yt; //this will get rid of the players too

	//delete hotspot and grid
	delete m_hotspot;
	delete m_grid;

	//delete referee
	delete m_referee;

	//get rid of all the text in the pool and visible vector
	//vector<TextField*>::iterator tpit;
	//for (tpit = m_visibleText.end() - 1; tpit >= m_visibleText.begin(); tpit--)
	//	m_visibleText.erase(tpit);
	m_visibleText.clear();
	m_textPool.clear();
	//erase the text pool separately because we don't want to delete what isn't there
	//for (tpit = m_textPool.end() - 1; tpit >= m_textPool.begin(); tpit--)
	//{
	//	delete (*tpit);
	//	m_textPool.erase(tpit);
	//}
}

void GameMechanic::HandleClick(int x, int y)
{
	//first of all, is the mouse in the area?
	if (m_hotspot->m_inArea)
	{
		//if so, first have the referee check if the player can drop a checker down the column
		//calculate the column first
		int column = m_grid->CalculateColumn(x);
		if (!m_referee->CheckChecker(m_grid, column))
		{
			//put the checker down the column
			POINT droppedChecker = m_yt->m_ownerPlayer->m_player->DropChecker(m_grid, column);
			
			//check if the player won
			if (m_referee->CheckWinner(m_grid, droppedChecker, m_yt->m_ownerPlayer->m_player))
			{
				m_referee->EndMatch(m_screen, m_yt->m_ownerPlayer->m_player);
				// Output a message onto the screen
				// Player so-and-so won!
				// [Click to start new match]
				RemoveText(TF_TURN); //remove the it's player #'s turn to play
				m_visibleText.push_back(GetTextField(TF_WINNER));
				//freeze input
				m_freeze.freeze_id = FREEZE_ID_NEWGAME;
				m_freeze.frozen = true;
			}
			else
			{
				m_yt->m_ownerPlayer = m_yt->m_ownerPlayer->m_next;
				m_currentPlayer = m_yt->m_ownerPlayer->m_player->m_id;
			}
		}
	}
}

void GameMechanic::HandleMovement(int x, int y)
{
	POINT max = m_hotspot->findMax(); //get the maximum coordinate of hotspot
	if (!m_hotspot->m_inArea)
	{
		//if it is not in the area already
		if ((x < max.x && x > m_hotspot->m_area.x) &&
			(y < max.y && y > m_hotspot->m_area.y))
		{
			m_hotspot->m_inArea = true;
			m_hotspot->onEnter();
		}
	}
	else
	{
		if ((x > max.x || x < m_hotspot->m_area.x) || 
			(y > max.y || y < m_hotspot->m_area.y))
		{
			m_hotspot->m_inArea = false;
			m_hotspot->onExit();
		}
	}	
}

void GameMechanic::PaintGame()
{
	//clear the screen
	SDL_FillRect(m_screen, NULL, SDL_MapRGB(m_screen->format, 0x00, 0x00, 0x00));
	//draw all the drawable elements
	m_grid->Draw(m_screen);
	vector<TextField*>::iterator tfit;
	for (tfit = m_visibleText.begin(); tfit < m_visibleText.end(); tfit++)
		(*tfit)->PrintText((*tfit)->GetPosition(), m_screen, (*tfit)->GetFont());

	//Draw the mouse cursor
	if (m_arrow)
	{
		SDL_ShowCursor(SDL_DISABLE);
		int xpos, ypos;
		SDL_GetMouseState(&xpos, &ypos);
		SDL_Rect dest;
		dest.x = xpos;
		dest.y = ypos;
		SDL_BlitSurface(m_arrowBitmap, NULL, m_screen, &dest);
	}
	else
		SDL_ShowCursor(SDL_ENABLE);
	
	//yay for double-buffering
	SDL_Flip(m_screen);
}

TextField* GameMechanic::GetTextField(int id)
{
	for (size_t i = 0; i < m_textPool.size(); i++)
	{
		if (m_textPool[i]->GetID() == id)
			return m_textPool[i];
	}
	return NULL;
}

void GameMechanic::RemoveText(int id)
{
	vector<TextField*>::iterator vtit;
	for (vtit = m_visibleText.begin(); vtit < m_visibleText.end(); vtit++)
	{
		if ((*vtit)->GetID() == id)
		{
			m_visibleText.erase(vtit);
			return;
		}
	}
}

void GameMechanic::NewGame()
{
	//reset the board
	m_grid->Reset();
	//go to the next player
	m_yt->m_ownerPlayer = m_yt->m_ownerPlayer->m_next;
	m_currentPlayer = m_yt->m_ownerPlayer->m_player->m_id;
	//output normal message
	RemoveText(TF_WINNER);
	m_visibleText.push_back(GetTextField(TF_TURN));
	//unfreeze
	m_freeze.frozen = false;
}

bool GameMechanic::QuitShield()
{
	int ret = MessageBox(NULL, TEXT("Are you sure want to quit?"), TEXT("Message"), MB_OKCANCEL);
	if (ret == IDOK)
		return true;
	return false;
}

void GameMechanic::ExitGame(bool& doneFlag)
{
	if (QuitShield())
	{
		vector<Player*> vPlayer;
		//cycle through the yourturn token
		PLNode* head = m_yt->m_head;
		PLNode* current = m_yt->m_head;
		do
		{
			vPlayer.push_back(current->m_player);
			current = current->m_next;
		}
		while (current != head);

		m_referee->EndGame(vPlayer);
		doneFlag = true; // quit the game loop
	}
}