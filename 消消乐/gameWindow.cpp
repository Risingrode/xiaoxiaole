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
		Sleep(3);//�����ٶ�/��Ӧ�ٶ�
		cleardevice();//��������߲㣬Ҫ�����������ͼ��
		show();//�ӵ�һ�㿪ʼ����ÿ�����ֻ���Լ���
		if (PeekMouseMsg(&m, 1))//EaysX���ڻ�ȡ�����Ϣ��1��ʾ���ȴ���0��ʾ�ȴ�
		{//�жϸð���Ϣ����˭��������������Լ�������Ҫ��Ӧ������ֱ�Ӵ�����
			if (gm.game.onAnime() && m.uMsg != WM_RBUTTONDOWN)
			{//�ڶ���������Ӧ����
				FlushMouseMsgBuffer();//EaysX������������Ϣ������
			}
			else if (gm.inArea(m.x, m.y))
			{//����ֻ��һ�����
				gm.dealMsg(m);
			}
		}
		FlushBatchDraw();//����ѭ����������ͼ��������ʾͼ��
	}
}

void gameWindow::show()
{
	gm.show();
}
