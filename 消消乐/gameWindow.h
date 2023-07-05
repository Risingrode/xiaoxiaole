#pragma once
#include<windows.h>

#include<easyx.h>

#include"gameManager.h"
class gameWindow : public drawArea
{
public:
	gameWindow();
	gameWindow(int x, int y, int w, int h);
	void start();
	void show();
private:
	bool checkFocus(MOUSEMSG& m);
	void dealMsg(MOUSEMSG& m);
public:
	gameManager gm;
	int delay_num;
	bool on_focusing;
};
