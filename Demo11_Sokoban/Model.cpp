#include "Model.h"
using namespace std;

Model::Model() :top(0), left(0), down(0), right(0), flag(0) {
	this->gameMapVec = {
		{
			{ 2, 2, 1, 1, 1, 2, 2, 2 },
			{ 2, 2, 1, 4, 1, 2, 2, 2 },
			{ 2, 2, 1, 2, 1, 1, 1, 1 },
			{ 1, 1, 1, 3, 2, 3, 4, 1 },
			{ 1, 4, 2, 3, 6, 1, 1, 1 },
			{ 1, 1, 1, 1, 3, 1, 2, 2 },
			{ 2, 2, 2, 1, 4, 1, 2, 2 },
			{ 2, 2, 2, 1, 1, 1, 2, 2 },
		},

		{
			{ 1, 1, 1, 1, 1, 1, 1, 1 },
			{ 1, 1, 6, 2, 1, 1, 1, 1 },
			{ 1, 1, 2, 3, 2, 2, 1, 1 },
			{ 1, 1, 1, 2, 1, 2, 1, 1 },
			{ 1, 4, 1, 2, 1, 2, 2, 1 },
			{ 1, 4, 3, 2, 2, 1, 2, 1 },
			{ 1, 4, 2, 2, 2, 3, 2, 1 },
			{ 1, 1, 1, 1, 1, 1, 1, 1 },
		},

		{
			{ 2, 2, 1, 1, 1, 1, 2, 2 },
			{ 2, 2, 1, 4, 4, 1, 2, 2 },
			{ 2, 1, 1, 2, 4, 1, 1, 2 },
			{ 2, 1, 2, 2, 3, 4, 1, 2 },
			{ 1, 1, 2, 3, 2, 2, 1, 1 },
			{ 1, 2, 2, 1, 3, 3, 2, 1 },
			{ 1, 2, 2, 6, 2, 2, 2, 1 },
			{ 1, 1, 1, 1, 1, 1, 1, 1 },
		}
	};
}

// 获取flag
int& Model::getFlag() {
	return this->flag;
}

// 加载资源
void Model::load(IMAGE& bk, vector<IMAGE>& img, vector<int>& imgIndex, const int pic_num, const int pic_size, const int width, const int height) {
	// 批量加载图片
	for (int i = 0; i < pic_num; i++) {
		char fileName[25];
		sprintf(fileName, "./resource/picture/%d.png", imgIndex[i]);
		loadimage(&img[i], fileName, pic_size, pic_size);
	}
	loadimage(&bk, "./resource/picture/bk.jpg", width, height * 1.3);
}

// 设置背景音乐
void Model::bgm() {
	mciSendString("open ./resource/music/bk.mp3 alias BGM", nullptr, 0, nullptr);
	mciSendString("play BGM repeat", nullptr, 0, nullptr);
}

// 初始化游戏数据
void Model::init(const int width, const int height, const int grid_num, const int pic_size, const int cas) {
	// 初始化游戏区域坐标
	this->left = width / 2 - (grid_num * pic_size) / 2;
	this->top = height / 2 - (grid_num * pic_size) / 2;
	this->right = this->left + grid_num * pic_size;
	this->down = this->top + grid_num * pic_size;
}

// 绘制开始界面
void Model::welcome(IMAGE& bk) {
	putimage(0, 0, &bk);
	setbkmode(TRANSPARENT);
	SetWindowText(GetHWnd(), "C++推箱子游戏");

	// 输出作者信息
	settextcolor(RGB(120, 120, 120));
	settextstyle(28, 0, "楷体", 0, 0, 1000, 0, 0, 0);
	outtextxy(478, 23, "推箱子游戏");

	settextcolor(RGB(115, 115, 115));
	settextstyle(26, 0, "楷体", 0, 0, 1000, 0, 0, 0);
	outtextxy(484, 63, "作者：初云");

	while (!_kbhit()) {
	}
	int temp = _getch(); // 获取此时的按键，避免对之后的按键响应产生影响
}

// 绘制游戏界面
void Model::draw(IMAGE& bk, vector<IMAGE>& img, const int grid_num, const int pic_size) {
	cleardevice();
	putimage(0, 0, &bk);

	// 输出作者信息
	settextcolor(RGB(120, 120, 120));
	settextstyle(28, 0, "楷体", 0, 0, 1000, 0, 0, 0);
	outtextxy(478, 23, "推箱子游戏");

	settextcolor(RGB(115, 115, 115));
	settextstyle(26, 0, "楷体", 0, 0, 1000, 0, 0, 0);
	outtextxy(484, 63, "作者：初云");

	// 绘制外部边框
	for (int x = 25; x >= 0; x--) {
		setlinecolor(RGB(160, 100, 67));
		rectangle(this->left - x, this->top - x, this->right + x, this->down + x);
	}
	for (int x = 45; x >= 25; x--) {
		setlinecolor(RGB(128, 64, 0));
		rectangle(this->left - x, this->top - x, this->right + x, this->down + x);
	}

	// 绘制游戏地图
	for (int i = 0; i < grid_num; i++) {
		for (int j = 0; j < grid_num; j++) {
			// 设置贴图位置坐标
			int x = this->left + pic_size * j;
			int y = this->top + pic_size * i;
			// 对图片进行分发
			switch (this->gameMapVec[flag][i][j]) {
			case 0:
				putimage(x, y, &img[0]);
				break;
			case 1:
				putimage(x, y, &img[1]);
				break;
			case 2:
				putimage(x, y, &img[2]);
				break;
			case 3:
				putimage(x, y, &img[3]);
				break;
			case 4:
				putimage(x, y, &img[4]);
				break;
			case 5:
				putimage(x, y, &img[5]);
				break;
			case 6: // 玩家与空地重合
			case 8: // 玩家与箱子停放点重合
				putimage(x, y, &img[6]);
				break;
			case 7: // 玩家与空地重合
			case 9: // 玩家与箱子停放点重合
				putimage(x, y, &img[7]);
				break;
			case 10: // 玩家与空地重合
			case 12: // 玩家与箱子停放点重合
				putimage(x, y, &img[8]);
				break;
			case 11: // 玩家与空地重合
			case 13: // 玩家与箱子停放点重合
				putimage(x, y, &img[9]);
				break;
			}
		}
	}
}

// 遇到空地或推箱目的地时将对玩家原位置进行更改
void Model::changOne(const int i, const int j) {
	// 将玩家移动前的位置设置为空地或者推箱目的地
	if (this->gameMapVec[flag][i][j] == 7 || this->gameMapVec[flag][i][j] == 9) { // 上
		this->gameMapVec[flag][i][j] -= 5;
	}
	if (this->gameMapVec[flag][i][j] == 6 || this->gameMapVec[flag][i][j] == 8) { // 下
		this->gameMapVec[flag][i][j] -= 4;
	}
	else if (this->gameMapVec[flag][i][j] == 10 || this->gameMapVec[flag][i][j] == 12) { // 左
		this->gameMapVec[flag][i][j] -= 8;
	}
	else if (this->gameMapVec[flag][i][j] == 11 || this->gameMapVec[flag][i][j] == 13) { // 右
		this->gameMapVec[flag][i][j] -= 9;
	}
}

// 按键响应
void Model::keyDown(const int grid_num) {
	// 判断是否有键盘消息
	while (_kbhit()) {
		// 用于接收用户按键
		char userKey = _getch();
		fflush(stdin); // 清空键盘缓冲区

		// 找到玩家位置
		int i, j = 0;
		for (i = 0; i < grid_num; i++) {
			for (j = 0; j < grid_num; j++) {
				// 玩家的不同朝向
				if (this->gameMapVec[flag][i][j] >= 6 && this->gameMapVec[flag][i][j] <= 13) {
					break;
				}
			}
			if (this->gameMapVec[flag][i][j] >= 6 && this->gameMapVec[flag][i][j] <= 13) {
				break;
			}
		}

		// 按键响应
		switch (userKey) {
		case'W': // 上
		case 'w':
		case 72:
			mciSendString("close click", nullptr, 0, nullptr);
			mciSendString("open ./resource/music/click.wav alias click", nullptr, 0, nullptr);
			mciSendString("play click", nullptr, 0, nullptr);
			if (this->gameMapVec[flag][i - 1][j] == 2 || this->gameMapVec[flag][i - 1][j] == 4) {
				// 如果遇到到是空地或推箱目的地
				this->changOne(i, j); // 玩家离开原位置
				this->gameMapVec[flag][i - 1][j] += 5;
			}
			if (this->gameMapVec[flag][i - 1][j] == 3 || this->gameMapVec[flag][i - 1][j] == 5) {
				// 如果遇到的是箱子或箱子在目的地时
				if (this->gameMapVec[flag][i - 2][j] == 2 || this->gameMapVec[flag][i - 2][j] == 4) {
					// 如果箱子旁的是空地或推箱目的地
					this->changOne(i, j); // 玩家离开原位置
					this->gameMapVec[flag][i - 1][j] += (5 - 1); // 玩家到达箱子位置
					this->gameMapVec[flag][i - 2][j] += 1; // 箱子到达新位置
				}
			}
			break;
		case'S': // 下
		case 's':
		case 80:
			mciSendString("close click", nullptr, 0, nullptr);
			mciSendString("open ./resource/music/click.wav alias click", nullptr, 0, nullptr);
			mciSendString("play click", nullptr, 0, nullptr);
			if (this->gameMapVec[flag][i + 1][j] == 2 || this->gameMapVec[flag][i + 1][j] == 4) {
				// 如果遇到到是空地或推箱目的地
				this->changOne(i, j); // 玩家离开原位置
				this->gameMapVec[flag][i + 1][j] += 4;
			}
			if (this->gameMapVec[flag][i + 1][j] == 3 || this->gameMapVec[flag][i + 1][j] == 5) {
				// 如果遇到的是箱子或箱子在目的地时
				if (this->gameMapVec[flag][i + 2][j] == 2 || this->gameMapVec[flag][i + 2][j] == 4) {
					// 如果箱子旁的是空地或推箱目的地
					this->changOne(i, j); // 玩家离开原位置
					this->gameMapVec[flag][i + 1][j] += (4 - 1); // 玩家到达箱子位置
					this->gameMapVec[flag][i + 2][j] += 1; // 箱子到达新位置
				}
			}
			break;
		case'A': // 左
		case 'a':
		case 75:
			mciSendString("close click", nullptr, 0, nullptr);
			mciSendString("open ./resource/music/click.wav alias click", nullptr, 0, nullptr);
			mciSendString("play click", nullptr, 0, nullptr);
			if (this->gameMapVec[flag][i][j - 1] == 2 || this->gameMapVec[flag][i][j - 1] == 4) {
				// 如果遇到到是空地或推箱目的地
				this->changOne(i, j); // 玩家离开原位置
				this->gameMapVec[flag][i][j - 1] += 8;
			}
			if (this->gameMapVec[flag][i][j - 1] == 3 || this->gameMapVec[flag][i][j - 1] == 5) {
				// 如果遇到的是箱子或箱子在目的地时
				if (this->gameMapVec[flag][i][j - 2] == 2 || this->gameMapVec[flag][i][j - 2] == 4) {
					// 如果箱子旁的是空地或推箱目的地
					this->changOne(i, j); // 玩家离开原位置
					this->gameMapVec[flag][i][j - 1] += (8 - 1); // 玩家到达箱子位置
					this->gameMapVec[flag][i][j - 2] += 1; // 箱子到达新位置
				}
			}
			break;
		case'D': // 右
		case 'd':
		case 77:
			mciSendString("close click", nullptr, 0, nullptr);
			mciSendString("open ./resource/music/click.wav alias click", nullptr, 0, nullptr);
			mciSendString("play click", nullptr, 0, nullptr);
			if (this->gameMapVec[flag][i][j + 1] == 2 || this->gameMapVec[flag][i][j + 1] == 4) {
				// 如果遇到到是空地或推箱目的地
				this->changOne(i, j); // 玩家离开原位置
				this->gameMapVec[flag][i][j + 1] += 9;
			}
			if (this->gameMapVec[flag][i][j + 1] == 3 || this->gameMapVec[flag][i][j + 1] == 5) {
				// 如果遇到的是箱子或箱子在目的地时
				if (this->gameMapVec[flag][i][j + 2] == 2 || this->gameMapVec[flag][i][j + 2] == 4) {
					// 如果箱子旁的是空地或推箱目的地
					this->changOne(i, j); // 玩家离开原位置
					this->gameMapVec[flag][i][j + 1] += (9 - 1); // 玩家到达箱子位置
					this->gameMapVec[flag][i][j + 2] += 1; // 箱子到达新位置
				}
			}
			break;
		}
	}
}

// 结束条件判断
int Model::over(const int grid_num) {
	// 状态标识，用于判断游戏是否结束
	int countFlag = 1;
	for (int i = 0; i < grid_num; i++) {
		for (int j = 0; j < grid_num; j++) {
			if (this->gameMapVec[flag][i][j] == 3) {
				// 如果游戏中仍存在未到目的地的箱子则游戏并未结束
				countFlag = 0;
				return countFlag;
			}
		}
	}
	// 如果游戏中不存在未到目的地的箱子则游戏结束
	return countFlag;
}

// 游戏结束动画设置
void Model::action(IMAGE& bk, vector<IMAGE>& img, const int grid_num, const int pic_size, const int width, const int height) {
	IMAGE end, endy;
	// 加载图片
	loadimage(&end, "./resource/picture/end.bmp");
	loadimage(&endy, "./resource/picture/endy.bmp");

	// 起始坐标设置
	double startX = width / 2 - 204.0 / 2;
	double startY = height;

	while (startY >= height / 2 - 54.0 / 2) {
		this->draw(bk, img, grid_num, pic_size);
		putimage(startX, startY, &endy, SRCAND); // 贴掩码图
		putimage(startX, startY, &end, SRCPAINT); // 贴源码图
		FlushBatchDraw();
		startY -= 0.25; // 控制动画速度
		Sleep(0); // 控制动画速度
	}
}