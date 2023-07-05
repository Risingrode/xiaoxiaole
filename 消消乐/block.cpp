#include"block.h"

BlockDrawLayer::BlockDrawLayer()
{
	setPos(0, 0);
	blocksize = 50;
	speed = 0;
}

BlockDrawLayer::BlockDrawLayer(int tx, int ty)
{
	setPos(tx, ty);
	blocksize = 50;
	speed = 0;
}

void BlockDrawLayer::addDestiny(MYPOINT new_destiny)
{
	if (destiny.size() > 0)
	{//�Ѿ�����
		MYPOINT last_destiny = destiny.at(destiny.size() - 1);
		if (last_destiny.x == new_destiny.x && last_destiny.y == new_destiny.y)
		{//���յ�һ�������񲻿���
			return;
		}
		else if (last_destiny.x == new_destiny.x || last_destiny.y == new_destiny.y)
		{//��Ȼ��Ϊ������ʱ�����жϣ�������Զ�����ܷ���ͬʱ���������յ㣬����д�������������һ��
			//��һ�����߼�λ�õ�ʱ�����������Σ����õ���������ˣ�
			destiny.at(destiny.size() - 1) = new_destiny;
			return;
		}
	}
	destiny.push_back(new_destiny);//���ܺϲ�����ȥǰһ���յ㣬��ȥ��һ�����ٶȲ���
}

void BlockDrawLayer::getSide(int tx, int ty)
{
	sidex = position.x - tx;
	sidey = position.y - ty;
}


void BlockDrawLayer::setPos(int tx, int ty)
{
	position.set(tx, ty);
}

MYPOINT BlockDrawLayer::closeTo()
{//�ж����˭����
	int tx = position.x - saved_position.x, ty = position.y - saved_position.y;
	//tx,ty��ƫ������������ڰ��������ж�Ϊ�����ƶ�
	tx = tx > blocksize / 2 ? 1 : tx < -blocksize / 2 ? -1 : 0;
	ty = ty > blocksize / 2 ? 1 : ty < -blocksize / 2 ? -1 : 0;
	tx = ty ? 0 : tx;
	MYPOINT rslt(ty, tx);//�±�������Ƿ���
	cout << "dir (" << ty << ',' << tx << ") ";
	return rslt;
}

bool BlockDrawLayer::atDestiny()
{
	return destiny.empty();
}

void BlockDrawLayer::move()
{//�����ƶ����λ��
	int gravity = 1;//���ٶȴ�С��Ϊ1����
	speed = speed + gravity;
	if (atDestiny()) {//û����Ҫ������ƶ��Ķ���
		return;
	}
	//�ֽ��ٶ���������x��y
	MYPOINT now_destiny = destiny.at(0);
	float distx = now_destiny.x - position.x;
	float disty = now_destiny.y - position.y;//�����ٶȷֽ�
	float speedx, speedy;
	int signx = now_destiny.x - position.x >= 0 ? 1 : -1;
	int signy = now_destiny.y - position.y >= 0 ? 1 : -1;//�ٶȷ���
	if (now_destiny.x - position.x && now_destiny.y - position.y)
	{//��ֹ������Ϊ0
		//vx :vy = distx:disty.��vx = distx/disty * x, vy = x 
		float ratio = abs((distx / disty));
		float xnum = sqrt(speed * speed / ratio);
		speedx = xnum * ratio * signx;
		speedy = xnum * signy;
	}
	else
	{//ֻ��һ��������ٶ�
		speedx = now_destiny.x - position.x == 0 ? 0 : (float)speed * signx;
		speedy = now_destiny.y - position.y == 0 ? 0 : (float)speed * signy;
	}
	if (abs(position.x - now_destiny.x) <= abs(speedx))
	{//x���жϾ��룬�������һ�θ��µ��ٶ�С��ֱ�ӵ����յ�
		position.x = now_destiny.x;
	}
	else {//
		position.x += (int)speedx;
	}
	if (abs(position.y - now_destiny.y) <= abs(speedy)) {
		position.y = now_destiny.y;
	}
	else {
		position.y += (int)speedy;
	}
	if (position.x == now_destiny.x && position.y == now_destiny.y)
	{//�����յ㣬����յ�
		destiny.erase(destiny.begin());
	}
}

Block::Block()
{
	drawlayer = new BlockDrawLayer;
	changed = 1;
	on_anime = 1;
	colorNo = blank;
	type = block;
}

Block::Block(int tx, int ty)
{
	drawlayer = new BlockDrawLayer(tx, ty);
}

Block::~Block()
{
	if (drawlayer) {
		delete drawlayer;
	}
}

void Block::show()
{
	int l = drawlayer->position.x - drawlayer->blocksize / 2, t = drawlayer->position.y - drawlayer->blocksize / 2;
	int r = drawlayer->position.x + drawlayer->blocksize / 2, b = drawlayer->position.y + drawlayer->blocksize / 2;
	setlinecolor(BLACK);
	setfillcolor(RGBcolor);
	fillrectangle(l, t, r, b);
}

void Block::mouseMove(int mx, int my)
{//�����������ƶ�ʱӦ�õִ��λ��
	int tx = drawlayer->saved_position.x, ty = drawlayer->saved_position.y;
	//��֤������ʮ���������ƶ�
	if (mx + drawlayer->sidex < tx - drawlayer->blocksize) {//�����Ǽ���λ�ã��������÷�Χ������Ӧ�ü���б�ʣ����������򵥵㣬��д
		drawlayer->setPos(tx - drawlayer->blocksize, ty);
	}
	else if (mx + drawlayer->sidex > tx + drawlayer->blocksize) {
		drawlayer->setPos(tx + drawlayer->blocksize, ty);
	}
	else if (my + drawlayer->sidey < ty - drawlayer->blocksize) {
		drawlayer->setPos(tx, ty - drawlayer->blocksize);
	}
	else if (my + drawlayer->sidey > ty + drawlayer->blocksize) {
		drawlayer->setPos(tx, ty + drawlayer->blocksize);
	}
	else {//���û�г���Χ
		int abx = mx - tx, aby = my - ty;
		if (aby < abx && aby > -abx || aby < -abx && aby > abx) {//����б���Լ����ƶ�����
			drawlayer->setPos(mx + drawlayer->sidex, ty);
		}
		else {
			drawlayer->setPos(tx, my + drawlayer->sidey);
		}
	}
}

void Block::setPosition(int tx, int ty)
{
	drawlayer->setPos(tx, ty);
}

void Block::move()
{//�ƶ��߼�����
	switch (on_anime)
	{
	case 1:
		if (!drawlayer->atDestiny()) {//û���յ㣬�����ƶ�
			drawlayer->move();
		}
		else {//���յ��ˣ����ò��ڶ������ٶȹ���
			on_anime = 0;
			drawlayer->speed = 0;
		}
		break;
	case 2://��Ҫ������ʧ����������������ȼ������ƶ������Է��ں��棬2
		if (drawlayer->blocksize > 2) {//��ʧ����δ����
			if (drawlayer->atDestiny()) {//��������ƶ����Ͳ���ʼ��ʧ
				drawlayer->blocksize -= 2;
			}
			else {//���ƶ��������ƶ�
				drawlayer->move();
			}
		}
		else {//��ʧ����
			on_anime = 0;
			drawlayer->blocksize = 0;//��������Ƿ���ʧ
		}
		break;
	case 0://û�ڶ���
		if (!drawlayer->atDestiny()) {//���û���յ㣨��ʵ��������µ��յ�
			on_anime = 1;
			drawlayer->move();//���ñ�ǣ������ƶ�
		}
	default:
		break;
	}
}

void Block::fade()
{
	on_anime = 2;//��ǿ�ʼ��ʧ
}

void Block::savePosition()
{
	drawlayer->saved_position = drawlayer->position;//����λ��
}

void Block::goBack()
{
	drawlayer->addDestiny(drawlayer->saved_position);//�ص�����λ�ã����Ŷ�����
}
