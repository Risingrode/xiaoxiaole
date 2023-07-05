#include<time.h>
#include"colorList.h"

colorList::colorList()
{
	refreshSeed();
	size = 0;
	msize = 10;
	for (int i = 0; i < msize; i++)
	{
		data[i] = 0;
	}
	addColor(BLACK);
}

void colorList::refreshSeed()
{
	srand((unsigned)time(NULL));
}

int colorList::getSize()
{
	return size;
}

int colorList::getMsize()
{
	return msize;
}

bool colorList::addColor(COLORREF new_color)
{
	if (size > msize)
		return 0;
	data[size++] = new_color;
	return 1;
}

blockColor colorList::getRandColor()
{
	int rslt = rand() % size;
	rslt = rslt == size - 1 ? rslt : rslt + 1;
	return blockColor(rslt);
}
