#include "Model.h"
using namespace std;

Model::Model() :count(0), score(0) {
	this->beginGame = creatButton(80, 500, 150, 50, "START", 38, 12, RGB(252, 252, 252), RGB(98, 147, 206));
	this->endGame = creatButton(420, 500, 150, 50, "END", 52, 12, RGB(252, 252, 252), RGB(98, 147, 206));
}

Model::~Model() {
	delete this->beginGame;
	this->beginGame = nullptr;
	delete this->endGame;
	this->endGame = nullptr;
}

// 获取score
int& Model::getScore() {
	return this->score;
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
void Model::load(IMAGE& bk, IMAGE& w1, IMAGE& w2, IMAGE& stage1, IMAGE stage2[], IMAGE& apron, IMAGE& nail, IMAGE& hp, IMAGE player[]) {
	loadimage(&bk, "./resource/picture/bk.png", pic_size, pic_size);
	loadimage(player, "./resource/picture/player.bmp");
	loadimage(player + 1, "./resource/picture/player_mask.bmp");
	loadimage(&apron, "./resource/picture/apron.png");
	loadimage(&nail, "./resource/picture/nail.png");
	loadimage(&hp, "./resource/picture/hp.png");
	loadimage(&w1, "./resource/picture/w1.png");
	loadimage(&w2, "./resource/picture/w2.png");
	loadimage(&stage1, "./resource/picture/stage01.png");
	loadimage(stage2, "./resource/picture/stage02.bmp");
	loadimage(stage2 + 1, "./resource/picture/stage02y.bmp");
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

// Timer定时器
bool Model::timer(const int ms, const int id) {
	// 设置为静态变量是为了让t1,t2只初始化一次
	static DWORD t[10];
	if (clock() - t[id] > ms) {
		t[id] = clock();
		return true;
	}
	return false;
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
	setlinecolor(RGB(79, 133, 200));
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

	// 初始情况下层数为0
	this->score = 0;
	this->count = 0;

	// 初始化木板
	for (int i = 0; i < board_num; i++) {
		struct Board newboard;
		newboard.width = 135;
		newboard.y = -20;
		this->boards.push_back(newboard);
	}

	// 初始化人物数据
	for (int i = 0; i < 4; i++) {
		this->role.dirnum[i] = 0;
		this->role.tempdir[i] = 0;
	}
	this->role.width = 60;
	this->role.height = 80;
	this->role.step = 0.95;
	this->role.x = width / 2 - role.width * 1.0 / 2;
	this->role.y = 10;
	this->role.dir = DOWN;
	this->role.flag = false;
	this->role.nail = false;
}

// 绘制开始界面
void Model::welcome(IMAGE& bk, IMAGE& w1, IMAGE& w2, IMAGE& nail, IMAGE& apron) {
	SetWindowText(GetHWnd(), "C++勇者闯关");
	cleardevice();
	this->drawBackgound(bk);
	putimage(-3, 140, &w1);
	putimage(5, 270, &w2);
	this->drawNail(nail);
	this->drawApron(apron);
	this->drawButton(beginGame);
	this->drawButton(endGame);
}

// 绘制游戏界面
void Model::draw(IMAGE& bk, IMAGE& nail, IMAGE& apron, IMAGE& stage1, IMAGE stage2[], IMAGE player[]) {
	cleardevice();
	this->drawBackgound(bk);

	// 显示游戏层数
	char scoreName[25];
	sprintf(scoreName, "当前层数：%03d", this->score);
	settextcolor(RGB(235, 235, 235));
	settextstyle(26, 0, "楷体", 0, 0, 1000, 0, 0, 0);
	outtextxy(38, 36, scoreName);

	this->drawApron(apron);
	for (int i = 0; i < board_num; i++) {
		this->drawBoard(i, stage1, stage2);
	}
	this->drawNail(nail);
	this->drawRole(player);
}

// 绘制背景
void Model::drawBackgound(IMAGE& bk) {
	for (int i = 0; i < row; i++) {
		for (int j = 0; j < col; j++) {
			// 将数组下标转化为坐标
			int x = pic_size * j;
			int y = pic_size * i;
			putimage(x, y, &bk);
		}
	}
}

// 绘制尖刺
void Model::drawNail(IMAGE& nail) {
	for (int i = 6; i + 25 <= width; i += 25) {
		this->drawAlpha(&nail, i, 0);
	}
}

// 绘制墙壁
void Model::drawApron(IMAGE& apron) {
	for (int i = 45; i <= height; i += 50) {
		putimage(0, i, &apron);
		putimage(width - 25, i, &apron);
	}
}

// 贴带尖刺木板图片
void Model::drawStage02(const int x, const int y, IMAGE stage2[]) {
	// 掩码图：SRCAND格式去贴图
	putimage(x, y, stage2 + 1, SRCAND);
	// 源码图：SRCPAINT格式去贴图
	putimage(x, y, stage2, SRCPAINT);
}

// 绘制木板
void Model::drawBoard(const int i, IMAGE& stage1, IMAGE stage2[]) {
	switch (this->boards[i].type) {
	case 1:
		putimage(this->boards[i].x, this->boards[i].y, &stage1);
		break;
	case 2:
		this->drawStage02(this->boards[i].x, this->boards[i].y, stage2);
		break;
	default:
		break;
	}
}

// 绘制角色
void Model::drawRole(IMAGE player[]) {
	switch (this->role.dir) {
	case UP:
		putimage(this->role.x, this->role.y, this->role.width, this->role.height, player + 1, this->role.dirnum[0] * this->role.width, this->role.height * 3, SRCAND);
		putimage(this->role.x, this->role.y, this->role.width, this->role.height, player, this->role.dirnum[0] * this->role.width, this->role.height * 3, SRCPAINT);
		break;
	case DOWN:
		putimage(this->role.x, this->role.y, this->role.width, this->role.height, player + 1, this->role.dirnum[1] * this->role.width, 0, SRCAND);
		putimage(this->role.x, this->role.y, this->role.width, this->role.height, player, this->role.dirnum[1] * this->role.width, 0, SRCPAINT);
		break;
	case LEFT:
		putimage(this->role.x, this->role.y, this->role.width, this->role.height, player + 1, this->role.dirnum[2] * this->role.width, this->role.height, SRCAND);
		putimage(this->role.x, this->role.y, this->role.width, this->role.height, player, this->role.dirnum[2] * this->role.width, this->role.height, SRCPAINT);
		break;
	case RIGHT:
		putimage(this->role.x, this->role.y, this->role.width, this->role.height, player + 1, this->role.dirnum[3] * this->role.width, this->role.height * 2, SRCAND);
		putimage(this->role.x, this->role.y, this->role.width, this->role.height, player, this->role.dirnum[3] * this->role.width, this->role.height * 2, SRCPAINT);
		break;
	default:
		break;
	}
}

// 木板的移动
void Model::moveBoard() {
	for (int i = 0; i < board_num; i++) {
		if (this->boards[i].y <= -this->boards[i].height && this->timer(700, 1) == true) {
			// 层数增加
			this->count++;
			// 设置木板类型随机
			int temp = rand() % 10;
			if (temp >= 7) {
				this->boards[i].type = 2;
			}
			else {
				this->boards[i].type = 1;
			}
			// 木板对应的高度设定
			if (this->boards[i].type == 1) {
				this->boards[i].height = 20;
			}
			else {
				this->boards[i].height = 46;
			}
			this->boards[i].x = rand() % (width - this->boards[i].width - 50) + 25.0;
			this->boards[i].y = height;
		}
		// 将第一块木板确定为正常木板
		this->boards[0].x = width / 2 - this->boards[0].width * 1.0 / 2;
		this->boards[0].type = 1;
		this->boards[0].height = 20;
		// 将第三块木板确定为尖刺木板
		this->boards[2].type = 2;
		this->boards[2].height = 56;
		if (this->boards[i].y <= -this->boards[i].height) {
			continue;
		}
		this->boards[i].y -= 0.4;
	}
}

// 计算游戏层数
void Model::sumScore() {
	int temp = 0;
	for (int i = 0; i < board_num; i++) {
		if (this->boards[i].y <= role.y) {
			temp++;
		}
	}
	this->score = this->count - 5 + temp + 1;
}

// 游戏结束人物动画
void Model::overAction(IMAGE& bk, IMAGE& nail, IMAGE& apron, IMAGE& stage1, IMAGE stage2[], IMAGE player[]) {
	// 设置下落速度
	double step = -2.00;

	// 绘制动画效果
	while (this->role.y <= height) {
		this->role.y += step;
		step += 0.0228;
		this->draw(bk, nail, apron, stage1, stage2, player);
		FlushBatchDraw();
	}
}

// 判断鼠标是否在按钮中
int Model::mouseInButton(struct Button* pButton, MOUSEMSG* msg) {
	if (msg->x >= pButton->x && msg->x <= pButton->x + pButton->width &&
		msg->y >= pButton->y && msg->y <= pButton->y + pButton->height) {
		pButton->buttoncolor = RGB(235, 235, 235);
		pButton->textcolor = RGB(94, 143, 204);
		return 1; // 返回值为1，表明鼠标在按钮中
	}
	pButton->buttoncolor = RGB(252, 252, 252);
	pButton->textcolor = RGB(98, 147, 206);
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

// 按键响应（异步按键响应）
void Model::keyDown() {
	// 判断当前Windows置顶窗口是否为当前绘图窗口
	if (GetForegroundWindow() == GetHWnd()) {
		if (GetAsyncKeyState(VK_LEFT) || GetAsyncKeyState('A')) {
			this->role.dir = LEFT;
			this->role.tempdir[2] += 0.028;
			if (this->role.tempdir[2] >= 4) {
				this->role.tempdir[2] = 0;
			}
			this->role.dirnum[2] = (int)this->role.tempdir[2];
			this->role.x -= this->role.step;
			if (this->role.x <= 25) {
				this->role.x = 25;
			}
		}
		if (GetAsyncKeyState(VK_RIGHT) || GetAsyncKeyState('D')) {
			this->role.dir = RIGHT;
			this->role.tempdir[3] += 0.028;
			if (this->role.tempdir[3] >= 4) {
				this->role.tempdir[3] = 0;
			}
			this->role.dirnum[3] = (int)this->role.tempdir[3];
			this->role.x += role.step;
			if (this->role.x >= width - this->role.width - 25.0) {
				this->role.x = width - this->role.width - 25.0;
			}
		}
	}
	// 判断角色是否在木板上
	for (int i = 0; i < board_num; i++) {
		double x = this->role.x + this->role.width * 1.0 / 2;
		double y = this->role.y + this->role.height;
		if (x >= this->boards[i].x && x <= this->boards[i].x + this->boards[i].width &&
			y >= this->boards[i].y && y <= this->boards[i].y + this->boards[i].height) {
			this->role.y = this->boards[i].y - this->role.height;
			this->role.flag = true;
			if (this->boards[i].type == 2) {
				this->role.nail = true;
			}
			break;
		}
		else {
			this->role.flag = false;
		}
	}
	if (this->role.flag == false) {
		this->role.y += 0.50;
	}
}

// 游戏胜负条件判断
bool Model::over() {
	// 角色碰到顶部尖刺
	if (this->role.y <= 8) {
		this->role.y = 8;
		return true; // 游戏返回值为true，则游戏结束
	}
	// 角色脱离游戏界面
	if (this->role.y >= height) {
		this->role.y = height;
		return true; // 游戏返回值为true，则游戏结束
	}
	//角色在带有尖刺的木板上
	if (this->role.nail == true) {
		return true; // 游戏返回值为true，则游戏结束、
	}
	return false; // 游戏返回值为false，则游戏继续
}