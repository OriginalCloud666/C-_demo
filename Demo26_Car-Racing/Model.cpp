#include "Model.h"
using namespace std;

Model::Model() {
	this->beginGame = creatButton(2 * pic_size - 95, 500, 150, 50, "START", 38, 12, RGB(252, 252, 252), RGB(110, 110, 110));
	this->endGame = creatButton(6 * pic_size - 55, 500, 150, 50, "END", 52, 12, RGB(252, 252, 252), RGB(110, 110, 110));
}

Model::~Model() {
	delete this->beginGame;
	this->beginGame = nullptr;
	delete this->endGame;
	this->endGame = nullptr;
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
void Model::load(IMAGE& bk, IMAGE& w1, IMAGE& w2, IMAGE& track, vector<IMAGE>& img, vector<int>& imgIndex) {
	// 批量加载图片
	for (int i = 0; i < pic_num; i++) {
		char fileName[25];
		sprintf(fileName, "./resource/picture/%d.png", imgIndex[i]);
		loadimage(&img[i], fileName);
	}
	// 加载背景图片
	loadimage(&bk, "./resource/picture/bk.png", pic_size, pic_size);
	// 加载赛道图片
	loadimage(&track, "./resource/picture/track.png", pic_size, pic_size);
	// 加载文字图片
	loadimage(&w1, "./resource/picture/w1.png");
	loadimage(&w2, "./resource/picture/w2.png");
}

// 加载音乐
void Model::bgm() {
	mciSendString("open ./resource/music/bk.mp3 alias BGM", nullptr, 0, nullptr);
	mciSendString("play BGM repeat", nullptr, 0, nullptr);
}

// 求两点之间的距离
double Model::distance(const double x1, const double y1, const double x2, const double y2) {
	// distance=根号下(x2-x1)^2+(y2-y1)^2
	return sqrt((x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2));
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
	setlinecolor(RGB(60, 60, 60));
	setlinestyle(PS_SOLID, 5);
	setfillcolor(pButton->buttoncolor);
	fillrectangle(pButton->x, pButton->y, pButton->x + pButton->width, pButton->y + pButton->height);
	settextcolor(pButton->textcolor);
	settextstyle(28, 0, "楷体", 0, 0, 1000, 0, 0, 0);
	outtextxy(pButton->x + pButton->dx, pButton->y + pButton->dy, pButton->text);
}

// 初始化游戏数据
void Model::init() {
	// 初始化随机数种子
	srand((unsigned)time(nullptr) * rand());

	// 初始化线条
	for (int i = 0; i < line_num; i++) {
		struct Line newline;
		newline.FL_x = pic_size * 3.0;
		newline.SL_x = pic_size * 5.0;
		newline.FL_y = pic_size * 1.5 * i - pic_size;
		newline.SL_y = pic_size * 1.5 * i - pic_size;
		newline.width = 5;
		newline.height = 30;
		newline.step = 0.8;
		this->lines.push_back(newline);
	}

	// 初始化角色
	this->role.x = pic_size * 2.0 + 20.0;
	this->role.y = height - 80.0;
	this->role.step = 0.8;

	// 初始化汽车
	this->cars = {
		{ pic_size * 3.0 + 20.0, 100, 0.8, 1, 2 },
		{ pic_size * 5.0 + 20.0, 500, 0.8, 2, 4 },
		{ pic_size * 4.0 + 20.0, 250, 0.8, 3, 3 },
	};
	for (int i = 0; i < car_num; i++) {
		this->cars[i].step = (rand() % 250) / 1000.0 + 0.20;
	}
}

// 开始界面绘制
void Model::welcome(IMAGE& bk, IMAGE& w1, IMAGE& w2, IMAGE& track, vector<IMAGE>& img) {
	SetWindowText(GetHWnd(), "C++赛车竞速");
	cleardevice();
	this->drawLayout(bk, track);
	this->drawLine();
	// 绘制其他车子
	for (int i = 0; i < car_num; i++) {
		putimage(this->cars[i].x, this->cars[i].y, &img[this->cars[i].kind]);
	}
	this->drawAlpha(&w1, 130, 115);
	this->drawAlpha(&w2, 160, 305);
	this->drawButton(this->beginGame);
	this->drawButton(this->endGame);
}

// 游戏界面绘制
void Model::draw(IMAGE& bk, IMAGE& track, vector<IMAGE>& img) {
	cleardevice();
	this->drawLayout(bk, track);
	this->drawLine();
	// 绘制其他车子
	for (int i = 0; i < car_num; i++) {
		putimage(this->cars[i].x, this->cars[i].y, &img[this->cars[i].kind]);
	}
	// 绘制角色车子
	this->drawAlpha(&img[0], this->role.x, this->role.y);
}

// 绘制赛道
void Model::drawLayout(IMAGE& bk, IMAGE& track) {
	for (int i = 0; i < row; i++) {
		for (int j = 0; j < col; j++) {
			// 将数组下标转化为坐标
			int x = pic_size * j;
			int y = pic_size * i;
			// 判断分发
			if (j >= 2 && j <= 5) {
				// 车道
				putimage(x, y, &track);
			}
			else {
				// 背景
				putimage(x, y, &bk);
			}
		}
	}
	// 地图美化
	setlinestyle(PS_SOLID, 5);
	setlinecolor(RGB(40, 40, 40));
	line(pic_size * 2, 0, pic_size * 2, height);
	line(pic_size * 6, 0, pic_size * 6, height);
	setlinecolor(RGB(245, 245, 245));
	line(pic_size * 4, 0, pic_size * 4, height);
}

// 绘制线条
void Model::drawLine() {
	for (int i = 0; i < line_num; i++) {
		setlinestyle(PS_SOLID, this->lines[i].width);
		setlinecolor(RGB(245, 245, 245));
		line(this->lines[i].FL_x, this->lines[i].FL_y, this->lines[i].FL_x, this->lines[i].FL_y + this->lines[i].height);
		line(this->lines[i].SL_x, this->lines[i].SL_y, this->lines[i].SL_x, this->lines[i].SL_y + this->lines[i].height);
	}
}

// 线段的移动
void Model::moveLine() {
	for (int i = line_num - 1; i >= 0; i--) {
		this->lines[i].FL_y += this->lines[i].step;
		this->lines[i].SL_y += this->lines[i].step;
		if (this->lines[i].FL_y >= height + pic_size / 2.0 || this->lines[i].SL_y >= height + pic_size / 2.0) {
			this->lines[i].FL_y = -pic_size;
			this->lines[i].SL_y = -pic_size;
		}
	}
}

// 汽车的移动
void Model::moveCar() {
	for (int i = 0; i < car_num; i++) {
		this->cars[i].y += this->cars[i].step;
		if (this->cars[i].y >= height) {
			this->cars[i].step = (rand() % 250) / 1000.0 + 0.20;
			this->cars[i].road = rand() % 4 + 1;
			while (this->cars[i].road == this->cars[(i + 1) % 3].road || this->cars[i].road == this->cars[(i + 2) % 3].road) {
				this->cars[i].road = rand() % 4 + 1;
			}
			this->cars[i].x = pic_size * (this->cars[i].road + 1.0) + 20;
			this->cars[i].y = -65;
		}
	}
}

// 判断鼠标是否在按钮中
int Model::mouseInButton(struct Button* pButton, MOUSEMSG* msg) {
	if (msg->x >= pButton->x && msg->x <= pButton->x + pButton->width &&
		msg->y >= pButton->y && msg->y <= pButton->y + pButton->height) {
		pButton->buttoncolor = RGB(235, 235, 235);
		pButton->textcolor = RGB(90, 90, 90);
		return 1; // 返回值为1，表明鼠标在按钮中
	}
	pButton->buttoncolor = RGB(252, 252, 252);
	pButton->textcolor = RGB(110, 110, 110);
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

// 按键控制（异步按键控制）
void Model::keyDown() {
	// 判断当前Windows前台应用程序活动窗口是否为当前easyx窗口
	if (GetForegroundWindow() == GetHWnd()) {
		if (GetAsyncKeyState(VK_UP) || GetAsyncKeyState('W')) {
			this->role.y -= this->role.step;
			if (this->role.y <= 3) {
				this->role.y = 3;
			}
		}
		if (GetAsyncKeyState(VK_DOWN) || GetAsyncKeyState('S')) {
			this->role.y += this->role.step;
			if (this->role.y >= height - 65.0) {
				this->role.y = height - 65.0;
			}
		}
		if (GetAsyncKeyState(VK_LEFT) || GetAsyncKeyState('A')) {
			this->role.x -= this->role.step;
			if (this->role.x <= pic_size * 2.0 + 3.0) {
				this->role.x = pic_size * 2.0 + 3.0;
			}
		}
		if (GetAsyncKeyState(VK_RIGHT) || GetAsyncKeyState('D')) {
			this->role.x += this->role.step;
			if (this->role.x >= width - 42.0 - pic_size * 2.0) {
				this->role.x = width - 42.0 - pic_size * 2.0;
			}
		}
	}
}

// 游戏结果判断
bool Model::over() {
	// 如果角色与其他车子相撞，则游戏结束
	for (int i = 0; i < car_num; i++) {
		if (this->distance(0, this->cars[i].y + 31, 0, this->role.y + 31) <= 62 - 2.0 &&
			this->distance(this->cars[i].x + 20, 0, this->role.x + 20, 0) <= 40 - 2.0) {
			return true; // 返回值为true，说明游戏结束
		}
	}
	return false; // 返回值为false，说明游戏未结束
}