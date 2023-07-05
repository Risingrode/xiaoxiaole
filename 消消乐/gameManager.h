#pragma once
#include"Gamer.h"
/*
	这是用于处理主游戏相关数据和区域的类，包含游戏相关的主要数据
	如地图，分数等等
*/

class gameManager :public drawArea
{
public:
	void init();
	bool inArea(int mx, int my);
	void show();
	void placeAt(int tx, int ty);
	void dealMsg(MOUSEMSG m);

	Gamer game;


};