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

// ������Դ
void Model::load(IMAGE& src_img) {
	loadimage(&src_img, "./resource/picture/bk.jpg", pic_width, pic_height);
}

// ��������
void Model::bgm() {
	mciSendString("open ./resource/music/bk.mp3 alias BGM", nullptr, 0, nullptr);
	mciSendString("play BGM repeat", nullptr, 0, nullptr);
}

// ��ʼ������
void Model::init(IMAGE& src_img, IMAGE& dest_img) {
	setbkmode(TRANSPARENT);
	SetWindowText(GetHWnd(), "C++�ɶ�����");

	// ��û�ͼ�豸���Դ�ָ��
	this->img_ptr1 = GetImageBuffer(&src_img);
	this->img_ptr2 = GetImageBuffer(&dest_img);

	// ��ʼ����������
	memset(this->buf, 0, (pic_height * pic_width + pic_width) * sizeof(short));
	memset(this->buf2, 0, (pic_height * pic_width + pic_width) * sizeof(short));
}

// ������һ��ʱ�����е�Ĳ���
void Model::nextFrame(const int damp) {
	for (int i = pic_width; i < pic_height * (pic_width - 1); i++) {
		// ������һʱ�����е�Ĳ���
		this->buf2[i] = ((this->buf[i - pic_width] + this->buf[i + pic_width] + this->buf[i - 1] + this->buf[i + 1]) >> 1) - this->buf2[i];

		// ����˥��
		this->buf2[i] -= this->buf2[i] >> damp; // ���ò�����˥���ٶ�
	}

	short* ptmp = this->buf;
	this->buf = this->buf2;
	this->buf2 = ptmp;
}

// ����ǰʱ�̲���Ӱ��֮���λͼ
void Model::renderRipple() {
	int i = 0;
	for (int y = 0; y < pic_height; y++) {
		for (int x = 0; x < pic_width; x++) {
			short data = 1024 - this->buf[i];

			// ƫ��
			int a = ((x - pic_width / 2) * data / 1024) + pic_width / 2;
			int b = ((y - pic_height / 2) * data / 1024) + pic_height / 2;

			// �߽紦��
			if (a >= pic_width)		a = pic_width - 1;
			if (a < 0)				a = 0;
			if (b >= pic_height)	b = pic_height - 1;
			if (b < 0)				b = 0;

			// ����ƫ��
			this->img_ptr2[i] = this->img_ptr1[a + (b * pic_width)];
			i++;
		}
	}
}

// ���ģ��Ͷʯͷ
void Model::disturb(const int x, const int y, const int stonesize, const int stoneweight) {
	// ͻ�Ʊ߽粻����
	if ((x >= pic_width - stonesize) ||
		(x < stonesize) ||
		(y >= pic_height - stonesize) ||
		(y < stonesize))
		return;

	for (int posx = x - stonesize; posx < x + stonesize; posx++) {
		for (int posy = y - stonesize; posy < y + stonesize; posy++) {
			// ����stonesizeΪ�뾶��Բ�ڵ����е���в������趨
			if ((posx - x) * (posx - x) + (posy - y) * (posy - y) < stonesize * stonesize) {
				// ����stoneweight�Ĵ�С����ʯͷ��ˮ���ʼ�Ĳ���
				this->buf[pic_width * posy + posx] += stoneweight;
			}
		}
	}
}

// ����֡��FPS
float Model::getFps() {
	// ��̬���ر���������ȫ�ֵ������ںͺ����ڲ��ֲ���������
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

// ������Ⱦ
void Model::renderFun(IMAGE& dest_img) {
	this->renderRipple();
	putimage(0, -135, &dest_img);

	settextcolor(RGB(210, 210, 210));
	settextstyle(25, 0, "����", 0, 0, 800, 0, 0, 0);
	outtextxy(15, 15, "�ɶ�ˮ��");

	settextcolor(RGB(210, 210, 210));
	settextstyle(20, 0, "����", 0, 0, 1000, 0, 0, 0);
	outtextxy(15, 50, "���ߣ�����");

	TCHAR s[20]; // �������飬�������ַ���ʽ���FPS֡��
	_stprintf(s, "FPS: %.1f", getFps());
	settextcolor(RGB(210, 210, 210));
	settextstyle(20, 0, "Times New Roman", 0, 0, 800, 0, 0, 0);
	outtextxy(28, 80, s);
}

// �����߼�
void Model::frameFun() {
	if (MouseHit()) {
		// ��ȡ�����Ϣ
		MOUSEMSG msg = GetMouseMsg();
		if (msg.uMsg == WM_MOUSEMOVE) {
			// ����ʯͷ��ˮ���ʼ������Χ��ʯͷ��С�����Լ���ʼ������С��ʯͷ������
			this->disturb(msg.x, msg.y + 135, 3, 256);
			this->flag = 1;
		}
		else if (msg.uMsg == WM_LBUTTONDOWN) {
			// ����ʯͷ��ˮ���ʼ������Χ��ʯͷ��С�����Լ���ʼ������С��ʯͷ������
			disturb(msg.x, msg.y + 135, 3, 2560 / 5 * 4);
			flag = 2;
		}
		else if (msg.uMsg == WM_RBUTTONDOWN) {
			// ����ʯͷ��ˮ���ʼ������Χ��ʯͷ��С�����Լ���ʼ������С��ʯͷ������
			disturb(msg.x, msg.y + 135, 100, 256);
			flag = 3;
		}
		else if (msg.uMsg == WM_MOUSEWHEEL) {
			for (int i = 0; i < width; i += 50) {
				for (int j = 0; j < height; j += 50) {
					// ����ʯͷ��ˮ���ʼ������Χ��ʯͷ��С�����Լ���ʼ������С��ʯͷ������
					disturb(i, j + 135, 3, 256);
				}
			}
			this->flag = 4;
		}
		FlushMouseMsgBuffer(); // ��������Ϣ������
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