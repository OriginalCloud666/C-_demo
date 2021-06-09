#include "Model.h"
using namespace std;

Model::Model() : offsetAngle(pi / 6), shortenRate(0.65), isShowAnimation(1) {}

//��������
void Model::bgm() {
	mciSendString("open ./resource/music/bk.mp3 alias BGM", nullptr, 0, nullptr);
	mciSendString("play BGM repeat", nullptr, 0, nullptr);
}

// ��[inputMin,inputMax]��Χ��input������ӳ��Ϊ[outputMin,outputMax]��Χ��output����
double Model::mapValue(double input, double inputMin, double inputMax, double outputMin, double outputMax) {
	double output;
	if (abs(input - inputMin) < 0.000001) { // ��ֹ���ֳ���0��bug����
		output = outputMin;
	}
	else {
		output = (input - inputMin) * (outputMax - outputMin) / (inputMax - inputMin) + outputMin;
	}
	return output;
}

// ����[min,max]֮������С��
double Model::randBetween(double min, double max) {
	double t = rand() / double(RAND_MAX); // ����[0,1]�����С��
	// ����mapValue��������ֵ�ķ�Χ��[0,1]ӳ�䵽[min,max]
	double r = this->mapValue(t, 0, 1, min, max);
	return r;
}

// ֦�����ɺͻ��Ƶݹ麯��
// ���������֦����ʼx��y���֦꣬�ɳ��ȣ�֦�ɽǶȣ�֦�ɻ�ͼ������ȣ��ڼ���
void Model::brunch(double x_start, double y_start, double length, double angle, double thickness, int generation) {
	// �������Ǻ��������ǰ֦�ɵ��յ�x��y����
	double x_end, y_end;
	x_end = x_start + length * cos(angle);
	y_end = y_start + length * sin(angle);

	// ������֦��
	setlinestyle(PS_SOLID, thickness); // ���õ�ǰ֦���߿�
	// ����֦��Ϊ�Һ�ɫ����������ڣ���֦���𽥱���
	COLORREF color = HSVtoRGB(15, 0.75, 0.4 + generation * 0.05);
	setlinecolor(color);//�趨��ǰ֦����ɫ

	line(x_start, y_start, x_end, y_end); // ������ǰ֦��(����)

	// �����֦�ɵĴ���
	int childGeneration = generation + 1;
	// �������ң��м�������֦�ɵĳ��ȣ��𽥱�̣�����һ�������
	double childLength = shortenRate * length;
	double leftChildLength = childLength * this->randBetween(0.9, 1.1);
	double rightChildLength = childLength * this->randBetween(0.9, 1.1);
	double centerChildLength = childLength * this->randBetween(0.8, 1.1);

	// ����֦�ɳ��ȴ���2�����Ҵ���С�ڵ���10ʱ���ݹ������֦��
	if (childLength >= 2 && childGeneration <= 9) {
		// ������֦�ɵĴ�ϸ���𽥱�ϸ
		double childThickness = thickness * 0.8;
		if (childThickness < 2) { // ֦�ɻ�ͼ��ϸ���߿�Ϊ2
			childThickness = 2;
		}
		// һ�����ʲ������ң��м���֦��
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
	else { // ��ĩ�˻���ӣ������һ����ɫ���Բ
		setlinestyle(PS_SOLID, 1); // �����߿�
		// ӣ����ɫHSVtoRGB(325, 0.3, 1),��һ�������
		COLORREF color = HSVtoRGB(this->randBetween(300, 350), this->randBetween(0.2, 0.3), 1);
		setlinecolor(color); // ����������ɫ
		setfillcolor(color); // ���������ɫ
		if (childLength <= 4) { // �����֦�ɳ���С�ڵ���4
			fillcircle(x_end, y_end, 2); // Բ�İ뾶Ϊ2(��С�Ϳ�������)
		}
		else {
			fillcircle(x_end, y_end, childLength / 2); // ��һ��Բ���뾶Ϊ��֦�ɳ��ȵ�һ��
		}
	}

	if (isShowAnimation == 1) { // ���isShowAnimationΪ1�������ӣ�������ɵĹ��̶���
		FlushBatchDraw(); // ��������
		Sleep(2);
	}
}

// ��ʼ������
void Model::init() {
	// ���ñ�����ɫ
	setbkcolor(RGB(244, 215, 215));
	SetWindowText(GetHWnd(), "C++ӣ����");
	cleardevice();

	// �������������
	srand((unsigned)time(nullptr));

	// �ݹ麯������
	this->brunch(width / 2, height, 0.45 * height * this->shortenRate, -pi / 2, 15 * this->shortenRate, 1);
}

// ������
void Model::mouseControl() {
	// �ж��Ƿ��������Ϣ
	if (MouseHit()) {
		MOUSEMSG msg = GetMouseMsg();
		// �ж��û����
		if (msg.uMsg == WM_MOUSEMOVE) { // ������ƶ�ʱ���趨�ݹ麯���Ĳ���
			// �������ң�������֦��ƫ�븸֦�ɵĽǶ��𽥱��
			this->offsetAngle = this->mapValue(msg.x, 0, width, pi / 10, pi / 4);
			// �����ϵ��£���֦�ɱȸ�֦�ɵĳ������̵ĸ���
			this->shortenRate = this->mapValue(msg.y, 0, height, 0.7, 0.3);
		}
		if (msg.uMsg == WM_LBUTTONDOWN) { // �����������ʱ���Ե�ǰ������ʼ����һ����
			cleardevice();
			this->brunch(width / 2, height, 0.45 * height * this->shortenRate, -pi / 2, 15 * this->shortenRate, 1);
			FlushBatchDraw();
		}
		if (msg.uMsg == WM_RBUTTONDOWN) { // ������Ҽ����ʱ���л��Ƿ���ʾ���̶���
			if (this->isShowAnimation == 1) {
				this->isShowAnimation = 0;
			}
			else if (this->isShowAnimation == 0) {
				this->isShowAnimation = 1;
			}
		}
	}
}