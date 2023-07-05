#pragma once
#include<conio.h>
#include"block.h"/*..\items\*/
#include"colorList.h"/*..\items\*/
#include<graphics.h>

class Gamer :public drawArea
{
public:
	~Gamer();
	void init(int tx, int ty, int col, int row);
	void eraseTest();
	void placeAt(int tx, int ty);
	bool inArea(int tx, int ty);
	void dealMsg(MOUSEMSG m);

	void show();
	void moveBlockBtoA(int ax, int ay, int bx, int by);
	void moveAllBlock();//删除方块后把上面的填下来

	void genAllBlock();//检测是否有空格，有就随机生成方块把那一列填满
	bool onAnime();
private:
	void genBlock(int locx, int locy, int blocknum);
	void eraseFadedBlocks();
	void dealAutoData();//处理非操作数据
	void dealLogicalData();//处理逻辑数据
	void dealAnimeData();//处理动画数据
private:
	void showScore();
	void showMap();
	void showBlocks();
public:
	//判断相关函数
	bool swapBlock(MYPOINT blk, MYPOINT dir, bool real_swap);
	bool judgeIfEnd();//ok
	bool judgeSwap(int x1, int y1, int x2, int y2, bool swap);
	bool judgeSwap(MYPOINT blk_a, MYPOINT blk_b, bool swap);
	bool judgeUpdate();
	bool judgeSingle(int x, int y, bool need_del);

public:
	bool on_focusing;
	int delay_num;
	int focusx, focusy;
	int score;
public:
	colorList colorlist;
	vector<MYPOINT> updatedBlockList;

private:
	MYPOINT focused_location;
	int blocksize;

	int rowNum, colNum;
	Block*** blockMap;
	bool need_help;
};
