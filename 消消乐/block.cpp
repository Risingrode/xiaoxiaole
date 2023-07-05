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
	{//已经有了
		MYPOINT last_destiny = destiny.at(destiny.size() - 1);
		if (last_destiny.x == new_destiny.x && last_destiny.y == new_destiny.y)
		{//俩终点一样，好像不可能
			return;
		}
		else if (last_destiny.x == new_destiny.x || last_destiny.y == new_destiny.y)
		{//虽然因为动画的时候不做判断，所以永远不可能发生同时处理两个终点，但是写出来好像更安心一点
			//万一处理逻辑位置的时候下落了两次，就用到这个函数了，
			destiny.at(destiny.size() - 1) = new_destiny;
			return;
		}
	}
	destiny.push_back(new_destiny);//不能合并，先去前一个终点，后去下一个，速度不变
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
{//判断想和谁交换
	int tx = position.x - saved_position.x, ty = position.y - saved_position.y;
	//tx,ty是偏移量，如果大于半个方块就判断为打算移动
	tx = tx > blocksize / 2 ? 1 : tx < -blocksize / 2 ? -1 : 0;
	ty = ty > blocksize / 2 ? 1 : ty < -blocksize / 2 ? -1 : 0;
	tx = ty ? 0 : tx;
	MYPOINT rslt(ty, tx);//下标和坐标是反的
	cout << "dir (" << ty << ',' << tx << ") ";
	return rslt;
}

bool BlockDrawLayer::atDestiny()
{
	return destiny.empty();
}

void BlockDrawLayer::move()
{//计算移动后的位置
	int gravity = 1;//加速度大小恒为1，简单
	speed = speed + gravity;
	if (atDestiny()) {//没有需要处理的移动的动画
		return;
	}
	//分解速度向量赋给x和y
	MYPOINT now_destiny = destiny.at(0);
	float distx = now_destiny.x - position.x;
	float disty = now_destiny.y - position.y;//计算速度分解
	float speedx, speedy;
	int signx = now_destiny.x - position.x >= 0 ? 1 : -1;
	int signy = now_destiny.y - position.y >= 0 ? 1 : -1;//速度方向
	if (now_destiny.x - position.x && now_destiny.y - position.y)
	{//防止被除数为0
		//vx :vy = distx:disty.设vx = distx/disty * x, vy = x 
		float ratio = abs((distx / disty));
		float xnum = sqrt(speed * speed / ratio);
		speedx = xnum * ratio * signx;
		speedy = xnum * signy;
	}
	else
	{//只有一个方向的速度
		speedx = now_destiny.x - position.x == 0 ? 0 : (float)speed * signx;
		speedy = now_destiny.y - position.y == 0 ? 0 : (float)speed * signy;
	}
	if (abs(position.x - now_destiny.x) <= abs(speedx))
	{//x轴判断距离，若距离比一次更新的速度小，直接到达终点
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
	{//到达终点，清除终点
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
{//计算跟随鼠标移动时应该抵达的位置
	int tx = drawlayer->saved_position.x, ty = drawlayer->saved_position.y;
	//保证方块在十字区域内移动
	if (mx + drawlayer->sidex < tx - drawlayer->blocksize) {//这里是极端位置，不超出该范围，本来应该计算斜率，但是这样简单点，好写
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
	else {//鼠标没有出范围
		int abx = mx - tx, aby = my - ty;
		if (aby < abx && aby > -abx || aby < -abx && aby > abx) {//计算斜率以计算移动方向
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
{//移动逻辑处理
	switch (on_anime)
	{
	case 1:
		if (!drawlayer->atDestiny()) {//没到终点，继续移动
			drawlayer->move();
		}
		else {//到终点了，设置不在动画，速度归零
			on_anime = 0;
			drawlayer->speed = 0;
		}
		break;
	case 2://需要播放消失动画，这个动画优先级低于移动，所以放在后面，2
		if (drawlayer->blocksize > 2) {//消失动画未结束
			if (drawlayer->atDestiny()) {//如果还在移动，就不开始消失
				drawlayer->blocksize -= 2;
			}
			else {//在移动，就先移动
				drawlayer->move();
			}
		}
		else {//消失结束
			on_anime = 0;
			drawlayer->blocksize = 0;//用来检测是否消失
		}
		break;
	case 0://没在动画
		if (!drawlayer->atDestiny()) {//如果没到终点（其实是添加了新的终点
			on_anime = 1;
			drawlayer->move();//重置标记，继续移动
		}
	default:
		break;
	}
}

void Block::fade()
{
	on_anime = 2;//标记开始消失
}

void Block::savePosition()
{
	drawlayer->saved_position = drawlayer->position;//保存位置
}

void Block::goBack()
{
	drawlayer->addDestiny(drawlayer->saved_position);//回到保存位置（播放动画）
}
