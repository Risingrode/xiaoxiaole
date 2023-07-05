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
	void moveAllBlock();//ɾ�������������������

	void genAllBlock();//����Ƿ��пո��о�������ɷ������һ������
	bool onAnime();
private:
	void genBlock(int locx, int locy, int blocknum);
	void eraseFadedBlocks();
	void dealAutoData();//����ǲ�������
	void dealLogicalData();//�����߼�����
	void dealAnimeData();//����������
private:
	void showScore();
	void showMap();
	void showBlocks();
public:
	//�ж���غ���
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
