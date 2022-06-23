#include "Model.h"
using namespace std;

Model::Model() :count(0) {}

// 获取count
int& Model::getCount()
{
	return this->count;
}

// 加载资源
void Model::load(IMAGE& bk1, IMAGE& bk2, IMAGE& w1, vector<IMAGE>& img, vector<int>& imgIndex, vector<int>& pic_width, vector<int>& pic_height)
{
	loadimage(&w1, "./resource/picture/w1.png");
	loadimage(&bk1, "./resource/picture/bk1.jpg", WIDTH, HEIGHT * 1.254);
	loadimage(&bk2, "./resource/picture/bk2.png", WIDTH, HEIGHT * 1.254);

	// 批量加载照片文件
	for (int i = 0; i < PIC_NUM; i++)
	{
		char fileName[25]; // 定义字符数组，用来存放带有拓展名的图片名称
		sprintf(fileName, "./resource/picture/%d.jpg", imgIndex[i]);
		loadimage(&img[i], fileName, pic_width[i], pic_height[i]);
	}
}

// 加载背景音乐
void Model::bgm()
{
	mciSendString("open ./resource/music/bk.mp3 alias BGM", nullptr, 0, nullptr);
	mciSendString("play BGM repeat", nullptr, 0, nullptr);
}

// 载入PNG图并去透明部分——背景图（比绘图窗口大）
void Model::drawAlphabk(IMAGE* picture, const int  picture_x, const int picture_y)
{
	// 变量初始化
	DWORD* dst = GetImageBuffer(); // GetImageBuffer()函数，用于获取绘图设备的显存指针，EASYX自带
	DWORD* draw = GetImageBuffer();
	DWORD* src = GetImageBuffer(picture) + 22 * WIDTH; // 获取picture的显存指针
	int picture_width = picture->getwidth(); // 获取picture的宽度，EASYX自带
	int picture_height = picture->getheight(); // 获取picture的高度，EASYX自带
	int graphWidth = getwidth(); // 获取绘图区的宽度，EASYX自带
	int graphHeight = getheight(); // 获取绘图区的高度，EASYX自带
	int dstX = 0; // 在显存里像素的角标

	// 实现透明贴图 公式：Cp=αp*FP+(1-αp)*BP，贝叶斯定理来进行点颜色的概率计算
	for (int iy = 0; iy < graphHeight; iy++)
	{
		for (int ix = 0; ix < graphWidth; ix++)
		{
			int srcX = ix + iy * graphWidth; // 在显存里像素的角标
			int sa = ((src[srcX] & 0xff000000) >> 24); // 0xAArrggbb;AA是透明度
			int sr = ((src[srcX] & 0xff0000) >> 16); // 获取RGB里的R
			int sg = ((src[srcX] & 0xff00) >> 8); // 获取RGB里的G
			int sb = src[srcX] & 0xff; // 获取RGB里的B
			if (ix >= 0 && ix <= graphWidth && iy >= 0 && iy <= graphHeight && dstX <= graphWidth * graphHeight)
			{
				dstX = (ix + picture_x) + (iy + picture_y) * graphWidth; // 在显存里像素的角标
				int dr = ((dst[dstX] & 0xff0000) >> 16);
				int dg = ((dst[dstX] & 0xff00) >> 8);
				int db = dst[dstX] & 0xff;
				draw[dstX] = ((sr * sa / 255 + dr * (255 - sa) / 255) << 16) // 公式： Cp=αp*FP+(1-αp)*BP；αp=sa/255 , FP=sr , BP=dr
					| ((sg * sa / 255 + dg * (255 - sa) / 255) << 8) // αp=sa/255 , FP=sg , BP=dg
					| (sb * sa / 255 + db * (255 - sa) / 255); // αp=sa/255 , FP=sb , BP=db
			}
		}
	}
}

// 载入PNG图并去透明部分——一般图片（比绘图窗口小）
void Model::drawAlpha(IMAGE* picture, const int  picture_x, const int picture_y)
{
	// 变量初始化
	DWORD* dst = GetImageBuffer(); // GetImageBuffer()函数，用于获取绘图设备的显存指针，EASYX自带
	DWORD* draw = GetImageBuffer();
	DWORD* src = GetImageBuffer(picture); // 获取picture的显存指针
	int picture_width = picture->getwidth(); // 获取picture的宽度，EASYX自带
	int picture_height = picture->getheight(); // 获取picture的高度，EASYX自带
	int graphWidth = getwidth(); // 获取绘图区的宽度，EASYX自带
	int graphHeight = getheight(); // 获取绘图区的高度，EASYX自带
	int dstX = 0; // 在显存里像素的角标

	// 实现透明贴图 公式：Cp=αp*FP+(1-αp)*BP，贝叶斯定理来进行点颜色的概率计算
	for (int iy = 0; iy < picture_height; iy++)
	{
		for (int ix = 0; ix < picture_width; ix++)
		{
			int srcX = ix + iy * picture_width; // 在显存里像素的角标
			int sa = ((src[srcX] & 0xff000000) >> 24); // 0xAArrggbb;AA是透明度
			int sr = ((src[srcX] & 0xff0000) >> 16); // 获取RGB里的R
			int sg = ((src[srcX] & 0xff00) >> 8); // 获取RGB里的G
			int sb = src[srcX] & 0xff; // 获取RGB里的B
			if (ix >= 0 && ix <= graphWidth && iy >= 0 && iy <= graphHeight && dstX <= graphWidth * graphHeight)
			{
				dstX = (ix + picture_x) + (iy + picture_y) * graphWidth; // 在显存里像素的角标
				int dr = ((dst[dstX] & 0xff0000) >> 16);
				int dg = ((dst[dstX] & 0xff00) >> 8);
				int db = dst[dstX] & 0xff;
				draw[dstX] = ((sr * sa / 255 + dr * (255 - sa) / 255) << 16) //公式： Cp=αp*FP+(1-αp)*BP；αp=sa/255 , FP=sr , BP=dr
					| ((sg * sa / 255 + dg * (255 - sa) / 255) << 8) //αp=sa/255 , FP=sg , BP=dg
					| (sb * sa / 255 + db * (255 - sa) / 255); //αp=sa/255 , FP=sb , BP=db
			}
		}
	}
}

// 绘制开始界面
void Model::welcome(IMAGE& bk1, IMAGE& w1)
{
	cleardevice();
	putimage(0, -22, &bk1);
	setbkmode(TRANSPARENT);
	SetWindowText(GetHWnd(), "C++音乐相册");

	// 输出作者信息
	settextcolor(WHITE);
	settextstyle(28, 0, "楷体", 0, 0, 1000, 0, 0, 0);
	outtextxy(15, 10, "音乐相册");
	drawAlpha(&w1, 10, 285);

	// 按任意键继续
	while (!_kbhit()) {}
}

// 绘制心形边框
void Model::drawBorder(const int x0, const int y0, const int size, COLORREF C, const int borderwidth)
{
	double m, n;
	double x, y;
	for (double i = 0; i <= 6.29; i = i + 0.01)
	{
		// 产生极坐标点
		m = i;
		n = -size * (((sin(i) * sqrt(fabs(cos(i)))) / (sin(i) + 1.4142)) - 2 * sin(i) + 2);
		// 转换为笛卡尔坐标
		x = n * cos(m) + x0;
		y = n * sin(m) + y0;
		// 绘制心形边框
		setfillcolor(C);
		solidcircle(x, y, borderwidth);
	}
}

// 在绘制心形边框处绘制字段
void Model::drawText(const double i, const int x0, const int y0, const int size, const char* s)
{
	double m, n;
	double x, y;

	// 产生极坐标点
	m = i;
	n = -size * (((sin(i) * sqrt(fabs(cos(i)))) / (sin(i) + 1.4142)) - 2 * sin(i) + 2);
	// 转换为笛卡尔坐标
	x = n * cos(m) + x0;
	y = n * sin(m) + y0;

	outtextxy(x - 11, y - 11, s); // 确保是文字中点在跟随线路移动
}

// 绘制心形图案
void Model::drawHeart(IMAGE& bk2, vector<IMAGE>& img, vector<int>& pic_x, vector<int>& pic_y, const int x, const int y, const int size, COLORREF C, const char s[][5], const double move, const double interval, const int time)
{
	settextcolor(C);
	settextstyle(22, 0, "楷体", 0, 0, 1000, 0, 0, 0);

	for (double i = 0 + move; i <= 6.29 + move; i = i + 0.01)
	{
		cleardevice(); // 在字出来之后清屏
		putimage(pic_x[this->count % PIC_NUM], pic_y[this->count % PIC_NUM], &img[this->count % PIC_NUM]);
		drawAlphabk(&bk2, 0, 0);

		// 输出作者信息
		settextcolor(WHITE);
		settextstyle(28, 0, "楷体", 0, 0, 1000, 0, 0, 0);
		outtextxy(15, 10, "音乐相册");

		// 按照间隔时间，对字体进行输出
		this->drawText(i, x, y, size, s[0]);
		this->drawText(i + interval, x, y, size, s[1]);
		this->drawText(i + interval * 2, x, y, size, s[2]);
		this->drawText(i + interval * 3, x, y, size, s[3]);
		this->drawText(i + interval * 4, x, y, size, s[4]);
		this->drawText(i + interval * 5, x, y, size, s[5]);
		this->drawText(i + interval * 6, x, y, size, s[6]);
		FlushBatchDraw();

		// 延时（单位是毫秒）
		Sleep(time);
	}
}
