#include "Model.h"
using namespace std;

// 设置背景音乐
void Model::bgm() {
	mciSendString("open ./resource/music/bk.mp3 alias BGM", nullptr, 0, nullptr);
	mciSendString("play BGM repeat", nullptr, 0, nullptr);
}

// 产生字符
char Model::createChar() {
	char temp;
	int flag = rand() % 3;

	// 随机产生不同的字符
	if (flag == 0) { // 产生大写字母
		temp = rand() % 26 + 'A';
	}
	else if (flag == 1) { // 产生小写字母
		temp = rand() % 26 + 'a';
	}
	else { // 产生数字
		temp = rand() % 10 + '0';
	}
	return temp;
}

// 初始化数据
void Model::init(const int str_num, const int str_size, const int str_width, const int height) {
	SetWindowText(GetHWnd(), "C++黑客帝国数字雨");
	// 设置随机数种子
	srand((unsigned)time(nullptr) * rand());
	// 对结构体进行初始化
	for (int i = 0; i < str_num; i++) {
		struct Rain newrain;
		newrain.x = i * str_width; // 对字符串x坐标初始化
		newrain.y = rand() % (height + str_size * str_width) - str_size * str_width;
		newrain.step = (rand() % 2000) / 1000.0 + 3;
		newrain.str = new char[str_size];
		for (int k = 0; k < str_size; k++) {
			newrain.str[k] = this->createChar();
		}
		this->rainVec.push_back(newrain);
	}
}

// 让数字雨不断变化
void Model::rainChange(const int str_num, const int str_size) {
	for (int i = 0; i < str_num; i++) {
		this->rainVec[rand() % str_num].str[rand() % str_size] = this->createChar();
	}
}

// 让数字雨不断下落
void Model::rainMove(const int str_num, const int str_size, const int str_width, const int height) {
	// 让数字雨下落
	for (int i = 0; i < str_num; i++) {
		this->rainVec[i].y += this->rainVec[i].step;
		// 设置界限，当数字雨超出屏幕时重新生成
		if (this->rainVec[i].y > height) {
			this->rainVec[i].y = -str_size * str_width; // 对字符串y坐标初始化
			this->rainVec[i].step = (rand() % 2000) / 1000.0 + 3; // 对字符串下落速度进行初始化
		}
	}
}

// 输出字符到绘图窗口
void Model::printChar(const int str_num, const int str_size, const int str_width) {
	// 清空屏幕
	cleardevice();
	// 设置字体格式
	settextstyle(0, 0, 0, 0, 0, 1000, 0, 0, 0);
	// 在绘图窗口上对字符串进行输出
	for (int i = 0; i < str_num; i++) {
		for (int k = 0; k < str_size; k++) {
			settextcolor(RGB(0, 15 + 12 * k, 0));
			outtextxy(this->rainVec[i].x + 2, this->rainVec[i].y + str_width * k, this->rainVec[i].str[k]);
		}
	}
}