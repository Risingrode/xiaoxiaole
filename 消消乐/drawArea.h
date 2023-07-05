#pragma once
#include<vector>

using namespace std;

class drawArea
{
public:
	drawArea();
	drawArea(int x, int y, int w, int h);
	int px, py;
	int width, height;
};