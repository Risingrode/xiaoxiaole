#pragma once
#include<iostream>
#include<vector>
#include<cmath>

#include<easyx.h>//需要安装EasyX
#include"blockType.h"
#include"myPoint.h"/*..\structs\*/
#include"drawArea.h"/*..\structs\*/

#ifndef BLOCKSIZE
#define BLOCKSIZE 50
#endif

using namespace std;

class BlockDrawLayer
{//绘图相关结构
public:
	BlockDrawLayer();
	BlockDrawLayer(int tx, int ty);
	void addDestiny(MYPOINT new_destiny);
	void getSide(int tx, int ty);
	void setPos(int tx, int ty);
	MYPOINT closeTo();
	bool atDestiny();
	void move();
public:
	float speed;
	int blocksize;
	int sidex, sidey;
	MYPOINT saved_position;
	MYPOINT position;
	vector<MYPOINT> destiny;
};

struct Block
{
	Block();
	Block(int tx, int ty);
	~Block();
	void show();
	void mouseMove(int mx, int my);
	void setPosition(int tx, int ty);
	void move();
	void fade();
	void savePosition();
	void goBack();
	BlockDrawLayer* drawlayer;
	blockType type;
	blockColor colorNo;
	COLORREF RGBcolor;
	bool changed;
	int on_anime;
	//0:不在动画 1：正在移动 2：正在消除
};

