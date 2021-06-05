#include "Model.h"
using namespace std;

Model::Model() :flag(0) {
	this->img_ptr1 = nullptr;
	this->img_ptr2 = nullptr;
	this->buf = new short[pic_height * width + pic_width];
	this->buf2 = new short[pic_height * width + pic_width];
}

Model::~Model() {
	delete this->img_ptr1;
	delete this->img_ptr2;
	delete this->buf;
	delete this->buf2;
}

// 加载资源
void Model::load(IMAGE& src_img) {
	loadimage(&src_img, "./resource/picture/bk.jpg", pic_width, pic_height);
}

// 播放音乐
void Model::bgm() {
	mciSendString("open ./resource/music/bk.mp3 alias BGM", nullptr, 0, nullptr);
	mciSendString("play BGM repeat", nullptr, 0, nullptr);
}

// 初始化数据
void Model::init(IMAGE& src_img, IMAGE& dest_img) {
	setbkmode(TRANSPARENT);
	SetWindowText(GetHWnd(), "C++律动波纹");

	// 获得绘图设备的显存指针
	this->img_ptr1 = GetImageBuffer(&src_img);
	this->img_ptr2 = GetImageBuffer(&dest_img);

	// 初始化波幅数组
	memset(this->buf, 0, (pic_height * pic_width + pic_width) * sizeof(short));
	memset(this->buf2, 0, (pic_height * pic_width + pic_width) * sizeof(short));
}

// 计算下一个时刻所有点的波幅
void Model::nextFrame(const int damp) {
	for (int i = pic_width; i < pic_height * (pic_width - 1); i++) {
		// 计算下一时刻所有点的波幅
		this->buf2[i] = ((this->buf[i - pic_width] + this->buf[i + pic_width] + this->buf[i - 1] + this->buf[i + 1]) >> 1) - this->buf2[i];

		// 波能衰减
		this->buf2[i] -= this->buf2[i] >> damp; // 设置波幅的衰减速度
	}

	short* ptmp = this->buf;
	this->buf = this->buf2;
	this->buf2 = ptmp;
}

// 处理当前时刻波幅影响之后的位图
void Model::renderRipple() {
	int i = 0;
	for (int y = 0; y < pic_height; y++) {
		for (int x = 0; x < pic_width; x++) {
			short data = 1024 - this->buf[i];

			// 偏移
			int a = ((x - pic_width / 2) * data / 1024) + pic_width / 2;
			int b = ((y - pic_height / 2) * data / 1024) + pic_height / 2;

			// 边界处理
			if (a >= pic_width)		a = pic_width - 1;
			if (a < 0)				a = 0;
			if (b >= pic_height)	b = pic_height - 1;
			if (b < 0)				b = 0;

			// 处理偏移
			this->img_ptr2[i] = this->img_ptr1[a + (b * pic_width)];
			i++;
		}
	}
}

// 鼠标模拟投石头
void Model::disturb(const int x, const int y, const int stonesize, const int stoneweight) {
	// 突破边界不处理
	if ((x >= pic_width - stonesize) ||
		(x < stonesize) ||
		(y >= pic_height - stonesize) ||
		(y < stonesize))
		return;

	for (int posx = x - stonesize; posx < x + stonesize; posx++) {
		for (int posy = y - stonesize; posy < y + stonesize; posy++) {
			// 对以stonesize为半径的圆内的所有点进行波幅的设定
			if ((posx - x) * (posx - x) + (posy - y) * (posy - y) < stonesize * stonesize) {
				// 根据stoneweight的大小设置石头入水后初始的波幅
				this->buf[pic_width * posy + posx] += stoneweight;
			}
		}
	}
}

// 计算帧率FPS
float Model::getFps() {
	// 静态本地变量：具有全局的生存期和函数内部局部的作用域
	static int i = 0;
	static int oldTime = GetTickCount64();
	static float fps;

	if (i > fps_count) {
		i = 0;
		int newTime = GetTickCount64();
		int elapsedTime = newTime - oldTime;
		fps = fps_count / (elapsedTime / 1000.0f);
		oldTime = newTime;
	}
	i++;
	return fps;
}

// 画面渲染
void Model::renderFun(IMAGE& dest_img) {
	this->renderRipple();
	putimage(0, -135, &dest_img);

	settextcolor(RGB(210, 210, 210));
	settextstyle(25, 0, "楷体", 0, 0, 800, 0, 0, 0);
	outtextxy(15, 15, "律动水波");

	settextcolor(RGB(210, 210, 210));
	settextstyle(20, 0, "楷体", 0, 0, 1000, 0, 0, 0);
	outtextxy(15, 50, "作者：初云");

	TCHAR s[20]; // 定义数组，用于以字符形式存放FPS帧率
	_stprintf(s, "FPS: %.1f", getFps());
	settextcolor(RGB(210, 210, 210));
	settextstyle(20, 0, "Times New Roman", 0, 0, 800, 0, 0, 0);
	outtextxy(28, 80, s);
}

// 画面逻辑
void Model::frameFun() {
	if (MouseHit()) {
		// 获取鼠标消息
		MOUSEMSG msg = GetMouseMsg();
		if (msg.uMsg == WM_MOUSEMOVE) {
			// 调整石头入水后初始波幅范围（石头大小），以及初始波幅大小（石头重量）
			this->disturb(msg.x, msg.y + 135, 3, 256);
			this->flag = 1;
		}
		else if (msg.uMsg == WM_LBUTTONDOWN) {
			// 调整石头入水后初始波幅范围（石头大小），以及初始波幅大小（石头重量）
			disturb(msg.x, msg.y + 135, 3, 2560 / 5 * 4);
			flag = 2;
		}
		else if (msg.uMsg == WM_RBUTTONDOWN) {
			// 调整石头入水后初始波幅范围（石头大小），以及初始波幅大小（石头重量）
			disturb(msg.x, msg.y + 135, 100, 256);
			flag = 3;
		}
		else if (msg.uMsg == WM_MOUSEWHEEL) {
			for (int i = 0; i < width; i += 50) {
				for (int j = 0; j < height; j += 50) {
					// 调整石头入水后初始波幅范围（石头大小），以及初始波幅大小（石头重量）
					disturb(i, j + 135, 3, 256);
				}
			}
			this->flag = 4;
		}
		FlushMouseMsgBuffer(); // 清空鼠标消息缓冲区
	}

	switch (flag) {
	case 1:
		nextFrame(5);
		break;
	case 2:
		nextFrame(10);
		break;
	case 3:
		nextFrame(5);
		break;
	case 4:
		nextFrame(5);
		break;
	default:
		nextFrame(5);
		break;
	}
}