#include "Model.h"
using namespace std;

Model::Model() :top(0), left(0), down(0), right(0) {}

// 加载资源
void Model::load(IMAGE& bk, IMAGE& white) {
	// 加载图片
	loadimage(&bk, "./resource/picture/bk.jpg", width, height);
	loadimage(&white, "./resource/picture/white.jpg", 533, 300);
}

// 设置背景音乐
void Model::bgm() {
	mciSendString("open ./resource/music/bk.mp3 alias BGM", nullptr, 0, nullptr);
	mciSendString("play BGM repeat", nullptr, 0, nullptr);
}

// 初始化游戏数据
void Model::init() {
	// 设置随机数种子
	srand((unsigned)time(nullptr) * rand());

	// 初始化游戏区域坐标
	this->left = width / 2 - (grid_num * grid_width) / 2;
	this->top = height / 2 - (grid_num * grid_height) / 2;
	this->right = this->left + grid_num * grid_width;
	this->down = this->top + grid_num * grid_height;

	// 初始化vector容器
	this->gameMapVec.clear();
	for (int i = 0; i < grid_num; i++) {
		vector<int> lineGrid;
		for (int j = 0; j < grid_num; j++) {
			lineGrid.push_back(i * grid_num + j);
		}
		this->gameMapVec.push_back(lineGrid);
	}

	// 随机交换vector容器的值
	for (int i = 0; i < grid_num * grid_num; i++) {
		int m = rand() % grid_num;
		int n = rand() % grid_num;
		int temp = this->gameMapVec[i / grid_num][i % grid_num];
		this->gameMapVec[i / grid_num][i % grid_num] = this->gameMapVec[m][n];
		this->gameMapVec[m][n] = temp;
	}
}

// 绘制开始界面
void Model::welcome(IMAGE& bk) {
	putimage(0, 0, &bk);
	setbkmode(TRANSPARENT);
	SetWindowText(GetHWnd(), "C++智力拼图游戏");

	// 输出作者信息
	settextcolor(RGB(220, 220, 220));
	settextstyle(31, 0, "楷体", 0, 0, 1000, 0, 0, 0);
	outtextxy(16, 12, "拼图游戏");

	settextcolor(RGB(215, 215, 215));
	settextstyle(26, 0, "楷体", 0, 0, 1000, 0, 0, 0);
	outtextxy(13, 52, "作者：初云");

	while (!_kbhit()) {
	}
}

// 绘制游戏界面
void Model::draw(IMAGE& bk, IMAGE& white) {
	cleardevice();

	// 游戏界面绘制贴图
	for (int i = 0; i < grid_num; i++) {
		for (int j = 0; j < grid_num; j++) {
			int x = this->left + grid_width * j;
			int y = this->top + grid_height * i;
			if (this->gameMapVec[i][j] == 15) {
				putimage(x, y, grid_width, grid_height, &white, 105, 95);
			}
			else {
				putimage(x, y, grid_width, grid_height, &bk, (this->gameMapVec[i][j] % grid_num) * grid_width, (this->gameMapVec[i][j] / grid_num) * grid_height);
			}
		}
	}
}

// 定位白块i
int Model::searchArray_i() {
	for (int i = 0; i < grid_num; i++) {
		for (int j = 0; j < grid_num; j++) {
			if (this->gameMapVec[i][j] == 15) {
				return i;
			}
		}
	}
	return -1; // 如果没有找到，则返回-1
}

// 定位白块j
int Model::searchArray_j() {
	for (int i = 0; i < grid_num; i++) {
		for (int j = 0; j < grid_num; j++) {
			if (this->gameMapVec[i][j] == 15) {
				return j;
			}
		}
	}
	return -1; // 如果没有找到，则返回-1
}

// 鼠标控制
void Model::mouseControl() {
	if (MouseHit()) { // 判断是否有鼠标消息
		MOUSEMSG msg = GetMouseMsg();
		// 判断用户点击的是否是游戏区域
		if (msg.x >= this->left && msg.x <= this->right &&
			msg.y >= this->top && msg.y <= this->down) {
			// 把鼠标坐标转化为数组下标
			int row = (msg.y - this->top) / grid_height; // 行
			int col = (msg.x - this->left) / grid_width; // 列
			// 搜寻获取白块的横纵坐标
			int white_i = this->searchArray_i();
			int white_j = this->searchArray_j();
			// 对鼠标消息进行分发
			switch (msg.uMsg) {
			case WM_LBUTTONDOWN: // 鼠标左键点击
				mciSendString("close click", nullptr, 0, nullptr);
				mciSendString("open ./resource/music/click.wav alias click", nullptr, 0, nullptr);
				mciSendString("play click", nullptr, 0, nullptr);
				// 判断四个方向的移动
				if (row == white_i + 1 && col == white_j) {//下
					this->gameMapVec[white_i][white_j] = this->gameMapVec[row][col];
					this->gameMapVec[row][col] = 15;
				}
				if (row == white_i - 1 && col == white_j) {//上
					this->gameMapVec[white_i][white_j] = this->gameMapVec[row][col];
					this->gameMapVec[row][col] = 15;
				}
				if (row == white_i && col == white_j + 1) {//右
					this->gameMapVec[white_i][white_j] = this->gameMapVec[row][col];
					this->gameMapVec[row][col] = 15;
				}
				if (row == white_i && col == white_j - 1) {//左
					this->gameMapVec[white_i][white_j] = this->gameMapVec[row][col];
					this->gameMapVec[row][col] = 15;
				}
			}
		}
	}
}

// 游戏结束判断
bool Model::over() {
	int count = 0;
	for (int i = 0; i < grid_num; i++) {
		for (int j = 0; j < grid_num; j++) {
			if (this->gameMapVec[i][j] != count) {
				return false; // 游戏未结束则返回false
			}
			count++;
		}
	}
	return true; // 游戏结束则返回true
}