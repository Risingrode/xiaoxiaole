#pragma once
#include"Gamer.h"
/*
	�������ڴ�������Ϸ������ݺ�������࣬������Ϸ��ص���Ҫ����
	���ͼ�������ȵ�
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