#include<easyx.h>
#include<conio.h>
#include<vector>
#include"gamewindow.h"/*..\src\mywindows\*/
using namespace std;
gameWindow gamewindow(0, 0, 800, 600);
int g_end;

void init()
{
	gamewindow.gm.init();
}

void startloop()//开始页面循环
{
	while (1)
	{
		Sleep(2);//执行挂起一段时间，也就是等待一段时间在继续执行,()里为毫秒
		cleardevice();//EasyX这个函数使用当前背景色清空绘图设备。
		int centx = 370, centy = 294;//控制开始页面点击开始的位置
		int polyx = 400, polyy = 300;//控制开始页面图形的位置
		int change = rand() % 1000;
		if (change >= 95) //抖动设置
		{
			polyx += rand() % 5;
			polyy += rand() % 5;
		}
		if (_kbhit())//检查控制台窗口的按键是否被按下
		{
			return ;
		}
		outtextxy(centx , centy , L"空格开始");//这个函数用于在指定位置输出字符串
		//绘制开始页面图形
		POINT pts[] = { {polyx - 50,polyy},{polyx,polyy - 50},{polyx + 50,polyy},{polyx,polyy + 50} };
		POINT pts2[] = { {polyx - 45,polyy},{polyx,polyy - 45},{polyx + 45,polyy},{polyx,polyy + 48} };
		// 
		polygon(pts, 4);//EasyX用于画无填充的多边形
		polygon(pts2, 4);

		// 画出上下两边的线
		line(polyx - 80, polyy + 6, polyx - 51, polyy + 6);//EasyX用于画直线
		line(polyx + 80, polyy - 6, polyx + 51, polyy - 6);

		// 画出左右两边的线
		line(0, polyy, polyx - 60, polyy);
		line(polyx + 60, polyy, 800, polyy);

		putpixel(polyx - 55, polyy, WHITE);//EasyX用于画点
		putpixel(polyx + 55, polyy, WHITE);
		putpixel(polyx - 85, polyy - 6, WHITE);
		putpixel(polyx + 85, polyy + 6, WHITE);
		FlushBatchDraw();//EasyX用于执行未完成的绘制任务
	}
}

int main()
{
	initgraph(800, 600);//EasyX用于初始化绘图窗口
	BeginBatchDraw();//EasyX用于开始批量绘图。执行后，任何绘图操作都将暂时不输出到绘图窗口上，直到执行 FlushBatchDraw 或 EndBatchDraw 才将之前的绘图输出
	init();
	startloop();
	gamewindow.start();
	EndBatchDraw();//EasyX用于结束批量绘制，并执行未完成的绘制任务
	_getch();//_getch() 函数读入一个字符时 不用 enter 代码会继续跑
	closegraph();//EasyX用于关闭绘图窗口
}
