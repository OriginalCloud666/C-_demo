#include "Model.h"
using namespace std;

Model::Model() :top(0), left(0), down(0), right(0), cas(0)
{
	this->beginGame = creatButton(WIDTH / 2 + 80, 500, 150, 50, "START", 38, 12, RGB(252, 252, 252), RGB(98, 147, 206));
	this->endGame = creatButton(WIDTH / 2 + 420, 500, 150, 50, "END", 52, 12, RGB(252, 252, 252), RGB(98, 147, 206));
}

Model::~Model()
{
	delete this->beginGame;
	this->beginGame = nullptr;
	delete this->endGame;
	this->endGame = nullptr;
}

int& Model::getCas()
{
	return this->cas;
}

// 获取beginGame
struct Button*& Model::getButton1()
{
	return this->beginGame;
}

// 获取endGame
struct Button*& Model::getButton2()
{
	return this->endGame;
}

// 加载资源
void Model::load(IMAGE& bk, IMAGE& w1, IMAGE& w2, IMAGE& man, IMAGE& brick, IMAGE& replace, IMAGE fire[], IMAGE door[], IMAGE ghost[], IMAGE character[])
{
	// 加载背景图片
	loadimage(&bk, "./resource/picture/bk.jpg", WIDTH, HEIGHT);
	// 加载人物图片
	loadimage(&man, "./resource/picture/man.png");
	// 加载砖块图片
	loadimage(&brick, "./resource/picture/brick.jpg", GRID_SIZE, GRID_SIZE);
	// 加载替代图片
	loadimage(&replace, "./resource/picture/replace.jpg", GRID_SIZE, GRID_SIZE);
	// 加载文字图片
	loadimage(&w1, "./resource/picture/w1.png");
	loadimage(&w2, "./resource/picture/w2.png");
	// 加载火焰图片
	loadimage(fire, "./resource/picture/fire2.jpg", GRID_SIZE, GRID_SIZE);
	loadimage(fire + 1, "./resource/picture/fire1.jpg", GRID_SIZE, GRID_SIZE);
	// 加载终点图片
	loadimage(door, "./resource/picture/door2.jpg", GRID_SIZE, GRID_SIZE);
	loadimage(door + 1, "./resource/picture/door1.jpg", GRID_SIZE, GRID_SIZE);
	// 加载幽灵图片
	loadimage(ghost, "./resource/picture/ghost2.jpg", GRID_SIZE, GRID_SIZE);
	loadimage(ghost + 1, "./resource/picture/ghost1.jpg", GRID_SIZE, GRID_SIZE);
	// 加载人物图片
	loadimage(character, "./resource/picture/character2.jpg", GRID_SIZE, GRID_SIZE);
	loadimage(character + 1, "./resource/picture/character1.jpg", GRID_SIZE, GRID_SIZE);
}

// 加载音乐
void Model::bgm()
{
	mciSendString("open ./resource/music/bk.mp3 alias BGM", nullptr, 0, nullptr);
	mciSendString("play BGM repeat", nullptr, 0, nullptr);
}

// 载入PNG图并去透明部分
void Model::drawAlpha(IMAGE* picture, const int  picture_x, const int picture_y)
{
	// 变量初始化
	DWORD* dst = GetImageBuffer(); // GetImageBuffer()函数，用于获取绘图设备的显存指针，EASYX自带
	DWORD* draw = GetImageBuffer();
	DWORD* src = GetImageBuffer(picture); // 获取picture的显存指针
	int picture_width = picture->getwidth(); // 获取picture的宽度，EASYX自带
	int picture_height = picture->getheight(); // 获取picture的高度，EASYX自带
	int graphWidth = getwidth(); // 获取绘图区的宽度，EASYX自带
	int graphHeight = getheight(); // 获取绘图区的高度，EASYX自带
	int dstX = 0; // 在显存里像素的角标

	// 实现透明贴图 公式：Cp=αp*FP+(1-αp)*BP，贝叶斯定理来进行点颜色的概率计算
	for (int iy = 0; iy < picture_height; iy++)
	{
		for (int ix = 0; ix < picture_width; ix++)
		{
			int srcX = ix + iy * picture_width; // 在显存里像素的角标
			int sa = ((src[srcX] & 0xff000000) >> 24); // 0xAArrggbb;AA是透明度
			int sr = ((src[srcX] & 0xff0000) >> 16); // 获取RGB里的R
			int sg = ((src[srcX] & 0xff00) >> 8); // 获取RGB里的G
			int sb = src[srcX] & 0xff; // 获取RGB里的B
			if (ix >= 0 && ix <= graphWidth && iy >= 0 && iy <= graphHeight && dstX <= graphWidth * graphHeight)
			{
				dstX = (ix + picture_x) + (iy + picture_y) * graphWidth; // 在显存里像素的角标
				int dr = ((dst[dstX] & 0xff0000) >> 16);
				int dg = ((dst[dstX] & 0xff00) >> 8);
				int db = dst[dstX] & 0xff;
				draw[dstX] = ((sr * sa / 255 + dr * (255 - sa) / 255) << 16) // 公式：Cp=αp*FP+(1-αp)*BP；αp=sa/255 , FP=sr , BP=dr
					| ((sg * sa / 255 + dg * (255 - sa) / 255) << 8) // αp=sa/255 , FP=sg , BP=dg
					| (sb * sa / 255 + db * (255 - sa) / 255); // αp=sa/255 , FP=sb , BP=db
			}
		}
	}
}

// 创建按钮
struct Button* Model::creatButton(const int x, const int y, const int width, const int height,
	const char* text, const int dx, const int dy, COLORREF buttoncolor, COLORREF textcolor)
{
	struct Button* pButton = new struct Button;
	pButton->x = x;
	pButton->y = y;
	pButton->width = width;
	pButton->height = height;
	pButton->text = (char*)malloc(strlen(text) + 1);
	strcpy(pButton->text, text);
	pButton->dx = dx;
	pButton->dy = dy;
	pButton->buttoncolor = buttoncolor;
	pButton->textcolor = textcolor;
	return pButton;
}

// 绘制按钮
void Model::drawButton(struct Button* pButton)
{
	setbkmode(TRANSPARENT);
	setlinecolor(RGB(79, 133, 200));
	setlinestyle(PS_SOLID, 5);
	setfillcolor(pButton->buttoncolor);
	fillrectangle(pButton->x, pButton->y, pButton->x + pButton->width, pButton->y + pButton->height);
	settextcolor(pButton->textcolor);
	settextstyle(28, 0, "楷体", 0, 0, 1000, 0, 0, 0);
	outtextxy(pButton->x + pButton->dx, pButton->y + pButton->dy, pButton->text);
}

// 初始化地图
void Model::map1()
{
	// 初始化四周的墙壁
	for (int i = 11; i <= 12; i++)
	{
		this->gameMapVec[0][i][5] = WALL;
	}
	for (int i = 6; i <= 8; i++)
	{
		this->gameMapVec[0][i][30] = WALL;
	}
	for (int i = 6; i <= 7; i++)
	{
		this->gameMapVec[0][2][i] = WALL;
	}
	for (int i = 19; i <= 22; i++)
	{
		this->gameMapVec[0][13][i] = WALL;
	}
	for (int i = 28; i <= 30; i++)
	{
		this->gameMapVec[0][13][i] = WALL;
	}
	this->gameMapVec[0][3][25] = WALL;
	this->gameMapVec[0][6][11] = WALL;
	this->gameMapVec[0][20][24] = WALL;
	this->gameMapVec[0][19][10] = WALL;
	this->gameMapVec[0][12][21] = WALL;

	// 初始化人物以及终点
	this->gameMapVec[0][12][22] = DOOR;
	this->gameMapVec[0][12][20] = ROLE;
}

// 初始化地图
void Model::map2() {
	// 初始化四周的墙壁
	for (int i = 3; i <= 5; i++)
	{
		this->gameMapVec[1][i][3] = WALL;
	}
	for (int i = 8; i <= 11; i++)
	{
		this->gameMapVec[1][i][13] = WALL;
	}
	for (int i = 3; i <= 7; i++)
	{
		this->gameMapVec[1][2][i] = WALL;
	}
	this->gameMapVec[1][4][6] = WALL;
	this->gameMapVec[1][3][9] = WALL;
	this->gameMapVec[1][1][29] = WALL;
	this->gameMapVec[1][1][33] = WALL;
	this->gameMapVec[1][5][34] = WALL;
	this->gameMapVec[1][8][12] = WALL;
	this->gameMapVec[1][13][7] = WALL;
	this->gameMapVec[1][10][8] = WALL;
	this->gameMapVec[1][19][8] = WALL;
	this->gameMapVec[1][14][9] = WALL;
	this->gameMapVec[1][20][7] = WALL;
	this->gameMapVec[1][11][12] = WALL;
	this->gameMapVec[1][13][26] = WALL;
	this->gameMapVec[1][17][25] = WALL;
	this->gameMapVec[1][14][30] = WALL;
	this->gameMapVec[1][18][33] = WALL;
	this->gameMapVec[1][21][29] = WALL;

	// 初始化火焰
	for (int i = 4; i <= 6; i++)
	{
		this->gameMapVec[1][i][5] = FIRE;
	}
	for (int i = 17; i <= 19; i++)
	{
		this->gameMapVec[1][i][7] = FIRE;
	}
	for (int i = 32; i <= 34; i++)
	{
		this->gameMapVec[1][3][i] = FIRE;
	}
	for (int i = 31; i <= 32; i++)
	{
		this->gameMapVec[1][14][i] = FIRE;
	}
	for (int i = 34; i <= 35; i++)
	{
		this->gameMapVec[1][14][i] = FIRE;
	}
	for (int i = 25; i <= 30; i++)
	{
		this->gameMapVec[1][19][i] = FIRE;
	}

	// 初始化幽灵
	this->gameMapVec[1][10][30] = GHOST;
	this->gameMapVec[1][6][10] = GHOST;

	// 初始化人物以及终点
	this->gameMapVec[1][18][8] = DOOR;
	this->gameMapVec[1][18][6] = ROLE;
}

// 初始化地图
void Model::map3()
{
	// 初始化四周的墙壁
	for (int i = 11; i <= 13; i++)
	{
		this->gameMapVec[2][i][2] = WALL;
	}
	for (int i = 20; i <= 22; i++)
	{
		this->gameMapVec[2][i][5] = WALL;
	}
	for (int i = 15; i <= 17; i++)
	{
		this->gameMapVec[2][i][10] = WALL;
	}
	for (int i = 1; i <= 2; i++)
	{
		this->gameMapVec[2][i][12] = WALL;
	}
	for (int i = 19; i <= 21; i++)
	{
		this->gameMapVec[2][i][15] = WALL;
	}
	for (int i = 3; i <= 5; i++)
	{
		this->gameMapVec[2][i][18] = WALL;
	}
	for (int i = 21; i <= 23; i++)
	{
		this->gameMapVec[2][i][20] = WALL;
	}
	for (int i = 10; i <= 12; i++)
	{
		this->gameMapVec[2][i][31] = WALL;
	}
	for (int i = 27; i <= 29; i++) {
		this->gameMapVec[2][3][i] = WALL;
	}
	for (int i = 31; i <= 33; i++)
	{
		this->gameMapVec[2][4][i] = WALL;
	}
	for (int i = 9; i <= 11; i++)
	{
		this->gameMapVec[2][5][i] = WALL;
	}
	for (int i = 7; i <= 8; i++)
	{
		this->gameMapVec[2][10][i] = WALL;
	}
	for (int i = 20; i <= 21; i++)
	{
		this->gameMapVec[2][13][i] = WALL;
	}
	for (int i = 27; i <= 29; i++)
	{
		this->gameMapVec[2][17][i] = WALL;
	}
	for (int i = 25; i <= 27; i++)
	{
		this->gameMapVec[2][21][i] = WALL;
	}
	this->gameMapVec[2][9][8] = WALL;
	this->gameMapVec[2][12][1] = WALL;
	this->gameMapVec[2][12][3] = WALL;
	this->gameMapVec[2][11][8] = WALL;
	this->gameMapVec[2][10][9] = WALL;
	this->gameMapVec[2][6][10] = WALL;
	this->gameMapVec[2][4][10] = WALL;
	this->gameMapVec[2][1][11] = WALL;
	this->gameMapVec[2][0][12] = WALL;
	this->gameMapVec[2][1][13] = WALL;
	this->gameMapVec[2][4][17] = WALL;
	this->gameMapVec[2][4][19] = WALL;
	this->gameMapVec[2][21][4] = WALL;
	this->gameMapVec[2][21][6] = WALL;
	this->gameMapVec[2][16][9] = WALL;
	this->gameMapVec[2][2][28] = WALL;
	this->gameMapVec[2][4][28] = WALL;
	this->gameMapVec[2][3][32] = WALL;
	this->gameMapVec[2][5][32] = WALL;
	this->gameMapVec[2][12][21] = WALL;
	this->gameMapVec[2][14][21] = WALL;
	this->gameMapVec[2][13][22] = WALL;
	this->gameMapVec[2][16][11] = WALL;
	this->gameMapVec[2][20][14] = WALL;
	this->gameMapVec[2][20][16] = WALL;
	this->gameMapVec[2][11][17] = WALL;
	this->gameMapVec[2][22][19] = WALL;
	this->gameMapVec[2][22][21] = WALL;
	this->gameMapVec[2][20][26] = WALL;
	this->gameMapVec[2][22][26] = WALL;
	this->gameMapVec[2][16][28] = WALL;
	this->gameMapVec[2][18][28] = WALL;
	this->gameMapVec[2][11][30] = WALL;
	this->gameMapVec[2][11][32] = WALL;

	// 初始化人物以及终点
	this->gameMapVec[2][20][4] = DOOR;
	this->gameMapVec[2][11][19] = ROLE;
}

// 游戏数据初始化
void Model::init()
{
	// 游戏区域初始化
	this->left = WIDTH / 2 - (COL * GRID_SIZE) / 2;
	this->top = HEIGHT / 2 - (ROW * GRID_SIZE) / 2;
	this->right = this->left + COL * GRID_SIZE;
	this->down = this->top + ROW * GRID_SIZE;

	// 游戏地图数据初始化
	this->gameMapVec.clear();
	for (int k = 0; k < MAXCAS; k++)
	{
		vector<vector<int>> MapGrid;
		for (int i = 0; i < ROW; i++)
		{
			vector<int> lineGrid;
			for (int j = 0; j < COL; j++)
			{
				lineGrid.push_back(0);
			}
			MapGrid.push_back(lineGrid);
		}
		this->gameMapVec.push_back(MapGrid);
	}

	// 地图数据初始化
	this->map1();
	this->map2();
	this->map3();

	// 玩家数据初始化
	this->searchRole();
	this->role.step = 1.4;
	this->role.dir = NONE;
}

// 开始界面绘制
void Model::welcome(IMAGE& bk, IMAGE& w1, IMAGE& w2, IMAGE& man)
{
	SetWindowText(GetHWnd(), "C++逃出生天");
	cleardevice();
	putimage(0, 0, &bk);
	putimage(WIDTH / 4 - 512 / 2 + 30, HEIGHT / 2 - 512 / 2, &man);
	this->drawAlpha(&w1, WIDTH / 2 + 125, 120);
	this->drawAlpha(&w2, WIDTH / 2 + 160, 300);
	this->drawButton(this->beginGame);
	this->drawButton(this->endGame);
}

// 游戏界面绘制
void Model::draw(IMAGE& bk, IMAGE& brick, IMAGE fire[], IMAGE door[], IMAGE ghost[], IMAGE character[])
{
	cleardevice();
	putimage(0, 0, &bk);

	// 绘制游戏区域地图
	for (int i = 0; i < ROW; i++)
	{
		for (int j = 0; j < COL; j++)
		{
			// 将数组下标转化为坐标
			int x = this->left + GRID_SIZE * j;
			int y = this->top + GRID_SIZE * i;
			// 对消息进行分发
			switch (this->gameMapVec[this->cas][i][j])
			{
			case BLANK:
				break;
			case ROLE:
				setlinecolor(RGB(128, 64, 0));
				setlinestyle(PS_SOLID, 4);
				setfillcolor(RGB(255, 138, 176));
				fillrectangle(x + 2, y + 2, x + GRID_SIZE - 2, y + GRID_SIZE - 2);
				break;
			case WALL:
				putimage(x, y, &brick);
				break;
			case FIRE:
				this->drawFire(x, y, fire);
				break;
			case GHOST:
				this->drawGhost(x, y, ghost);
				break;
			case DOOR:
			case (DOOR + 2):
				this->drawDoor(x, y, door);
				break;
			default:
				break;
			}
		}
	}
	// 绘制人物
	this->drawRole(character);

	// 绘制外部边框
	for (int x = 25; x >= 0; x--)
	{
		setlinecolor(RGB(128, 64, 0));
		rectangle(this->left - x, this->top - x, this->right + x, this->down + x);
	}
	for (int x = 50; x >= 25; x--)
	{
		setlinecolor(RGB(165, 103, 69));
		rectangle(this->left - x, this->top - x, this->right + x, this->down + x);
	}
	for (int x = 70; x >= 50; x--)
	{
		setlinecolor(RGB(128, 64, 0));
		rectangle(this->left - x, this->top - x, this->right + x, this->down + x);
	}
}

// 贴火焰图片
void Model::drawFire(const int x, const int y, IMAGE fire[])
{
	// 源码图：SRCPAINT格式去贴图
	putimage(x, y, fire + 1, SRCPAINT);
	// 掩码图：SRCAND格式去贴图
	putimage(x, y, fire, SRCAND);
}

// 贴终点图片
void Model::drawDoor(const int x, const int y, IMAGE door[])
{
	// 源码图：SRCPAINT格式去贴图
	putimage(x, y, door + 1, SRCPAINT);
	// 掩码图：SRCAND格式去贴图
	putimage(x, y, door, SRCAND);
}

// 贴幽灵图片
void Model::drawGhost(const int x, const int y, IMAGE ghost[])
{
	// 源码图：SRCPAINT格式去贴图
	putimage(x, y, ghost + 1, SRCPAINT);
	// 掩码图：SRCAND格式去贴图
	putimage(x, y, ghost, SRCAND);
}

// 贴人物图片
void Model::drawRole(IMAGE character[])
{
	//源码图：SRCPAINT格式去贴图
	putimage(this->role.x, this->role.y, character + 1, SRCPAINT);
	//掩码图：SRCAND格式去贴图
	putimage(this->role.x, this->role.y, character, SRCAND);
}

// 搜寻玩家位置
void Model::searchRole()
{
	for (int i = 0; i < ROW; i++)
	{
		for (int j = 0; j < COL; j++)
		{
			if (this->gameMapVec[this->cas][i][j] == ROLE)
			{
				this->role.x = j * GRID_SIZE + this->left;
				this->role.y = i * GRID_SIZE + this->top;
				break;
			}
		}
	}
}

// 玩家自动移动
void Model::moveRole(IMAGE& bk, IMAGE& brick, IMAGE& replace, IMAGE fire[], IMAGE door[], IMAGE ghost[], IMAGE character[])
{
	// 记录玩家速度初始值
	static double tempstep = this->role.step;
	// 记录玩家所碰墙壁坐标值
	double x = 0, y = 0;
	// 将玩家中点坐标转化为数组下标
	int r = (this->role.y + GRID_SIZE / 2 - this->top) / GRID_SIZE;
	int c = (this->role.x + GRID_SIZE / 2 - this->left) / GRID_SIZE;
	// 判断移动方向
	if (this->role.dir == UP)
	{ // 向上移动
		this->role.y -= this->role.step;
		this->role.step += 0.1;
		if (r - 1 >= 0 && this->gameMapVec[this->cas][r - 1][c] == WALL && this->role.y <= this->top + GRID_SIZE * (r - 1) + GRID_SIZE)
		{
			this->role.y = this->top + GRID_SIZE * (r - 1) + GRID_SIZE;
			if (this->gameMapVec[this->cas][r][c] != ROLE)
			{
				this->gameMapVec[this->cas][r][c] += 2;
			}
			this->role.dir = NONE;
			this->role.step = tempstep;

			// 先将地图绘制一遍
			this->draw(bk, brick, fire, door, ghost, character);
			FlushBatchDraw();

			// 获取玩家上边墙壁的坐标
			x = c * GRID_SIZE + this->left;
			y = (r - 1) * GRID_SIZE + this->top;

			// 播放撞击音乐
			mciSendString("close collision", nullptr, 0, nullptr);
			mciSendString("open ./resource/music/collision.mp3 alias collision", nullptr, 0, nullptr);
			mciSendString("play collision", nullptr, 0, nullptr);

			// 撞击墙壁动画效果设置
			for (int i = 0; i < 4; i++)
			{
				putimage(x, y, &replace);
				y--;
				putimage(x, y, &brick);
				Sleep(30);
				FlushBatchDraw();
			}
			for (int i = 0; i < 4; i++)
			{
				putimage(x, y, &replace);
				y++;
				putimage(x, y, &brick);
				Sleep(30);
				FlushBatchDraw();
			}
		}
	}
	else if (this->role.dir == DOWN)
	{ // 向下移动
		this->role.y += this->role.step;
		this->role.step += 0.1;
		if (r + 1 < ROW && this->gameMapVec[this->cas][r + 1][c] == WALL && this->role.y >= this->top + GRID_SIZE * (r + 1) - GRID_SIZE)
		{
			this->role.y = this->top + GRID_SIZE * (r + 1) - GRID_SIZE;
			if (this->gameMapVec[this->cas][r][c] != ROLE)
			{
				this->gameMapVec[this->cas][r][c] += 2;
			}
			this->role.dir = NONE;
			this->role.step = tempstep;

			// 先将地图绘制一遍
			this->draw(bk, brick, fire, door, ghost, character);
			FlushBatchDraw();

			// 获取玩家上边墙壁的坐标
			x = c * GRID_SIZE + this->left;
			y = (r + 1) * GRID_SIZE + this->top;

			// 播放撞击音乐
			mciSendString("close collision", nullptr, 0, nullptr);
			mciSendString("open ./resource/music/collision.mp3 alias collision", nullptr, 0, nullptr);
			mciSendString("play collision", nullptr, 0, nullptr);

			// 撞击墙壁动画效果设置
			for (int i = 0; i < 4; i++)
			{
				putimage(x, y, &replace);
				y++;
				putimage(x, y, &brick);
				Sleep(30);
				FlushBatchDraw();
			}
			for (int i = 0; i < 4; i++)
			{
				putimage(x, y, &replace);
				y--;
				putimage(x, y, &brick);
				Sleep(30);
				FlushBatchDraw();
			}
		}
	}
	else if (this->role.dir == LEFT)
	{ // 向左移动
		this->role.x -= this->role.step;
		this->role.step += 0.1;
		if (c - 1 >= 0 && this->gameMapVec[this->cas][r][c - 1] == WALL && this->role.x <= this->left + GRID_SIZE * (c - 1) + GRID_SIZE)
		{
			this->role.x = this->left + GRID_SIZE * (c - 1) + GRID_SIZE;
			if (this->gameMapVec[this->cas][r][c] != ROLE)
			{
				this->gameMapVec[this->cas][r][c] += 2;
			}
			this->role.dir = NONE;
			this->role.step = tempstep;

			// 先将地图绘制一遍
			this->draw(bk, brick, fire, door, ghost, character);
			FlushBatchDraw();

			// 获取玩家左边墙壁的坐标
			x = (c - 1) * GRID_SIZE + this->left;
			y = r * GRID_SIZE + this->top;

			//播放撞击音乐
			mciSendString("close collision", nullptr, 0, nullptr);
			mciSendString("open ./resource/music/collision.mp3 alias collision", nullptr, 0, nullptr);
			mciSendString("play collision", nullptr, 0, nullptr);

			// 撞击墙壁动画效果设置
			for (int i = 0; i < 4; i++)
			{
				putimage(x, y, &replace);
				x--;
				putimage(x, y, &brick);
				Sleep(30);
				FlushBatchDraw();
			}
			for (int i = 0; i < 4; i++)
			{
				putimage(x, y, &replace);
				x++;
				putimage(x, y, &brick);
				Sleep(30);
				FlushBatchDraw();
			}
		}
	}
	else if (this->role.dir == RIGHT)
	{ // 向右移动
		this->role.x += this->role.step;
		this->role.step += 0.1;
		if (c + 1 < COL && this->gameMapVec[this->cas][r][c + 1] == WALL && this->role.x >= this->left + GRID_SIZE * (c + 1) - GRID_SIZE)
		{
			this->role.x = this->left + GRID_SIZE * (c + 1) - GRID_SIZE;
			if (this->gameMapVec[this->cas][r][c] != ROLE)
			{
				this->gameMapVec[this->cas][r][c] += 2;
			}
			this->role.dir = NONE;
			this->role.step = tempstep;

			// 先将地图绘制一遍
			this->draw(bk, brick, fire, door, ghost, character);
			FlushBatchDraw();

			// 获取玩家右边墙壁的坐标
			x = (c + 1) * GRID_SIZE + this->left;
			y = r * GRID_SIZE + this->top;

			// 播放撞击音乐
			mciSendString("close collision", nullptr, 0, nullptr);
			mciSendString("open ./resource/music/collision.mp3 alias collision", nullptr, 0, nullptr);
			mciSendString("play collision", nullptr, 0, nullptr);

			// 撞击墙壁动画效果设置
			for (int i = 0; i < 4; i++)
			{
				putimage(x, y, &replace);
				x++;
				putimage(x, y, &brick);
				Sleep(30);
				FlushBatchDraw();
			}
			for (int i = 0; i < 4; i++)
			{
				putimage(x, y, &replace);
				x--;
				putimage(x, y, &brick);
				Sleep(30);
				FlushBatchDraw();
			}
		}
	}
}

// 判断鼠标是否在按钮中
int Model::mouseInButton(struct Button* pButton, MOUSEMSG* msg)
{
	if (msg->x >= pButton->x && msg->x <= pButton->x + pButton->width &&
		msg->y >= pButton->y && msg->y <= pButton->y + pButton->height)
	{
		pButton->buttoncolor = RGB(235, 235, 235);
		pButton->textcolor = RGB(94, 143, 204);
		return 1; // 返回值为1，表明鼠标在按钮中
	}
	pButton->buttoncolor = RGB(252, 252, 252);
	pButton->textcolor = RGB(98, 143, 204);
	return 0; // 返回值为0，表明鼠标不在按钮中
}

// 鼠标控制
int Model::mouseControl(struct Button* pButton1, struct Button* pButton2)
{
	// 判断是否有鼠标消息
	if (MouseHit())
	{
		MOUSEMSG msg = GetMouseMsg();
		// 判断用户是否点击按钮
		if (this->mouseInButton(pButton1, &msg) == 1 && msg.uMsg == WM_LBUTTONDOWN)
		{
			mciSendString("close click", nullptr, 0, nullptr);
			mciSendString("open ./resource/music/click.wav alias click", nullptr, 0, nullptr);
			mciSendString("play click", nullptr, 0, nullptr);
			return 1; // 返回值为1，进入游戏
		}
		else if (this->mouseInButton(pButton2, &msg) == 1 && msg.uMsg == WM_LBUTTONDOWN)
		{
			mciSendString("close click", nullptr, 0, nullptr);
			mciSendString("open ./resource/music/click.wav alias click", nullptr, 0, nullptr);
			mciSendString("play click", nullptr, 0, nullptr);
			exit(0); // 游戏结束
		}
	}
	return 0;
}

// 按键响应（同步按键响应）
void Model::keyDown()
{
	// 判断是否有按键消息
	while (_kbhit())
	{
		// 用于接收按键消息
		char userKey = _getch();
		fflush(stdin); // 清空键盘消息缓冲区

		switch (userKey)
		{
		case 'W':
		case 'w':
		case 72:
			if (this->role.dir == NONE)
			{
				this->role.dir = UP;
			}
			break;
		case 'S':
		case 's':
		case 80:
			if (this->role.dir == NONE)
			{
				this->role.dir = DOWN;
			}
			break;
		case 'A':
		case 'a':
		case 75:
			if (this->role.dir == NONE)
			{
				this->role.dir = LEFT;
			}
			break;
		case 'D':
		case 'd':
		case 77:
			if (this->role.dir == NONE)
			{
				this->role.dir = RIGHT;
			}
			break;
		default:
			break;
		}
	}
}

// 游戏结束条件1
int Model::condition1()
{
	// 玩家到达终点
	int r = (this->role.y + GRID_SIZE / 2 - this->top) / GRID_SIZE;
	int c = (this->role.x + GRID_SIZE / 2 - this->left) / GRID_SIZE;
	if (this->gameMapVec[this->cas][r][c] == DOOR + 2)
	{
		return 1; // 返回值为1，说明条件满足
	}
	return 0; // 返回值为0，说明条件不满足
}

// 游戏结束条件2
int Model::condition2()
{
	// 玩家离开游戏区域或碰到火焰和幽灵
	int r = (this->role.y + GRID_SIZE / 2 - this->top) / GRID_SIZE;
	int c = (this->role.x + GRID_SIZE / 2 - this->left) / GRID_SIZE;
	if (this->role.x <= this->left || this->role.x >= this->right * 1.0 - GRID_SIZE ||
		this->role.y <= this->top || this->role.y >= this->down * 1.0 - GRID_SIZE ||
		this->gameMapVec[this->cas][r][c] == GHOST ||
		this->gameMapVec[this->cas][r][c] == FIRE)
	{
		return 1; // 返回值为1，说明条件满足
	}
	return 0; // 返回值为0，说明条件不满足
}

// 游戏结束判断
bool Model::over()
{
	if (this->condition1() == 1 || this->condition2() == 1)
	{
		return true; // 返回值为true,说明游戏结束
	}
	return false; // 返回值为false,说明游戏未结束
}
