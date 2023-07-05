#pragma once
#include<easyx.h>

#include"blockType.h"

struct colorList
{
	colorList();
	void refreshSeed();
	int getSize();
	int getMsize();
	bool addColor(COLORREF new_color);
	blockColor getRandColor();
	COLORREF data[10];
private:
	int size;
	int msize;
};