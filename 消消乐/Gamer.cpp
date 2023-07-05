#include"Gamer.h"
Gamer::~Gamer()
{
	for (int i = 0; i < rowNum; i++)
	{
		for (int j = 0; j < colNum; j++)
		{
			if (blockMap[i][j] != 0)
			{
				delete blockMap[i][j];
			}
		}
	}
}
void Gamer::init(int tx, int ty, int col, int row)
{
	need_help = 0;
	score = 0;//分数为0
	blocksize = 50;//方格大小50
	width = blocksize * col;
	height = blocksize * row;//计算响应区域
	focusx = 0, focusy = 0;//选择方块的下标
	rowNum = row;
	colNum = col;//记录大小
	const int colorNum = 6;//颜色数量
	placeAt(tx, ty);//设置位置

	COLORREF tcolorlist[colorNum] = { RGB(255, 99, 71),  RGB(102, 204, 255), RGB(127,255,0), RGB(255, 255, 0) , RGB(192, 48, 198), RGB(0, 153, 139) };
	for (int i = 0; i < colorNum; i++)
	{//初始化颜色列表
		colorlist.addColor(tcolorlist[i]);
	}
	//初始化地图
	blockMap = new Block * *[rowNum];
	for (int i = 0; i < rowNum; i++)
		blockMap[i] = new Block * [colNum];

	for (int i = 0; i < rowNum; i++)
		for (int j = 0; j < colNum; j++)
			blockMap[i][j] = NULL;
	//生成所有方块
	genAllBlock();
}

void Gamer::eraseTest()
{
	int delnum = 10;
	for (int i = 0; i < delnum; i++)
	{
		int tstrow = rand() % rowNum;
		int tstcol = rand() % colNum;
		if (blockMap[tstrow][tstcol])
		{
			blockMap[tstrow][tstcol]->fade();
		}
	}
}

void Gamer::placeAt(int tx, int ty)
{
	px = tx, py = ty;
}

bool Gamer::inArea(int tx, int ty)
{//是否在区域内
	if (tx >= px && tx <= px + width && ty >= py && ty <= py + height) {
		return 1;
	}
	return 0;
}

void Gamer::dealMsg(MOUSEMSG m)
{
	switch (m.uMsg)
	{
	case WM_LBUTTONDOWN:
		if (!on_focusing)
		{//如果没有选择其他的方格（本来也不可能同时选两个）
			on_focusing = 1;//更新标志
			focusy = (m.x - px) / blocksize;
			focusx = (m.y - py) / blocksize;//计算点击的坐标
			cout << '(' << focusx << ',' << focusy << ')' << " ";//测试点击坐标
			blockMap[focusx][focusy]->drawlayer->getSide(m.x, m.y);//取得点击偏移量
			focused_location.x = focusx, focused_location.y = focusy;//记录点击坐标
			blockMap[focusx][focusy]->savePosition();//使点击的方块记录自己应该回到的位置
		}
		break;
	case WM_MOUSEMOVE:
		if (on_focusing)
		{//如果选中了方块，做出响应，直接把当前坐标设置为鼠标的位置加偏移量，应该回到的位置在左键按下的时候记录了
			blockMap[focused_location.x][focused_location.y]->mouseMove(m.x, m.y);
		}
		break;
	case WM_LBUTTONUP:
		if (on_focusing) {//先判断有没有选中，防止点别的地方移过来误判断
			on_focusing = 0;
			MYPOINT mpoint(focusx, focusy);
			if (!swapBlock(mpoint, blockMap[focusx][focusy]->drawlayer->closeTo(), 1))
			{//尝试交换，如果不能交换,再放回去
				blockMap[focusx][focusy]->goBack();
			}
		}
		break;
	case WM_RBUTTONDOWN:
		need_help = 1;
		break;
	default:
		break;
	}
}

bool Gamer::onAnime()
{//遍历整个地图，如果有在动画的返回1
	bool block_on_anime = 0;
	for (int i = 0; i < rowNum; i++)
	{
		for (int j = 0; j < colNum; j++)
		{
			if (blockMap[i][j] && blockMap[i][j]->on_anime) {
				return 1;
			}
		}
	}
	return 0;
}

void Gamer::dealAutoData()
{//处理自动事件
	//没有播放动画时处理数据，否则播放动画
	if (!onAnime())
	{//没在播放动画
		dealLogicalData();
	}
	//处理动画
	dealAnimeData();
}

void Gamer::dealLogicalData()
{//处理逻辑数据部分
	eraseFadedBlocks();//消掉消失动画播放完的方块
	moveAllBlock();//再检测有没有需要移动的,指需要下落的
	genAllBlock();//检查有没有空的填上
	judgeUpdate();//检查改变的方块有没有可以消除的
	if (!onAnime())
	{//最后检测是不是结束了，没有可以消的了
		if (judgeIfEnd())
		{
			for (int i = 0; i < rowNum; i++)
				for (int j = 0; j < colNum; j++)
				{
					blockMap[i][j]->fade();
				}
		}
	}
}

void Gamer::dealAnimeData()
{//更新地图所有方块的动画数据（不是显示）
	for (int i = 0; i < rowNum; i++)
	{
		for (int j = 0; j < colNum; j++)
		{//处理每个方块自己的信息
			if (!on_focusing || (on_focusing && !(i == focused_location.x && j == focused_location.y)))
			{//不是被选中的，被选中的方块的更新在输入处理那，这是自动更新的部分
				if (blockMap[i][j] && blockMap[i][j]->drawlayer)
				{//安全检测
					blockMap[i][j]->move();
				}
			}
		}
	}
}

void Gamer::show()
{
	dealAutoData();//先更新数据，再根据数据画图
	setlinecolor(WHITE);//设置绘图区域
	rectangle(px - 5, py - 5, px + width + 5, py + height + 5);
	rectangle(px - 7, py - 7, px + width + 7, py + height + 7);//先画地图边框
	HRGN rgn = CreateRectRgn(px, py, px + blocksize * colNum + 1, py + blocksize * rowNum + 1);
	setcliprgn(rgn);//防止小方块绘制出框
	DeleteObject(rgn);// 不再使用 rgn，清理 rgn 占用的系统资源
	showMap();//先画地图格子
	showBlocks();//画每个小方格
	setcliprgn(NULL);//去除裁剪区
	showScore();//显示分数

}
void Gamer::showScore()
{
	settextstyle(36, 0, L"微软雅黑", 0, 0, 400, false, false, false, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, PROOF_QUALITY, DEFAULT_PITCH);//EasyX用于设置当前文字样式
	settextcolor(WHITE);//EasyX用于设置当前文字颜色
	wchar_t s[20];
	swprintf_s(s, L"%d 分　　", score);
	outtextxy(px + colNum * blocksize + 100, py + rowNum * blocksize - blocksize, s);
}

void Gamer::showMap()
{
	for (int i = 0; i < rowNum; i++)
	{
		for (int j = 0; j < colNum; j++)
		{
			rectangle(px + blocksize * j, py + blocksize * i,
				px + blocksize * (j + 1), py + blocksize * (i + 1));
		}
	}
}

void Gamer::showBlocks()
{//画方块
	for (int i = 0; i < rowNum; i++) {
		for (int j = 0; j < colNum; j++) {
			if (!on_focusing || (on_focusing && !(i == focused_location.x && j == focused_location.y)))
			{//没在交互或者不是被选中的那个
				if (blockMap[i][j]) {//不是空的，画这个方块
					blockMap[i][j]->show();
				}
			}
		}
	}
	if (on_focusing) {//被选中的最后画，省的被覆盖
		blockMap[focused_location.x][focused_location.y]->show();
	}
}

void Gamer::moveBlockBtoA(int ax, int ay, int bx, int by)
{
	if (blockMap[ax][ay] == NULL && blockMap[bx][by])
	{
		blockMap[ax][ay] = blockMap[bx][by];
		blockMap[bx][by] = NULL;
		MYPOINT new_destiny(px + blocksize * ay + blocksize / 2, py + blocksize * ax + blocksize / 2);
		blockMap[ax][ay]->drawlayer->addDestiny(new_destiny);
		blockMap[ax][ay]->changed = 1;
	}
}

void Gamer::moveAllBlock()
{//下落处理
	for (int col = 0; col < colNum; col++)
	{//找空的，上面还有方块的，把这一列都拉到底
		for (int row = 1; row < rowNum; row++)
		{//第一排不用看，没有可填的
			if (!blockMap[row][col] && blockMap[row - 1][col])
			{//空的，上面还有方块的
				int bot;
				for (bot = row; bot < rowNum; bot++)
				{//从第一个空的开始找
					if (blockMap[bot][col]) {//找到底
						break;
					}
				}
				int bnum = bot - row;//有多少空的
				bot--;//不算那个有方块的
				for (int t = bot; t >= bnum; t--)
				{//把上面的拉下来
					moveBlockBtoA(t, col, t - bnum, col);//这里带更新了
				}
				//break;//由于每一行可能有很多截，所以不能直接退出，还得继续看是不是还有可以下落的
			}
		}
	}
}

void Gamer::genAllBlock()
{//何时生成，生成什么
	for (int col = 0; col < colNum; col++)
	{//对于每一列
		if (blockMap[0][col] == NULL)
		{//如果这一列的第一个格子是空的
			int blocknum = 1;//需要填充的格子数
			while (blocknum < rowNum && blockMap[blocknum][col] == NULL)
			{//找到这一列需要加多少方块
				blocknum++;
			}
			for (int row = 0; row < blocknum; row++)
			{//生成这一列的方块
				genBlock(row, col, blocknum);
			}
		}
	}
}

void Gamer::genBlock(int locx, int locy, int blocknum)
{
	blockMap[locx][locy] = new Block;//生成新方块
	blockColor tcolorNo = colorlist.getRandColor();//随意取一个颜色
	blockMap[locx][locy]->colorNo = tcolorNo;
	blockMap[locx][locy]->RGBcolor = colorlist.data[tcolorNo];//设置颜色
	MYPOINT new_destiny(px + locy * blocksize + blocksize / 2, py + locx * blocksize + blocksize / 2);//计算要到达的位置
	blockMap[locx][locy]->setPosition(new_destiny.x, py - (blocknum - locx) * blocksize + blocksize / 2);//计算图像生成位置
	blockMap[locx][locy]->drawlayer->addDestiny(new_destiny);//添加动画终点
}

void Gamer::eraseFadedBlocks()
{//删除消失动画播放完的方块
	for (int i = rowNum - 1; i >= 0; i--)
	{//从最后一行开始找
		for (int j = 0; j < colNum; j++)
		{//从下往上找消失动画播放完的，删掉，然后让上面的掉下来。
			if (blockMap[i][j]) {//检测存在方块，安全验证
				if (blockMap[i][j]->on_anime == 0 && blockMap[i][j]->drawlayer->blocksize == 0)
				{//删掉消失播放完的，条件是没有播放动画且大小为0，为了有可能的原地生成动画，还得加上在播放动画的判断
					delete blockMap[i][j];
					blockMap[i][j] = NULL;//删掉，指空
				}
			}
		}
	}
}

bool Gamer::swapBlock(MYPOINT blk, MYPOINT dir, bool real_swap)
{//调用judgeswap
	int indexx = blk.x + dir.x, indexy = blk.y + dir.y;
	if (indexx >= rowNum || indexx < 0 || indexy >= colNum || indexy < 0)
	{//虽然地图不会响应越界交换，但是为了安全
		return 0;
	}
	MYPOINT blk_b(indexx, indexy);
	MYPOINT blkpos = blockMap[blk.x][blk.y]->drawlayer->saved_position;
	MYPOINT blk_bpos = blockMap[indexx][indexy]->drawlayer->position;
	swap(blockMap[blk.x][blk.y], blockMap[indexx][indexy]);//交换
	if (judgeSwap(blk, blk_b, 0))
	{//交换
		if (real_swap)
		{
			blockMap[blk.x][blk.y]->changed = 1;
			blockMap[blk.x][blk.y]->drawlayer->addDestiny(blkpos);
			blockMap[indexx][indexy]->changed = 1;
			blockMap[indexx][indexy]->drawlayer->addDestiny(blk_bpos);
			return 1;
		}
		else
		{
			swap(blockMap[blk.x][blk.y], blockMap[indexx][indexy]);
			return 1;
		}

	}
	else {
		swap(blockMap[blk.x][blk.y], blockMap[indexx][indexy]);
	}
	return 0;
}

bool Gamer::judgeIfEnd()
{//用来判断当前是否是死局
	static bool nd = 0;
	for (int i = 0; i < rowNum - 1; i++) {
		for (int j = 0; j < colNum - 1; j++) {//每一个
			MYPOINT swappos(i, j), swap1dir(1, 0), swap2dir(0, 1);
			bool rightswap = swapBlock(swappos, swap1dir, 0);
			bool leftswap = swapBlock(swappos, swap2dir, 0);
			if (rightswap || leftswap) {//判断如果和右边下边交换能消除
				if (need_help)
				{
					nd = !nd;

					need_help = 0;
					//cout << '(' << swappos.x << ',' << swappos.y << ')' << endl;
				}
				if (rightswap && nd)
				{
					blockMap[i][j]->savePosition();
					swapBlock(swappos, swap1dir, 1);
					return 0;
				}
				if (leftswap && nd)
				{
					blockMap[i][j]->savePosition();
					swapBlock(swappos, swap2dir, 1);
					return 0;
				}


				return 0;
			}
		}
	}
	return 1;
}
bool Gamer::judgeSwap(int x1, int y1, int x2, int y2, bool swap)
{
	MYPOINT pa(x1, y1), pb(x2, y2);
	return judgeSwap(pa, pb, swap);

}

bool Gamer::judgeSwap(MYPOINT blk_a, MYPOINT blk_b, bool swap)
{//swap = 1:真的交换
	if (judgeSingle(blk_a.x, blk_a.y, swap) ||
		judgeSingle(blk_b.x, blk_b.y, swap))
	{//如果可以交换
		return 1;
	}
	return 0;
}

bool Gamer::judgeUpdate()
{
	for (int i = 0; i < rowNum; i++) {
		for (int j = 0; j < colNum; j++) {
			if (blockMap[i][j] && blockMap[i][j]->changed) {
				judgeSingle(i, j, 1);
			}
		}
	}
	return 0;
}

bool Gamer::judgeSingle(int x, int y, bool need_del)
{//可以直接返回分数
	//向左右搜索
	if (blockMap[x][y]->on_anime)
	{
		return 0;
	}
	int tempx = x, tempy = y;
	bool** visited = new bool* [rowNum];
	for (int i = 0; i < rowNum; i++)
	{
		visited[i] = new bool[colNum];
	}
	for (int i = 0; i < rowNum; i++)
	{
		for (int j = 0; j < colNum; j++)
		{
			visited[i][j] = 0;
		}
	}
	visited[tempx][tempy] = 1;
	int horiz_num = 1, verti_num = 1;
	vector<MYPOINT> h_same, v_same;
	MYPOINT newp(tempx, tempy);
	h_same.push_back(newp);
	v_same.push_back(newp);
	int hlast = 0, vlast = 0;

	while (hlast < h_same.size() || vlast < v_same.size())
	{//当
		if (hlast < h_same.size())
		{
			tempx = h_same.at(hlast).x;
			tempy = h_same.at(hlast).y;
			hlast++;
			int th_num = 1;
			for (int col = tempy - 1; col >= 0; col--)
				if (blockMap[tempx][col] && blockMap[tempx][col]->colorNo == blockMap[tempx][tempy]->colorNo && !visited[tempx][col]) th_num++;
				else break;

			for (int col = tempy + 1; col < colNum; col++)
				if (blockMap[tempx][col] && blockMap[tempx][col]->colorNo == blockMap[tempx][tempy]->colorNo && !visited[tempx][col]) th_num++;
				else break;

			if (th_num >= 3) {
				horiz_num += th_num - 1;
				for (int col = tempy - 1; col >= 0; col--)
					if (blockMap[tempx][col] && blockMap[tempx][col]->colorNo == blockMap[tempx][tempy]->colorNo && !visited[tempx][col]) {
						visited[tempx][col] = 1;
						newp.set(tempx, col);
						v_same.push_back(newp);
					}
					else break;
				for (int col = tempy + 1; col < colNum; col++)
					if (blockMap[tempx][col] && blockMap[tempx][col]->colorNo == blockMap[tempx][tempy]->colorNo && !visited[tempx][col]) {
						visited[tempx][col] = 1;
						newp.set(tempx, col);
						v_same.push_back(newp);
					}
					else break;
			}
		}
		if (vlast < v_same.size())
		{
			tempx = v_same.at(vlast).x;
			tempy = v_same.at(vlast).y;
			vlast++;
			int tv_num = 1;
			for (int row = tempx - 1; row >= 0; row--)
				if (blockMap[row][tempy] && blockMap[row][tempy]->colorNo == blockMap[tempx][tempy]->colorNo && !visited[row][tempy]) tv_num++;
				else break;
			for (int row = tempx + 1; row < rowNum; row++)
				if (blockMap[row][tempy] && blockMap[row][tempy]->colorNo == blockMap[tempx][tempy]->colorNo && !visited[row][tempy]) tv_num++;
				else break;
			if (tv_num >= 3) {
				verti_num += tv_num - 1;
				for (int row = tempx - 1; row >= 0; row--)
					if (blockMap[row][tempy] && blockMap[row][tempy]->colorNo == blockMap[tempx][tempy]->colorNo && !visited[row][tempy])
					{
						visited[row][tempy] = 1;
						newp.set(row, tempy);
						h_same.push_back(newp);
					}
					else break;
				for (int row = tempx + 1; row < rowNum; row++)
					if (blockMap[row][tempy] && blockMap[row][tempy]->colorNo == blockMap[tempx][tempy]->colorNo && !visited[row][tempy])
					{
						visited[row][tempy] = 1;
						newp.set(row, tempy);
						h_same.push_back(newp);
					}
					else break;
			}
		}
	}
	int sumnum = horiz_num + verti_num - 1;
	if (sumnum < 3 && blockMap[x][y])
	{
		blockMap[x][y]->changed = 0;
		return 0;
	}
	//计算分数
	if (need_del)
	{
		score += (int)(10 + ((double)sumnum - 3) * 10 * (1 + ((double)sumnum - 3) * 0.1));
	}
	vector<MYPOINT> sum;
	for (int i = 0; i < h_same.size(); i++)
	{
		sum.push_back(h_same[i]);
	}
	for (int i = 0; i < v_same.size(); i++)
	{
		sum.push_back(v_same[i]);
	}
	if (sumnum >= 3 && need_del)
	{
		for (int i = 0; i < sum.size(); i++)
		{
			blockMap[sum[i].x][sum[i].y]->fade();
		}
	}
	for (int i = 0; i < rowNum; i++)
	{
		delete[] visited[i];
	}

	delete[] visited;

	return 1;
}
