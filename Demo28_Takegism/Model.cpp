#include "Model.h"
using namespace std;

Model::Model() :top(0), left(0), down(0), right(0) {
	this->beginGame = creatButton(width / 2 + 80, 500, 150, 50, "START", 38, 12, RGB(252, 252, 252), RGB(154, 208, 216));
	this->endGame = creatButton(width / 2 + 420, 500, 150, 50, "END", 52, 12, RGB(252, 252, 252), RGB(154, 208, 216));
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
void Model::load(IMAGE& bk, IMAGE& w1, IMAGE& w2, IMAGE enem[], IMAGE play[]) {
	// 加载背景图片
	loadimage(&bk, "./resource/picture/bk.jpg", width, height * 1.243);
	// 加载文字图片
	loadimage(&w1, "./resource/picture/w1.png");
	loadimage(&w2, "./resource/picture/w2.png");
	// 加载玩家图片
	loadimage(play, "./resource/picture/role.bmp", grid_size, grid_size);
	loadimage(play + 1, "./resource/picture/role_mask.bmp", grid_size, grid_size);
	// 加载敌人图片
	loadimage(enem, "./resource/picture/sound.png", grid_size, grid_size);
	loadimage(enem + 1, "./resource/picture/sound_mask.png", grid_size, grid_size);
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
	setlinecolor(RGB(154, 208, 216));
	setlinestyle(PS_SOLID, 5);
	setfillcolor(pButton->buttoncolor);
	fillrectangle(pButton->x, pButton->y, pButton->x + pButton->width, pButton->y + pButton->height);
	settextcolor(pButton->textcolor);
	settextstyle(28, 0, "楷体", 0, 0, 1000, 0, 0, 0);
	outtextxy(pButton->x + pButton->dx, pButton->y + pButton->dy, pButton->text);
}

// 初始化墙壁
void Model::initWall() {
	// 初始化四周的墙壁
	for (int i = 0; i < row; i++) {
		this->gameMapVec[i][0] = WALL;
		this->gameMapVec[i][col - 1] = WALL;
	}
	for (int i = 0; i < col; i++) {
		this->gameMapVec[0][i] = WALL;
		this->gameMapVec[row - 1][i] = WALL;
	}
	// 初始化竖向的墙壁
	for (int i = 1; i < row / 3 - 2; i++) {
		this->gameMapVec[i][col / 2 + 4] = WALL;
	}
	for (int i = row - 8; i < row - 1; i++) {
		this->gameMapVec[i][col - 8] = WALL;
	}
	for (int i = row - 3; i < row - 1; i++) {
		this->gameMapVec[i][col - 5] = WALL;
	}
	for (int i = row - 1; i > row - 9; i--) {
		this->gameMapVec[i][col / 3 - 10] = WALL;
	}
	for (int i = row - 12; i > row / 2 - 15; i--) {
		this->gameMapVec[i][col / 4 + 6] = WALL;
	}
	for (int i = row - 14; i > row / 2 - 1; i--) {
		this->gameMapVec[i][col / 2 + 7] = WALL;
	}
	for (int i = row - 12; i > row / 2 + 2; i--) {
		this->gameMapVec[i][col / 2 + 14] = WALL;
	}
	for (int i = row - 14; i > row / 2 - 1; i--) {
		this->gameMapVec[i][col / 2 + 21] = WALL;
	}
	for (int i = 5; i < row / 2 - 6; i++) {
		this->gameMapVec[i][col / 4 - 4] = WALL;
	}
	// 初始化横向的墙壁
	for (int i = col / 2; i < col - 1; i++) {
		this->gameMapVec[row / 2 - 1][i] = WALL;
	}
	for (int i = col / 2; i > 0; i--) {
		this->gameMapVec[4][i] = WALL;
	}
	for (int i = col / 4 + 6; i < col / 2 + 4; i++) {
		this->gameMapVec[row / 2 - 6][i] = WALL;
	}
	for (int i = col / 4 + 6; i < col / 2 + 4; i++) {
		this->gameMapVec[row / 2 + 4][i] = WALL;
	}
	for (int i = col / 4 + 6; i > col / 4 - 12; i--) {
		this->gameMapVec[row / 2 + 1][i] = WALL;
	}
	for (int i = col - 5; i < col - 1; i++) {
		this->gameMapVec[row - 6][i] = WALL;
	}
	for (int i = col / 3 - 8; i < col - 1; i++) {
		this->gameMapVec[row - 11][i] = WALL;
	}
	for (int i = 1; i < col / 3 - 12; i++) {
		this->gameMapVec[row - 11][i] = WALL;
	}
	for (int i = col - 2; i > col - 4; i--) {
		this->gameMapVec[row - 11][i] = BLANK;
	}
	// 开个缺口
	this->gameMapVec[4][4] = BLANK;
	this->gameMapVec[4][5] = BLANK;
	// 设置终点
	this->gameMapVec[row - 3][col - 3] = DOOR;
}

// 游戏数据初始化
void Model::init() {
	// 设置背景颜色
	setbkcolor(RGB(245, 245, 245));

	// 游戏区域初始化
	this->left = width / 2 - (col * grid_size) / 2;
	this->top = height / 2 - (row * grid_size) / 2;
	this->right = this->left + col * grid_size;
	this->down = this->top + row * grid_size;

	// 游戏地图数据初始化
	this->gameMapVec.clear();
	for (int i = 0; i < row; i++) {
		vector<int> lineGrid;
		for (int j = 0; j < col; j++) {
			lineGrid.push_back(0);
		}
		this->gameMapVec.push_back(lineGrid);
	}

	// 墙体数据初始化
	this->initWall();

	// 玩家数据初始化
	this->role.x = 2 * grid_size;
	this->role.y = 2 * grid_size;
	this->role.step = 3.2;
	this->role.dir = NONE;

	// 敌人数据初始化
	for (int i = 0; i < enemy_num; i++) {
		struct Enemy newenemy;
		newenemy.x = this->left + grid_size * -10;
		newenemy.y = this->left + grid_size * -10;
		newenemy.step = 4.15;
		newenemy.tempstep = newenemy.step;
		newenemy.steprange = 0.105;
		newenemy.flag = 0;
		this->enemys.push_back(newenemy);
	}
	this->enemys[0].x = this->left + grid_size * 15;
	this->enemys[0].y = this->left + grid_size * 15;
	this->enemys[1].x = this->left + grid_size * 28;
	this->enemys[1].y = this->left + grid_size * 8;
	this->enemys[2].x = this->left + grid_size * 47;
	this->enemys[2].y = this->left + grid_size * 18;
	this->enemys[3].x = this->left + grid_size * 22;
	this->enemys[3].y = this->left + grid_size * 1;
	this->enemys[4].x = this->left + grid_size * 6;
	this->enemys[4].y = this->left + grid_size * 7;
	this->enemys[5].x = this->left + grid_size * 14;
	this->enemys[5].y = this->left + grid_size * 21;
	this->enemys[6].x = this->left + grid_size * 44;
	this->enemys[6].y = this->left + grid_size * 4;
	this->enemys[7].x = this->left + grid_size * 30;
	this->enemys[7].y = this->left + grid_size * 24;
	this->enemys[8].x = this->left + grid_size * 35;
	this->enemys[8].y = this->left + grid_size * 6;
	this->enemys[9].x = this->left + grid_size * 1;
	this->enemys[9].y = this->left + grid_size * 19;
	this->enemys[10].x = this->left + grid_size * 25;
	this->enemys[10].y = this->left + grid_size * 30;
	this->enemys[11].x = this->left + grid_size * 57;
	this->enemys[11].y = this->left + grid_size * 30;
	this->enemys[12].x = this->left + grid_size * 8;
	this->enemys[12].y = this->left + grid_size * 13;
	this->enemys[13].x = this->left + grid_size * 49;
	this->enemys[13].y = this->left + grid_size * 10;
	this->enemys[14].x = this->left + grid_size * 43;
	this->enemys[14].y = this->left + grid_size * 30;
	this->enemys[15].x = this->left + grid_size * 19;
	this->enemys[15].y = this->left + grid_size * 10;
	this->enemys[16].x = this->left + grid_size * 42;
	this->enemys[16].y = this->left + grid_size * 12;
	this->enemys[17].x = this->left + grid_size * 6;
	this->enemys[17].y = this->left + grid_size * 30;
	this->enemys[18].x = this->left + grid_size * 57;
	this->enemys[18].y = this->left + grid_size * 6;
	this->enemys[19].x = this->left + grid_size * 59;
	this->enemys[19].y = this->left + grid_size * 21;
	this->enemys[20].x = this->left + grid_size * 27;
	this->enemys[20].y = this->left + grid_size * 17;
	this->enemys[21].x = this->left + grid_size * 34;
	this->enemys[21].y = this->left + grid_size * 30;
	this->enemys[22].x = this->left + grid_size * 16;
	this->enemys[22].y = this->left + grid_size * 30;
	this->enemys[23].x = this->left + grid_size * 52;
	this->enemys[23].y = this->left + grid_size * 30;
}

// 开始界面绘制
void Model::welcome(IMAGE& bk, IMAGE& w1, IMAGE& w2) {
	SetWindowText(GetHWnd(), "C++密室逃脱");
	cleardevice();
	putimage(0, -160, &bk);
	this->drawAlpha(&w1, width / 2 + 125, 120);
	this->drawAlpha(&w2, width / 2 + 160, 300);
	this->drawButton(this->beginGame);
	this->drawButton(this->endGame);
}

// 游戏界面绘制
void Model::draw(IMAGE play[], IMAGE enem[]) {
	cleardevice();

	// 绘制游戏区域地图
	for (int i = 0; i < row; i++) {
		for (int j = 0; j < col; j++) {
			// 将数组下标转化为坐标
			int x = this->left + grid_size * j;
			int y = this->top + grid_size * i;
			// 对消息进行分发
			switch (this->gameMapVec[i][j]) {
			case BLANK:
				break;
			case WALL:
				setlinestyle(PS_SOLID, 1);
				setlinecolor(RGB(255, 255, 255));
				setfillcolor(RGB(93, 107, 153));
				fillrectangle(x, y, x + grid_size, y + grid_size);
				break;
			case DOOR:
				setfillcolor(RGB(255, 128, 128));
				solidrectangle(x - grid_size / 2, y - grid_size / 2, x + grid_size + grid_size / 2, y + grid_size + grid_size / 2);
				clearrectangle(x - grid_size / 4, y - grid_size / 4, x + grid_size + grid_size / 4, y + grid_size + grid_size / 4);
				setfillcolor(RGB(255, 128, 128));
				solidrectangle(x, y, x + grid_size, y + grid_size);
				break;
			default:
				break;
			}
		}
	}

	// 绘制人物以及敌人
	this->drawRole(&this->role, play);
	for (int i = 0; i < enemy_num; i++) {
		this->drawEnemy(&this->enemys[i], enem);
	}
}

// 贴玩家图片
void Model::drawRole(struct Role* role, IMAGE play[]) {
	// 掩码图：SRCAND格式去贴图
	putimage(role->x, role->y, play + 1, SRCAND);
	// 源码图：SRCPAINT格式去贴图
	putimage(role->x, role->y, play, SRCPAINT);
}

// 贴敌人图片
void Model::drawEnemy(struct Enemy* enemy, IMAGE enem[]) {
	// 掩码图：SRCAND格式去贴图
	putimage(enemy->x, enemy->y, enem + 1, SRCAND);
	// 源码图：SRCPAINT格式去贴图
	putimage(enemy->x, enemy->y, enem, SRCPAINT);
}

// 玩家自动移动
void Model::moveRole() {
	// 将玩家中点坐标转化为数组下标
	int r = (this->role.y + grid_size / 2 - this->top) / grid_size;
	int c = (this->role.x + grid_size / 2 - this->left) / grid_size;
	// 将玩家四个边角坐标转化为数组下标
	int r1 = (this->role.y - this->top) / grid_size;
	int r2 = (this->role.y + grid_size - this->top) / grid_size;
	int c1 = (this->role.x - this->left) / grid_size;
	int c2 = (this->role.x + grid_size - this->left) / grid_size;
	// 判断移动方向
	if (this->role.dir == UP) { // 向上移动
		this->role.y -= this->role.step;
		if ((this->gameMapVec[r - 1][c1] == WALL || this->gameMapVec[r - 1][c2] == WALL) && this->role.y <= this->top + grid_size * (r - 1) + grid_size) {
			this->role.y = this->top + grid_size * (r - 1) + grid_size + 2;
			this->role.dir = DOWN;
		}
	}
	else if (this->role.dir == DOWN) { // 向下移动
		this->role.y += this->role.step;
		if ((this->gameMapVec[r + 1][c1] == WALL || this->gameMapVec[r + 1][c2] == WALL) && this->role.y >= this->top + grid_size * (r + 1) - grid_size) {
			this->role.y = this->top + grid_size * (r + 1) - grid_size - 2;
			this->role.dir = UP;
		}
	}
	else if (this->role.dir == LEFT) { // 向左移动
		this->role.x -= this->role.step;
		if ((this->gameMapVec[r1][c - 1] == WALL || this->gameMapVec[r2][c - 1] == WALL) && this->role.x <= this->left + grid_size * (c - 1) + grid_size) {
			this->role.x = this->left + grid_size * (c - 1) + grid_size + 2;
			this->role.dir = RIGHT;
		}
	}
	else if (this->role.dir == RIGHT) { // 向右移动
		this->role.x += this->role.step;
		if ((this->gameMapVec[r1][c + 1] == WALL || this->gameMapVec[r2][c + 1] == WALL) && this->role.x >= this->left + grid_size * (c + 1) - grid_size) {
			this->role.x = this->left + grid_size * (c + 1) - grid_size - 2;
			this->role.dir = LEFT;
		}
	}
}

// 敌人的自动移动
void Model::moveEnemy() {
	for (int i = 0; i < enemy_num; i++) {
		if (this->enemys[i].flag == 0) {
			this->enemys[i].step -= this->enemys[i].steprange;
			if (this->enemys[i].step <= -this->enemys[i].tempstep) {
				this->enemys[i].step = -this->enemys[i].tempstep;
				this->enemys[i].flag = 1;
			}
		}
		else if (this->enemys[i].flag == 1) {
			this->enemys[i].step += this->enemys[i].steprange;
			if (this->enemys[i].step >= this->enemys[i].tempstep) {
				this->enemys[i].step = this->enemys[i].tempstep;
				this->enemys[i].flag = 0;
			}
		}
		if (i < enemy_num / 6) {
			this->enemys[i].x += this->enemys[i].step * 0.93;
		}
		else if (i >= enemy_num / 6 && i < enemy_num / 3) {
			this->enemys[i].x -= this->enemys[i].step * 0.93;
		}
		else if (i >= enemy_num / 3 && i < enemy_num / 2) {
			this->enemys[i].y += this->enemys[i].step * 0.93;
		}
		else if (i >= enemy_num / 2 && i < enemy_num / 3 * 2) {
			this->enemys[i].y -= this->enemys[i].step * 0.93;
		}
		else if (i >= enemy_num / 3 * 2 && i < enemy_num / 3 * 2 + 2) {
			this->enemys[i].x += this->enemys[i].step * 2 / 3 * 0.9;
			this->enemys[i].y += this->enemys[i].step * 4 / 5 * 0.9;
		}
		else if (i >= enemy_num / 6 * 5 - 2 && i < enemy_num / 6 * 5) {
			this->enemys[i].x -= this->enemys[i].step * 2 / 3 * 0.9;
			this->enemys[i].y -= this->enemys[i].step * 4 / 5 * 0.9;
		}
		else if (i >= enemy_num / 6 * 5 && i < enemy_num / 6 * 5 + 2) {
			this->enemys[i].x -= this->enemys[i].step * 2 / 3 * 0.9;
			this->enemys[i].y += this->enemys[i].step * 4 / 5 * 0.9;
		}
		else {
			this->enemys[i].x += this->enemys[i].step * 2 / 3 * 0.9;
			this->enemys[i].y -= this->enemys[i].step * 4 / 5 * 0.9;
		}
	}
}

// 判断鼠标是否在按钮中
int Model::mouseInButton(struct Button* pButton, MOUSEMSG* msg) {
	if (msg->x >= pButton->x && msg->x <= pButton->x + pButton->width &&
		msg->y >= pButton->y && msg->y <= pButton->y + pButton->height) {
		pButton->buttoncolor = RGB(235, 235, 235);
		pButton->textcolor = RGB(137, 201, 209);
		return 1; // 返回值为1，表明鼠标在按钮中
	}
	pButton->buttoncolor = RGB(252, 252, 252);
	pButton->textcolor = RGB(154, 208, 216);
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

// 按键响应（同步按键响应）
void Model::keyDown() {
	// 判断是否有按键消息
	while (_kbhit()) {
		// 用于接收按键消息
		char userKey = _getch();
		fflush(stdin); // 清空键盘消息缓冲区
		// 对按键消息进行分发
		switch (userKey) {
		case 'W':
		case 'w':
		case 72:
			this->role.dir = UP;
			break;
		case 'S':
		case 's':
		case 80:
			this->role.dir = DOWN;
			break;
		case 'A':
		case 'a':
		case 75:
			this->role.dir = LEFT;
			break;
		case 'D':
		case 'd':
		case 77:
			this->role.dir = RIGHT;
			break;
		default:
			break;
		}
	}
}

// 游戏结束条件1
int Model::condition1() {
	// 当玩家与障碍物碰撞时游戏结束
	for (int i = 0; i < enemy_num; i++) {
		if (this->distance(this->enemys[i].x, this->enemys[i].y, this->role.x, this->role.y) <= 15) {
			return 1; // 返回值为1，说明条件满足
		}
	}
	return 0; // 返回值为0，说明条件未满足
}

// 游戏结束条件2
int Model::condition2() {
	// 玩家到达终点时游戏结束
	// 将玩家中点坐标转化为数组下标
	int r = (this->role.y + grid_size / 2 - this->top) / grid_size;
	int c = (this->role.x + grid_size / 2 - this->left) / grid_size;
	if (this->gameMapVec[r][c] == DOOR) {
		return 1; // 返回值为1，说明条件满足
	}
	return 0; // 返回值为0，说明条件未满足
}

// 游戏结束判断
bool Model::over() {
	if (this->condition1() == 1 || this->condition2() == 1) {
		return true; // 返回值为true，说明游戏结束
	}
	return false; // 返回值为false，说明游戏未结束
}