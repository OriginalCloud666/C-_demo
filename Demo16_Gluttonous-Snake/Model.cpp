#include "Model.h"
using namespace std;

Model::Model() {
	this->beginGame = creatButton(80, 220, 150, 50, "开始游戏", 18, 12, WHITE, RGB(150, 150, 150));
	this->endGame = creatButton(395, 220, 150, 50, "结束游戏", 18, 12, WHITE, RGB(150, 150, 150));
}

// 获取food
struct Food& Model::getFood() {
	return this->food;
}

// 获取snake
struct Snake& Model::getSnake() {
	return this->snake;
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
void Model::load(IMAGE& bk) {
	loadimage(&bk, "./resource/picture/bk.jpg", width * 1.094, height * 1.728);
}

// 设置背景音乐
void Model::bgm() {
	mciSendString("open ./resource/music/bk.mp3 alias BGM", nullptr, 0, nullptr);
	mciSendString("play BGM repeat", nullptr, 0, nullptr);
}

// 创建按钮
struct Button* Model::creatButton(const int x, const int y, const int width, const int height,
	const char* text, const int dx, const int dy, COLORREF buttoncolor, COLORREF textcolor) {
	struct Button* pButton = (struct Button*)malloc(sizeof(struct Button));
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
	setlinecolor(RGB(240, 240, 240));
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

	// 对蛇的数据进行初始化
	this->snake.size = 5; // 蛇最初只有五节
	this->snake.dir = RIGHT; // 让蛇头起始方向向右
	struct Point newpoint = { 0, 0 };
	this->snake.point.push_back(newpoint);
	// 对蛇身数据进行初始化
	for (int i = 5; i < snake_size; i++) {
		struct Point newpoint = { -100, -100 };
		this->snake.point.push_back(newpoint);
	}
}

// 绘制开始界面
void Model::welcome(IMAGE& bk) {
	cleardevice();
	putimage(-30, -235, &bk);
	setbkmode(TRANSPARENT);
	SetWindowText(GetHWnd(), "C++贪吃蛇");

	this->drawButton(beginGame);
	this->drawButton(endGame);

	// 输出作者信息
	settextcolor(RGB(175, 175, 175));
	settextstyle(25, 0, "楷体", 0, 0, 1000, 0, 0, 0);
	outtextxy(12, 17, "贪吃蛇游戏");

	settextcolor(RGB(175, 175, 175));
	settextstyle(23, 0, "楷体", 0, 0, 1000, 0, 0, 0);
	outtextxy(16, 54, "作者：初云");
}

// 绘制游戏界面
void Model::draw(IMAGE& bk) {
	cleardevice();
	putimage(-30, -235, &bk);

	// 输出作者信息
	settextcolor(RGB(175, 175, 175));
	settextstyle(25, 0, "楷体", 0, 0, 1000, 0, 0, 0);
	outtextxy(12, 17, "贪吃蛇游戏");

	settextcolor(RGB(175, 175, 175));
	settextstyle(23, 0, "楷体", 0, 0, 1000, 0, 0, 0);
	outtextxy(16, 54, "作者：初云");

	// 绘制整个地图
	for (int i = 0; i < grid_row; i++) {
		for (int j = 0; j < grid_col; j++) {
			// 设置贴图位置坐标
			int x = grid_width * j;
			int y = grid_height * i;
			// 对地图格子进行绘制
			setlinecolor(RGB(240, 240, 240));
			setlinestyle(PS_SOLID, 3);
			setfillcolor(WHITE);
			fillrectangle(x, y, x + grid_width, y + grid_height);
		}
	}

	// 绘制整个蛇
	for (int i = 1; i < this->snake.size; i++) {
		setlinecolor(RGB(240, 240, 240));
		setlinestyle(PS_SOLID, 3);
		setfillcolor(RGB(252, 208, 199));
		fillrectangle(this->snake.point[i].x, this->snake.point[i].y, this->snake.point[i].x + grid_width, this->snake.point[i].y + grid_height);
	}
	// 绘制蛇的头部
	setlinecolor(RGB(240, 240, 240));
	setlinestyle(PS_SOLID, 3);
	setfillcolor(RGB(244, 165, 215));
	fillrectangle(this->snake.point[0].x, this->snake.point[0].y, this->snake.point[0].x + grid_width, this->snake.point[0].y + grid_height);

	// 绘制食物
	if (this->food.flag == 1) {
		setlinecolor(RGB(240, 240, 240));
		setlinestyle(PS_SOLID, 3);
		setfillcolor(RGB(91, 173, 255));
		fillrectangle(this->food.point.x, this->food.point.y, this->food.point.x + grid_width, this->food.point.y + grid_height);
	}
}

// 蛇的移动
void Model::snackMove() {
	// 蛇身的移动处理
	for (int i = this->snake.size - 1; i > 0; i--) {
		this->snake.point[i].x = this->snake.point[i - 1].x;
		this->snake.point[i].y = this->snake.point[i - 1].y;
	}
	switch (this->snake.dir) {
	case UP:
		snake.point[0].y -= grid_width;
		break;
	case DOWN:
		snake.point[0].y += grid_height;
		break;
	case LEFT:
		snake.point[0].x -= grid_width;
		break;
	case RIGHT:
		snake.point[0].x += grid_width;
		break;
	}
}

// 食物被生成
void Model::creatFood() {
	// 让食物与蛇头对齐
	this->food.point.x = rand() % (width / grid_width) * grid_width;
	this->food.point.y = rand() % (height / grid_height) * grid_height;
	// 食物不能出现在蛇身上
	while (1) {
		int flag = 0; // 状态标识，用于判断食物是否出现在蛇身上
		for (int i = 0; i < this->snake.size; i++) {
			if (this->snake.point[i].x == this->food.point.x && this->snake.point[i].y == this->food.point.y) {
				this->food.point.x = rand() % (width / grid_width) * grid_width;
				this->food.point.y = rand() % (height / grid_height) * grid_height;
				flag = 1;
			}
		}
		if (flag == 0) { // 如果食物没有出现在蛇身上任何一节，则跳出循环
			this->food.flag = 1; // 食物存在
			break;
		}
	}
}

// 食物被吃掉
void Model::eatFood() {
	if (this->snake.point[0].x == this->food.point.x && this->snake.point[0].y == this->food.point.y) {
		mciSendString("close eat", nullptr, 0, nullptr);
		mciSendString("open ./resource/music/eat.wav alias eat", nullptr, 0, nullptr);
		mciSendString("play eat", nullptr, 0, nullptr);
		this->snake.size++;
		this->food.flag = 0;//食物被吃掉后消失
	}
}

// 按键响应（同步）
void Model::keyDown() {
	// 判断是否有按键消息
	while (_kbhit()) {
		// 用于接收用户按键
		char userKey = _getch();
		fflush(stdin); // 清空键盘消息缓冲区

		switch (userKey) {
		case 'W':
		case 'w':
		case 72:
			if (this->snake.dir != DOWN) {
				this->snake.dir = UP;
			}
			break;
		case 'S':
		case 's':
		case 80:
			if (this->snake.dir != UP) {
				this->snake.dir = DOWN;
			}
			break;
		case 'A':
		case 'a':
		case 75:
			if (this->snake.dir != RIGHT) {
				this->snake.dir = LEFT;
			}
			break;
		case 'D':
		case 'd':
		case 77:
			if (this->snake.dir != LEFT) {
				this->snake.dir = RIGHT;
			}
			break;
		}
	}
}

// 判断鼠标是否在按钮中
int Model::mouseInButton(struct Button* pButton, MOUSEMSG* msg) {
	if (msg->x >= pButton->x && msg->x <= pButton->x + pButton->width &&
		msg->y >= pButton->y && msg->y <= pButton->y + pButton->height) {
		pButton->buttoncolor = RGB(220, 220, 220);
		pButton->textcolor = RGB(120, 120, 120);
		return 1; // 返回值为1，表明鼠标在按钮中
	}
	pButton->buttoncolor = WHITE;
	pButton->textcolor = RGB(150, 150, 150);
	return 0; // 返回值为0，表明鼠标不在按钮中
}

// 鼠标控制
int Model::mouseControl(struct Button* pButton1, struct Button* pButton2) {
	// 判断是否有鼠标消息
	if (MouseHit()) {
		MOUSEMSG msg = GetMouseMsg();
		// 判断用户是否点击按钮
		if (this->mouseInButton(pButton1, &msg) == 1 && msg.uMsg == WM_LBUTTONDOWN) {
			mciSendString("close end", nullptr, 0, nullptr);
			mciSendString("open ./resource/music/click.wav alias click", nullptr, 0, nullptr);
			mciSendString("play click", nullptr, 0, nullptr);
			return 1;
		}
		else if (this->mouseInButton(pButton2, &msg) == 1 && msg.uMsg == WM_LBUTTONDOWN) {
			mciSendString("close end", nullptr, 0, nullptr);
			mciSendString("open ./resource/music/click.wav alias click", nullptr, 0, nullptr);
			mciSendString("play click", nullptr, 0, nullptr);
			exit(0); // 游戏结束
		}
	}
	return 0;
}

// 游戏结束条件1
int Model::condition1() {
	// 当蛇撞到自己时游戏结束
	for (int i = 1; i < this->snake.size; i++) {
		if (this->snake.point[i].x == this->snake.point[0].x && this->snake.point[i].y == this->snake.point[0].y) {
			return 1; // 返回值为1时游戏结束
		}
	}
	return 0; // 返回值为0时游戏继续
}

// 游戏结束条件2
int Model::condition2() {
	// 当蛇撞到墙壁时游戏结束
	if (this->snake.point[0].x < 0) { // 撞到左边
		return 1; // 返回值为1时游戏结束
	}
	else if (this->snake.point[0].x > width - grid_width) {
		return 1; // 返回值为1时游戏结束
	}
	else if (this->snake.point[0].y < 0) {
		return 1; // 返回值为1时游戏结束
	}
	else if (this->snake.point[0].y > height - grid_height) {
		return 1; // 返回值为1时游戏结束
	}
	return 0; // 返回值为0时游戏继续
}

// 游戏结束条件判断
bool Model::over() {
	// 如果两个条件中有一个成立则游戏结束
	if (this->condition1() == 1 || this->condition2() == 1) {
		return true; // 返回值为1时游戏结束
	}
	return false; // 返回值为0时游戏继续
}