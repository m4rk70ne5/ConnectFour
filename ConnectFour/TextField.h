#include <SDL/SDL.h>
#include <SDL_ttf.h>
#include <windows.h>
#include <vector>
#include <string>
#include <sstream>

using namespace std;

extern SDL_Color green;

class TextBlock
{
    protected:
        bool m_newLine;  //if true, skip to the next line
        SDL_Color m_color;
    public:
        TextBlock(bool, SDL_Color = green);
        static POINT RenderText(POINT, SDL_Surface*, TTF_Font*, string, SDL_Color, bool);
        virtual POINT PrintText(POINT, SDL_Surface*, TTF_Font*) = 0; //returns the size of the surface
};

class NumberField : public TextBlock
{
    protected:
        int* m_displayInt;
    public:
        NumberField(int*, bool, SDL_Color);
        static string intToString(int);
        POINT PrintText(POINT, SDL_Surface*, TTF_Font*);
};

class StringField : public TextBlock
{
    protected:
        string m_displayString;
    public:
        StringField(string, bool, SDL_Color);
        POINT PrintText(POINT, SDL_Surface*, TTF_Font*);
};

//composite class
class TextField : public TextBlock
{
    protected:
        vector<TextBlock*> m_vtb;
        TTF_Font* m_font;
        POINT m_position;
		int m_id;
    public:
        TextField(POINT, string, int, int);
        ~TextField();
        void Add(TextBlock*);
        POINT PrintText(POINT, SDL_Surface*, TTF_Font*);
        POINT GetPosition() { return m_position; };
        TTF_Font* GetFont() { return m_font; };
		int GetID() { return m_id; };
};
