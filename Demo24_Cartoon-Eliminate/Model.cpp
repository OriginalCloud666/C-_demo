#include "Model.h"
using namespace std;

Model::Model() :top(0), left(0), down(0), right(0), state(true) {
	begin.x = -1; begin.y = -1;
	end.x = -1; end.y = -1;
	this->beginGame = creatButton(80, 500, 150, 50, "START", 38, 12, RGB(252, 252, 252), RGB(150, 150, 150));
	this->endGame = creatButton(420, 500, 150, 50, "END", 52, 12, RGB(252, 252, 252), RGB(150, 150, 150));
}

Model::~Model() {
	delete this->beginGame;
	this->beginGame = nullptr;
	delete this->endGame;
	this->endGame = nullptr;
}

// 获取开始点
POINT& Model::getBegin() {
	return this->begin;
}

// 获取结束点
POINT& Model::getEnd() {
	return this->end;
}

// 获取MapVec
vector<vector<int>>& Model::getMapVec() {
	return this->gameMapVec;
}

// 获取beginGame
struct Button*& Model::getButton1() {
	return this->beginGame;
}

// 获取endGame
struct Button*& Model::getButton2() {
	return this->endGame;
}

// 加载资源
void Model::load(IMAGE& bk, IMAGE& w1, IMAGE& w2, vector<IMAGE>& img, vector<int>& imgIndex) {
	// 批量加载图片
	for (int i = 0; i < pic_num; i++) {
		char fileName[25];
		sprintf(fileName, "./resource/picture/%d.png", imgIndex[i]);
		loadimage(&img[i], fileName, pic_size - 10, pic_size - 10);
	}
	loadimage(&bk, "./resource/picture/bk.png", width, height);
	loadimage(&w1, "./resource/picture/w1.png");
	loadimage(&w2, "./resource/picture/w2.png");
}

// 加载音乐
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

// 创建按钮
struct Button* Model::creatButton(const int x, const int y, const int width, const int height,
	const char* text, const int dx, const int dy, COLORREF buttoncolor, COLORREF textcolor) {
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
void Model::drawButton(struct Button* pButton) {
	setbkmode(TRANSPARENT);
	setlinecolor(RGB(160, 160, 160));
	setlinestyle(PS_SOLID, 5);
	setfillcolor(pButton->buttoncolor);
	fillrectangle(pButton->x, pButton->y, pButton->x + pButton->width, pButton->y + pButton->height);
	settextcolor(pButton->textcolor);
	settextstyle(28, 0, "楷体", 0, 0, 1000, 0, 0, 0);
	outtextxy(pButton->x + pButton->dx, pButton->y + pButton->dy, pButton->text);
}

// 初始化数据
void Model::init() {
	// 设置随机数种子
	srand((unsigned)time(nullptr) * rand());

	// 初始化游戏行列以及游戏区域坐标
	this->left = width / 2 - (grid_num * pic_size) / 2;
	this->top = height / 2 - (grid_num * pic_size) / 2;
	this->right = this->left + grid_num * pic_size;
	this->down = this->top + grid_num * pic_size;

	// 初始化vector容器
	this->gameMapVec.clear();
	for (int i = 0; i < grid_num; i++) {
		vector<int> lineGrid;
		for (int j = 0; j < grid_num; j++) {
			lineGrid.push_back(j % 9);
		}
		this->gameMapVec.push_back(lineGrid);
	}

	// 打乱vector容器数据
	for (int i = 0; i < grid_num; i++) {
		for (int j = 0; j < grid_num; j++) {
			int r1 = rand() % grid_num;
			int c1 = rand() % grid_num;
			int r2 = rand() % grid_num;
			int c2 = rand() % grid_num;
			int temp = this->gameMapVec[r1][c1];
			this->gameMapVec[r1][c1] = this->gameMapVec[r2][c2];
			this->gameMapVec[r2][c2] = temp;
		}
	}
}

// 绘制开始界面
void Model::welcome(IMAGE& bk, IMAGE& w1, IMAGE& w2) {
	SetWindowText(GetHWnd(), "C++卡通消消");
	setbkcolor(RGB(240, 240, 240));
	cleardevice();

	this->drawAlpha(&bk, 0, 0);
	this->drawAlpha(&w1, 131, 100);
	this->drawAlpha(&w2, 161, 290);
	this->drawButton(this->beginGame);
	this->drawButton(this->endGame);
}

// 绘制游戏界面
void Model::draw(IMAGE& bk, vector<IMAGE>& img) {
	setbkcolor(RGB(240, 240, 240));
	cleardevice();
	drawAlpha(&bk, 0, 0);

	// 绘制游戏区域边框
	setfillcolor(RGB(255, 255, 255));
	setlinecolor(RGB(210, 210, 210));
	setlinestyle(PS_SOLID, 10);
	fillroundrect(this->left - 10, this->top - 10, this->right + 10, this->down + 10, 1, 1);

	// 绘制游戏区域地图
	for (int i = 0; i < grid_num; i++) {
		for (int j = 0; j < grid_num; j++) {
			// 下标转化为坐标
			int x = j * pic_size + this->left;
			int y = i * pic_size + this->top;
			// 信息进行分发
			switch (this->gameMapVec[i][j]) {
			case 0:
				this->drawAlpha(&img[0], x + 6, y + 6);
				this->drawFim(x, y);
				break;
			case 1:
				this->drawAlpha(&img[1], x + 6, y + 6);
				this->drawFim(x, y);
				break;
			case 2:
				this->drawAlpha(&img[2], x + 6, y + 6);
				this->drawFim(x, y);
				break;
			case 3:
				this->drawAlpha(&img[3], x + 6, y + 6);
				this->drawFim(x, y);
				break;
			case 4:
				this->drawAlpha(&img[4], x + 6, y + 6);
				this->drawFim(x, y);
				break;
			case 5:
				this->drawAlpha(&img[5], x + 6, y + 6);
				this->drawFim(x, y);
				break;
			case 6:
				this->drawAlpha(&img[6], x + 6, y + 6);
				this->drawFim(x, y);
				break;
			case 7:
				this->drawAlpha(&img[7], x + 6, y + 6);
				this->drawFim(x, y);
				break;
			case 8:
				this->drawAlpha(&img[8], x + 6, y + 6);
				this->drawFim(x, y);
				break;
			default:
				break;
			}
		}
	}

	// 绘制选择边框
	this->drawChoseFim();
}

// 绘制边框
void Model::drawFim(const int x, const int y) {
	setlinecolor(RGB(250, 250, 250));
	setlinestyle(PS_SOLID, 5);
	roundrect(x, y, x + pic_size, y + pic_size, 1, 1);
	setlinecolor(RGB(200, 200, 200));
	setlinestyle(PS_SOLID, 5);
	roundrect(x + 5, y + 5, x + pic_size - 5, y + pic_size - 5, 1, 1);
	setlinecolor(RGB(250, 250, 250));
	setlinestyle(PS_SOLID, 3);
	roundrect(x + 7, y + 7, x + pic_size - 7, y + pic_size - 7, 1, 1);
}

// 绘制选择框
void Model::drawChoseFim() {
	// 下标转化为坐标
	int begin_x = this->begin.y * pic_size + this->left;
	int begin_y = this->begin.x * pic_size + this->top;
	int end_x = this->end.y * pic_size + this->left;
	int end_y = this->end.x * pic_size + this->top;
	setlinecolor(RGB(120, 120, 120));
	setlinestyle(PS_SOLID, 5);
	if (this->begin.x != -1 && this->begin.y != -1) {
		roundrect(begin_x + 5, begin_y + 5, begin_x + pic_size - 5, begin_y + pic_size - 5, 1, 1);
	}
	if (this->end.x != -1 && this->end.y != -1) {
		roundrect(end_x + 5, end_y + 5, end_x + pic_size - 5, end_y + pic_size - 5, 1, 1);
	}
}

// 判断是否是空白
bool Model::isBlock(const int r, const int c) {
	if (this->gameMapVec[r][c] == -1) {
		return false; // 是空白则返回false
	}
	return true; // 有阻碍则返回true
}

// 判断两张图片是否相同
bool Model::isSeem() {
	if (this->gameMapVec[this->begin.x][this->begin.y] != this->gameMapVec[this->end.x][this->end.y]) {
		return false; // 不同则返回false
	}
	return true; // 相同则返回true
}

// 判断水平方向是否能够消除
bool Model::horizon(POINT begin_, POINT end_) {
	// 判断是否已经选中
	if ((begin_.x == -1 && begin_.y == -1) || (end_.x == -1 && end_.y == -1)) {
		return false;
	}
	// 判断是否在同一水平方向
	if (begin_.x != end_.x) {
		return false;
	}
	// 判断是否点击的同一个
	if (begin_.x == end_.x && begin_.y == end_.y) {
		return false;
	}
	// 判断两个卡通图片之间是否有阻碍
	int maxc = max(begin_.y, end_.y);
	int minc = min(begin_.y, end_.y);
	// 从小到大遍历，并逐个判断
	for (int i = minc + 1; i < maxc; i++) {
		if (this->isBlock(begin_.x, i) == true) { // 如果有阻碍则直接退出
			return false;
		}
	}
	return true;
}

// 判断竖直方向是否能够消除
bool Model::vertical(POINT begin_, POINT end_) {
	// 判断是否已经选中
	if ((begin_.x == -1 && begin_.y == -1) || (end_.x == -1 && end_.y == -1)) {
		return false;
	}
	// 判断是否在同一竖直方向
	if (begin_.y != end_.y) {
		return false;
	}
	// 判断是否点击的同一个
	if (begin_.x == end_.x && begin_.y == end_.y) {
		return false;
	}
	// 判断两个卡通图片之间是否有阻碍
	int maxc = max(begin_.x, end_.x);
	int minc = min(begin_.x, end_.x);
	// 从小到大遍历，并逐个判断
	for (int i = minc + 1; i < maxc; i++) {
		if (this->isBlock(i, begin_.y) == true) { // 如果有阻碍则直接退出
			return false;
		}
	}
	return true;
}

// 判断一个拐角是否能删除
bool Model::turn_once(POINT begin_, POINT end_) {
	// 一个拐点检测可以分解为一个水平检测和一个垂直检测
	// A点到B点能否连接可转化为满足一下两个条件的任意一点:
	// 1.A点至C点的水平检测以及C点至B点的垂直检测通过
	// 2.A点至D点的垂直检测以及D点至B点的水平检测通过

	// 判断是否已经选中
	if ((begin_.x == -1 && begin_.y == -1) || (end_.x == -1 && end_.y == -1)) {
		return false;
	}
	// 判断是否点击的同一个
	if (begin_.x == end_.x && begin_.y == end_.y) {
		return false;
	}
	POINT c, d;
	c.x = begin_.x; c.y = end_.y;
	d.x = end_.x; d.y = begin_.y;
	// 判断拐点检测是否通过
	if (this->isBlock(c.x, c.y) == false && this->horizon(begin_, c) == true && this->vertical(c, end_) == true) {
		return true;
	}
	if (this->isBlock(d.x, d.y) == false && this->vertical(begin_, d) == true && this->horizon(d, end_) == true) {
		return true;
	}
	return false;
}

// 判断两个拐角是否能删除
bool Model::turn_twice(POINT begin_, POINT end_) {
	// 两个拐点检测可以分解为一个拐角检测和一个水平或垂直检测
	// A点到B点能否连接可转化为满足一下两个条件的任意一点：
	// 1.A点至C点可通过一个拐角连接，C点至B点通过水平或垂直连接
	// 2.A点至C点可通过水平或垂直连接，C点至B点通过一个拐角连接

	// 判断是否已经选中
	if ((begin_.x == -1 && begin_.y == -1) || (end_.x == -1 && end_.y == -1)) {
		return false;
	}
	// 判断是否点击的同一个
	if (begin_.x == end_.x && begin_.y == end_.y) {
		return false;
	}

	for (int i = 0; i < grid_num; i++) {
		for (int j = 0; j < grid_num; j++) {
			POINT m;
			m.x = i; m.y = j;
			if (i != begin_.x && i != end_.x && j != begin_.y && j != end_.y) {
				continue;
			}
			if ((i == begin_.x && j == begin_.y) || (i == end_.x && j == end_.y)) {
				continue;
			}
			if (this->isBlock(i, j) == true) {
				continue;
			}
			if (this->turn_once(begin_, m) == true && (this->vertical(m, end_) == true || this->horizon(m, end_) == true)) {
				return true;
			}
			if (this->turn_once(m, end_) == true && (this->vertical(begin_, m) == true || this->horizon(begin_, m) == true)) {
				return true;
			}
		}
	}
	return false;
}

// 消除整体判断
bool Model::reMove() {
	bool ret = false;
	// 水平方向检测
	ret = this->horizon(this->begin, this->end);
	if (ret == true) {
		return true;
	}
	// 竖直方向检测
	ret = this->vertical(this->begin, this->end);
	if (ret == true) {
		return true;
	}
	// 一次拐角检测
	ret = this->turn_once(this->begin, this->end);
	if (ret == true) {
		return true;
	}
	// 两次拐角检测
	ret = this->turn_twice(this->begin, this->end);
	if (ret == true) {
		return true;
	}
	return false;
}

// 判断鼠标是否在按钮中
int Model::mouseInButton(struct Button* pButton, MOUSEMSG* msg) {
	if (msg->x >= pButton->x && msg->x <= pButton->x + pButton->width &&
		msg->y >= pButton->y && msg->y <= pButton->y + pButton->height) {
		pButton->buttoncolor = RGB(235, 235, 235);
		pButton->textcolor = RGB(135, 135, 135);
		return 1; // 返回值为1，表明鼠标在按钮中
	}
	pButton->buttoncolor = RGB(252, 252, 252);
	pButton->textcolor = RGB(150, 150, 150);
	return 0; // 返回值为0，说明不在按钮中
}

// 鼠标控制
int Model::mouseControl(struct Button* pButton1, struct Button* pButton2) {
	// 判断是否有鼠标消息
	if (MouseHit()) {
		MOUSEMSG msg = GetMouseMsg();
		// 判断用户是否点击按钮
		if (this->mouseInButton(pButton1, &msg) == 1 && msg.uMsg == WM_LBUTTONDOWN) {
			mciSendString("close click", nullptr, 0, nullptr);
			mciSendString("open ./resource/music/click.wav alias click", nullptr, 0, nullptr);
			mciSendString("play click", nullptr, 0, nullptr);
			return 1; // 返回值为1，进行游戏
		}
		else if (this->mouseInButton(pButton2, &msg) == 1 && msg.uMsg == WM_LBUTTONDOWN) {
			mciSendString("close click", nullptr, 0, nullptr);
			mciSendString("open ./resource/music/click.wav alias click", nullptr, 0, nullptr);
			mciSendString("play click", nullptr, 0, nullptr);
			exit(0); // 游戏结束
		}
	}
	return 0;
}

// 鼠标控制2
void Model::mouseControl2() {
	// 判断是否有鼠标消息
	if (MouseHit()) {
		MOUSEMSG msg = GetMouseMsg();
		// 判断用户是否点击游戏区域
		if (msg.x >= this->left && msg.x <= this->right &&
			msg.y >= this->top && msg.y <= this->down) {
			// 把鼠标消息转化为数组下标
			int row = (msg.y - this->top) / pic_size;
			int col = (msg.x - this->left) / pic_size;
			// 对鼠标消息进行分发
			switch (msg.uMsg) {
			case WM_LBUTTONDOWN: // 鼠标左键点击
				mciSendString("close click", nullptr, 0, nullptr);
				mciSendString("open ./resource/music/click.wav alias click", nullptr, 0, nullptr);
				mciSendString("play click", nullptr, 0, nullptr);
				if (this->state == true && this->gameMapVec[row][col] != -1) {
					this->begin.x = row;
					this->begin.y = col;
					this->state = false;
					// 如若重复点击一个图片
					if (this->begin.x == this->end.x && this->begin.y == this->end.y) {
						this->begin.x = -1; this->begin.y = -1;
						this->end.x = -1; this->end.y = -1;
					}
				}
				else if (this->state == false && this->gameMapVec[row][col] != -1) {
					this->end.x = row;
					this->end.y = col;
					this->state = true;
					// 如若重复点击一个图片
					if (this->begin.x == this->end.x && this->begin.y == this->end.y) {
						this->begin.x = -1; this->begin.y = -1;
						this->end.x = -1; this->end.y = -1;
					}
				}
				break;
			}
		}
	}
}

// 游戏胜负判断
bool Model::over() {
	int count = 0;
	// 当图片全部消失后游戏结束
	for (int i = 0; i < grid_num; i++) {
		for (int j = 0; j < grid_num; j++) {
			if (this->gameMapVec[i][j] == -1) {
				count++;
			}
		}
	}
	if (count == grid_num * grid_num) {
		return true; // 返回值为1时，游戏结束
	}
	return false; // 返回值为0时，游戏未结束
}