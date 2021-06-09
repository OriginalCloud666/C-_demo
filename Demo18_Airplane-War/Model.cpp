#include "Model.h"
using namespace std;

Model::Model() :score(0), cas(0) {
	this->beginGame = creatButton(80, 465, 150, 50, "START", 38, 12, RGB(252, 252, 252), RGB(150, 150, 150));
	this->endGame = creatButton(420, 465, 150, 50, "END", 52, 12, RGB(252, 252, 252), RGB(150, 150, 150));
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

// 获取role
Plane& Model::getRole() {
	return this->role;
}

// 获取beginGame
struct Button*& Model::getButton1() {
	return this->beginGame;
}

// 获取endGame
struct Button*& Model::getButton2() {
	return this->endGame;
}

// 获取enemy1
vector<Plane>& Model::getEnemy1() {
	return this->enemy1;
}

// 获取enemy2
vector<Plane>& Model::getEnemy2() {
	return this->enemy2;
}

// 加载资源
void Model::load(IMAGE& bk, IMAGE& begin, IMAGE& author, IMAGE plane[][2], IMAGE enemy_big[], IMAGE enemy_small[], IMAGE bullet[]) {
	// 加载背景图片
	loadimage(&bk, "./resource/picture/bk.jpg", width, height * 1.299);
	// 加载开始图片
	loadimage(&begin, "./resource/picture/begin.png");
	// 加载作者信息图片
	loadimage(&author, "./resource/picture/author.png");
	// 加载飞机正常图片
	loadimage(plane[0], "./resource/picture/planeNormal_1.jpg", 110, 112);
	loadimage(plane[0] + 1, "./resource/picture/planeNormal_2.jpg", 110, 112);
	// 加载飞机被击毁图片
	loadimage(plane[1], "./resource/picture/planeExplode_1.jpg", 110, 112);
	loadimage(plane[1] + 1, "./resource/picture/planeExplode_2.jpg", 110, 112);
	// 加载敌军大型飞机
	loadimage(enemy_big, "./resource/picture/enemyPlane_1.jpg", 95, 135);
	loadimage(enemy_big + 1, "./resource/picture/enemyPlane_2.jpg", 95, 135);
	// 加载敌军小型飞机
	loadimage(enemy_small, "./resource/picture/enemy_1.jpg", 48, 36);
	loadimage(enemy_small + 1, "./resource/picture/enemy_2.jpg", 48, 36);
	// 加载飞机子弹图片
	loadimage(bullet, "./resource/picture/bullet_1.jpg");
	loadimage(bullet + 1, "./resource/picture/bullet_2.jpg");
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

// 求两点之间的距离
double Model::distance(const double x1, const double y1, const double x2, const double y2) {
	// distance=根号下(x2-x1)^2+(y2-y1)^2
	return sqrt((x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2));
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

// 初始化游戏数据
void Model::init() {
	// 设置随机数种子
	srand((unsigned)time(nullptr) * rand());

	this->score = 0;
	this->cas = 0;

	// 对我军飞机进行初始化
	this->role.set(width / 2 - 48.0, height - 105.0, 1, 110, 112, 1, true);

	// 对敌军飞机进行初始化
	for (int i = 0; i < enemy1_num; i++) {
		Plane newplane1;
		newplane1.set(rand() % (width - newplane1.getWidth()), height, (rand() % 100 / 1000.0) + 0.4, 95, 135, 6, true);
		this->enemy1.push_back(newplane1);
	}
	for (int i = 0; i < enemy2_num; i++) {
		Plane newplane2;
		newplane2.set(rand() % (width - newplane2.getWidth()), height, (rand() % 100 / 1000.0) + 0.8, 48, 36, 1, true);
		this->enemy2.push_back(newplane2);
	}
}

// 绘制开始界面
void Model::welcome(IMAGE& bk, IMAGE& begin, IMAGE& author) {
	cleardevice();
	putimage(0, -132, &bk);
	SetWindowText(GetHWnd(), "C++飞机大战");

	this->drawButton(this->beginGame); // 绘制开始按钮
	this->drawButton(this->endGame); // 绘制结束按钮
	this->drawAlpha(&begin, 25, 130); // 贴开始图片
	this->drawAlpha(&author, 160, 300); // 贴作者信息
}

// 绘制游戏界面
void Model::draw(IMAGE& bk, IMAGE enemy_big[], IMAGE enemy_small[], IMAGE plane[][2], IMAGE bullet[]) {
	cleardevice();
	putimage(0, -132, &bk);

	// 绘制敌军飞机
	for (int i = 0; i < enemy1_num; i++) {
		if (this->enemy1[i].getFlag() == true) {
			this->enemy1[i].draw(enemy_big);
		}
	}
	for (int i = 0; i < enemy2_num; i++) {
		if (this->enemy2[i].getFlag() == true) {
			this->enemy2[i].draw(enemy_small);
		}
	}

	// 绘制我军飞机
	if (this->role.getFlag() == true) {
		this->role.draw(plane[this->cas]);
		this->role.drawBullet(bullet);
	}
}

// 移动敌机
void Model::moveEnemy() {
	for (int i = 0; i < enemy1_num; i++) {
		if (this->enemy1[i].getFlag() == true) {
			this->enemy1[i].get_y() += this->enemy1[i].getStep();
			if (this->enemy1[i].get_y() > height && this->timer(10000, 1) == true) {
				this->enemy1[i].getFlag() = false;
				this->enemy1[i].get_x() = rand() % (width - this->enemy1[i].getWidth());
				this->enemy1[i].get_y() = -height / 6;
				this->enemy1[i].getFlag() = true;
			}
		}
	}
	for (int i = 0; i < enemy2_num; i++) {
		if (this->enemy2[i].getFlag() == true) {
			this->enemy2[i].get_y() += this->enemy2[i].getStep();
			if (this->enemy2[i].get_y() > height && timer(300, 2) == true) {
				this->enemy2[i].getFlag() = false;
				this->enemy2[i].get_x() = rand() % (width - this->enemy2[i].getWidth());
				this->enemy2[i].get_y() = -height / 6;
				this->enemy2[i].getFlag() = true;
			}
		}
	}
}

// 敌机碰撞消失
void Model::hitEnemy(vector<Plane>& enemy, const int enemy_num, const int hp) {
	for (int i = 0; i < enemy_num; i++) {
		if (enemy[i].getFlag() == true) {
			// 对子弹进行遍历
			for (list<Bullet>::iterator iter = this->role.getBullet().begin(); iter != this->role.getBullet().end(); iter++) {
				if (iter->x + 9 >= enemy[i].get_x() &&
					iter->x + 9 <= enemy[i].get_x() + enemy[i].getWidth() &&
					iter->y + 24 >= enemy[i].get_y() &&
					iter->y + 24 <= enemy[i].get_y() + enemy[i].getHeight()) {
					// 删除当前节点
					this->role.getBullet().pop_front();
					// 被击中飞机血量减少
					enemy[i].getHp()--;
					// 血量为0时，飞机被击毁
					if (enemy[i].getHp() <= 0) {
						enemy[i].get_y() = height * 1.2;
						enemy[i].getHp() = hp;
						this->score += hp;
					}
					break;
				}
			}
		}
	}
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

// 按键响应（异步按键控制）
void Model::keyDown1() {
	// 判断当前Windows前台应用程序活动窗口是否为当前easyx绘图窗口
	if (GetForegroundWindow() == GetHWnd()) {
		if (GetAsyncKeyState(VK_UP) || GetAsyncKeyState('W')) { // 检测上键
			this->role.get_y() -= this->role.getStep();
			if (this->role.get_y() <= 0) {
				this->role.get_y() = 0;
			}
		}
		if (GetAsyncKeyState(VK_DOWN) || GetAsyncKeyState('S')) { // 检测下键
			this->role.get_y() += this->role.getStep();
			if (this->role.get_y() >= height - 55.0) {
				this->role.get_y() = height - 55.0;
			}
		}
		if (GetAsyncKeyState(VK_LEFT) || GetAsyncKeyState('A')) { // 检测左键
			this->role.get_x() -= this->role.getStep();
			if (this->role.get_x() <= -44) {
				this->role.get_x() = -44;
			}
		}
		if (GetAsyncKeyState(VK_RIGHT) || GetAsyncKeyState('D')) { // 检测右键
			this->role.get_x() += this->role.getStep();
			if (this->role.get_x() >= width - 54.0) {
				this->role.get_x() = width - 54.0;
			}
		}
	}
}

// 按键响应（同步按键控制）
void Model::keyDown2() {
	// 判断是否有按键消息
	while (_kbhit()) {
		// 用于接收按键消息
		char userKey = _getch();
		fflush(stdin); // 清空键盘消息缓冲区

		// 对按键消息进行分发
		switch (userKey) {
		case ' ':
			// 按下空格则创造子弹
			this->role.createBullet(bullet_step);
			break;
		}
	}
}

// 游戏结束条件
bool Model::condition(vector<Plane>& enemy) {
	// 当敌机与我军飞机发生碰撞时，游戏结束
	for (int i = 0; i < enemy.size(); i++) {
		if (enemy[i].getFlag() == true) {
			if (this->distance(enemy[i].get_x() + enemy[i].getWidth() / 2, enemy[i].get_y() + enemy[i].getHeight() / 2, this->role.get_x() + this->role.getWidth() / 2, this->role.get_y() + this->role.getHeight() / 2) <= 55) {
				this->cas++;
				return true; // 满足条件则返回true
			}
		}
	}
	return false; // 不满足条件则返回false
}

// 游戏结束判断
bool Model::over() {
	if (this->condition(this->enemy1) == true || this->condition(this->enemy2) == true) {
		return true; // 返回值为true时游戏结束
	}
	return false; // 返回值为false时游戏未结束
}