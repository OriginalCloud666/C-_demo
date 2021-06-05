#include "Model.h"
using namespace std;

Model::Model() :top(0), left(0), down(0), right(0) {
	this->gameMapVec = {
		{ 0, 1, 0, 1, 0, 1, 0, 1 },
		{ 1, 0, 1, 0, 1, 0, 1, 0 },
		{ 0, 2, 0, 2, 0, 2, 0, 2 },
		{ 2, 0, 2, 0, 2, 0, 2, 0 },
		{ 0, 3, 0, 3, 0, 3, 0, 3 },
		{ 3, 0, 3, 0, 3, 0, 3, 0 },
		{ 0, 4, 0, 4, 0, 4, 0, 4 },
		{ 4, 0, 4, 0, 4, 0, 4, 0 },
	};
}

// 获取ball
struct Ball& Model::getBall() {
	return this->ball;
}

// 加载资源
void Model::load(IMAGE& bk, const int width, const int height) {
	loadimage(&bk, "./resource/picture/bk.jpg", width, height * 1.58);
}

// 设置背景音乐
void Model::bgm() {
	mciSendString("open ./resource/music/bk.mp3 alias BGM", nullptr, 0, nullptr);
	mciSendString("play BGM repeat", nullptr, 0, nullptr);
}

//游戏数据初始化
void Model::init(const int brick_width, const int brick_height, const int brick_row, const int brick_col, const int width, const int height) {
	// 砖块数据初始化
	this->left = width / 2 - (brick_width * brick_col) / 2;
	this->top = height / 2 - (brick_height * (brick_row + 16)) / 2;
	this->right = this->left + brick_width * brick_col;
	this->down = this->top + brick_height * brick_row;

	// 木板数据初始化
	this->board.width = 200;
	this->board.height = 30;
	this->board.step = 3.5;
	this->board.x = (double)width / 2 - this->board.width / 2;
	this->board.y = (double)height - this->board.height;

	// 小球数据初始化
	this->ball.r = 15;
	this->ball.x = this->board.x + this->board.width / 2;
	this->ball.y = this->board.y - this->ball.r;
	this->ball.dx = 2.2;
	this->ball.dy = 2.2;
	this->ball.step = sqrt(this->ball.dx * this->ball.dx + this->ball.dy * this->ball.dy);
	this->ball.radian = atan2(this->ball.dy, -this->ball.dx);
}

// 绘制开始界面
void Model::welcome(IMAGE& bk) {
	putimage(0, -290, &bk);
	setbkcolor(RGB(200, 191, 231));
	setbkmode(TRANSPARENT);
	SetWindowText(GetHWnd(), "C++打砖块游戏");

	// 输出作者信息
	settextcolor(RGB(215, 215, 215));
	settextstyle(28, 0, "楷体", 0, 0, 1000, 0, 0, 0);
	outtextxy(13, 16, "打砖块游戏");

	settextcolor(RGB(215, 215, 215));
	settextstyle(27, 0, "楷体", 0, 0, 1000, 0, 0, 0);
	outtextxy(15, 55, "作者：初云");

	while (!_kbhit()) {
	}
}

// 绘制游戏界面
void Model::draw(const int brick_width, const int brick_height, const int brick_row, const int brick_col, const int height) {
	cleardevice();

	// 绘制灰白色分界线
	setfillcolor(RGB(237, 234, 247));
	solidrectangle(this->left, this->top, this->right, this->down / 2);

	// 绘制灰淡淡淡紫色分界线
	setfillcolor(RGB(232, 227, 244));
	solidrectangle(this->left, this->down / 2, this->right, this->down);

	// 绘制淡淡紫色分界
	setfillcolor(RGB(225, 220, 241));
	solidrectangle(this->left, this->down, this->right, this->down + (height - brick_row * brick_height) / 3 - 10);

	// 绘制淡紫色分界
	setfillcolor(RGB(214, 207, 237));
	solidrectangle(this->left, this->down + (height - brick_row * brick_height) / 3 - 10, this->right, this->down + (height - brick_row * brick_height) * 2 / 3 - 20);

	// 绘制砖块
	for (int i = 0; i < brick_row; i++) {
		for (int j = 0; j < brick_col; j++) {
			// 将数组下标转化为坐标
			int x = this->left + brick_width * j;
			int y = this->top + brick_height * i;
			// 对数组信息进行分发
			switch (this->gameMapVec[i][j]) {
			case -1:
				break;
			case 0:
				setlinecolor(RGB(255, 224, 90));
				setlinestyle(PS_SOLID, 2);
				setfillcolor(RGB(255, 204, 0));
				fillrectangle(x, y, x + brick_width, y + brick_height);
				break;
			case 1:
				setlinecolor(RGB(255, 183, 218));
				setlinestyle(PS_SOLID, 2);
				setfillcolor(RGB(255, 153, 204));
				fillrectangle(x, y, x + brick_width, y + brick_height);
				break;
			case 2:
				setlinecolor(RGB(120, 215, 255));
				setlinestyle(PS_SOLID, 2);
				setfillcolor(RGB(30, 167, 225));
				fillrectangle(x, y, x + brick_width, y + brick_height);
				break;
			case 3:
				setlinecolor(RGB(190, 229, 127));
				setlinestyle(PS_SOLID, 2);
				setfillcolor(RGB(128, 190, 31));
				fillrectangle(x, y, x + brick_width, y + brick_height);
				break;
			case 4:
				setlinecolor(RGB(255, 160, 98));
				setlinestyle(PS_SOLID, 2);
				setfillcolor(RGB(232, 106, 23));
				fillrectangle(x, y, x + brick_width, y + brick_height);
				break;
			}
		}
	}

	// 绘制木板
	setlinecolor(RGB(100, 50, 0));
	setlinestyle(PS_SOLID, 2);
	setfillcolor(RGB(115, 57, 0));
	fillrectangle(this->board.x, this->board.y, this->board.x + this->board.width, this->board.y + this->board.height);

	// 绘制小球
	setlinecolor(RGB(220, 220, 220));
	setlinestyle(PS_SOLID, 2);
	setfillcolor(RGB(110, 110, 110));
	solidcircle(this->ball.x, this->ball.y, this->ball.r);
}

// 求两点间距离
double Model::distance(const double x1, const double y1, const double x2, const double y2) {
	// distance=根号下((x2-x1)^2+(y2-y1)^2)
	return sqrt((x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2));
}

// 木板移动（异步按键控制）
void Model::keyDown1(const int width) {
	if (GetAsyncKeyState(VK_LEFT) || GetAsyncKeyState('A')) { // 向左移动
		this->board.x -= this->board.step;
		if (this->board.x <= 0) { // 设置左移界限
			this->board.x = 0;
		}
	}
	if (GetAsyncKeyState(VK_RIGHT) || GetAsyncKeyState('D')) { // 向右移动
		this->board.x += this->board.step;
		if (this->board.x >= (double)width - this->board.width) { // 设置左移界限
			this->board.x = (double)width - this->board.width;
		}
	}
}

// 小球发射控制（同步按键控制）
void Model::keyDown2() {
	while (_kbhit()) {
		// 用于接收用户按键
		char userKey = _getch();
		fflush(stdin); // 清空键盘缓冲区

		switch (userKey) {
		case' ': // 空格控制发射开关
			this->ball.flag = 1;
			mciSendString("close click", nullptr, 0, nullptr);
			mciSendString("open ./resource/music/click.wav alias click", nullptr, 0, nullptr);
			mciSendString("play click", nullptr, 0, nullptr);
			break;
		}
	}
}

// 小球的移动相撞
void Model::ballMove(const int width, const int height, const double pi) {
	// 当小球碰到左右壁时
	if (((this->ball.x <= this->ball.r) || (this->ball.x >= width - this->ball.r)) && (this->ball.y <= (double)height - this->board.height)) {
		this->ball.dx = -this->ball.dx; // 将球反射
		if (this->ball.x <= this->ball.r) {
			this->ball.x = this->ball.r;
		}
		if (this->ball.x >= width - this->ball.r) {
			this->ball.x = width - this->ball.r;
		}
	}
	// 当小球碰到上壁时
	if (this->ball.y <= this->ball.r) {
		this->ball.dy = -this->ball.dy; // 将球反射
		if (this->ball.y <= this->ball.r) {
			this->ball.y = this->ball.r;
		}
	}

	// 当小球碰到木板左右边时
	if (this->ball.y > this->board.y + 2 && this->ball.y < this->board.y + this->board.height - 2 &&
		this->ball.x > this->board.x - this->ball.r + 2 && this->ball.x < this->board.x + this->board.width + this->ball.r - 2) {
		this->ball.dx = -this->ball.dx; // 将球反射
		if (fabs(this->board.x - this->ball.x) < this->ball.r && this->board.x - this->ball.x > 0) {
			this->ball.x = this->board.x - this->ball.r;
		}
		if (fabs(this->ball.x - (this->board.x + this->board.width)) < this->ball.r && this->ball.x - (this->board.x + this->board.width) > 0) {
			this->ball.x = this->board.x + this->board.width + this->ball.r;
		}
	}
	// 当小球碰到木板上下边时
	if (this->ball.x > this->board.x + 2 && this->ball.x < this->board.x + this->board.width - 2 &&
		this->ball.y > this->board.y - this->ball.r + 2 && this->ball.y < this->board.y + this->board.height + this->ball.r - 2) {
		this->ball.dy = -this->ball.dy; // 将球反射
		if (fabs(this->board.y - this->ball.y) < this->ball.r && this->board.y - this->ball.y > 0) {
			this->ball.y = this->board.y - this->ball.r;
		}
		if (fabs(this->ball.y - (this->board.y + this->board.height)) < this->ball.r && this->ball.y - (this->board.y + this->board.height) > 0) {
			this->ball.y = this->board.y + this->board.height + this->ball.r;
		}
	}

	// 当小球碰到木板四个角时
	if (this->distance(this->ball.x, this->ball.y, this->board.x, this->board.y) <= this->ball.r - 1) { // 左上角
		this->ball.radian = atan2(this->ball.y - this->board.y, this->ball.x - this->board.x);
		// 对撞击角度进行限制
		if (this->ball.radian <= -pi * 7 / 8) {
			this->ball.radian = -pi * 7 / 8;
		}
		else if (this->ball.radian >= -pi * 5 / 8) {
			this->ball.radian = -pi * 5 / 8;
		}
		this->ball.dx = cos(this->ball.radian) * this->ball.step;
		this->ball.dy = sin(this->ball.radian) * this->ball.step;
		this->ball.dx = -this->ball.dx; // 将球反射
		this->ball.dy = -this->ball.dy; // 将球反射
		this->ball.x = this->board.x + cos(this->ball.radian) * this->ball.r;
		this->ball.y = this->board.y + sin(this->ball.radian) * this->ball.r;
	}
	else if (this->distance(this->ball.x, this->ball.y, this->board.x + this->board.width, this->board.y) <= this->ball.r - 1) { // 右上角
		this->ball.radian = atan2(this->ball.y - this->board.y, this->ball.x - (this->board.x + this->board.width));
		// 对撞击角度进行限制
		if (this->ball.radian <= -pi * 3 / 8) {
			this->ball.radian = -pi * 3 / 8;
		}
		else if (this->ball.radian >= -pi * 1 / 8) {
			this->ball.radian = -pi * 1 / 8;
		}
		this->ball.dx = cos(this->ball.radian) * this->ball.step;
		this->ball.dy = sin(this->ball.radian) * this->ball.step;
		this->ball.dx = -this->ball.dx; // 将球反射
		this->ball.dy = -this->ball.dy; // 将球反射
		this->ball.x = this->board.x + this->board.width + cos(this->ball.radian) * this->ball.r;
		this->ball.y = this->board.y + sin(this->ball.radian) * this->ball.r;
	}
	else if (this->distance(this->ball.x, this->ball.y, this->board.x, this->board.y + this->board.height) <= this->ball.r - 1) { // 左下角
		this->ball.radian = atan2(this->ball.y - (this->board.y + this->board.height), this->ball.x - this->board.x);
		// 对撞击角度进行限制
		if (this->ball.radian <= pi * 5 / 8) {
			this->ball.radian = pi * 5 / 8;
		}
		else if (this->ball.radian >= pi * 7 / 8) {
			this->ball.radian = pi * 7 / 8;
		}
		this->ball.dx = cos(this->ball.radian) * this->ball.step;
		this->ball.dy = sin(this->ball.radian) * this->ball.step;
		this->ball.dx = -this->ball.dx; // 将球反射
		this->ball.dy = -this->ball.dy; // 将球反射
		this->ball.x = this->board.x + cos(this->ball.radian) * this->ball.r;
		this->ball.y = this->board.y + this->board.height + sin(this->ball.radian) * this->ball.r;
	}
	else if (this->distance(this->ball.x, this->ball.y, this->board.x + this->board.width, this->board.y + this->board.height) <= this->ball.r - 1) { // 右下角
		this->ball.radian = atan2(this->ball.y - (this->board.y + this->board.height), this->ball.x - (this->board.x + this->board.width));
		// 对撞击角度进行限制
		if (this->ball.radian <= pi * 1 / 8) {
			this->ball.radian = pi * 1 / 8;
		}
		else if (this->ball.radian >= pi * 3 / 8) {
			this->ball.radian = pi * 3 / 8;
		}
		this->ball.dx = cos(this->ball.radian) * this->ball.step;
		this->ball.dy = sin(this->ball.radian) * this->ball.step;
		this->ball.dx = -this->ball.dx; // 将球反射
		this->ball.dy = -this->ball.dy; // 将球反射
		this->ball.x = this->board.x + this->board.width + cos(this->ball.radian) * this->ball.r;
		this->ball.y = this->board.y + this->board.height + sin(this->ball.radian) * this->ball.r;
	}

	if (this->ball.flag == 1) { // 小球与木板脱离后
		if (this->ball.y >= height + this->ball.r || this->ball.x <= -this->ball.r || this->ball.x >= width + this->ball.r) {
			// 小球飞出界面后重新回到木板上
			this->ball.flag = 0;
		}
		this->ball.x -= this->ball.dx;
		this->ball.y -= this->ball.dy;
	}
	if (ball.flag == 0) { // 小球未脱离木板时
		this->ball.x = this->board.x + this->board.width / 2;
		this->ball.y = this->board.y - this->ball.r - 2;
		this->ball.dx = 2.2;
		this->ball.dy = 2.2;
		this->ball.step = sqrt(this->ball.dx * this->ball.dx + this->ball.dy * this->ball.dy);
		this->ball.radian = atan2(this->ball.dy, -this->ball.dx);
	}
}

// 小球与砖块的移动碰撞
void Model::ballCrash(const int brick_row, const int brick_col, const int brick_width, const int brick_height, const double pi) {
	for (int i = 0; i < brick_row; i++) {
		for (int j = 0; j < brick_col; j++) {
			// 将数组下标转化为坐标
			int x = this->left + brick_width * j;
			int y = this->top + brick_height * i;
			// 当砖块不存在时跳过本次循环
			if (this->gameMapVec[i][j] == -1) {
				continue;
			}
			// 当小球碰到砖块上下边时
			if (this->ball.x > x + 2.0 && this->ball.x < x + (double)brick_width - 2 &&
				this->ball.y > y - this->ball.r + 2 && this->ball.y < y + (double)brick_height + this->ball.r - 2) {
				this->ball.dy = -this->ball.dy; // 将球反射
				if (fabs(y - this->ball.y) < this->ball.r && y - this->ball.y > 0) {
					this->ball.y = y - this->ball.r;
				}
				if (fabs(this->ball.y - (y + (double)brick_height)) < this->ball.r && this->ball.y - (y + (double)brick_height) > 0) {
					this->ball.y = y + (double)brick_height + this->ball.r;
				}
				this->gameMapVec[i][j] = -1;
				mciSendString("close crash", nullptr, 0, nullptr);
				mciSendString("open ./resource/music/crash.wav alias crash", nullptr, 0, nullptr);
				mciSendString("play crash", nullptr, 0, nullptr);
			}
			// 当小球碰到砖块左右边时
			if (this->ball.y > y + 2.0 && this->ball.y < y + (double)brick_height - 2 &&
				this->ball.x > x - this->ball.r + 2 && this->ball.x < x + (double)brick_width + this->ball.r - 2) {
				this->ball.dx = -ball.dx;//将球反射
				if (fabs(x - this->ball.x) < this->ball.r && x - this->ball.x > 0) {
					this->ball.x = x - this->ball.r;
				}
				if (fabs(this->ball.x - (x + (double)brick_width)) < this->ball.r && this->ball.x - (x + (double)brick_width) > 0) {
					this->ball.x = x + (double)brick_width + this->ball.r;
				}
				this->gameMapVec[i][j] = -1;
				mciSendString("close crash", nullptr, 0, nullptr);
				mciSendString("open ./resource/music/crash.wav alias crash", nullptr, 0, nullptr);
				mciSendString("play crash", nullptr, 0, nullptr);
			}
			// 当小球碰到砖块四个角时，撞击的边角不同，角度的计算方式不同
			if (this->distance(this->ball.x, this->ball.y, x, y) <= this->ball.r - 1 && this->ball.x < x && this->ball.y < y &&
				(this->gameMapVec[i][j - 1] == -1 || j == 0) && (this->gameMapVec[i - 1][j] == -1 || i == 0)) { // 左上角
				this->ball.radian = atan2(this->ball.y - y, this->ball.x - x);
				// 对撞击角度进行限制
				if (this->ball.radian <= -pi * 7 / 8) {
					this->ball.radian = -pi * 7 / 8;
				}
				else if (this->ball.radian >= -pi * 5 / 8) {
					this->ball.radian = -pi * 5 / 8;
				}
				this->ball.dx = cos(this->ball.radian) * this->ball.step;
				this->ball.dy = sin(this->ball.radian) * this->ball.step;
				this->ball.dx = -this->ball.dx;//将球反射
				this->ball.dy = -this->ball.dy;//将球反射
				this->ball.x = x + cos(this->ball.radian) * this->ball.r;
				this->ball.y = y + sin(this->ball.radian) * this->ball.r;
				this->gameMapVec[i][j] = -1;
				mciSendString("close crash", nullptr, 0, nullptr);
				mciSendString("open ./resource/music/crash.wav alias crash", nullptr, 0, nullptr);
				mciSendString("play crash", nullptr, 0, nullptr);
			}
			else if (this->distance(this->ball.x, this->ball.y, x + (double)brick_width, y) <= this->ball.r - 1 &&
				this->ball.x > x + (double)brick_width && this->ball.y < y &&
				(this->gameMapVec[i][j + 1] == -1 || j == brick_col - 1) &&
				(this->gameMapVec[i - 1][j] == -1 || i == 0)) { // 右上角
				this->ball.radian = atan2(this->ball.y - y, this->ball.x - (x + (double)brick_width));
				// 对撞击角度进行限制
				if (this->ball.radian <= -pi * 3 / 8) {
					this->ball.radian = -pi * 3 / 8;
				}
				else if (this->ball.radian >= -pi * 1 / 8) {
					this->ball.radian = -pi * 1 / 8;
				}
				this->ball.dx = cos(this->ball.radian) * this->ball.step;
				this->ball.dy = sin(this->ball.radian) * this->ball.step;
				this->ball.dx = -this->ball.dx;//将球反射
				this->ball.dy = -this->ball.dy;//将球反射
				this->ball.x = x + (double)brick_width + cos(this->ball.radian) * this->ball.r;
				this->ball.y = y + sin(this->ball.radian) * this->ball.r;
				this->gameMapVec[i][j] = -1;
				mciSendString("close crash", nullptr, 0, nullptr);
				mciSendString("open ./resource/music/crash.wav alias crash", nullptr, 0, nullptr);
				mciSendString("play crash", nullptr, 0, nullptr);
			}
			else if (this->distance(this->ball.x, this->ball.y, x, y + (double)brick_height) <= this->ball.r - 1 &&
				this->ball.x < x && this->ball.y > y + (double)brick_height &&
				(this->gameMapVec[i][j - 1] == -1 || j == 0) &&
				(this->gameMapVec[i + 1][j] == -1 || i == brick_row - 1)) { // 左下角
				this->ball.radian = atan2(this->ball.y - (y + (double)brick_height), this->ball.x - x);
				// 对撞击角度进行限制
				if (this->ball.radian <= pi * 5 / 8) {
					this->ball.radian = pi * 5 / 8;
				}
				else if (this->ball.radian >= pi * 7 / 8) {
					this->ball.radian = pi * 7 / 8;
				}
				this->ball.dx = cos(this->ball.radian) * this->ball.step;
				this->ball.dy = sin(this->ball.radian) * this->ball.step;
				this->ball.dx = -this->ball.dx;//将球反射
				this->ball.dy = -this->ball.dy;//将球反射
				this->ball.x = x + cos(this->ball.radian) * this->ball.r;
				this->ball.y = y + (double)brick_height + sin(this->ball.radian) * this->ball.r;
				this->gameMapVec[i][j] = -1;
				mciSendString("close crash", nullptr, 0, nullptr);
				mciSendString("open ./resource/music/crash.wav alias crash", nullptr, 0, nullptr);
				mciSendString("play crash", nullptr, 0, nullptr);
			}
			else if (this->distance(this->ball.x, this->ball.y, x + (double)brick_width, y + (double)brick_height) <= this->ball.r - 1 &&
				this->ball.x > x + (double)brick_width && this->ball.y > y + (double)brick_height &&
				(this->gameMapVec[i][j + 1] == -1 || j == brick_col - 1) &&
				(this->gameMapVec[i + 1][j] == -1 || i == brick_row - 1)) { // 右下角
				this->ball.radian = atan2(this->ball.y - (y + (double)brick_height), this->ball.x - (x + (double)brick_width));
				// 对撞击角度进行限制
				if (this->ball.radian <= pi * 1 / 8) {
					this->ball.radian = pi * 1 / 8;
				}
				else if (this->ball.radian >= pi * 3 / 8) {
					this->ball.radian = pi * 3 / 8;
				}
				this->ball.dx = cos(this->ball.radian) * this->ball.step;
				this->ball.dy = sin(this->ball.radian) * this->ball.step;
				this->ball.dx = -this->ball.dx;//将球反射
				this->ball.dy = -this->ball.dy;//将球反射
				this->ball.x = x + (double)brick_width + cos(this->ball.radian) * this->ball.r;
				this->ball.y = y + (double)brick_height + sin(this->ball.radian) * this->ball.r;
				this->gameMapVec[i][j] = -1;
				mciSendString("close crash", nullptr, 0, nullptr);
				mciSendString("open ./resource/music/crash.wav alias crash", nullptr, 0, nullptr);
				mciSendString("play crash", nullptr, 0, nullptr);
			}
		}
	}
}

// 结束条件判断
bool Model::over(const int brick_row, const int brick_col) {
	// 判断数组中是否全部为-1
	for (int i = 0; i < brick_row; i++) {
		for (int j = 0; j < brick_col; j++) {
			if (this->gameMapVec[i][j] != -1) {
				return false; // 游戏未结束
			}
		}
	}
	return true; // 游戏结束
}