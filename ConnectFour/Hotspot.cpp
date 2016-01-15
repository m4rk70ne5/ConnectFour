#include "Hotspot.h"

Hotspot::Hotspot(POINT min, POINT max, voidFunc enter, voidFunc exit)
{
	m_inArea = false;
	//create SDL rect
	int width = max.x - min.x;
	int height = max.y - min.y;

	m_area.x = (int)min.x;
	m_area.y = (int)min.y;
	m_area.w = width;
	m_area.h = height;

	onEnter = enter;
	onExit = exit;
}

POINT Hotspot::findMax()
{
	POINT pt;
	pt.x = m_area.x + m_area.w;
	pt.y = m_area.y + m_area.h;

	return pt;
}