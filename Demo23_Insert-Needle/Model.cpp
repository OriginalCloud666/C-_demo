#include "Model.h"
using namespace std;

Model::Model() :needleNum(0), finaltime(0.0) {
	this->beginGame = creatButton(80, 500, 150, 50, "START", 38, 12, RGB(252, 252, 252), RGB(112, 177, 205));
	this->endGame = creatButton(420, 500, 150, 50, "END", 52, 12, RGB(252, 252, 252), RGB(112, 177, 205));
}

Model::~Model() {
	delete this->beginGame;
	this->beginGame = nullptr;
	delete this->endGame;
	this->endGame = nullptr;
}

// 获取needleNum
int& Model::getNeedleNum() {
	return this->needleNum;
}

// 获取finaltime
double& Model::getFinaltime() {
	return this->finaltime;
}

// 获取role1
struct Role& Model::getRole1() {
	return this->role1;
}

// 获取role2
struct Role& Model::getRole2() {
	return this->role2;
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
	//批量加载图片
	for (int i = 0; i < pic_num; i++) {
		char fileName[25];
		sprintf(fileName, "./resource/picture/%d.jpg", imgIndex[i]);
		loadimage(&img[i], fileName);
	}
	// 加载背景图片
	loadimage(&bk, "./resource/picture/bk.jpg", width, height);
	// 加载文字图片
	loadimage(&w1, "./resource/picture/w1.png");
	loadimage(&w2, "./resource/picture/w2.png");
}

// 加载音乐
void Model::bgm() {
	mciSendString("open ./resource/music/bk.mp3 alias BGM", nullptr, 0, nullptr);
	mciSendString("play BGM repeat", nullptr, 0, nullptr);
}

// Timer定时器
int Model::timer(time_t sec, int id) {
	static time_t start[3] = { clock(), clock(), clock() };
	time_t end = clock();
	if (end - start[id] >= sec) {
		start[id] = end;
		return 1;
	}
	return 0;
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
	setlinecolor(RGB(130, 188, 213));
	setlinestyle(PS_SOLID, 5);
	setfillcolor(pButton->buttoncolor);
	fillrectangle(pButton->x, pButton->y, pButton->x + pButton->width, pButton->y + pButton->height);
	settextcolor(pButton->textcolor);
	settextstyle(28, 0, "楷体", 0, 0, 1000, 0, 0, 0);
	outtextxy(pButton->x + pButton->dx, pButton->y + pButton->dy, pButton->text);
}

// 游戏数据初始化
void Model::init() {
	this->finaltime = 30;
	this->needleNum = 0;

	// 针的数据初始化
	for (int i = 0; i < needle_num; i++) {
		struct Needle newneedle;
		newneedle.x = -width / 2;
		newneedle.y = -height / 2;
		newneedle.len = 120;
		newneedle.angle = 0;
		newneedle.rotateSpeed = (pi / 360) / 4 * 3;
		newneedle.flag = -1;
		this->needleVec.push_back(newneedle);
	}

	// 玩家数据初始化
	this->role1.x = width / 2 - 62.5 + 3;
	this->role1.y = 0;
	this->role1.ball.x = width / 2;
	this->role1.ball.y = 13;
	this->role1.ball.r = 13;
	this->role1.step = 5;
	this->role1.score = 0;
	this->role1.count = 0;
	this->role2.x = width / 2 - 62.5 + 3;
	this->role2.y = height - 125.0;
	this->role2.ball.x = width / 2;
	this->role2.ball.y = height - 13.0;
	this->role2.ball.r = 13;
	this->role2.step = 5;
	this->role2.score = 0;
	this->role2.count = 0;

	// 中部球数据初始化
	this->ball.x = width / 2;
	this->ball.y = height / 2;
	this->ball.r = 60;
}

// 开始界面绘制
void Model::welcome(IMAGE& bk, IMAGE& w1, IMAGE& w2) {
	SetWindowText(GetHWnd(), "C++见缝插针");
	setbkcolor(RGB(240, 240, 240));
	cleardevice();

	putimage(0, 0, &bk);
	this->drawAlpha(&w1, 127, 150);
	this->drawAlpha(&w2, 160, 320);
	this->drawButton(beginGame);
	this->drawButton(endGame);
}

// 绘制游戏界面
void Model::draw(IMAGE& bk, vector<IMAGE>& img) {
	cleardevice();
	putimage(0, 0, &bk);
	this->drawRole1(img);
	this->drawRole2(img);

	// 绘制玩家尾部的圆球
	settextstyle(20, 0, "楷体", 0, 0, 1000, 0, 0, 0);
	setfillcolor(RGB(123, 206, 227));

	char countName1[10];
	sprintf(countName1, "%02d", this->role1.count + 1);
	static double x1 = this->role1.ball.x;
	static double y1 = this->role1.ball.y;
	solidcircle(x1, y1, this->role1.ball.r + 10);
	clearcircle(x1, y1, this->role1.ball.r + 5);
	solidcircle(this->role1.ball.x, this->role1.ball.y, this->role1.ball.r);
	outtextxy(x1 - 10, y1 - 9, countName1);

	char countName2[10];
	sprintf(countName2, "%02d", this->role2.count + 1);
	static double x2 = this->role2.ball.x;
	static double y2 = this->role2.ball.y;
	solidcircle(x2, y2, this->role2.ball.r + 10);
	clearcircle(x2, y2, this->role2.ball.r + 5);
	solidcircle(this->role2.ball.x, this->role2.ball.y, this->role2.ball.r);
	outtextxy(x2 - 10, y2 - 9, countName2);

	// 绘制针数以及得分
	settextstyle(22, 0, "楷体", 0, 0, 1000, 0, ANTIALIASED_QUALITY, 0); // 抗锯齿
	settextcolor(RGB(46, 110, 168));

	char scoreName1[10];
	sprintf(scoreName1, "%02d", this->role1.score);
	outtextxy(60, 20, "得分：");
	outtextxy(130, 20, scoreName1);
	sprintf(countName1, "%02d", this->role1.count);
	outtextxy(60, 60, "针数：");
	outtextxy(130, 60, countName1);

	char scoreName2[10];
	sprintf(scoreName2, "%02d", this->role2.score);
	outtextxy(width - 160, height - 90, "得分：");
	outtextxy(width - 90, height - 90, scoreName2);
	sprintf(countName2, "%02d", this->role2.count);
	outtextxy(width - 160, height - 50, "针数：");
	outtextxy(width - 90, height - 50, countName2);

	// 绘制中间的圆球
	setfillcolor(RGB(123, 206, 227));
	solidcircle(this->ball.x, this->ball.y, this->ball.r);
	clearcircle(this->ball.x, this->ball.y, this->ball.r - 10);
	solidcircle(this->ball.x, this->ball.y, this->ball.r - 20);

	// 绘制中间圆球内的倒计时
	settextstyle(22, 0, "楷体", 0, 0, 1000, 0, 0, 0);
	settextcolor(RGB(245, 245, 245));
	char fileName[10];
	sprintf(fileName, "%05.2f", finaltime);
	outtextxy(this->ball.x - 34, this->ball.y - 22, "倒计时");
	outtextxy(this->ball.x - 30, this->ball.y + 4, fileName);

	// 绘制旋转针
	for (int i = 0; i < needleNum; i++) {
		setlinecolor(RGB(123, 206, 227));
		setlinestyle(PS_SOLID, 6);
		int x = this->ball.x + cos(this->needleVec[i].angle) * this->ball.r;
		int y = this->ball.y + sin(this->needleVec[i].angle) * this->ball.r;
		line(x, y, this->needleVec[i].x, this->needleVec[i].y);
		solidcircle(this->needleVec[i].x, this->needleVec[i].y, this->role1.ball.r);
	}
}

// 贴上方玩家图片
void Model::drawRole1(vector<IMAGE>& img) {
	// 源码图：SRCPAINT格式去贴图
	putimage(this->role1.x, this->role1.y, &img[2], SRCPAINT);
	// 掩码图：SRCAND格式去贴图
	putimage(this->role1.x, this->role1.y, &img[3], SRCAND);
}

// 贴下方玩家图片
void Model::drawRole2(vector<IMAGE>& img) {
	// 源码图：SRCPAINT格式去贴图
	putimage(this->role2.x, this->role2.y, &img[0], SRCPAINT);
	// 掩码图：SRCAND格式去贴图
	putimage(this->role2.x, this->role2.y, &img[1], SRCAND);
}

// 针的旋转
void Model::rotateNeedle() {
	for (int i = 0; i < needle_num; i++) {
		this->needleVec[i].angle += this->needleVec[i].rotateSpeed;
		//给针的角度设置一个界限
		if (this->needleVec[i].angle > pi) {
			this->needleVec[i].angle -= 2 * pi;
		}
		this->needleVec[i].x = ball.x + cos(this->needleVec[i].angle) * (ball.r + this->needleVec[i].len);
		this->needleVec[i].y = ball.y + sin(this->needleVec[i].angle) * (ball.r + this->needleVec[i].len);
	}
}

// 判断鼠标是否在按钮中
int Model::mouseInButton(struct Button* pButton, MOUSEMSG* msg) {
	if (msg->x >= pButton->x && msg->x <= pButton->x + pButton->width &&
		msg->y >= pButton->y && msg->y <= pButton->y + pButton->height) {
		pButton->buttoncolor = RGB(235, 235, 235);
		pButton->textcolor = RGB(96, 169, 200);
		return 1; // 返回值为1，表明鼠标在按钮中
	}
	pButton->buttoncolor = RGB(252, 252, 252);
	pButton->textcolor = RGB(112, 177, 205);
	return 0; // 返回值为0，表明鼠标不在按钮中
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
			return 1; // 返回值为1，进入游戏
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

// 按键响应（同步按键响应）
void Model::keyDown(IMAGE& bk, vector<IMAGE>& img) {
	// 判断是否有按键消息
	while (_kbhit()) {
		// 用于接收用户按键消息
		char userKey = _getch();
		fflush(stdin); // 清空键盘消息缓冲区

		switch (userKey) {
		case ' ':
			mciSendString("close shoot2", nullptr, 0, nullptr);
			mciSendString("open ./resource/music/shoot2.mp3 alias shoot2", nullptr, 0, nullptr);
			mciSendString("play shoot2", nullptr, 0, nullptr);
			// 玩家尾部球射击动画
			while (this->role1.ball.y <= this->ball.y - (this->ball.r + this->needleVec[0].len)) {
				this->role1.ball.y += this->role1.step;
				this->rotateNeedle();
				this->draw(bk, img);
				FlushBatchDraw();
			}
			this->role1.ball.y = 12;
			this->needleNum++;
			this->needleVec[this->needleNum - 1].angle = -pi / 2;
			this->needleVec[this->needleNum - 1].x = this->ball.x + cos(this->needleVec[this->needleNum - 1].angle) * (this->ball.r + this->needleVec[this->needleNum - 1].len);
			this->needleVec[this->needleNum - 1].y = this->ball.y + sin(this->needleVec[this->needleNum - 1].angle) * (this->ball.r + this->needleVec[this->needleNum - 1].len);
			this->role1.count++;
			this->role1.score++;
			for (int i = 0; i < this->needleNum - 1; i++) {
				// 如果两针之间离得很近，认为碰撞，针消失
				if (fabs(this->needleVec[this->needleNum - 1].angle - this->needleVec[i].angle) < pi / 25) {
					mciSendString("close shoot1", nullptr, 0, nullptr);
					mciSendString("open ./resource/music/shoot1.wav alias shoot1", nullptr, 0, nullptr);
					mciSendString("play shoot1", nullptr, 0, nullptr);
					this->needleVec[this->needleNum - 1].x = -width / 2;
					this->needleVec[this->needleNum - 1].y = -height / 2;
					this->needleVec[this->needleNum - 1].angle = 0;
					this->needleNum--;
					this->role1.score--;
					break;
				}
			}
			break;
		case '0':
			mciSendString("close shoot2", nullptr, 0, nullptr);
			mciSendString("open ./resource/music/shoot2.mp3 alias shoot2", nullptr, 0, nullptr);
			mciSendString("play shoot2", nullptr, 0, nullptr);
			// 玩家尾部球射击动画
			while (this->role2.ball.y >= this->ball.y + (this->ball.r + this->needleVec[0].len)) {
				this->role2.ball.y -= this->role2.step;
				this->rotateNeedle();
				this->draw(bk, img);
				FlushBatchDraw();
			}
			this->role2.ball.y = height - 12.0;
			this->needleNum++;
			this->needleVec[this->needleNum - 1].angle = pi / 2;
			this->needleVec[this->needleNum - 1].x = this->ball.x + cos(this->needleVec[this->needleNum - 1].angle) * (this->ball.r + this->needleVec[this->needleNum - 1].len);
			this->needleVec[this->needleNum - 1].y = this->ball.y + sin(this->needleVec[this->needleNum - 1].angle) * (this->ball.r + this->needleVec[this->needleNum - 1].len);
			this->role2.count++;
			this->role2.score++;
			for (int i = 0; i < this->needleNum - 1; i++) {
				// 如果两针之间离得很近，认为碰撞，针消失
				if (fabs(this->needleVec[this->needleNum - 1].angle - this->needleVec[i].angle) < pi / 25) {
					mciSendString("close shoot1", nullptr, 0, nullptr);
					mciSendString("open ./resource/music/shoot1.wav alias shoot1", nullptr, 0, nullptr);
					mciSendString("play shoot1", nullptr, 0, nullptr);
					this->needleVec[this->needleNum - 1].x = -width / 2;
					this->needleVec[this->needleNum - 1].y = -height / 2;
					this->needleVec[this->needleNum - 1].angle = 0;
					this->needleNum--;
					this->role2.score--;
					break;
				}
			}
			break;
		default:
			break;
		}
	}
}

// 游戏胜负判断
bool Model::over() {
	if (this->finaltime <= 0) {
		this->finaltime = 0;
		return true; // 返回1时，游戏结束
	}
	return false; // 返回0时，游戏未结束
}