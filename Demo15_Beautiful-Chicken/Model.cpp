#include "Model.h"
using namespace std;

Model::Model() :top(0), left(0), down(0), right(0), flag(0) {
	this->beginGame = creatButton(562, 642, pic_size, pic_size, RGB(218, 224, 219), RGB(196, 204, 198));
	this->gameMapVec = {
		{
			{ 1, 1, 1, 1, 1, 1, 1, 1, },
			{ 1, 2, 3, 0, 0, 3, 2, 1, },
			{ 1, 0, 1, 0, 1, 1, 0, 1, },
			{ 1, 0, 0, 4, 0, 0, 0, 1, },
			{ 1, 0, 0, 0, 0, 0, 0, 1, },
			{ 1, 0, 1, 0, 1, 1, 0, 1, },
			{ 1, 2, 3, 0, 0, 3, 2, 1, },
			{ 1, 1, 1, 1, 1, 1, 1, 1, },
		},
		{
			{ 1, 1, 1, 1, 1, 1, 1, 1, },
			{ 1, 2, 3, 0, 0, 3, 2, 1, },
			{ 1, 0, 1, 0, 1, 1, 0, 1, },
			{ 1, 2, 3, 4, 0, 3, 2, 1, },
			{ 1, 0, 0, 0, 0, 0, 0, 1, },
			{ 1, 0, 1, 0, 1, 1, 0, 1, },
			{ 1, 2, 3, 0, 0, 3, 2, 1, },
			{ 1, 1, 1, 1, 1, 1, 1, 1, },
		},
		{
			{ 1, 1, 1, 1, 1, 1, 1, 1, },
			{ 1, 2, 0, 0, 0, 0, 2, 1, },
			{ 1, 3, 1, 0, 1, 1, 3, 1, },
			{ 1, 0, 0, 4, 0, 0, 0, 1, },
			{ 1, 0, 0, 0, 0, 0, 0, 1, },
			{ 1, 3, 1, 0, 1, 1, 3, 1, },
			{ 1, 2, 0, 0, 0, 0, 2, 1, },
			{ 1, 1, 1, 1, 1, 1, 1, 1, },
		},
	};
}

// 获取flag
int& Model::getFlag() {
	return this->flag;
}

// 获取beginGame
Button*& Model::getBeginGame() {
	return this->beginGame;
}

// 加载资源
void Model::load(IMAGE& bk, IMAGE& button, vector<IMAGE>& img, vector<int>& imgIndex) {
	// 批量加载贴图
	for (int i = 0; i < pic_num; i++) {
		char fileName[25];
		sprintf(fileName, "./resource/picture/%d.bmp", imgIndex[i]);
		loadimage(&img[i], fileName, pic_size, pic_size);
	}
	loadimage(&bk, "./resource/picture/bk.jpg", width, height * 1.256);
	loadimage(&button, "./resource/picture/7.bmp", beginGame->width, beginGame->height);
}

// 设置背景音乐
void Model::bgm() {
	// 播放音乐
	mciSendString("open ./resource/music/bk.mp3 alias BGM", nullptr, 0, nullptr);
	mciSendString("play BGM repeat", nullptr, 0, nullptr);
}

// 创建按钮
struct Button* Model::creatButton(int x, int y, int width, int height, COLORREF color1, COLORREF color2) {
	struct Button* pButton = (struct Button*)malloc(sizeof(struct Button));
	pButton->x = x;
	pButton->y = y;
	pButton->width = width;
	pButton->height = height;
	pButton->color1 = color1;
	pButton->color2 = color2;
	return pButton;
}

// 绘制按钮
void Model::drawButton(struct Button* pButton, IMAGE& button) {
	// 输出按钮图片
	putimage(pButton->x, pButton->y, &button);
	// 绘制按钮边框
	for (int x = 8; x >= 0; x--) {
		setlinecolor(pButton->color1);
		rectangle(pButton->x - x, pButton->y - x, pButton->x + pButton->width + x, pButton->y + pButton->height + x);
	}
	for (int x = 16; x >= 8; x--) {
		setlinecolor(pButton->color2);
		rectangle(pButton->x - x, pButton->y - x, pButton->x + pButton->width + x, pButton->y + pButton->height + x);
	}
}

// 初始化游戏数据
void Model::init() {
	// 游戏地图数据初始化
	this->left = width / 2 - (grid_num * pic_size) / 2;
	this->top = height / 2 - (grid_num * pic_size) / 2;
	this->right = this->left + grid_num * pic_size;
	this->down = this->top + grid_num * pic_size;

	// 对vector容器进行初始化
	this->gameMapVec = {
		{
			{ 1, 1, 1, 1, 1, 1, 1, 1, },
			{ 1, 2, 3, 0, 0, 3, 2, 1, },
			{ 1, 0, 1, 0, 1, 1, 0, 1, },
			{ 1, 0, 0, 4, 0, 0, 0, 1, },
			{ 1, 0, 0, 0, 0, 0, 0, 1, },
			{ 1, 0, 1, 0, 1, 1, 0, 1, },
			{ 1, 2, 3, 0, 0, 3, 2, 1, },
			{ 1, 1, 1, 1, 1, 1, 1, 1, },
		},
		{
			{ 1, 1, 1, 1, 1, 1, 1, 1, },
			{ 1, 2, 3, 0, 0, 3, 2, 1, },
			{ 1, 0, 1, 0, 1, 1, 0, 1, },
			{ 1, 2, 3, 4, 0, 3, 2, 1, },
			{ 1, 0, 0, 0, 0, 0, 0, 1, },
			{ 1, 0, 1, 0, 1, 1, 0, 1, },
			{ 1, 2, 3, 0, 0, 3, 2, 1, },
			{ 1, 1, 1, 1, 1, 1, 1, 1, },
		},
		{
			{ 1, 1, 1, 1, 1, 1, 1, 1, },
			{ 1, 2, 0, 0, 0, 0, 2, 1, },
			{ 1, 3, 1, 0, 1, 1, 3, 1, },
			{ 1, 0, 0, 4, 0, 0, 0, 1, },
			{ 1, 0, 0, 0, 0, 0, 0, 1, },
			{ 1, 3, 1, 0, 1, 1, 3, 1, },
			{ 1, 2, 0, 0, 0, 0, 2, 1, },
			{ 1, 1, 1, 1, 1, 1, 1, 1, },
		},
	};
}

// 绘制开始界面
void Model::welcome(IMAGE& bk) {
	putimage(0, -2, &bk);
	setbkmode(TRANSPARENT);
	SetWindowText(GetHWnd(), "C++鸡你太美");

	// 输出作者信息
	settextcolor(RGB(240, 240, 240));
	settextstyle(31, 0, "楷体", 0, 0, 1000, 0, 0, 0);
	outtextxy(16, 12, "鸡你太美");

	settextcolor(RGB(240, 240, 240));
	settextstyle(26, 0, "楷体", 0, 0, 1000, 0, 0, 0);
	outtextxy(13, 52, "作者：初云");

	while (!_kbhit()) {
	}
	int temp = _getch(); // 获取当前按键，避免对后边的按键响应产生影响
}

// 绘制游戏界面
void Model::draw(IMAGE& bk, IMAGE& button, vector<IMAGE>& img) {
	cleardevice();
	putimage(0, -2, &bk);
	this->drawButton(beginGame, button);

	// 绘制作者信息
	setbkmode(TRANSPARENT);
	settextcolor(RGB(235, 235, 235));
	settextstyle(31, 0, "楷体", 0, 0, 1000, 0, 0, 0);
	outtextxy(16, 12, "鸡你太美");

	settextcolor(RGB(240, 240, 240));
	settextstyle(26, 0, "楷体", 0, 0, 1000, 0, 0, 0);
	outtextxy(13, 52, "作者：初云");

	// 绘制外部边框
	for (int x = 25; x >= 0; x--) {
		setlinecolor(RGB(218, 224, 219));
		rectangle(this->left - x, this->top - x, this->right + x, this->down + x);
	}
	for (int x = 50; x >= 25; x--) {
		setlinecolor(RGB(196, 204, 198));
		rectangle(this->left - x, this->top - x, this->right + x, this->down + x);
	}

	// 绘制游戏地图
	for (int i = 0; i < grid_num; i++) {
		for (int j = 0; j < grid_num; j++) {
			// 设置贴图位置坐标
			int x = this->left + pic_size * j;
			int y = this->top + pic_size * i;
			// 对图片信息进行分发
			switch (this->gameMapVec[this->flag][i][j]) {
			case 0: // 空地
				putimage(x, y, &img[0]);
				break;
			case 1: // 墙壁
				putimage(x, y, &img[1]);
				break;
			case 2: // 推箱点
				putimage(x, y, &img[2]);
				break;
			case 3: // 箱子
				putimage(x, y, &img[3]);
				break;
			case 4: // 人物与空地重合
			case 6: // 人物与推箱点重合
				putimage(x, y, &img[4]);
				break;
			case 5: // 箱子与推箱点重合
				putimage(x, y, &img[5]);
				break;
			}
		}
	}
}

// 找到并返回人物横坐标
int Model::search_i() {
	for (int i = 0; i < grid_num; i++) {
		for (int j = 0; j < grid_num; j++) {
			if (this->gameMapVec[this->flag][i][j] == 4 || this->gameMapVec[this->flag][i][j] == 6) {
				return i; // 找到人物
			}
		}
	}
	return -1; // 未找到人物
}

// 找到并返回人物的纵坐标
int Model::search_j() {
	for (int i = 0; i < grid_num; i++) {
		for (int j = 0; j < grid_num; j++) {
			if (this->gameMapVec[this->flag][i][j] == 4 || this->gameMapVec[this->flag][i][j] == 6) {
				return j; // 找到人物
			}
		}
	}
	return -1; // 未找到人物
}

// 按键控制
void Model::keyDown() {
	// 判断是否有按键消息
	while (_kbhit()) {
		// 用于接收用户按键信息
		char userKey = _getch();
		fflush(stdin); // 清空键盘消息缓冲区

		// 找到玩家位置
		int i = this->search_i();
		int j = this->search_j();

		switch (userKey) {
		case 'W': // 上
		case 'w':
		case 72:
			mciSendString("close click", nullptr, 0, nullptr);
			mciSendString("open ./resource/music/click.wav alias click", nullptr, 0, nullptr);
			mciSendString("play click", nullptr, 0, nullptr);
			if (this->gameMapVec[this->flag][i - 1][j] == 0 || this->gameMapVec[this->flag][i - 1][j] == 2) {
				// 如果上方是推箱点或者空地
				this->gameMapVec[this->flag][i][j] -= 4; // 原来位置玩家离开
				this->gameMapVec[this->flag][i - 1][j] += 4; // 新的位置玩家到来
			}
			if (this->gameMapVec[this->flag][i - 1][j] == 3 || this->gameMapVec[this->flag][i - 1][j] == 5) {
				// 如果上方是箱子或者箱子在目的地时
				if (this->gameMapVec[this->flag][i - 2][j] == 0 || this->gameMapVec[this->flag][i - 2][j] == 2) {
					// 如果箱子旁边是空地或者推箱点时
					this->gameMapVec[this->flag][i][j] -= 4; // 原来位置玩家离开
					this->gameMapVec[this->flag][i - 1][j] += 1; // 玩家到达箱子原位置
					this->gameMapVec[this->flag][i - 2][j] += 3; // 箱子到达新位置
				}
			}
			break;
		case 'S': // 下
		case 's':
		case 80:
			mciSendString("close click", nullptr, 0, nullptr);
			mciSendString("open ./resource/music/click.wav alias click", nullptr, 0, nullptr);
			mciSendString("play click", nullptr, 0, nullptr);
			if (this->gameMapVec[this->flag][i + 1][j] == 0 || this->gameMapVec[this->flag][i + 1][j] == 2) {
				// 如果下方是推箱点或者空地
				this->gameMapVec[this->flag][i][j] -= 4; // 原来位置玩家离开
				this->gameMapVec[this->flag][i + 1][j] += 4; // 新的位置玩家到来
			}
			if (this->gameMapVec[this->flag][i + 1][j] == 3 || this->gameMapVec[this->flag][i + 1][j] == 5) {
				// 如果下方是箱子或者箱子在目的地时
				if (this->gameMapVec[this->flag][i + 2][j] == 0 || this->gameMapVec[this->flag][i + 2][j] == 2) {
					//如果箱子旁边是空地或者推箱点时
					this->gameMapVec[this->flag][i][j] -= 4; // 原来位置玩家离开
					this->gameMapVec[this->flag][i + 1][j] += 1; // 玩家到达箱子原位置
					this->gameMapVec[this->flag][i + 2][j] += 3; // 箱子到达新位置
				}
			}
			break;
		case 'A': // 左
		case 'a':
		case 75:
			mciSendString("close click", nullptr, 0, nullptr);
			mciSendString("open ./resource/music/click.wav alias click", nullptr, 0, nullptr);
			mciSendString("play click", nullptr, 0, nullptr);
			if (this->gameMapVec[this->flag][i][j - 1] == 0 || this->gameMapVec[this->flag][i][j - 1] == 2) {
				// 如果左方是推箱点或者空地
				this->gameMapVec[this->flag][i][j] -= 4; // 原来位置玩家离开
				this->gameMapVec[this->flag][i][j - 1] += 4; // 新的位置玩家到来
			}
			if (this->gameMapVec[this->flag][i][j - 1] == 3 || this->gameMapVec[this->flag][i][j - 1] == 5) {
				// 如果下方是箱子或者箱子在目的地时
				if (this->gameMapVec[this->flag][i][j - 2] == 0 || this->gameMapVec[this->flag][i][j - 2] == 2) {
					// 如果箱子旁边是空地或者推箱点时
					this->gameMapVec[this->flag][i][j] -= 4; // 原来位置玩家离开
					this->gameMapVec[this->flag][i][j - 1] += 1; // 玩家到达箱子原位置
					this->gameMapVec[this->flag][i][j - 2] += 3; // 箱子到达新位置
				}
			}
			break;
		case 'D': // 右
		case 'd':
		case 77:
			mciSendString("close click", nullptr, 0, nullptr);
			mciSendString("open ./resource/music/click.wav alias click", nullptr, 0, nullptr);
			mciSendString("play click", nullptr, 0, nullptr);
			if (this->gameMapVec[this->flag][i][j + 1] == 0 || this->gameMapVec[this->flag][i][j + 1] == 2) {
				// 如果右方是推箱点或者空地
				this->gameMapVec[this->flag][i][j] -= 4; // 原来位置玩家离开
				this->gameMapVec[this->flag][i][j + 1] += 4; // 新的位置玩家到来
			}
			if (this->gameMapVec[this->flag][i][j + 1] == 3 || this->gameMapVec[this->flag][i][j + 1] == 5) {
				// 如果下方是箱子或者箱子在目的地时
				if (this->gameMapVec[this->flag][i][j + 2] == 0 || this->gameMapVec[this->flag][i][j + 2] == 2) {
					// 如果箱子旁边是空地或者推箱点时
					this->gameMapVec[this->flag][i][j] -= 4; // 原来位置玩家离开
					this->gameMapVec[this->flag][i][j + 1] += 1; // 玩家到达箱子原位置
					this->gameMapVec[this->flag][i][j + 2] += 3; // 箱子到达新位置
				}
			}
			break;
		}
	}
}

// 判断鼠标是否在按钮中
int Model::mouseInButton(struct Button* pButton, MOUSEMSG* msg) {
	if (msg->x >= pButton->x && msg->x <= pButton->x + pButton->width &&
		msg->y >= pButton->y && msg->y <= pButton->y + pButton->height) {
		pButton->color1 = RGB(185, 196, 187);
		pButton->color2 = RGB(165, 177, 167);
		return 1; // 返回值为1表明鼠标在按钮中
	}
	pButton->color1 = RGB(218, 224, 219);
	pButton->color2 = RGB(196, 204, 198);
	return 0; // 返回值为0表明鼠标不在按钮中
}

// 鼠标控制
void Model::mouseControl(struct Button* pButton) {
	// 判断是否有鼠标消息
	if (MouseHit()) {
		MOUSEMSG msg = GetMouseMsg();
		// 判断用户是否点击按钮区域
		if (this->mouseInButton(pButton, &msg) == 1 && msg.uMsg == WM_LBUTTONDOWN) {
			mciSendString("close reset", nullptr, 0, nullptr);
			mciSendString("open ./resource/music/reset.mp3 alias reset", nullptr, 0, nullptr);
			mciSendString("play reset", nullptr, 0, nullptr);
			this->init();
		}
	}
}

// 游戏结束条件判断
bool Model::over() {
	// 如果一个箱子都没有了那么游戏结束
	for (int i = 0; i < grid_num; i++) {
		for (int j = 0; j < grid_num; j++) {
			if (this->gameMapVec[this->flag][i][j] == 3) {
				return false; // 游戏未结束
			}
		}
	}
	return true; // 游戏结束
}