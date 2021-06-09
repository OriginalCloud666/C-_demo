#include "Model.h"
using namespace std;

Model::Model() : offsetAngle(pi / 6), shortenRate(0.65), isShowAnimation(1) {}

//加载音乐
void Model::bgm() {
	mciSendString("open ./resource/music/bk.mp3 alias BGM", nullptr, 0, nullptr);
	mciSendString("play BGM repeat", nullptr, 0, nullptr);
}

// 把[inputMin,inputMax]范围的input变量，映射为[outputMin,outputMax]范围的output变量
double Model::mapValue(double input, double inputMin, double inputMax, double outputMin, double outputMax) {
	double output;
	if (abs(input - inputMin) < 0.000001) { // 防止出现除以0的bug出现
		output = outputMin;
	}
	else {
		output = (input - inputMin) * (outputMax - outputMin) / (inputMax - inputMin) + outputMin;
	}
	return output;
}

// 生成[min,max]之间的随机小数
double Model::randBetween(double min, double max) {
	double t = rand() / double(RAND_MAX); // 生成[0,1]的随机小数
	// 调用mapValue函数，把值的范围从[0,1]映射到[min,max]
	double r = this->mapValue(t, 0, 1, min, max);
	return r;
}

// 枝干生成和绘制递归函数
// 输入参数：枝干起始x，y坐标，枝干长度，枝干角度，枝干绘图线条宽度，第几代
void Model::brunch(double x_start, double y_start, double length, double angle, double thickness, int generation) {
	// 利用三角函数求出当前枝干的终点x，y坐标
	double x_end, y_end;
	x_end = x_start + length * cos(angle);
	y_end = y_start + length * sin(angle);

	// 画线条枝干
	setlinestyle(PS_SOLID, thickness); // 设置当前枝干线宽
	// 设置枝干为灰褐色，主树干最黑，子枝干逐渐变亮
	COLORREF color = HSVtoRGB(15, 0.75, 0.4 + generation * 0.05);
	setlinecolor(color);//设定当前枝干颜色

	line(x_start, y_start, x_end, y_end); // 画出当前枝干(画线)

	// 求出子枝干的代数
	int childGeneration = generation + 1;
	// 生成左，右，中间三个子枝干的长度，逐渐变短，并有一定随机性
	double childLength = shortenRate * length;
	double leftChildLength = childLength * this->randBetween(0.9, 1.1);
	double rightChildLength = childLength * this->randBetween(0.9, 1.1);
	double centerChildLength = childLength * this->randBetween(0.8, 1.1);

	// 当子枝干长度大于2，并且代数小于等于10时，递归调用子枝干
	if (childLength >= 2 && childGeneration <= 9) {
		// 生成子枝干的粗细，逐渐变细
		double childThickness = thickness * 0.8;
		if (childThickness < 2) { // 枝干绘图最细的线宽为2
			childThickness = 2;
		}
		// 一定概率产生左，右，中间子枝干
		if (this->randBetween(0, 1) < 0.95) {
			this->brunch(x_end, y_end, leftChildLength, angle + offsetAngle * this->randBetween(0.5, 1), childThickness, childGeneration);
		}
		if (this->randBetween(0, 1) < 0.95) {
			this->brunch(x_end, y_end, rightChildLength, angle - offsetAngle * this->randBetween(0.5, 1), childThickness, childGeneration);
		}
		if (this->randBetween(0, 1) < 0.85) {
			this->brunch(x_end, y_end, centerChildLength, angle + offsetAngle / 5 * this->randBetween(-1, 1), childThickness, childGeneration);
		}
	}
	else { // 最末端绘制樱花，画一个粉色填充圆
		setlinestyle(PS_SOLID, 1); // 设置线宽
		// 樱花粉色HSVtoRGB(325, 0.3, 1),有一定随机性
		COLORREF color = HSVtoRGB(this->randBetween(300, 350), this->randBetween(0.2, 0.3), 1);
		setlinecolor(color); // 设置线条颜色
		setfillcolor(color); // 设置填充颜色
		if (childLength <= 4) { // 如果子枝干长度小于等于4
			fillcircle(x_end, y_end, 2); // 圆的半径为2(再小就看不清了)
		}
		else {
			fillcircle(x_end, y_end, childLength / 2); // 画一个圆，半径为子枝干长度的一半
		}
	}

	if (isShowAnimation == 1) { // 如果isShowAnimation为1，则绘制樱花树生成的过程动画
		FlushBatchDraw(); // 批量绘制
		Sleep(2);
	}
}

// 初始化数据
void Model::init() {
	// 设置背景颜色
	setbkcolor(RGB(244, 215, 215));
	SetWindowText(GetHWnd(), "C++樱花树");
	cleardevice();

	// 设置随机数种子
	srand((unsigned)time(nullptr));

	// 递归函数调用
	this->brunch(width / 2, height, 0.45 * height * this->shortenRate, -pi / 2, 15 * this->shortenRate, 1);
}

// 鼠标控制
void Model::mouseControl() {
	// 判断是否有鼠标消息
	if (MouseHit()) {
		MOUSEMSG msg = GetMouseMsg();
		// 判断用户点击
		if (msg.uMsg == WM_MOUSEMOVE) { // 当鼠标移动时，设定递归函数的参数
			// 鼠标从左到右，左右子枝干偏离父枝干的角度逐渐变大
			this->offsetAngle = this->mapValue(msg.x, 0, width, pi / 10, pi / 4);
			// 鼠标从上到下，子枝干比父枝干的长度缩短的更快
			this->shortenRate = this->mapValue(msg.y, 0, height, 0.7, 0.3);
		}
		if (msg.uMsg == WM_LBUTTONDOWN) { // 当鼠标左键点击时，以当前参数开始绘制一棵树
			cleardevice();
			this->brunch(width / 2, height, 0.45 * height * this->shortenRate, -pi / 2, 15 * this->shortenRate, 1);
			FlushBatchDraw();
		}
		if (msg.uMsg == WM_RBUTTONDOWN) { // 当鼠标右键点击时，切换是否显示过程动画
			if (this->isShowAnimation == 1) {
				this->isShowAnimation = 0;
			}
			else if (this->isShowAnimation == 0) {
				this->isShowAnimation = 1;
			}
		}
	}
}