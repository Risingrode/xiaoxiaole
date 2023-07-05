#include"drawArea.h"

drawArea::drawArea()
{
	px = 0, py = 0;
	height = 800;
	width = 600;
}

drawArea::drawArea(int x, int y, int w, int h)
{
	px = x;
	py = y;
	width = w;
	height = h;
}
