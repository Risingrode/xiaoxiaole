#include"gameWindow.h"

gameWindow::gameWindow()
{

}


gameWindow::gameWindow(int x, int y, int w, int h) :drawArea(x, y, w, h)
{

}

void gameWindow::start()
{
	MOUSEMSG m;
	while (1)
	{
		Sleep(3);//动画速度/响应速度
		cleardevice();//由于是最高层，要由它清除整个图像
		show();//从第一层开始画，每个组件只画自己。
		if (PeekMouseMsg(&m, 1))//EaysX用于获取鼠标消息，1表示不等待，0表示等待
		{//判断该把信息传给谁。由于这个窗口自己并不需要响应操作，直接传就行
			if (gm.game.onAnime() && m.uMsg != WM_RBUTTONDOWN)
			{//在动画，不响应操作
				FlushMouseMsgBuffer();//EaysX用于清空鼠标消息缓冲区
			}
			else if (gm.inArea(m.x, m.y))
			{//现在只有一个组件
				gm.dealMsg(m);
			}
		}
		FlushBatchDraw();//本次循环结束，绘图结束，显示图像
	}
}

void gameWindow::show()
{
	gm.show();
}
