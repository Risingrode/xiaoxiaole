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
	score = 0;//����Ϊ0
	blocksize = 50;//�����С50
	width = blocksize * col;
	height = blocksize * row;//������Ӧ����
	focusx = 0, focusy = 0;//ѡ�񷽿���±�
	rowNum = row;
	colNum = col;//��¼��С
	const int colorNum = 6;//��ɫ����
	placeAt(tx, ty);//����λ��

	COLORREF tcolorlist[colorNum] = { RGB(255, 99, 71),  RGB(102, 204, 255), RGB(127,255,0), RGB(255, 255, 0) , RGB(192, 48, 198), RGB(0, 153, 139) };
	for (int i = 0; i < colorNum; i++)
	{//��ʼ����ɫ�б�
		colorlist.addColor(tcolorlist[i]);
	}
	//��ʼ����ͼ
	blockMap = new Block * *[rowNum];
	for (int i = 0; i < rowNum; i++)
		blockMap[i] = new Block * [colNum];

	for (int i = 0; i < rowNum; i++)
		for (int j = 0; j < colNum; j++)
			blockMap[i][j] = NULL;
	//�������з���
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
{//�Ƿ���������
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
		{//���û��ѡ�������ķ��񣨱���Ҳ������ͬʱѡ������
			on_focusing = 1;//���±�־
			focusy = (m.x - px) / blocksize;
			focusx = (m.y - py) / blocksize;//������������
			cout << '(' << focusx << ',' << focusy << ')' << " ";//���Ե������
			blockMap[focusx][focusy]->drawlayer->getSide(m.x, m.y);//ȡ�õ��ƫ����
			focused_location.x = focusx, focused_location.y = focusy;//��¼�������
			blockMap[focusx][focusy]->savePosition();//ʹ����ķ����¼�Լ�Ӧ�ûص���λ��
		}
		break;
	case WM_MOUSEMOVE:
		if (on_focusing)
		{//���ѡ���˷��飬������Ӧ��ֱ�Ӱѵ�ǰ��������Ϊ����λ�ü�ƫ������Ӧ�ûص���λ����������µ�ʱ���¼��
			blockMap[focused_location.x][focused_location.y]->mouseMove(m.x, m.y);
		}
		break;
	case WM_LBUTTONUP:
		if (on_focusing) {//���ж���û��ѡ�У���ֹ���ĵط��ƹ������ж�
			on_focusing = 0;
			MYPOINT mpoint(focusx, focusy);
			if (!swapBlock(mpoint, blockMap[focusx][focusy]->drawlayer->closeTo(), 1))
			{//���Խ�����������ܽ���,�ٷŻ�ȥ
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
{//����������ͼ��������ڶ����ķ���1
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
{//�����Զ��¼�
	//û�в��Ŷ���ʱ�������ݣ����򲥷Ŷ���
	if (!onAnime())
	{//û�ڲ��Ŷ���
		dealLogicalData();
	}
	//������
	dealAnimeData();
}

void Gamer::dealLogicalData()
{//�����߼����ݲ���
	eraseFadedBlocks();//������ʧ����������ķ���
	moveAllBlock();//�ټ����û����Ҫ�ƶ���,ָ��Ҫ�����
	genAllBlock();//�����û�пյ�����
	judgeUpdate();//���ı�ķ�����û�п���������
	if (!onAnime())
	{//������ǲ��ǽ����ˣ�û�п���������
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
{//���µ�ͼ���з���Ķ������ݣ�������ʾ��
	for (int i = 0; i < rowNum; i++)
	{
		for (int j = 0; j < colNum; j++)
		{//����ÿ�������Լ�����Ϣ
			if (!on_focusing || (on_focusing && !(i == focused_location.x && j == focused_location.y)))
			{//���Ǳ�ѡ�еģ���ѡ�еķ���ĸ��������봦���ǣ������Զ����µĲ���
				if (blockMap[i][j] && blockMap[i][j]->drawlayer)
				{//��ȫ���
					blockMap[i][j]->move();
				}
			}
		}
	}
}

void Gamer::show()
{
	dealAutoData();//�ȸ������ݣ��ٸ������ݻ�ͼ
	setlinecolor(WHITE);//���û�ͼ����
	rectangle(px - 5, py - 5, px + width + 5, py + height + 5);
	rectangle(px - 7, py - 7, px + width + 7, py + height + 7);//�Ȼ���ͼ�߿�
	HRGN rgn = CreateRectRgn(px, py, px + blocksize * colNum + 1, py + blocksize * rowNum + 1);
	setcliprgn(rgn);//��ֹС������Ƴ���
	DeleteObject(rgn);// ����ʹ�� rgn������ rgn ռ�õ�ϵͳ��Դ
	showMap();//�Ȼ���ͼ����
	showBlocks();//��ÿ��С����
	setcliprgn(NULL);//ȥ���ü���
	showScore();//��ʾ����

}
void Gamer::showScore()
{
	settextstyle(36, 0, L"΢���ź�", 0, 0, 400, false, false, false, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, PROOF_QUALITY, DEFAULT_PITCH);//EasyX�������õ�ǰ������ʽ
	settextcolor(WHITE);//EasyX�������õ�ǰ������ɫ
	wchar_t s[20];
	swprintf_s(s, L"%d �֡���", score);
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
{//������
	for (int i = 0; i < rowNum; i++) {
		for (int j = 0; j < colNum; j++) {
			if (!on_focusing || (on_focusing && !(i == focused_location.x && j == focused_location.y)))
			{//û�ڽ������߲��Ǳ�ѡ�е��Ǹ�
				if (blockMap[i][j]) {//���ǿյģ����������
					blockMap[i][j]->show();
				}
			}
		}
	}
	if (on_focusing) {//��ѡ�е���󻭣�ʡ�ı�����
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
{//���䴦��
	for (int col = 0; col < colNum; col++)
	{//�ҿյģ����滹�з���ģ�����һ�ж�������
		for (int row = 1; row < rowNum; row++)
		{//��һ�Ų��ÿ���û�п����
			if (!blockMap[row][col] && blockMap[row - 1][col])
			{//�յģ����滹�з����
				int bot;
				for (bot = row; bot < rowNum; bot++)
				{//�ӵ�һ���յĿ�ʼ��
					if (blockMap[bot][col]) {//�ҵ���
						break;
					}
				}
				int bnum = bot - row;//�ж��ٿյ�
				bot--;//�����Ǹ��з����
				for (int t = bot; t >= bnum; t--)
				{//�������������
					moveBlockBtoA(t, col, t - bnum, col);//�����������
				}
				//break;//����ÿһ�п����кܶ�أ����Բ���ֱ���˳������ü������ǲ��ǻ��п��������
			}
		}
	}
}

void Gamer::genAllBlock()
{//��ʱ���ɣ�����ʲô
	for (int col = 0; col < colNum; col++)
	{//����ÿһ��
		if (blockMap[0][col] == NULL)
		{//�����һ�еĵ�һ�������ǿյ�
			int blocknum = 1;//��Ҫ���ĸ�����
			while (blocknum < rowNum && blockMap[blocknum][col] == NULL)
			{//�ҵ���һ����Ҫ�Ӷ��ٷ���
				blocknum++;
			}
			for (int row = 0; row < blocknum; row++)
			{//������һ�еķ���
				genBlock(row, col, blocknum);
			}
		}
	}
}

void Gamer::genBlock(int locx, int locy, int blocknum)
{
	blockMap[locx][locy] = new Block;//�����·���
	blockColor tcolorNo = colorlist.getRandColor();//����ȡһ����ɫ
	blockMap[locx][locy]->colorNo = tcolorNo;
	blockMap[locx][locy]->RGBcolor = colorlist.data[tcolorNo];//������ɫ
	MYPOINT new_destiny(px + locy * blocksize + blocksize / 2, py + locx * blocksize + blocksize / 2);//����Ҫ�����λ��
	blockMap[locx][locy]->setPosition(new_destiny.x, py - (blocknum - locx) * blocksize + blocksize / 2);//����ͼ������λ��
	blockMap[locx][locy]->drawlayer->addDestiny(new_destiny);//��Ӷ����յ�
}

void Gamer::eraseFadedBlocks()
{//ɾ����ʧ����������ķ���
	for (int i = rowNum - 1; i >= 0; i--)
	{//�����һ�п�ʼ��
		for (int j = 0; j < colNum; j++)
		{//������������ʧ����������ģ�ɾ����Ȼ��������ĵ�������
			if (blockMap[i][j]) {//�����ڷ��飬��ȫ��֤
				if (blockMap[i][j]->on_anime == 0 && blockMap[i][j]->drawlayer->blocksize == 0)
				{//ɾ����ʧ������ģ�������û�в��Ŷ����Ҵ�СΪ0��Ϊ���п��ܵ�ԭ�����ɶ��������ü����ڲ��Ŷ������ж�
					delete blockMap[i][j];
					blockMap[i][j] = NULL;//ɾ����ָ��
				}
			}
		}
	}
}

bool Gamer::swapBlock(MYPOINT blk, MYPOINT dir, bool real_swap)
{//����judgeswap
	int indexx = blk.x + dir.x, indexy = blk.y + dir.y;
	if (indexx >= rowNum || indexx < 0 || indexy >= colNum || indexy < 0)
	{//��Ȼ��ͼ������ӦԽ�罻��������Ϊ�˰�ȫ
		return 0;
	}
	MYPOINT blk_b(indexx, indexy);
	MYPOINT blkpos = blockMap[blk.x][blk.y]->drawlayer->saved_position;
	MYPOINT blk_bpos = blockMap[indexx][indexy]->drawlayer->position;
	swap(blockMap[blk.x][blk.y], blockMap[indexx][indexy]);//����
	if (judgeSwap(blk, blk_b, 0))
	{//����
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
{//�����жϵ�ǰ�Ƿ�������
	static bool nd = 0;
	for (int i = 0; i < rowNum - 1; i++) {
		for (int j = 0; j < colNum - 1; j++) {//ÿһ��
			MYPOINT swappos(i, j), swap1dir(1, 0), swap2dir(0, 1);
			bool rightswap = swapBlock(swappos, swap1dir, 0);
			bool leftswap = swapBlock(swappos, swap2dir, 0);
			if (rightswap || leftswap) {//�ж�������ұ��±߽���������
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
{//swap = 1:��Ľ���
	if (judgeSingle(blk_a.x, blk_a.y, swap) ||
		judgeSingle(blk_b.x, blk_b.y, swap))
	{//������Խ���
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
{//����ֱ�ӷ��ط���
	//����������
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
	{//��
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
	//�������
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
