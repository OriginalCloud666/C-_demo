#include "Model.h"
using namespace std;

Model::Model() :score(0), speed(0), bullet_num(0) {
	this->bat.cx = 0;
	this->bat.cy = 0;
	this->bat.len = 0;
	this->bat.x = 0;
	this->bat.y = 0;
	this->bat.radian = 0;
}

// 加载资源
void Model::load(IMAGE& bk, IMAGE& w1, IMAGE& w2, IMAGE& w3, IMAGE& w4, const int width, const int height) {
	loadimage(&bk, "./resource/picture/bk.jpg", width, height * 1.58);
	loadimage(&w1, "./resource/picture/w1.png", 850, 142);
	loadimage(&w2, "./resource/picture/w2.png", 500, 83);
	loadimage(&w3, "./resource/picture/w3.png", 630, 136);
	loadimage(&w4, "./resource/picture/w4.png", 500, 83);
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

// 初始化数据
void Model::init(const int bull_num, const int balloon_num, const int width, const int height) {
	// 设置随机数种子
	srand((unsigned)time(nullptr) * rand());

	// 初始化数据
	this->score = 0;
	this->speed = 1;
	this->bullet_num = bull_num;

	// 对炮台进行初始化
	this->bat.cx = width / 2; // 炮台中心位置x坐标
	this->bat.cy = height - 10.0; // 炮台中心位置y坐标
	this->bat.radian = -3.1415926 / 2; // 炮管角度
	this->bat.len = 90; // 炮管长度
	this->bat.x = this->bat.cx; // 炮管末端x坐标
	this->bat.y = this->bat.cy - this->bat.len; // 炮管末端y坐标

	// 对子弹进行初始化
	for (int i = 0; i < bull_num; i++) {
		struct Bullet newbullet;
		newbullet.x = -width;
		newbullet.y = -height;
		newbullet.step = 1; // 子弹速度
		newbullet.flag = false; // 初始条件下子弹全不存在
		this->bullVec.push_back(newbullet);
	}

	// 对气球进行初始化
	for (int i = 0; i < balloon_num; i++) {
		struct Balloon newballoon;
		newballoon.x = rand() % width - 18.0;
		newballoon.y = rand() % height - 28.0;
		newballoon.step = (rand() % 500 / 1000.0) + 0.6; // 气球每次的位移
		newballoon.flag = true; // 初始条件下气球存在
		newballoon.color = RGB(rand() % 256, rand() % 256, rand() % 256); // 气球颜色
		this->ballVec.push_back(newballoon);
	}
}

// 绘制开始界面
void Model::welcome(IMAGE& bk, IMAGE& w1, IMAGE& w2, IMAGE& w3, IMAGE& w4) {
	putimage(0, -182, &bk);
	setbkmode(TRANSPARENT);
	SetWindowText(GetHWnd(), "C++打气球游戏");

	// 输出作者信息
	settextcolor(RGB(253, 253, 253));
	settextstyle(30, 0, "楷体", 0, 0, 1000, 0, 0, 0);
	outtextxy(17, 16, "打气球游戏");

	settextcolor(RGB(253, 253, 253));
	settextstyle(28, 0, "楷体", 0, 0, 800, 0, 0, 0);
	outtextxy(20, 57, "作者：初云");

	this->drawAlpha(&w1, 132, 170);
	this->drawAlpha(&w2, 190, 275);
	this->drawAlpha(&w3, 17, 340);
	this->drawAlpha(&w4, 207, 415);

	while (!_kbhit()) {
	}
}

//游戏界面绘制
void Model::draw(IMAGE& bk, const int bull_num, const int balloon_num, const int height) {
	cleardevice();//清屏
	putimage(0, -182, &bk);

	// 输出作者信息
	settextcolor(RGB(253, 253, 253));
	settextstyle(30, 0, "楷体", 0, 0, 1000, 0, 0, 0);
	outtextxy(17, 16, "打气球游戏");

	settextcolor(RGB(253, 253, 253));
	settextstyle(28, 0, "楷体", 0, 0, 800, 0, 0, 0);
	outtextxy(20, 57, "作者：初云");

	// 显示游戏得分
	settextcolor(RGB(254, 254, 254));
	settextstyle(28, 0, "楷体", 0, 0, 800, 0, 0, 0);
	char scoreName[20];
	sprintf(scoreName, "游戏得分：%d", this->score);
	outtextxy(36, height - 55, scoreName);

	// 显示子弹数量
	settextcolor(RGB(255, 255, 255));
	settextstyle(25, 0, "楷体", 0, 0, 800, 0, 0, 0);
	char bullName[25];
	sprintf(bullName, "子弹数量：%02d/%d", this->bullet_num, bull_num);
	outtextxy(412, height - 45, bullName);

	// 显示子弹速度能级
	settextcolor(RGB(255, 255, 255));
	settextstyle(25, 0, "楷体", 0, 0, 800, 0, 0, 0);
	char speedName[25];
	sprintf(speedName, "子弹速度能级：%d", this->speed);
	outtextxy(413, height - 85, speedName);

	// 绘制气球
	for (int i = 0; i < balloon_num; i++) {
		if (this->ballVec[i].flag == true) {
			// 绘制气球头部椭圆
			setfillcolor(this->ballVec[i].color);
			solidellipse(this->ballVec[i].x, this->ballVec[i].y, this->ballVec[i].x + 35, this->ballVec[i].y + 55);

			// 绘制气球尾部尾巴
			setlinecolor(WHITE);
			setlinestyle(PS_SOLID, 2);
			arc(this->ballVec[i].x, this->ballVec[i].y + 55, this->ballVec[i].x + 25, this->ballVec[i].y + 55 + 27, 0, 1.2);

			// 绘制气球内部的弧线，让其看起来更像气球
			setlinestyle(PS_SOLID, 3);
			arc(this->ballVec[i].x + 10, this->ballVec[i].y + 10, this->ballVec[i].x + 10 + 15, this->ballVec[i].y + 10 + 37, 0, 1.2);
		}
	}

	// 绘制炮台底座
	setlinecolor(RGB(245, 245, 245));
	setlinestyle(PS_DASHDOT, 5);
	circle(this->bat.cx, this->bat.cy, 70);

	// 绘制炮台中心
	setfillcolor(RGB(45, 45, 45));
	solidcircle(this->bat.cx, this->bat.cy, 8);

	// 绘制炮管
	setlinecolor(RGB(45, 45, 45));
	setlinestyle(PS_SOLID, 5);
	line(this->bat.cx, this->bat.cy, this->bat.x, this->bat.y);

	// 绘制子弹
	for (int i = 0; i < bull_num; i++) {
		if (this->bullVec[i].flag == true) {
			setfillcolor(this->bullVec[i].color);
			solidcircle(this->bullVec[i].x, this->bullVec[i].y, 5);
		}
	}
}

// 产生子弹
void Model::creatBullet(const int bull_num) {
	for (int i = 0; i < bull_num; i++) {
		if (this->bullVec[i].flag == false) { // 如果子弹不存在，则创造子弹
			this->bullVec[i].flag = true;
			this->bullVec[i].x = bat.x; // 设置子弹x坐标
			this->bullVec[i].y = bat.y; // 设置子弹y坐标
			this->bullVec[i].color = RGB(rand() % 256, rand() % 256, rand() % 256); // 设置子弹颜色
			this->bullVec[i].vx = this->bullVec[i].step * cos(bat.radian); // 设置子弹x轴上每次移动的距离
			this->bullVec[i].vy = this->bullVec[i].step * sin(bat.radian); // 设置子弹y轴上每次移动的距离
			this->bullet_num--; // 子弹为true，则子弹数量减少
			break;
		}
	}
}

// 加速子弹
void Model::speedBullet(const int bull_num) {
	mciSendString("close up", nullptr, 0, nullptr);
	mciSendString("open ./resource/music/up.wav alias up", nullptr, 0, nullptr);
	mciSendString("play up", nullptr, 0, nullptr);
	for (int i = 0; i < bull_num; i++) {
		this->bullVec[i].step += 1;
		if (this->bullVec[i].step > 3) {
			this->bullVec[i].step -= 3;
		}
	}
	this->speed = this->bullVec[0].step; // 取第一颗子弹的每次位移为速度能级
}

// 填充子弹
void Model::fillBullet(const int bull_num, const int width, const int height) {
	mciSendString("close fill", nullptr, 0, nullptr);
	mciSendString("open ./resource/music/fillbullet.mp3 alias fill", nullptr, 0, nullptr);
	mciSendString("play fill", nullptr, 0, nullptr);
	this->bullet_num = 0;
	for (int i = 0; i < bull_num; i++) {
		// 判断子弹是否超出了边界
		if (this->bullVec[i].x<0 || this->bullVec[i].x>width || this->bullVec[i].y<0 || this->bullVec[i].y>height) {
			this->bullVec[i].x = -width; // 子弹超出边界后，将子弹归到初始位置
			this->bullVec[i].y = -height; // 子弹超出边界后，将子弹归到初始位置
			this->bullVec[i].flag = false; // 子弹超出边界后，子弹视作false
		}
		// 重新计算子弹数量
		if (this->bullVec[i].flag == false) {
			this->bullet_num++; // 子弹为false，则子弹数量增加
		}
	}
}

// 子弹的移动
void Model::moveBullet(const int bull_num) {
	for (int i = 0; i < bull_num; i++) {
		if (this->bullVec[i].flag == true) { // 存在的子弹是可以移动的
			this->bullVec[i].x += this->bullVec[i].vx;
			this->bullVec[i].y += this->bullVec[i].vy;
		}
	}
}

// 产生气球
void Model::creatBalloon(const int i, const int width, const int height) {
	this->ballVec[i].x = rand() % width - 18.0; // 设置气球x坐标（确保x坐标表示的是气球中点x坐标）
	this->ballVec[i].y = height; // 设置气球y坐标（确保y坐标表示的是气球中点的y坐标）
	this->ballVec[i].step = (rand() % 500) / 1000.0 + 0.6; // 设置气球每次的位移
	this->ballVec[i].flag = true; // 新产生的气球视作true
	this->ballVec[i].color = RGB(rand() % 256, rand() % 256, rand() % 256); // 设置气球颜色
}

// 气球碰撞击破
void Model::crashBalloon(const int bull_num, const int balloon_num, const int width, const int height) {
	// 遍历气球
	for (int i = 0; i < balloon_num; i++) {
		if (this->ballVec[i].flag == true) {
			// 遍历子弹
			for (int k = 0; k < bull_num; k++) {
				// 判断坐标是否发生了碰撞
				if (this->bullVec[k].x > this->ballVec[i].x && this->bullVec[k].x < this->ballVec[i].x + 35 &&
					this->bullVec[k].y > this->ballVec[i].y && this->bullVec[k].y < this->ballVec[i].y + 55) {
					mciSendString("close boom", nullptr, 0, nullptr);
					mciSendString("open ./resource/music/boom.wav alias boom", nullptr, 0, nullptr);
					mciSendString("play boom", nullptr, 0, nullptr);
					this->bullVec[k].x = -width; // 碰撞之后子弹消失（横坐标左移至界面外）
					this->bullVec[k].y = -height; // 碰撞之后子弹消失（纵坐标上移至界面外）
					this->ballVec[i].flag = false; // 碰撞之后气球消失
					this->score++; // 气球爆炸后，得分加一
					this->creatBalloon(i, width, height); // 气球碰撞消失后产生新的气球
				}
			}
		}
	}
}

// 气球的移动
void Model::moveBalloon(const int balloon_num, const int width, const int height) {
	for (int i = 0; i < balloon_num; i++) {
		if (this->ballVec[i].flag == true) {
			this->ballVec[i].y -= this->ballVec[i].step;
			if (this->ballVec[i].y < -55) {
				this->ballVec[i].flag = false; // 气球飞出界面后，将气球视作false
				this->creatBalloon(i, width, height);
			}
		}
	}
}

// 鼠标消息响应
void Model::mouseControl(const int bull_num, const int width, const int height) {
	// 判断是否有鼠标消息
	if (MouseHit()) {
		// 获取鼠标信息
		MOUSEMSG msg = GetMouseMsg();
		this->bat.radian = atan2(msg.y - this->bat.cy, msg.x - this->bat.cx); // atan2()函数用于获取y/x的反正切值（以弧度表示）
		this->bat.x = this->bat.cx + cos(bat.radian) * this->bat.len;
		this->bat.y = this->bat.cy + sin(bat.radian) * this->bat.len;
		// 如果左键按下，发射子弹
		if (msg.uMsg == WM_LBUTTONDOWN) {
			this->creatBullet(bull_num);
		}
		// 如果右键按下，加速子弹
		if (msg.uMsg == WM_RBUTTONDOWN) {
			this->speedBullet(bull_num);
		}
		// 如果滑轮滚动，填充子弹
		if (msg.uMsg == WM_MOUSEWHEEL) {
			this->fillBullet(bull_num, width, height);
		}
	}
}

// 键盘消息响应（同步响应）
void Model::keyDown(const int bull_num, const int width, const int height) {
	// 用来接收用户按键
	char key;
	// 判断是否有键盘消息
	while (_kbhit()) {
		key = (char)_getch();
		fflush(stdin); // 清空键盘消息缓冲区
		switch (key) {
			// 上方向键进行子弹的提速
		case'W':
		case'w':
		case 72:
			this->speedBullet(bull_num);
			break;
			// 下方向键进行子弹的填充
		case'S':
		case's':
		case 80:
			this->fillBullet(bull_num, width, height);
			break;
			// 空格键来进行子弹的发射
		case' ':
			this->creatBullet(bull_num);
			break;
		}
	}
}

// 按键异步响应
void Model::keyDown2(const double barrel_speed) {
	if (GetForegroundWindow() == GetHWnd()) {
		// 判断当前Windows前台窗口是否为当前绘图窗口
		if ((GetAsyncKeyState(VK_LEFT) || GetAsyncKeyState('A')) && 0x8000) { // 检测左键
			// 左方向键将炮管向左移动
			this->bat.radian = this->bat.radian - barrel_speed;
			if (this->bat.radian < -3.1415926) {
				this->bat.radian = this->bat.radian + barrel_speed;
			}
		}
		if ((GetAsyncKeyState(VK_RIGHT) || GetAsyncKeyState('D')) && 0x8000) { // 检测右键
			// 右方向键将炮管向右移动
			this->bat.radian = this->bat.radian + barrel_speed;
			if (this->bat.radian > 0) {
				this->bat.radian = this->bat.radian - barrel_speed;
			}
		}
		this->bat.x = this->bat.cx + cos(this->bat.radian) * this->bat.len;
		this->bat.y = this->bat.cy + sin(this->bat.radian) * this->bat.len;
	}
	fflush(stdin); // 清空键盘消息缓冲区
}