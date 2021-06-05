#include "Model.h"
using namespace std;

Model::Model() :top(0), left(0), down(0), right(0) {}

// 加载资源
void Model::load(IMAGE& bk) {
	loadimage(&bk, "./resource/picture/bk.jpg", 924, 520);
}

// 播放音乐
void Model::bgm() {
	mciSendString("open ./resource/music/bk.mp3 alias BGM", nullptr, 0, nullptr);
	mciSendString("play BGM repeat", nullptr, 0, nullptr);
}

// 初始化数据
void Model::init() {
	// 初始化游戏区域坐标
	this->left = width / 2 - (grid_num * grid_length) / 2; // 游戏区域左上角x坐标
	this->top = height / 2 - (grid_num * grid_length) / 2; // 游戏区域左上角y坐标
	this->right = this->left + (grid_num * grid_length); // 游戏区域右下角x坐标
	this->down = this->top + (grid_num * grid_length); // 游戏区域右下角y坐标

	// 初始化vector容器
	this->gameMapVec.clear();
	for (int i = 0; i < grid_num; i++) {
		vector<int> lineGrid;
		for (int j = 0; j < grid_num; j++) {
			lineGrid.push_back(1);
		}
		this->gameMapVec.push_back(lineGrid);
	}
}

// 开始界面绘制
void Model::welcome(IMAGE& bk) {
	putimage(-50, -49, &bk);
	setbkmode(TRANSPARENT);
	SetWindowText(GetHWnd(), "C++涂格子游戏");

	// 输出作者信息
	settextcolor(RGB(100, 230, 230));
	settextstyle(50, 0, "楷体", 0, 0, 600, 0, 0, 0);
	outtextxy(180, 150, "涂格子游戏");
	settextstyle(30, 0, "楷体", 0, 0, 600, 0, 0, 0);
	outtextxy(220, 280, "作者：初云");

	// 绘制整个界面外边框
	for (int x = 10; x < 20; x++) {
		setlinecolor(RGB(0, 255 - 13 * (x - 10), 255 - 13 * (x - 10)));
		rectangle(x - 10, x - 10, width + 10 - x, height + 10 - x);
	}
	settextcolor(WHITE);
	settextstyle(30, 0, "楷体", 0, 0, 600, 0, 0, 0);
	outtextxy(200, 225, "按任意键继续！");

	while (!_kbhit()) {
	}
}

// 游戏界面绘制
void Model::draw(IMAGE& bk) {
	cleardevice();
	putimage(-50, -49, &bk);

	// 绘制格子
	setlinecolor(RGB(0, 200, 200));

	// 绘制竖线
	for (int x = this->left; x <= right; x += grid_length) {
		line(x, this->top, x, this->down);
	}

	// 绘制横线
	for (int y = this->top; y <= this->down; y += grid_length) {
		line(this->left, y, this->right, y);
	}

	// 绘制外边框
	for (int x = 20; x > 10; x--) {
		setlinecolor(RGB(0, 255 - 13 * (x - 10), 255 - 13 * (x - 10)));
		rectangle(this->left - x, this->top - x, this->right + x, this->down + x);
	}

	// 绘制整个界面外边框
	for (int x = 10; x < 20; x++) {
		setlinecolor(RGB(0, 255 - 13 * (x - 10), 255 - 13 * (x - 10)));
		rectangle(x - 10, x - 10, width + 10 - x, height + 10 - x);
	}

	// 根据数组数据绘制格子颜色，判断数组里的值是1还是-1
	for (int i = 0; i < grid_num; i++) {
		for (int k = 0; k < grid_num; k++) {
			int x = k * grid_length + left; // 每个格子左上角的x坐标
			int y = i * grid_length + top; // 每个格子左上角的y坐标
			if (this->gameMapVec[i][k] == 1) {
				setfillcolor(BLACK);
				solidrectangle(x + 1, y + 1, x + grid_length - 1, y + grid_length - 1);
			}
			else {
				setfillcolor(RGB(0, 120, 120));
				solidrectangle(x + 1, y + 1, x + grid_length - 1, y + grid_length - 1);
			}
		}
	}
}

// 鼠标消息响应
void Model::mouseControl() {
	// 判断是否有鼠标消息
	if (MouseHit()) {
		// 获取鼠标消息
		MOUSEMSG msg = GetMouseMsg();

		// 判断用户是否点击的是游戏区域
		if (msg.x >= this->left && msg.x <= this->right &&
			msg.y >= this->top && msg.y <= this->down &&
			msg.uMsg == WM_LBUTTONDOWN) {
			// 把鼠标坐标转换为数组下标
			int row = (msg.y - this->top) / grid_length;
			int col = (msg.x - this->left) / grid_length;
			this->gameMapVec[row][col] = -this->gameMapVec[row][col];

			// 改变周围四个格子
			if (row - 1 >= 0) {
				this->gameMapVec[row - 1][col] = -this->gameMapVec[row - 1][col];
			}
			if (row + 1 < grid_num) {
				this->gameMapVec[row + 1][col] = -this->gameMapVec[row + 1][col];
			}
			if (col - 1 >= 0) {
				this->gameMapVec[row][col - 1] = -this->gameMapVec[row][col - 1];
			}
			if (col + 1 < grid_num) {
				this->gameMapVec[row][col + 1] = -this->gameMapVec[row][col + 1];
			}
		}
	}
}

// 游戏结束判断
int Model::over() { // 判断是否达到通关条件
	int count = 0; // 计数器
	for (int i = 0; i < grid_num; i++) {
		for (int k = 0; k < grid_num; k++) {
			if (this->gameMapVec[i][k] == -1) {
				count++;
			}
		}
	}
	return count;
}