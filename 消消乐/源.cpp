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

void startloop()//��ʼҳ��ѭ��
{
	while (1)
	{
		Sleep(2);//ִ�й���һ��ʱ�䣬Ҳ���ǵȴ�һ��ʱ���ڼ���ִ��,()��Ϊ����
		cleardevice();//EasyX�������ʹ�õ�ǰ����ɫ��ջ�ͼ�豸��
		int centx = 370, centy = 294;//���ƿ�ʼҳ������ʼ��λ��
		int polyx = 400, polyy = 300;//���ƿ�ʼҳ��ͼ�ε�λ��
		int change = rand() % 1000;
		if (change >= 95) //��������
		{
			polyx += rand() % 5;
			polyy += rand() % 5;
		}
		if (_kbhit())//������̨���ڵİ����Ƿ񱻰���
		{
			return ;
		}
		outtextxy(centx , centy , L"�ո�ʼ");//�������������ָ��λ������ַ���
		//���ƿ�ʼҳ��ͼ��
		POINT pts[] = { {polyx - 50,polyy},{polyx,polyy - 50},{polyx + 50,polyy},{polyx,polyy + 50} };
		POINT pts2[] = { {polyx - 45,polyy},{polyx,polyy - 45},{polyx + 45,polyy},{polyx,polyy + 48} };
		// 
		polygon(pts, 4);//EasyX���ڻ������Ķ����
		polygon(pts2, 4);

		// �����������ߵ���
		line(polyx - 80, polyy + 6, polyx - 51, polyy + 6);//EasyX���ڻ�ֱ��
		line(polyx + 80, polyy - 6, polyx + 51, polyy - 6);

		// �����������ߵ���
		line(0, polyy, polyx - 60, polyy);
		line(polyx + 60, polyy, 800, polyy);

		putpixel(polyx - 55, polyy, WHITE);//EasyX���ڻ���
		putpixel(polyx + 55, polyy, WHITE);
		putpixel(polyx - 85, polyy - 6, WHITE);
		putpixel(polyx + 85, polyy + 6, WHITE);
		FlushBatchDraw();//EasyX����ִ��δ��ɵĻ�������
	}
}

int main()
{
	initgraph(800, 600);//EasyX���ڳ�ʼ����ͼ����
	BeginBatchDraw();//EasyX���ڿ�ʼ������ͼ��ִ�к��κλ�ͼ����������ʱ���������ͼ�����ϣ�ֱ��ִ�� FlushBatchDraw �� EndBatchDraw �Ž�֮ǰ�Ļ�ͼ���
	init();
	startloop();
	gamewindow.start();
	EndBatchDraw();//EasyX���ڽ����������ƣ���ִ��δ��ɵĻ�������
	_getch();//_getch() ��������һ���ַ�ʱ ���� enter ����������
	closegraph();//EasyX���ڹرջ�ͼ����
}
