#include "Model.h"
using namespace std;

// 加载资源
void Model::load(IMAGE& bk, int width, int height) {
	loadimage(&bk, "./resource/picture/bk.jpg", width * 1.035, height * 1.02);
}

// 播放音乐
void Model::bgm() {
	mciSendString("open ./resource/music/bk.mp3 alias BGM", nullptr, 0, nullptr);
	mciSendString("play BGM repeat", nullptr, 0, nullptr);
}

// 初始化星星
struct Star Model::initStar(const int height) {
	struct Star newstar;
	newstar.x = 0;
	newstar.y = rand() % height; // 星星的变换在整个界面进行
	newstar.radius = (rand() % 3000) / 1000.0 + 1;
	newstar.step = (rand() % 5000) / 1000.0 + 1;
	newstar.color = (int)(newstar.step * 255 / 6.0 + 0.5); // 速度越快，颜色越亮
	if (newstar.color <= 160) { // 避免星星亮度太低
		newstar.color = 160;
	}
	return newstar;
}

// 移动星星
void Model::moveStar(const int i, const int width, const int height) {
	this->startVec[i].x += this->startVec[i].step;
	if (this->startVec[i].x > width) {
		this->startVec[i] = Model::initStar(height);
	}

	// 绘制新的星星
	setfillcolor(RGB(this->startVec[i].color, this->startVec[i].color, this->startVec[i].color));
	solidcircle((int)this->startVec[i].x, this->startVec[i].y, this->startVec[i].radius);
}

// 初始化数据
void Model::init(const int star_num, const int width, const int height) {
	// 设置随机数种子
	srand((unsigned)time(nullptr));

	// 初始化所有星星
	for (int i = 0; i < star_num; i++) {
		this->startVec.push_back(initStar(height));
		this->startVec[i].x = rand() % width;
	}
}

// 绘制程序界面
void Model::draw(IMAGE& bk) {
	putimage(0, 0, &bk);
	setbkmode(TRANSPARENT);
	SetWindowText(GetHWnd(), "C++闪烁星星");

	// 输出作者信息
	settextcolor(RGB(175, 175, 175));
	settextstyle(22, 0, "楷体", 0, 0, 1000, 0, 0, 0);
	outtextxy(30, 20, "二维星星");

	settextcolor(RGB(170, 170, 170));
	settextstyle(18, 0, "楷体", 0, 0, 900, 0, 0, 0);
	outtextxy(27, 50, "作者：初云");
}