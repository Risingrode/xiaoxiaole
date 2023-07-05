#include"gameManager.h"

void gameManager::init()
{
	game.init(px + 100, py + 100, 8, 6);
}

bool gameManager::inArea(int tx, int ty)
{
	if (tx >= px && tx <= px + width && ty >= py && ty <= py + height)
	{
		return 1;
	}
	return 0;
}

void gameManager::show()
{
	game.show();
}

void gameManager::placeAt(int tx, int ty)
{

}

void gameManager::dealMsg(MOUSEMSG m)
{
	if (game.inArea(m.x, m.y))
	{//点击到游戏区域，传递
		//game.eraseTest();
		game.dealMsg(m);
	}
}
