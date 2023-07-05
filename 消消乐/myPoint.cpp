#include"myPoint.h"

MYPOINT::MYPOINT()
{
	set(0, 0);
}

MYPOINT::MYPOINT(int tx, int ty)
{
	set(tx, ty);
}

void MYPOINT::set(int tx, int ty)
{
	x = tx, y = ty;
}
