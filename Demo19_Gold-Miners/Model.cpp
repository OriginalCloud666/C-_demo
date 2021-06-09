#include "Model.h"
using namespace std;

Model::Model() :aim(0), coin(0), cas(0), tempcas(0.0) {
	this->beginGame = creatButton(80, 500, 150, 50, "START", 38, 12, RGB(210, 152, 64), RGB(240, 240, 240));
	this->endGame = creatButton(420, 500, 150, 50, "END", 52, 12, RGB(210, 152, 64), RGB(240, 240, 240));
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
void Model::load(IMAGE& bk, IMAGE& w1, IMAGE& w2, IMAGE rolen[][2], vector<IMAGE>& img, vector<int>& imgIndex) {
	// 批量加载图片
	for (int i = 0; i < pic_num; i++) {
		char fileName[25];
		sprintf(fileName, "./resource/picture/%d.jpg", imgIndex[i]);
		loadimage(&img[i], fileName);
	}
	// 加载背景图片
	loadimage(&bk, "./resource/picture/bk.jpg", width * 1, height * 0.85);
	// 加载人物图片
	loadimage(rolen[0], "./resource/picture/role1.jpg");
	loadimage(rolen[0] + 1, "./resource/picture/roley1.jpg");
	loadimage(rolen[1], "./resource/picture/role2.jpg");
	loadimage(rolen[1] + 1, "./resource/picture/roley2.jpg");
	// 加载文字图片
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
	setlinecolor(RGB(180, 124, 41));
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

	this->cas = 0;
	this->tempcas = 0;
	this->coin = 0;
	this->aim = 650;

	// 初始化人物
	this->role.width = 140;
	this->role.height = 120;
	this->role.x = width / 2 - this->role.width / 2;
	this->role.y = 4;

	// 初始化钩子
	this->hook.start_x = width / 2 - 20.0;
	this->hook.start_y = this->role.height - 20.0;
	this->hook.len = 50;
	this->hook.radian = pi / 2;
	this->hook.end_x = this->hook.start_x + cos(this->hook.radian) * this->hook.len;
	this->hook.end_y = this->hook.start_y + sin(this->hook.radian) * this->hook.len;
	this->hook.dir = 1; // 初始情况下，钩子向右移动
	this->hook.rockflag = true; // 初始情况下，钩子移动

	// 初始化物品
	for (int i = 0; i < gold1_num; i++) {
		struct Mine newgold;
		newgold.x = 0;
		newgold.y = 0;
		newgold.value = 120;
		newgold.width = 110;
		newgold.height = 97;
		newgold.weight = 0.8;
		newgold.flag = true; // 初始情况下金子存在
		this->gold1.push_back(newgold);
	}
	this->gold1[0].x = 30; this->gold1[0].y = 510;
	this->gold1[1].x = 265; this->gold1[1].y = 585;
	this->gold1[2].x = 500; this->gold1[2].y = 445;
	for (int i = 0; i < gold2_num; i++) {
		struct Mine newgold;
		newgold.x = 0;
		newgold.y = 0;
		newgold.value = 50;
		newgold.width = 50;
		newgold.height = 44;
		newgold.weight = 0.4;
		newgold.flag = true; // 初始情况下金子存在
		this->gold2.push_back(newgold);
	}
	this->gold2[0].x = 100; this->gold2[0].y = 350;
	this->gold2[1].x = 260; this->gold2[1].y = 370;
	this->gold2[2].x = 520; this->gold2[2].y = 345;
	for (int i = 0; i < stone_num; i++) {
		struct Mine newstone;
		newstone.x = 0;
		newstone.y = 0;
		newstone.value = 20;
		newstone.width = 80;
		newstone.height = 67;
		newstone.weight = 1.0;
		newstone.flag = true; // 初始情况下石头存在
		this->stone.push_back(newstone);
	}
	this->stone[0].x = 200; this->stone[0].y = 465;
	this->stone[1].x = 395; this->stone[1].y = 380;
	this->stone[2].x = 40; this->stone[2].y = 220;
	for (int i = 0; i < sack_num; i++) {
		struct Mine newsack;
		newsack.x = 0;
		newsack.y = 0;
		newsack.value = (rand() % 11 + 2) * 10;
		newsack.width = 60;
		newsack.height = 50;
		newsack.weight = 0.2;
		newsack.flag = true; // 初始情况下袋子存在
		this->sack.push_back(newsack);
	}
	this->sack[0].x = 320; this->sack[0].y = 480;
	this->sack[1].x = 540; this->sack[1].y = 180;
	this->sack[2].x = 220; this->sack[2].y = 220;
	this->sack[3].x = 420; this->sack[3].y = 240;
}

// 绘制开始界面
void Model::welcome(IMAGE& bk, IMAGE& w1, IMAGE& w2, IMAGE rolen[][2], vector<IMAGE> img) {
	setbkcolor(RGB(223, 172, 11));
	cleardevice();
	SetWindowText(GetHWnd(), "C++黄金矿工");

	setfillcolor(RGB(42, 63, 156));
	solidcircle(width / 2, this->role.height + 5, this->role.height);
	putimage(0, this->role.height + 5, &bk);
	this->drawRole(rolen);
	this->drawGold1(img);
	this->drawGold2(img);
	this->drawStone(img);
	this->drawSack(img);
	this->drawButton(this->beginGame);
	this->drawButton(this->endGame);
	this->drawHook();
	drawAlpha(&w1, 20, 160);
	drawAlpha(&w2, 160, 320);

	//显示金币数量
	settextstyle(26, 0, "楷体", 0, 0, 1000, 0, 0, 0);
	settextcolor(RGB(240, 240, 240));
	char coinName[25];
	sprintf(coinName, "金钱：$%d", coin);
	outtextxy(7, 23, coinName);
	char aimName[25];
	sprintf(aimName, "目标钱数：$%d", aim);
	outtextxy(5, 69, aimName);
}

// 游戏界面绘制
void Model::draw(IMAGE& bk, IMAGE rolen[][2], vector<IMAGE> img) {
	cleardevice();
	setfillcolor(RGB(42, 63, 156));
	solidcircle(width / 2, this->role.height + 5, this->role.height);
	putimage(0, this->role.height + 5, &bk);
	this->drawRole(rolen);
	this->drawGold1(img);
	this->drawGold2(img);
	this->drawStone(img);
	this->drawSack(img);
	this->drawHook();

	// 显示金币数量
	settextstyle(26, 0, "楷体", 0, 0, 1000, 0, 0, 0);
	settextcolor(RGB(240, 240, 240));
	char coinName[25];
	sprintf(coinName, "金钱：$%d", this->coin);
	outtextxy(7, 23, coinName);
	char aimName[25];
	sprintf(aimName, "目标钱数：$%d", this->aim);
	outtextxy(5, 69, aimName);
}

// 绘制钩子
void Model::drawHook() {
	setlinestyle(PS_SOLID, 3);
	setlinecolor(RGB(83, 83, 83));
	line(this->hook.start_x, this->hook.start_y, this->hook.end_x, this->hook.end_y);

	int length1 = 20;
	int Rend_x = this->hook.end_x + length1 * cos(pi / 5.0 - pi / 2 + this->hook.radian);
	int Rend_y = this->hook.end_y + length1 * sin(pi / 5.0 - pi / 2 + this->hook.radian);
	int Lend_x = this->hook.end_x - length1 * cos(pi / 5.0 + pi / 2 - this->hook.radian);
	int Lend_y = this->hook.end_y + length1 * sin(pi / 5.0 + pi / 2 - this->hook.radian);
	setlinestyle(PS_SOLID, 4);
	setlinecolor(RGB(134, 134, 134));
	line(this->hook.end_x, this->hook.end_y + 1, Rend_x, Rend_y);
	line(this->hook.end_x, this->hook.end_y + 1, Lend_x, Lend_y);

	int length2 = 10;
	int Rend_xx = Rend_x - length2 * cos(-pi / 5.0 - pi / 2 + this->hook.radian);
	int Rend_yy = Rend_y - length2 * sin(-pi / 5.0 - pi / 2 + this->hook.radian);
	int Lend_xx = Lend_x + length2 * cos(-pi / 5.0 + pi / 2 - this->hook.radian);
	int Lend_yy = Lend_y - length2 * sin(-pi / 5.0 + pi / 2 - this->hook.radian);
	setlinestyle(PS_SOLID, 3);
	setlinecolor(RGB(134, 134, 134));
	line(Rend_x, Rend_y, Rend_xx, Rend_yy);
	line(Lend_x, Lend_y, Lend_xx, Lend_yy);
}

// 贴矿工图片
void Model::drawRole(IMAGE rolen[][2]) {
	// 掩码图：SRCAND格式去贴图
	putimage(this->role.x, this->role.y, rolen[this->cas] + 1, SRCAND);
	// 源码图：SRCPAINT格式去贴图
	putimage(this->role.x, this->role.y, rolen[this->cas], SRCPAINT);
}

// 贴大金子图片
void Model::drawGold1(vector<IMAGE> img) {
	for (int i = 0; i < gold1_num; i++) {
		if (this->gold1[i].flag == true) {
			// 掩码图：SRCAND格式去贴图
			putimage(this->gold1[i].x, this->gold1[i].y, &img[0], SRCAND);
			// 源码图：SRCPAINT格式去贴图
			putimage(this->gold1[i].x, this->gold1[i].y, &img[1], SRCPAINT);
		}
	}
}

// 贴小金子图片
void Model::drawGold2(vector<IMAGE> img) {
	for (int i = 0; i < gold2_num; i++) {
		if (this->gold2[i].flag == true) {
			// 掩码图：SRCAND格式去贴图
			putimage(this->gold2[i].x, this->gold2[i].y, &img[2], SRCAND);
			// 源码图：SRCPAINT格式去贴图
			putimage(this->gold2[i].x, this->gold2[i].y, &img[3], SRCPAINT);
		}
	}
}

// 贴石头图片
void Model::drawStone(vector<IMAGE> img) {
	for (int i = 0; i < stone_num; i++) {
		if (this->stone[i].flag == true) {
			// 掩码图：SRCAND格式去贴图
			putimage(this->stone[i].x, this->stone[i].y, &img[4], SRCAND);
			// 源码图：SRCPAINT格式去贴图
			putimage(this->stone[i].x, this->stone[i].y, &img[5], SRCPAINT);
		}
	}
}

// 贴袋子图片
void Model::drawSack(vector<IMAGE> img) {
	for (int i = 0; i < sack_num; i++) {
		if (this->sack[i].flag == true) {
			// 掩码图：SRCAND格式去贴图
			putimage(this->sack[i].x, this->sack[i].y, &img[6], SRCAND);
			// 源码图：SRCPAINT格式去贴图
			putimage(this->sack[i].x, this->sack[i].y, &img[7], SRCPAINT);
		}
	}
}

// 钩子摆动
void Model::rockHook() {
	if (this->hook.rockflag == true) {
		// 防止移动出现越界
		if (this->hook.radian > pi) { // 防止向左越界
			this->hook.radian = pi;
			this->hook.radian -= 0.0044;
			this->hook.dir = 1; // 越界后开始向右移动
		}
		else if (this->hook.radian < 0) { // 防止向右越界
			this->hook.radian = 0;
			this->hook.radian += 0.0044;
			this->hook.dir = 0; // 越界后开始向左移动
		}

		// 控制钩子移动方向
		if (this->hook.dir == 1) { // 向右移动
			this->hook.radian -= 0.0044;
		}
		else if (this->hook.dir == 0) { // 向左移动
			this->hook.radian += 0.0044;
		}
	}
	else {
		// 人物收钩子动画
		this->tempcas += 0.022;
		if (this->tempcas >= 2) {
			this->tempcas = 0;
		}
		this->cas = (int)this->tempcas;
		// 钩子的伸缩
		if (this->hook.flexflag == true) { // 伸
			this->hook.len += 1.2;
			if (this->hook.len >= 630) {
				this->hook.len = 630;
				this->hook.flexflag = false;
			}
		}
		else if (this->hook.flexflag == false) { // 缩
			this->hook.len -= 1.2;
			if (this->hook.len <= 50) {
				this->hook.len = 50;
				this->hook.rockflag = true;
			}
		}
	}
	this->hook.end_x = this->hook.start_x + cos(this->hook.radian) * this->hook.len;
	this->hook.end_y = this->hook.start_y + sin(this->hook.radian) * this->hook.len;
}

// 钩子抓取物品
void Model::grabHook() {
	// 设置x，y坐标
	int length = 25;
	double x = this->hook.end_x + length * cos(this->hook.radian);
	double y = this->hook.end_y + length * sin(this->hook.radian);

	// 钩子与物品接触
	for (int i = 0; i < gold1_num; i++) {
		if (this->gold1[i].flag == true &&
			x >= this->gold1[i].x + 5 && x <= this->gold1[i].x + 5 + this->gold1[i].width &&
			y >= this->gold1[i].y + 5 && y <= this->gold1[i].y + 5 + this->gold1[i].height) {
			this->gold1[i].x = x - 48;
			this->gold1[i].y = y - 15;
			this->hook.flexflag = false;
			this->hook.len += this->gold1[i].weight;
			if (this->hook.rockflag == true) {
				this->gold1[i].flag = false;
				this->coin += this->gold1[i].value;
			}
		}
	}
	for (int i = 0; i < gold2_num; i++) {
		if (this->gold2[i].flag == true &&
			x >= this->gold2[i].x && x <= this->gold2[i].x + this->gold2[i].width &&
			y >= this->gold2[i].y && y <= this->gold2[i].y + this->gold2[i].height) {
			this->gold2[i].x = x - 23;
			this->gold2[i].y = y - 10;
			this->hook.flexflag = false;
			this->hook.len += this->gold2[i].weight;
			if (this->hook.rockflag == true) {
				this->gold2[i].flag = false;
				this->coin += this->gold2[i].value;
			}
		}
	}
	for (int i = 0; i < stone_num; i++) {
		if (this->stone[i].flag == true &&
			x >= this->stone[i].x && x <= this->stone[i].x + this->stone[i].width &&
			y >= this->stone[i].y && y <= this->stone[i].y + this->stone[i].height) {
			this->stone[i].x = x - 48;
			this->stone[i].y = y - 10;
			this->hook.flexflag = false;
			this->hook.len += this->stone[i].weight;
			if (this->hook.rockflag == true) {
				this->stone[i].flag = false;
				this->coin += this->stone[i].value;
			}
		}
	}
	for (int i = 0; i < sack_num; i++) {
		if (this->sack[i].flag == true &&
			x >= this->sack[i].x && x <= this->sack[i].x + this->sack[i].width &&
			y >= this->sack[i].y && y <= this->sack[i].y + this->sack[i].height) {
			this->sack[i].x = x - 20;
			this->sack[i].y = y - 10;
			this->hook.flexflag = false;
			this->hook.len += this->sack[i].weight;
			if (this->hook.rockflag == true) {
				this->sack[i].flag = false;
				this->coin += this->sack[i].value;
			}
		}
	}
}

// 判断鼠标是否在按钮中
int Model::mouseInButton(struct Button* pButton, MOUSEMSG* msg) {
	if (msg->x >= pButton->x && msg->x <= pButton->x + pButton->width &&
		msg->y >= pButton->y && msg->y <= pButton->y + pButton->height) {
		pButton->buttoncolor = RGB(230, 121, 55);
		pButton->textcolor = RGB(135, 135, 135);
		return 1; // 返回值为1，表明鼠标在按钮中
	}
	pButton->buttoncolor = RGB(234, 139, 83);
	pButton->textcolor = RGB(240, 240, 240);
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
void Model::keyDown() {
	// 判断是否有按键消息
	while (_kbhit()) {
		// 用于接收用户信息
		char userKey = _getch();
		fflush(stdin); // 清空键盘消息缓冲区

		switch (userKey) {
		case ' ':
			this->hook.rockflag = false;
			this->hook.flexflag = true;
			break;
		}
	}
}

// 游戏结束条件判断
bool Model::over() {
	if (this->coin >= 650) {
		return true; // 返回值为true时游戏结束
	}
	return false; // 返回值为false时游戏继续
}