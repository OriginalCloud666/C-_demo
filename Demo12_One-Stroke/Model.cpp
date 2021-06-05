#include "Model.h"
using namespace std;

Model::Model() :top(0), left(0), down(0), right(0), flag(0) {
	this->gameMapVec = {
		{
			{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
			{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
			{ 0, 2, 1, 1, 1, 1, 1, 1, 1, 0 },
			{ 0, 0, 0, 0, 0, 1, 1, 1, 1, 0 },
			{ 0, 0, 0, 0, 0, 1, 1, 1, 1, 0 },
			{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
			{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
			{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
			{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
			{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
		},

		{
			{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
			{ 0, 0, 1, 1, 1, 1, 1, 1, 0, 0 },
			{ 0, 0, 1, 1, 1, 1, 1, 1, 0, 0 },
			{ 0, 0, 1, 1, 1, 1, 1, 1, 0, 0 },
			{ 0, 0, 1, 1, 2, 1, 1, 1, 0, 0 },
			{ 0, 0, 1, 1, 1, 1, 1, 1, 0, 0 },
			{ 0, 0, 1, 1, 1, 1, 1, 1, 0, 0 },
			{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
			{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
			{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
		},

		{
			{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
			{ 0, 0, 1, 1, 1, 1, 1, 1, 0, 0 },
			{ 0, 0, 1, 0, 0, 0, 0, 1, 0, 0 },
			{ 0, 0, 1, 0, 1, 1, 1, 1, 0, 0 },
			{ 0, 0, 1, 0, 1, 1, 0, 0, 0, 0 },
			{ 0, 0, 1, 1, 1, 1, 0, 0, 0, 0 },
			{ 0, 0, 1, 1, 2, 1, 0, 0, 0, 0 },
			{ 0, 0, 1, 1, 1, 1, 0, 0, 0, 0 },
			{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
			{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
		}
	};
}

// 获取count
int& Model::getCount() {
	return this->count;
}

// 获取flag
int& Model::getFlag() {
	return this->flag;
}

// 获取role
struct Role& Model::getRole() {
	return this->role;
}

// 获取point
vector<Point>& Model::getPoint() {
	return this->point;
}

//加载资源
void Model::load(IMAGE& bk, IMAGE& w1, IMAGE& w2) {
	// 加载背景图片
	loadimage(&bk, "./resource/picture/bk.jpg");
	// 加载文字图片
	loadimage(&w1, "./resource/picture/w1.png", 900, 150);
	loadimage(&w2, "./resource/picture/w2.png", 680, 113);
}

// 设置背景音乐
void Model::bgm() {
	mciSendString("open ./resource/music/bk.mp3 alias BGM", nullptr, 0, nullptr);
	mciSendString("play BGM repeat", nullptr, 0, nullptr);
}

// 载入PNG图并去透明部分
void Model::drawAlpha(IMAGE* picture, const int  picture_x, const int picture_y) {
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
	for (int iy = 0; iy < picture_height; iy++) {
		for (int ix = 0; ix < picture_width; ix++) {
			int srcX = ix + iy * picture_width; // 在显存里像素的角标
			int sa = ((src[srcX] & 0xff000000) >> 24); // 0xAArrggbb;AA是透明度
			int sr = ((src[srcX] & 0xff0000) >> 16); // 获取RGB里的R
			int sg = ((src[srcX] & 0xff00) >> 8); // 获取RGB里的G
			int sb = src[srcX] & 0xff; // 获取RGB里的B
			if (ix >= 0 && ix <= graphWidth && iy >= 0 && iy <= graphHeight && dstX <= graphWidth * graphHeight) {
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

// 初始化游戏数据
void Model::init() {
	// 初始化游戏区域坐标
	this->left = width / 2 - (grid_num * grid_size) / 2;
	this->top = height / 2 - (grid_num * grid_size) / 2;
	this->right = this->left + grid_num * grid_size;
	this->down = this->top + grid_num * grid_size;

	// 初始化vector容器
	this->point.clear();
	for (int i = 0; i < point_num; i++) {
		this->point.push_back({ 0, 0 });
	}
}

// 绘制内含白色矩形的填充矩形（角色）
void Model::drawRole(const double x, const double y, const int border_size) {
	// 填充外部颜色
	setfillcolor(RGB(40, 39, 96));
	solidrectangle(x, y, x + grid_size, y + grid_size);
	// 填充内部白色矩形
	setfillcolor(RGB(245, 245, 245));
	solidrectangle(x + (grid_size / 2 - border_size / 2), y + (grid_size / 2 - border_size / 2),
		x + (grid_size / 2 + border_size / 2), y + (grid_size / 2 + border_size / 2));
}

// 绘制橙色矩形（墙壁）
void Model::drawWall(const double x, const double y) {
	// 绘制带边框的矩形
	setlinecolor(RGB(240, 240, 240));
	setlinestyle(PS_SOLID, 1);
	setfillcolor(RGB(200, 106, 55));
	fillrectangle(x, y, x + grid_size, y + grid_size);
}

// 画线的过程
void Model::drawLine(struct Point* begin, struct Point* end) {
	// 将数组下标转化为坐标
	const int bx = this->left + begin->col * grid_size + grid_size / 2;
	const int by = this->top + begin->row * grid_size + grid_size / 2;
	const int ex = this->left + end->col * grid_size + grid_size / 2;
	const int ey = this->top + end->row * grid_size + grid_size / 2;
	setlinecolor(WHITE);
	setlinestyle(PS_SOLID, 5);
	line(bx, by, ex, ey);
}

// 绘制开始界面
void Model::welcome(IMAGE& bk, IMAGE& w1, IMAGE& w2) {
	putimage(-25, -83, &bk);
	setbkmode(TRANSPARENT);
	SetWindowText(GetHWnd(), "C++一笔画涂格子");

	setbkcolor(RGB(31, 61, 110));
	drawAlpha(&w1, 95, 475);
	drawAlpha(&w2, 175, 580);

	while (!_kbhit()) {
	}
}

// 绘制游戏界面
void Model::draw() {
	cleardevice();

	// 输出作者信息
	settextcolor(RGB(215, 215, 215));
	settextstyle(25, 0, "楷体", 0, 0, 1000, 0, 0, 0);
	outtextxy(12, 17, "一笔画游戏");

	settextcolor(RGB(215, 215, 215));
	settextstyle(23, 0, "楷体", 0, 0, 1000, 0, 0, 0);
	outtextxy(16, 54, "作者：初云");

	// 绘制当前关卡信息
	char casName[25];
	sprintf(casName, "当前关卡：第%d关", this->flag + 1);
	settextcolor(RGB(215, 215, 215));
	settextstyle(29, 0, "楷体", 0, 0, 1000, 0, 0, 0);
	outtextxy(205, 658, casName);

	// 绘制游戏区域外部边框
	for (int x = 15; x >= 0; x--) {
		setlinecolor(RGB(40, 39, 96));
		rectangle(this->left - x, this->top - x, this->right + x, this->down + x);
	}

	// 绘制游戏区域地图
	for (int i = 0; i < grid_num; i++) {
		for (int j = 0; j < grid_num; j++) {
			// 把数组下标转化为坐标
			double x = this->left + (double)grid_size * j;
			double y = this->top + (double)grid_size * i;
			// 对贴图信息进行分发
			switch (this->gameMapVec[this->flag][i][j]) {
			case BLANK:
				break;
			case WALL:
				this->drawWall(x, y);
				break;
			case ROLE:
				this->drawRole(x, y, grid_size / 5);
				break;
			case BOARD:
				this->drawRole(x, y, 1);
				break;
			}
		}
	}
}

// 找寻角色位置
void Model::searchRolePos() {
	for (int i = 0; i < grid_num; i++) {
		for (int j = 0; j < grid_num; j++) {
			if (this->gameMapVec[this->flag][i][j] == ROLE) {
				// 获取角色行列位置
				this->role.row = i;
				this->role.col = j;
			}
		}
	}
	return;
}

// 记录位置信息的点的初始化
void Model::pointInit() {
	// 初始化vector容器
	for (vector<Point>::iterator iter = this->point.begin(); iter != this->point.end(); iter++) {
		*iter = { 0, 0 };
	}
	// 获取玩家的坐标位置
	this->searchRolePos();
	// 记录起始点的位置
	this->point[0].row = this->role.row;
	this->point[0].col = this->role.col;
	// 对计数器初始化
	this->count = 1;
	// 对方向进行初始化
	this->role.flag = -1;
}

// 移动标准判断
int Model::roleStop(const int dir) {
	// 向左移动，左侧是墙壁
	if (dir == LEFT && this->gameMapVec[this->flag][this->role.row][this->role.col - 1] == WALL) {
		return 1; // 返回1，表示能够走
	}
	// 向右移动，右侧是墙壁
	if (dir == RIGHT && this->gameMapVec[this->flag][this->role.row][this->role.col + 1] == WALL) {
		return 1; // 返回1，表示能够走
	}
	// 向上移动，上侧是墙壁
	if (dir == UP && this->gameMapVec[this->flag][this->role.row - 1][this->role.col] == WALL) {
		return 1; // 返回1，表示能够走
	}
	// 向下移动，下侧是墙壁
	if (dir == DOWN && this->gameMapVec[this->flag][this->role.row + 1][this->role.col] == WALL) {
		return 1; // 返回1，表示能够走
	}
	return -1; // 返回-1，表示不能移动
}

// 角色移动过程
void Model::moveRole(const int dir) {
	switch (dir) {
	case UP:
		if (this->role.row == this->point[0].row && this->role.col == this->point[0].col) {
			this->gameMapVec[this->flag][this->role.row][this->role.col] = ROLE;
			this->role.row--;
			break;
		}
		this->gameMapVec[this->flag][this->role.row][this->role.col] = BOARD; // 原本玩家位置变为已走状态
		this->role.row--;
		break;
	case DOWN:
		if (this->role.row == this->point[0].row && this->role.col == this->point[0].col) {
			this->gameMapVec[this->flag][this->role.row][this->role.col] = ROLE;
			this->role.row++;
			break;
		}
		this->gameMapVec[this->flag][this->role.row][this->role.col] = BOARD; // 原本玩家位置变为已走状态
		this->role.row++;
		break;
	case LEFT:
		if (this->role.row == this->point[0].row && this->role.col == this->point[0].col) {
			this->gameMapVec[this->flag][this->role.row][this->role.col] = ROLE;
			this->role.col--;
			break;
		}
		this->gameMapVec[this->flag][this->role.row][this->role.col] = BOARD; // 原本玩家位置变为已走状态
		this->role.col--;
		break;
	case RIGHT:
		if (this->role.row == this->point[0].row && this->role.col == this->point[0].col) {
			this->gameMapVec[this->flag][this->role.row][this->role.col] = ROLE;
			this->role.col++;
			break;
		}
		this->gameMapVec[this->flag][this->role.row][this->role.col] = BOARD; // 原本玩家位置变为已走状态
		this->role.col++;
		break;
	}
	this->gameMapVec[this->flag][this->role.row][this->role.col] = ROLE; // 新的地方变成人的位置
}

// 按键响应
void Model::keyDown() {
	// 判断是否有键盘消息
	while (_kbhit()) {
		// 用于接收用户按键
		char userKey = _getch();
		fflush(stdin); // 清空键盘缓冲区

		switch (userKey) {
		case'W': // 上
		case 'w':
		case 72:
			mciSendString("close click", nullptr, 0, nullptr);
			mciSendString("open ./resource/music/click.wav alias click", nullptr, 0, nullptr);
			mciSendString("play click", nullptr, 0, nullptr);
			this->role.flag = UP;
			break;
		case'S': // 下
		case 's':
		case 80:
			mciSendString("close click", nullptr, 0, nullptr);
			mciSendString("open ./resource/music/click.wav alias click", nullptr, 0, nullptr);
			mciSendString("play click", nullptr, 0, nullptr);
			this->role.flag = DOWN;
			break;
		case'A': // 左
		case 'a':
		case 75:
			mciSendString("close click", nullptr, 0, nullptr);
			mciSendString("open ./resource/music/click.wav alias click", nullptr, 0, nullptr);
			mciSendString("play click", nullptr, 0, nullptr);
			this->role.flag = LEFT;
			break;
		case'D': // 右
		case 'd':
		case 77:
			mciSendString("close click", nullptr, 0, nullptr);
			mciSendString("open ./resource/music/click.wav alias click", nullptr, 0, nullptr);
			mciSendString("play click", nullptr, 0, nullptr);
			this->role.flag = RIGHT;
			break;
		}
	}
}

// 结束条件判断
bool Model::over() {
	// 判断数组中还有没有墙
	for (int i = 0; i < grid_num; i++) {
		for (int j = 0; j < grid_num; j++) {
			if (this->gameMapVec[this->flag][i][j] == WALL) {
				return false; // 游戏未结束
			}
		}
	}
	return true; // 游戏结束
}