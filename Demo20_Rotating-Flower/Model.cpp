#include "Model.h"
using namespace std;

// 加载音乐
void Model::bgm() {
	mciSendString("open ./resource/music/bk.mp3 alias BGM", nullptr, 0, nullptr);
	mciSendString("play BGM repeat", nullptr, 0, nullptr);
}

// Timer定时器
int Model::timer(time_t sec, int id) {
	static time_t start[3] = { clock(), clock(), clock() };
	time_t end = clock();
	if (end - start[id] >= sec) {
		start[id] = end;
		return 1;
	}
	return 0;
}

// 初始化数据
void Model::init() {
	// 设置背景颜色
	setbkcolor(RGB(244, 215, 215));
	SetWindowText(GetHWnd(), "C++旋转花");

	// 对旋转小球进行初始化
	for (int i = 0; i < ball_num; i++) {
		struct Ball newball;
		newball.x = width * 1.0 / 2 + 165.0 * cos(pi / 2 - i * pi / 8);
		newball.y = height * 1.0 / 2 - 165.0 * cos(i * pi / 8);
		newball.r = 7;
		newball.step = 5.4;
		newball.tempstep = newball.step;
		newball.flag = 0;
		newball.juge = 0;
		if (i < 5) {
			newball.color = RGB(255 - 50 * i, 50 * i, 0);
		}
		else if (i >= 5 && i < 10) {
			newball.color = RGB(0, 255 - 50 * (i % 5), 50 * (i % 5));
		}
		else {
			newball.color = RGB(40 * (i % 10), 0, 255 - 40 * (i % 10));
		}
		this->balls.push_back(newball);
	}
}

// 绘制程序界面
void Model::draw() {
	cleardevice();

	// 绘制旋转小球
	for (int i = 0; i < ball_num; i++) {
		setfillcolor(this->balls[i].color);
		solidcircle(this->balls[i].x, this->balls[i].y, this->balls[i].r);
	}
}

// 小球移动条件判断
void Model::judeMove() {
	static int k = 0;
	if (k >= 16) {
		goto out;
	}
	if (this->timer(206.6, 0) == 1) {
		this->balls[k].juge = 1;
		k++;
	}
out:;
}

// 小球的移动
void Model::ballMove() {
	this->judeMove();
	for (int i = 0; i < ball_num; i++) {
		// 判读当前小球是否可以开始移动
		if (this->balls[i].juge == 1) {
			// 小球来回移动效果设置
			if (this->balls[i].flag == 0) {
				this->balls[i].step -= 0.1285;
				if (this->balls[i].step <= -this->balls[i].tempstep) {
					this->balls[i].step = -this->balls[i].tempstep;
					this->balls[i].flag = 1;
				}
			}
			else if (this->balls[i].flag == 1) {
				this->balls[i].step += 0.1285;
				if (this->balls[i].step >= this->balls[i].tempstep) {
					this->balls[i].step = this->balls[i].tempstep;
					this->balls[i].flag = 0;
				}
			}
			this->balls[i].x += this->balls[i].step * cos(pi / 2 - i * pi / 8);
			this->balls[i].y -= this->balls[i].step * cos(i * pi / 8);
		}
	}
}