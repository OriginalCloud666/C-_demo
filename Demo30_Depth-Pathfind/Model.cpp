#include "Model.h"
using namespace std;

Model::Model() :top(0), left(0), down(0), right(0) {
	this->gameMapVec = {
		{ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
		{ 1, 3, 1, 0, 1, 1, 1, 1, 1, 4, 1, 1 },
		{ 1, 0, 1, 0, 1, 0, 0, 0, 1, 0, 1, 1 },
		{ 1, 0, 1, 0, 0, 0, 1, 1, 1, 0, 1, 1 },
		{ 1, 0, 1, 0, 1, 0, 1, 1, 1, 0, 1, 1 },
		{ 1, 0, 0, 0, 1, 0, 1, 1, 1, 0, 1, 1 },
		{ 1, 0, 1, 1, 1, 0, 1, 1, 1, 0, 1, 1 },
		{ 1, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 1 },
		{ 1, 0, 1, 1, 1, 0, 1, 1, 0, 1, 1, 1 },
		{ 1, 0, 1, 0, 0, 0, 1, 1, 0, 1, 1, 1 },
		{ 1, 0, 1, 1, 1, 0, 1, 1, 0, 1, 1, 1 },
		{ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
	};
	this->beginPoint = { 1, 1 };
	this->endPoint = { 9, 1 };
	this->beginGame = creatButton(80, 500, 150, 50, "START", 38, 12, RGB(252, 252, 252), RGB(189, 134, 130));
	this->endGame = creatButton(420, 500, 150, 50, "END", 52, 12, RGB(252, 252, 252), RGB(189, 134, 130));
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
void Model::load(IMAGE& w1, IMAGE& ren, IMAGE& road, IMAGE& wall) {
	// 加载图片
	loadimage(&w1, "./resource/picture/w1.png");
	loadimage(&ren, "./resource/picture/ren.bmp", grid_width, grid_height);
	loadimage(&road, "./resource/picture/road.bmp", grid_width, grid_height);
	loadimage(&wall, "./resource/picture/wall.bmp", grid_width, grid_height);
}

// 加载音乐
void Model::bgm() {
	mciSendString("open ./resource/music/bk.mp3 alias BGM", nullptr, 0, nullptr);
	mciSendString("play BGM repeat", nullptr, 0, nullptr);
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
	setlinecolor(RGB(210, 118, 119));
	setlinestyle(PS_SOLID, 5);
	setfillcolor(pButton->buttoncolor);
	fillrectangle(pButton->x, pButton->y, pButton->x + pButton->width, pButton->y + pButton->height);
	settextcolor(pButton->textcolor);
	settextstyle(28, 0, "楷体", 0, 0, 1000, 0, 0, 0);
	outtextxy(pButton->x + pButton->dx, pButton->y + pButton->dy, pButton->text);
}

// 游戏数据初始化
void Model::init() {
	// 设置背景颜色
	setbkcolor(RGB(245, 215, 215));

	// 游戏地图数据初始化
	this->left = width / 2 - (col * grid_width) / 2;
	this->top = height / 2 - (row * grid_height) / 2;
	this->right = this->left + col * grid_width;
	this->down = this->top + row * grid_height;

	// 游戏各点试探情况初始化
	this->gameNodeVec.clear();
	for (int i = 0; i < row; i++) {
		vector<PathNode> lineGrid;
		for (int j = 0; j < col; j++) {
			struct PathNode newPoint = { 0, 0 };
			lineGrid.push_back(newPoint);
		}
		this->gameNodeVec.push_back(lineGrid);
	}

	// 起点标记为走过
	this->gameNodeVec[this->beginPoint.y][this->beginPoint.x].isFind = true;
	// 初始情况下，起点入栈
	this->mystack.push(this->beginPoint);
}

// 绘制开始界面
void Model::welcome(IMAGE& w1) {
	SetWindowText(GetHWnd(), "C++可视化深度寻路");
	cleardevice();
	putimage(-40, 140, &w1);
	this->drawButton(this->beginGame);
	this->drawButton(this->endGame);
}

// 绘制游戏界面
void Model::draw(IMAGE& road, IMAGE& wall, IMAGE& ren) {
	cleardevice();

	// 绘制游戏地图
	for (int i = 0; i < row; i++) {
		for (int j = 0; j < col; j++) {
			// 设置贴图位置坐标
			int x = this->left + grid_width * j;
			int y = this->top + grid_height * i;
			// 对图片信息进行分发
			switch (this->gameMapVec[i][j]) {
			case 0://空地
				putimage(x, y, &road);
				break;
			case 1: // 砖块
				putimage(x, y, &wall);
				break;
			case 2: // 路径
				setfillcolor(RGB(107, 146, 189));
				putimage(x, y, &road);
				solidcircle(x + grid_width / 2, y + grid_height / 2, 6);
				break;
			case 3: // 角色
				putimage(x, y, &ren);
				break;
			case 4: // 终点
				setfillcolor(RGB(210, 118, 119));
				putimage(x, y, &road);
				solidcircle(x + grid_width / 2, y + grid_height / 2, 10);
				break;
			default:
				break;
			}
		}
	}

	// 绘制外部边框
	for (int x = 25; x >= 15; x--) {
		setlinecolor(RGB(210, 118, 119));
		rectangle(this->left - x, this->top - x, this->right + x, this->down + x);
	}
}

// 深度寻路算法
void Model::depthFinding(IMAGE& road, IMAGE& wall, IMAGE& ren) {
	Point currentPoint = beginPoint; // 当前点
	Point searchPoint; // 试探点

	while (1) {
		searchPoint = currentPoint;
		// 查看当前点的当前试探方向
		switch (this->gameNodeVec[currentPoint.y][searchPoint.x].dir) {
		case UP:
			// 当前试探点位当前点上方点
			searchPoint.y--;
			// 改变当前点试探方向
			this->gameNodeVec[currentPoint.y][currentPoint.x].dir++;

			// 判断当前点是否走过，是否为空地而不是墙壁
			if (this->gameNodeVec[searchPoint.y][searchPoint.x].isFind == 0 &&
				this->gameMapVec[searchPoint.y][searchPoint.x] != 1) {
				// 能走则标记点走过，当前试探点入栈，当前点移动到试探点位置
				this->gameNodeVec[searchPoint.y][searchPoint.x].isFind = 1;
				this->mystack.push(searchPoint);
				this->gameMapVec[searchPoint.y][searchPoint.x] = 3;
				this->gameMapVec[currentPoint.y][currentPoint.x] = 2;
				currentPoint = searchPoint;
				Sleep(1000);
				this->draw(road, wall, ren);
				FlushBatchDraw();
			}
			break;
		case RIGHT:
			// 当前试探点位当前点右方点
			searchPoint.x++;
			// 改变当前点试探方向
			this->gameNodeVec[currentPoint.y][currentPoint.x].dir++;

			// 判断当前点是否走过，是否为路径而不是墙壁
			if (this->gameNodeVec[searchPoint.y][searchPoint.x].isFind == 0 &&
				this->gameMapVec[searchPoint.y][searchPoint.x] != 1) {
				// 能走则标记点走过，当前试探点入栈，当前点移动到试探点位置
				this->gameNodeVec[searchPoint.y][searchPoint.x].isFind = 1;
				this->mystack.push(searchPoint);
				this->gameMapVec[searchPoint.y][searchPoint.x] = 3;
				this->gameMapVec[currentPoint.y][currentPoint.x] = 2;
				currentPoint = searchPoint;
				Sleep(1000);
				this->draw(road, wall, ren);
				FlushBatchDraw();
			}
			break;
		case DOWN:
			// 当前试探点位当前点下方点
			searchPoint.y++;
			// 改变当前点试探方向
			this->gameNodeVec[currentPoint.y][currentPoint.x].dir++;

			// 判断当前点是否走过，是否为路径而不是墙壁
			if (this->gameNodeVec[searchPoint.y][searchPoint.x].isFind == 0 &&
				this->gameMapVec[searchPoint.y][searchPoint.x] != 1) {
				// 能走则标记点走过，当前试探点入栈，当前点移动到试探点位置
				this->gameNodeVec[searchPoint.y][searchPoint.x].isFind = 1;
				this->mystack.push(searchPoint);
				this->gameMapVec[searchPoint.y][searchPoint.x] = 3;
				this->gameMapVec[currentPoint.y][currentPoint.x] = 2;
				currentPoint = searchPoint;
				Sleep(1000);
				this->draw(road, wall, ren);
				FlushBatchDraw();
			}
			break;
		case LEFT:
			// 当前试探点位当前点左方点
			searchPoint.x--;

			// 判断当前点是否走过，是否为路径而不是墙壁
			if (this->gameNodeVec[searchPoint.y][searchPoint.x].isFind == 0 &&
				this->gameMapVec[searchPoint.y][searchPoint.x] != 1) {
				// 能走则标记点走过，当前试探点入栈，当前点移动到试探点位置
				this->gameNodeVec[searchPoint.y][searchPoint.x].isFind = 1;
				this->mystack.push(searchPoint);
				this->gameMapVec[searchPoint.y][searchPoint.x] = 3;
				this->gameMapVec[currentPoint.y][currentPoint.x] = 2;
				currentPoint = searchPoint;
				Sleep(1000);
				this->draw(road, wall, ren);
				FlushBatchDraw();
			}
			else { // 顺时针试探到最后便是左方向，此时如若不能走则回退
				// 清空栈顶元素
				this->mystack.pop();
				this->gameMapVec[currentPoint.y][currentPoint.x] = 2;
				// 将如今新的栈顶元素赋给currentPoint
				currentPoint = mystack.top();
				this->gameMapVec[currentPoint.y][currentPoint.x] = 3;
				Sleep(1000);
				this->draw(road, wall, ren);
				FlushBatchDraw();
			}
			break;
		}

		// 如果寻路到了终点，则跳出循环
		if (currentPoint.y == endPoint.y &&
			currentPoint.x == endPoint.x) {
			mciSendString("close win", nullptr, 0, nullptr);
			mciSendString("open ./resource/music/win.wav alias win", NULL, 0, NULL);
			mciSendString("play win", nullptr, 0, nullptr);
			Sleep(1000);
			this->draw(road, wall, ren);
			FlushBatchDraw();
			break;
		}

		// 如果栈空了，则跳出循环
		if (mystack.empty()) {
			break;
		}
	}
}

// 判断鼠标是否在按钮中
int Model::mouseInButton(struct Button* pButton, MOUSEMSG* msg) {
	if (msg->x >= pButton->x && msg->x <= pButton->x + pButton->width &&
		msg->y >= pButton->y && msg->y <= pButton->y + pButton->height) {
		pButton->buttoncolor = RGB(235, 235, 235);
		pButton->textcolor = RGB(189, 134, 130);
		return 1; // 返回值为1，表明鼠标在按钮中
	}
	pButton->buttoncolor = RGB(252, 252, 252);
	pButton->textcolor = RGB(189, 134, 130);
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