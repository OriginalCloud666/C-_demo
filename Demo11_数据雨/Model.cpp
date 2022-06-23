#include "Model.h"
using namespace std;

// 设置背景音乐
void Model::bgm()
{
	mciSendString("open ./resource/music/bk.mp3 alias BGM", nullptr, 0, nullptr);
	mciSendString("play BGM repeat", nullptr, 0, nullptr);
}

// 产生字符
char Model::createChar()
{
	char temp;
	int flag = rand() % 3;

	// 随机产生不同的字符
	if (flag == 0) // 产生大写字母
	{
		temp = rand() % 26 + 'A';
	}
	else if (flag == 1) // 产生小写字母
	{
		temp = rand() % 26 + 'a';
	}
	else // 产生数字
	{
		temp = rand() % 10 + '0';
	}
	return temp;
}

// 初始化数据
void Model::init()
{
	SetWindowText(GetHWnd(), "C++黑客帝国数字雨");
	// 设置随机数种子
	srand((unsigned)time(nullptr) * rand());
	// 对结构体进行初始化
	for (int i = 0; i < STR_NUM; i++)
	{
		struct Rain newrain;
		newrain.x = i * STR_WIDTH; // 对字符串x坐标初始化
		newrain.y = rand() % (HEIGHT + STR_SIZE * STR_WIDTH) - STR_SIZE * STR_WIDTH;
		newrain.step = (rand() % 2000) / 1000.0 + 3;
		newrain.str = new char[STR_SIZE];
		for (int k = 0; k < STR_SIZE; k++)
		{
			newrain.str[k] = this->createChar();
		}
		this->rainVec.push_back(newrain);
	}
}

// 让数字雨不断变化
void Model::rainChange()
{
	for (int i = 0; i < STR_NUM; i++)
	{
		this->rainVec[rand() % STR_NUM].str[rand() % STR_SIZE] = this->createChar();
	}
}

// 让数字雨不断下落
void Model::rainMove()
{
	// 让数字雨下落
	for (int i = 0; i < STR_NUM; i++)
	{
		this->rainVec[i].y += this->rainVec[i].step;
		// 设置界限，当数字雨超出屏幕时重新生成
		if (this->rainVec[i].y > HEIGHT)
		{
			this->rainVec[i].y = -STR_SIZE * STR_WIDTH; // 对字符串y坐标初始化
			this->rainVec[i].step = (rand() % 2000) / 1000.0 + 3; // 对字符串下落速度进行初始化
		}
	}
}

// 输出字符到绘图窗口
void Model::printChar()
{
	// 清空屏幕
	cleardevice();
	// 设置字体格式
	settextstyle(0, 0, 0, 0, 0, 1000, 0, 0, 0);
	// 在绘图窗口上对字符串进行输出
	for (int i = 0; i < STR_NUM; i++)
	{
		for (int k = 0; k < STR_SIZE; k++)
		{
			settextcolor(RGB(0, 15 + 12 * k, 0));
			outtextxy(this->rainVec[i].x + 2, this->rainVec[i].y + STR_WIDTH * k, this->rainVec[i].str[k]);
		}
	}
}
