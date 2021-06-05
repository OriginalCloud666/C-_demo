#include "Model.h"
using namespace std;

// 加载资源
void Model::load(IMAGE& bk, IMAGE& w1, IMAGE& w2, IMAGE& w3) {
	// 加载图片
	loadimage(&bk, "./resource/picture/bk.jpg", width, height * 1.11);
	loadimage(&w1, "./resource/picture/w1.png", 700, 151);
	loadimage(&w2, "./resource/picture/w2.png", 480, 104);
	loadimage(&w3, "./resource/picture/w3.png", 750, 125);
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

// 求两点之间的距离
double Model::distance(const double x1, const double y1, const double x2, const double y2) {
	// distance=根号下(x2-x1)^2+(y2-y1)^2
	return sqrt((x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2));
}

// 使得窗口随着玩家移动
void Model::updatePos() {
	// 使得玩家在窗口正中间
	this->g_pos.x = this->roleVec[0].x - width / 2;
	this->g_pos.y = this->roleVec[0].y - height / 2;
}

// 设置窗口以及游戏地图的背景颜色
void Model::setMap(IMAGE& map) {
	setbkcolor(RGB(244, 215, 215));
	cleardevice();

	// 绘图默认是绘制到窗口上的，我们要指定绘图设备
	SetWorkingImage(&map); // 指定绘图设备为map

	setbkcolor(RGB(255, 255, 255));
	cleardevice();

	SetWorkingImage(); // 恢复默认绘图设备——窗口
}

// 初始化游戏数据
void Model::init() {
	// 设置随机数种子
	srand((unsigned)time(nullptr) * rand());

	// 初始化玩家数据
	struct Role newrole;
	newrole.r = 25;
	newrole.x = rand() % (int)(map_width - 2 * newrole.r) + newrole.r;
	newrole.y = rand() % (int)(map_height - 2 * newrole.r) + newrole.r;
	newrole.movestep = 8;
	newrole.radian = -pi / 2;
	newrole.cx = newrole.x;
	newrole.cy = newrole.y - newrole.r;
	newrole.flag = true; // 初始状态玩家存活
	newrole.color = RGB(244, 215, 215);
	this->roleVec.push_back(newrole);

	// 初始化玩家分身数据
	for (int i = 1; i < derived_num; i++) {
		struct Role newrole;
		newrole.x = this->roleVec[0].x;
		newrole.y = this->roleVec[0].y;
		newrole.radian = -pi / 2;
		newrole.cx = newrole.x; // 设置炮管末端x坐标
		newrole.cy = newrole.y - newrole.r; // 设置炮管末端y坐标
		newrole.shootstep = 9; // 初始条件下玩家分身发射速度
		newrole.flag = false; // 初始条件下玩家分身全不存在
		this->roleVec.push_back(newrole);
	}

	// 初始化食物数据
	for (int i = 0; i < food_num; i++) {
		struct Food newfood;
		newfood.r = rand() % 6 + 1.0;
		newfood.x = rand() % (int)(map_width - 2 * newfood.r) + newfood.r;
		newfood.y = rand() % (int)(map_height - 2 * newfood.r) + newfood.r;
		newfood.flag = true; // 初始状态食物存在
		newfood.color = RGB(rand() % 256, rand() % 256, rand() % 256);
		this->foodVec.push_back(newfood);
	}

	// 初始化子弹数据
	for (int i = 0; i < bullet_num; i++) {
		struct Bullet newbull;
		newbull.x = this->roleVec[0].x;
		newbull.y = this->roleVec[0].y;
		newbull.r = 10;
		newbull.shootstep = 10; // 设置子弹速度
		newbull.flag = false; // 初始条件下子弹全不存在
		this->bullVec.push_back(newbull);
	}
}

// 绘制开始界面
void Model::welcome(IMAGE& bk, IMAGE& w1, IMAGE& w2, IMAGE& w3) {
	putimage(0, -78, &bk);
	setbkmode(TRANSPARENT);
	SetWindowText(GetHWnd(), "C++球球大作战");

	drawAlpha(&w1, 60, 105);
	drawAlpha(&w2, 125, 334);
	drawAlpha(&w3, 85, 224);

	while (!_kbhit()) {
	}
}

// 绘制游戏界面
void Model::draw(IMAGE& map) {
	cleardevice();

	// 绘图默认是绘制到窗口上的，我们要指定绘图设备
	SetWorkingImage(&map); // 指定绘图设备为map

	cleardevice();

	// 对食物进行绘制
	for (int i = 0; i < food_num; i++) {
		if (this->foodVec[i].flag == true) {
			// 如果存在则进行绘制
			setfillcolor(this->foodVec[i].color);
			solidcircle(this->foodVec[i].x, this->foodVec[i].y, this->foodVec[i].r);
		}
	}

	// 对角色进行绘制
	for (int i = 0; i < derived_num; i++) {
		if (this->roleVec[i].flag == true) {
			// 对角色外围炮管进行绘制
			setfillcolor(this->roleVec[i].color);
			// 绘制外围扇形
			solidpie(this->roleVec[i].x - this->roleVec[i].r * 1.4, this->roleVec[i].y - this->roleVec[i].r * 1.4, this->roleVec[i].x + this->roleVec[i].r * 1.4, this->roleVec[i].y + this->roleVec[i].r * 1.4, -this->roleVec[0].radian - pi / 9, -this->roleVec[0].radian + pi / 9);
			clearpie(this->roleVec[i].x - this->roleVec[i].r * 1.2, this->roleVec[i].y - this->roleVec[i].r * 1.2, this->roleVec[i].x + this->roleVec[i].r * 1.2, this->roleVec[i].y + this->roleVec[i].r * 1.2, -this->roleVec[0].radian - pi / 9 - 0.1, -this->roleVec[0].radian + pi / 9 + 0.1);
			// 绘制外围三角形
			POINT point[3] = {
				point[0].x = this->roleVec[i].x + cos(this->roleVec[0].radian) * this->roleVec[i].r * 1.7, point[0].y = this->roleVec[i].y + sin(this->roleVec[0].radian) * this->roleVec[i].r * 1.7,
				point[1].x = this->roleVec[i].x + cos(this->roleVec[0].radian - pi / 12) * this->roleVec[i].r * 1.38, point[1].y = this->roleVec[i].y + sin(this->roleVec[0].radian - pi / 12) * this->roleVec[i].r * 1.38,
				point[2].x = this->roleVec[i].x + cos(this->roleVec[0].radian + pi / 12) * this->roleVec[i].r * 1.38, point[2].y = this->roleVec[i].y + sin(this->roleVec[0].radian + pi / 12) * this->roleVec[i].r * 1.38,
			};
			solidpolygon(point, 3);

			// 对角色进行绘制
			setfillcolor(this->roleVec[i].color);
			solidcircle(this->roleVec[i].x, this->roleVec[i].y, this->roleVec[i].r);
		}
	}

	// 对子弹进行绘制
	for (int i = 0; i < bullet_num; i++) {
		if (this->bullVec[i].flag == true) {
			setfillcolor(this->bullVec[i].color);
			solidcircle(this->bullVec[i].x, this->bullVec[i].y, this->bullVec[i].r);
		}
	}

	SetWorkingImage(); // 恢复默认绘图设备——窗口
	this->updatePos(); // 更新位置
	putimage(0, 0, width, height, &map, g_pos.x, g_pos.y); // 对绘制后的地图进行输出
	FlushBatchDraw(); // 双缓冲图像输出
}

// 食物被吃掉后，重新产生食物
void Model::creatFood(const int i) {
	this->foodVec[i].flag = true; // 将食物状态跟新为存在
	this->foodVec[i].r = rand() % 8 + 1.0; // 设置食物大小随机
	this->foodVec[i].x = rand() % (int)(map_width - 2 * this->foodVec[i].r) + this->foodVec[i].r;
	this->foodVec[i].y = rand() % (int)(map_height - 2 * this->foodVec[i].r) + this->foodVec[i].r;
	this->foodVec[i].color = RGB(rand() % 256, rand() % 256, rand() % 256);
}

// 吃掉食物
void Model::eatFood(struct Role* role, const int k) {
	// 食物被吃掉
	for (int i = 0; i < food_num; i++) {
		if (this->foodVec[i].flag == true) { // 如果食物存在
			if (this->distance(role->x, role->y, this->foodVec[i].x, this->foodVec[i].y) < role->r) {
				// 如果食物和玩家两者圆心距小于玩家半径，则食物被吃掉
				this->foodVec[i].flag = false; // 食物被吃掉食物消失
				role->r += this->foodVec[i].r / 6; // 吃掉食物后变大
				role->movestep -= this->foodVec[i].r / 180; // 吃掉食物后速度变慢
				if (role->r > 200) { // 对玩家的最大体型做一个限制
					role->r = 200;
				}
				if (role->movestep < 2.0) { // 对玩家的最小速度做一个限制
					role->movestep = 2.0;
				}
			}
		}
		else { // 如果食物不存在
			this->creatFood(i);
		}
	}

	// 子弹被吃掉
	for (int i = 0; i < bullet_num; i++) {
		if (this->bullVec[i].flag == true) { // 如果子弹存在
			if (this->distance(role->x, role->y, this->bullVec[i].x, this->bullVec[i].y) < role->r) {
				// 如果子弹和玩家两者圆心距小于玩家半径，则食物被吃掉
				this->bullVec[i].flag = false; // 子弹被吃掉子弹消失
				role->r += 10.0 / 6; // 吃掉食物后变大
				role->movestep -= 10.0 / 180; // 吃掉食物后速度变慢
				if (role->r > 200) { // 对玩家的最大体型做一个限制
					role->r = 200;
				}
				if (role->movestep < 2.0) { // 对玩家的最小速度做一个限制
					role->movestep = 2.0;
				}
			}
		}
	}
}

// 产生子弹
void Model::creatBullet(struct Role* role, const double radian) {
	for (int i = 0; i < bullet_num; i++) {
		if (this->bullVec[i].flag == false) { // 如果子弹不存在则创造子弹
			this->bullVec[i].flag = true;
			this->bullVec[i].curradian = radian;
			this->bullVec[i].x = role->cx; // 设置子弹x坐标
			this->bullVec[i].y = role->cy; // 设置子弹y坐标
			this->bullVec[i].color = role->color; // 设置子弹颜色
			this->bullVec[i].shootstep = 10; // 设置子弹速度
			this->bullVec[i].vx = this->bullVec[i].shootstep * cos(this->bullVec[i].curradian);
			this->bullVec[i].vy = this->bullVec[i].shootstep * sin(this->bullVec[i].curradian);
			role->r -= 10.0 / 6; // 子弹发射出去后，玩家体型减小
			role->movestep += 10.0 / 180; // 子弹发射出去后，玩家速度变快
			break;
		}
	}
}

// 子弹的移动
void Model::moveBullet() {
	for (int i = 0; i < bullet_num; i++) {
		if (this->bullVec[i].flag == true) { // 存在的子弹是可以移动的
			if (this->bullVec[i].shootstep <= 0) {
				continue;
			}
			if (this->bullVec[i].x <= 15 || this->bullVec[i].x >= map_width - 15.0 || this->bullVec[i].y <= 15 || this->bullVec[i].y >= map_height - 15.0) {
				continue;
			}
			this->bullVec[i].x += this->bullVec[i].vx;
			this->bullVec[i].y += this->bullVec[i].vy;
			this->bullVec[i].shootstep -= 0.35;
			this->bullVec[i].vx = this->bullVec[i].shootstep * cos(this->bullVec[i].curradian); // 设置每次子弹在x轴上移动距离
			this->bullVec[i].vy = this->bullVec[i].shootstep * sin(this->bullVec[i].curradian); // 设置每次子弹在y轴上移动距离
		}
	}
}

// 分身实现
void Model::creatRole(struct Role* role, const double radian) {
	for (int i = 1; i < derived_num; i++) {
		if (this->roleVec[i].flag == false) {//如果玩家分身不存在则创造分身
			this->roleVec[i].flag = true;
			this->roleVec[i].x = role->cx; // 设置玩家分身x坐标
			this->roleVec[i].y = role->cy; // 设置玩家分身y坐标
			this->roleVec[i].color = role->color; // 设置玩家分身颜色
			this->roleVec[i].r = role->r / 2; // 设置玩家分身大小
			this->roleVec[i].shootstep = 9; // 设置玩家分身发射速度
			role->r /= 2; // 分身后玩家体型减小
			role->movestep += role->r / 30; // 分身后玩家速度增大
			this->roleVec[i].curradian = radian;
			this->roleVec[i].movestep = role->movestep;
			this->roleVec[i].vx = this->roleVec[i].shootstep * cos(this->roleVec[i].curradian); // 设置每次分身在x轴上移动距离
			this->roleVec[i].vy = this->roleVec[i].shootstep * sin(this->roleVec[i].curradian); // 设置每次分身在y轴上移动距离
			break;
		}
	}
}

// 分身移动
void Model::moveRole() {
	for (int i = 1; i < derived_num; i++) {
		if (this->roleVec[i].flag == true) { // 存在的玩家分身是可以移动的
			if (this->roleVec[i].shootstep <= 0) {
				continue;
			}
			if (this->roleVec[i].x <= (5 + this->roleVec[i].r) ||
				this->roleVec[i].x >= map_width - (5 + this->roleVec[i].r) ||
				this->roleVec[i].y <= (5 + this->roleVec[i].r) ||
				this->roleVec[i].y >= map_height - (5 + this->roleVec[i].r)) {
				continue;
			}
			this->roleVec[i].x += this->roleVec[i].vx;
			this->roleVec[i].y += this->roleVec[i].vy;
			this->roleVec[i].shootstep -= 0.35;
			this->roleVec[i].vx = this->roleVec[i].shootstep * cos(this->roleVec[i].curradian); // 设置每次玩家分身在x轴上移动距离
			this->roleVec[i].vy = this->roleVec[i].shootstep * sin(this->roleVec[i].curradian); // 设置每次玩家分身在y轴上移动距离
		}
	}
}

// 分身融合
void Model::mixRole(struct Role* role) {
	// 玩家分身与玩家的融合
	for (int i = 1; i < derived_num; i++) {
		if (this->roleVec[i].flag == true) { // 如果玩家或分身存在
			if (this->distance(role->x, role->y, this->roleVec[i].x, this->roleVec[i].y) < role->r) {
				// 如果分身之间的圆心距小于分身半径，则分身融合
				this->roleVec[i].flag = false; // 融合后分身消失
				role->r += this->roleVec[i].r; // 融合后体型变大
				role->movestep -= this->roleVec[i].r / 30; // 融合后速度变慢
				if (role->r > 200) { // 对玩家的最大体型做一个限制
					role->r = 200;
				}
				if (role->movestep < 2.0) { // 对玩家的最小速度做一个限制
					role->movestep = 2.0;
				}
			}
		}
	}
}

// 角色活动
void Model::actRole() {
	for (int i = 0; i < derived_num; i++) {
		if (this->roleVec[i].flag == true) {//存在的玩家或分身可以吃食物
			this->eatFood(&this->roleVec[i], i);
		}
	}
	this->mixRole(&this->roleVec[0]);
}

// 玩家移动（异步按键控制）
void Model::keyDown1() {
	// 判断当前Windows前台应用程序活动窗口是否为当前easyx绘图窗口
	if (GetForegroundWindow() == GetHWnd()) {
		if ((GetAsyncKeyState(VK_UP) || GetAsyncKeyState('W'))) { // 检测上键
			for (int i = 0; i < this->roleVec.size(); i++) {
				if (this->roleVec[i].flag == true) {
					if (this->roleVec[i].y <= (5 + this->roleVec[i].r)) {
						continue;
					}
					this->roleVec[i].y -= this->roleVec[i].movestep;
				}
			}
		}
		if ((GetAsyncKeyState(VK_DOWN) || GetAsyncKeyState('S'))) { // 检测下键
			for (int i = 0; i < this->roleVec.size(); i++) {
				if (this->roleVec[i].flag == true) {
					if (this->roleVec[i].y >= map_height - (5 + this->roleVec[i].r)) {
						continue;
					}
					this->roleVec[i].y += this->roleVec[i].movestep;
				}
			}
		}
		if ((GetAsyncKeyState(VK_LEFT) || GetAsyncKeyState('A'))) { // 检测左键
			for (int i = 0; i < this->roleVec.size(); i++) {
				if (this->roleVec[i].flag == true) {
					if (this->roleVec[i].x <= (5 + this->roleVec[i].r)) {
						continue;
					}
					this->roleVec[i].x -= this->roleVec[i].movestep;
				}
			}
		}
		if ((GetAsyncKeyState(VK_RIGHT) || GetAsyncKeyState('D'))) { // 检测右键
			for (int i = 0; i < this->roleVec.size(); i++) {
				if (this->roleVec[i].flag == true) {
					if (this->roleVec[i].x >= map_width - (5 + this->roleVec[i].r)) {
						continue;
					}
					this->roleVec[i].x += this->roleVec[i].movestep;
				}
			}
		}
		if (GetAsyncKeyState('J')) { // 检测'J'键
			this->roleVec[0].radian -= 0.1;
		}
		if (GetAsyncKeyState('K')) { // 检测'K'键
			this->roleVec[0].radian += 0.1;
		}
	}
	for (int i = 0; i < this->roleVec.size(); i++) {
		if (this->roleVec[i].flag == true) {
			this->roleVec[i].cx = this->roleVec[i].x + cos(this->roleVec[0].radian) * this->roleVec[i].r;
			this->roleVec[i].cy = this->roleVec[i].y + sin(this->roleVec[0].radian) * this->roleVec[i].r;
		}
	}
}

// 玩家功能（同步按键控制）
void Model::keyDown2() {
	// 接收用户信息
	char key;
	// 判断是否有键盘消息
	while (_kbhit()) {
		key = (char)_getch();
		fflush(stdin); // 清空键盘消息缓冲区
		switch (key) {
			// 'U'键发射子弹
		case'U':
		case'u':
			for (int i = 0; i < derived_num; i++) {
				if (this->roleVec[i].flag == true && this->roleVec[i].r > 25) {
					this->creatBullet(&this->roleVec[i], this->roleVec[0].radian);
				}
			}
			break;
			// 'I'键玩家分解
		case'I':
		case'i':
			for (int i = derived_num - 1; i >= 0; i--) {
				if (this->roleVec[i].flag == true && this->roleVec[i].r > 60) {
					this->creatRole(&this->roleVec[i], this->roleVec[0].radian);
				}
			}
			break;
		}
	}
}

// 鼠标操作
void Model::mouseControl() {
	// 判断是否有鼠标消息
	if (MouseHit()) {
		// 获取鼠标消息
		MOUSEMSG msg = GetMouseMsg();
		this->roleVec[0].radian = atan2((double)(msg.y - height / 2.0), (double)(msg.x - width / 2.0));
		// atan2()函数用于获取y/x的反正切值（以弧度表示）
		this->roleVec[0].cx = this->roleVec[0].x + cos(this->roleVec[0].radian) * this->roleVec[0].r;
		this->roleVec[0].cy = this->roleVec[0].y + sin(this->roleVec[0].radian) * this->roleVec[0].r;
		// 如果左键按下，发射子弹
		if (msg.uMsg == WM_LBUTTONDOWN) {
			for (int i = 0; i < derived_num; i++) {
				if (this->roleVec[i].flag == true && this->roleVec[i].r > 25) {
					this->creatBullet(&this->roleVec[i], this->roleVec[0].radian);
				}
			}
		}
		// 如果右键按下，玩家分解
		if (msg.uMsg == WM_RBUTTONDOWN) {
			for (int i = derived_num - 1; i >= 0; i--) {
				if (this->roleVec[i].flag == true && this->roleVec[i].r > 60) {
					this->creatRole(&this->roleVec[i], this->roleVec[0].radian);
				}
			}
		}
	}
}