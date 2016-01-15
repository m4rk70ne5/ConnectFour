#include "TextField.h"

SDL_Color green = {0x00, 0xFF, 0x00};

TextBlock::TextBlock(bool newLine, SDL_Color color) : m_newLine(newLine), m_color(color)
{

}

POINT TextBlock::RenderText(POINT base, SDL_Surface* pSurface, TTF_Font* pFont, string displayString, SDL_Color color, bool newLine)
{
    POINT newOffset;

    SDL_Surface* textSurface = TTF_RenderText_Blended(pFont, displayString.c_str(), color);
    if (newLine == true)
    {
        newOffset.x = 0;
        newOffset.y = textSurface->h;
    }
    else
    {
        newOffset.x = textSurface->w;
        newOffset.y = 0;
    }
    SDL_Rect destRect;
    destRect.x = (int)base.x;
    destRect.y = (int)base.y;
    SDL_BlitSurface(textSurface, NULL, pSurface, &destRect);
    SDL_FreeSurface(textSurface);
    return newOffset;
}

NumberField::NumberField(int* pInt, bool newLine, SDL_Color color) : TextBlock(newLine, color), m_displayInt(pInt)
{
}

string NumberField::intToString(int numberToConvert)
{
    stringstream ss;
	string convertedNumber;
	ss.clear();
	ss << numberToConvert;
	convertedNumber = ss.str();
	//char buffer[sizeof(int)*8+1];
    //_itoa(numberToConvert, buffer, 10);
    //string newString;
    //newString.assign(buffer);
    return convertedNumber;
}

POINT NumberField::PrintText(POINT base, SDL_Surface* pSurface, TTF_Font* pFont)
{
    POINT offset;
    string stringToRender = intToString(*m_displayInt);
    offset = RenderText(base, pSurface, pFont, stringToRender, m_color, m_newLine);
    return offset;
}

StringField::StringField(string displayString, bool newLine, SDL_Color color) : TextBlock(newLine, color), m_displayString(displayString)
{
}

POINT StringField::PrintText(POINT base, SDL_Surface* pSurface, TTF_Font* pFont)
{
    POINT offset;
    offset = RenderText(base, pSurface, pFont, m_displayString, m_color, m_newLine);
    return offset;
}

TextField::TextField(POINT position, string fontPath, int fontSize, int id) : TextBlock(true), m_position(position), m_id(id)
{
    m_font = TTF_OpenFont(fontPath.c_str(), fontSize);
}

void TextField::Add(TextBlock* pTB)
{
    m_vtb.push_back(pTB);
}

POINT TextField::PrintText(POINT base, SDL_Surface* pSurface, TTF_Font* pFont)
{
    vector<TextBlock*>::iterator it;
    POINT original_base = base; //so new line will work
    POINT offset = {0, 0}; //for calculating where we are after a textfield is drawn
    for (it = m_vtb.begin(); it < m_vtb.end(); it++)
    {
        //todo:  increase base upon each iteration according to a certain width or height
        POINT temp = (*it)->PrintText(base, pSurface, pFont); // the font gets overriden by first call
        offset.x += temp.x;
        offset.y += temp.y;
        if (temp.x == 0)
        {
            base.x = original_base.x; // new line!
            offset.x = 0;
        }
        else
            base.x += temp.x;
        base.y += temp.y;

    }
    return offset;
}

TextField::~TextField()
{
    TTF_CloseFont(m_font);
    vector<TextBlock*>::iterator it;
    for (it = m_vtb.end(); it >= m_vtb.begin(); it--)
    {
        if ((*it) != NULL)
        {
            delete (*it);
            *it = NULL;
        }
    }
}

