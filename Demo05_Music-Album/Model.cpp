#include "Model.h"
using namespace std;

Model::Model() :count(0) {}

// ��ȡcount
int& Model::getCount() {
	return this->count;
}

// ������Դ
void Model::load(IMAGE& bk1, IMAGE& bk2, IMAGE& w1, vector<IMAGE>& img, vector<int>& imgIndex, vector<int>& pic_width, vector<int>& pic_height) {
	loadimage(&w1, "./resource/picture/w1.png");
	loadimage(&bk1, "./resource/picture/bk1.jpg", width, height * 1.254);
	loadimage(&bk2, "./resource/picture/bk2.png", width, height * 1.254);

	// ����������Ƭ�ļ�
	for (int i = 0; i < pic_num; i++) {
		char fileName[25]; // �����ַ����飬������Ŵ�����չ����ͼƬ����
		sprintf(fileName, "./resource/picture/%d.jpg", imgIndex[i]);
		loadimage(&img[i], fileName, pic_width[i], pic_height[i]);
	}
}

// ���ر�������
void Model::bgm() {
	mciSendString("open ./resource/music/bk.mp3 alias BGM", nullptr, 0, nullptr);
	mciSendString("play BGM repeat", nullptr, 0, nullptr);
}

// ����PNGͼ��ȥ͸�����֡�������ͼ���Ȼ�ͼ���ڴ�
void Model::drawAlphabk(IMAGE* picture, const int  picture_x, const int picture_y) {
	// ������ʼ��
	DWORD* dst = GetImageBuffer(); // GetImageBuffer()���������ڻ�ȡ��ͼ�豸���Դ�ָ�룬EASYX�Դ�
	DWORD* draw = GetImageBuffer();
	DWORD* src = GetImageBuffer(picture) + 22 * width; // ��ȡpicture���Դ�ָ��
	int picture_width = picture->getwidth(); // ��ȡpicture�Ŀ�ȣ�EASYX�Դ�
	int picture_height = picture->getheight(); // ��ȡpicture�ĸ߶ȣ�EASYX�Դ�
	int graphWidth = getwidth(); // ��ȡ��ͼ���Ŀ�ȣ�EASYX�Դ�
	int graphHeight = getheight(); // ��ȡ��ͼ���ĸ߶ȣ�EASYX�Դ�
	int dstX = 0; // ���Դ������صĽǱ�

	// ʵ��͸����ͼ ��ʽ��Cp=��p*FP+(1-��p)*BP����Ҷ˹���������е���ɫ�ĸ��ʼ���
	for (int iy = 0; iy < graphHeight; iy++) {
		for (int ix = 0; ix < graphWidth; ix++) {
			int srcX = ix + iy * graphWidth; // ���Դ������صĽǱ�
			int sa = ((src[srcX] & 0xff000000) >> 24); // 0xAArrggbb;AA��͸����
			int sr = ((src[srcX] & 0xff0000) >> 16); // ��ȡRGB���R
			int sg = ((src[srcX] & 0xff00) >> 8); // ��ȡRGB���G
			int sb = src[srcX] & 0xff; // ��ȡRGB���B
			if (ix >= 0 && ix <= graphWidth && iy >= 0 && iy <= graphHeight && dstX <= graphWidth * graphHeight) {
				dstX = (ix + picture_x) + (iy + picture_y) * graphWidth; // ���Դ������صĽǱ�
				int dr = ((dst[dstX] & 0xff0000) >> 16);
				int dg = ((dst[dstX] & 0xff00) >> 8);
				int db = dst[dstX] & 0xff;
				draw[dstX] = ((sr * sa / 255 + dr * (255 - sa) / 255) << 16) // ��ʽ�� Cp=��p*FP+(1-��p)*BP����p=sa/255 , FP=sr , BP=dr
					| ((sg * sa / 255 + dg * (255 - sa) / 255) << 8) // ��p=sa/255 , FP=sg , BP=dg
					| (sb * sa / 255 + db * (255 - sa) / 255); // ��p=sa/255 , FP=sb , BP=db
			}
		}
	}
}

// ����PNGͼ��ȥ͸�����֡���һ��ͼƬ���Ȼ�ͼ����С��
void Model::drawAlpha(IMAGE* picture, const int  picture_x, const int picture_y) {
	// ������ʼ��
	DWORD* dst = GetImageBuffer(); // GetImageBuffer()���������ڻ�ȡ��ͼ�豸���Դ�ָ�룬EASYX�Դ�
	DWORD* draw = GetImageBuffer();
	DWORD* src = GetImageBuffer(picture); // ��ȡpicture���Դ�ָ��
	int picture_width = picture->getwidth(); // ��ȡpicture�Ŀ�ȣ�EASYX�Դ�
	int picture_height = picture->getheight(); // ��ȡpicture�ĸ߶ȣ�EASYX�Դ�
	int graphWidth = getwidth(); // ��ȡ��ͼ���Ŀ�ȣ�EASYX�Դ�
	int graphHeight = getheight(); // ��ȡ��ͼ���ĸ߶ȣ�EASYX�Դ�
	int dstX = 0; // ���Դ������صĽǱ�

	// ʵ��͸����ͼ ��ʽ��Cp=��p*FP+(1-��p)*BP����Ҷ˹���������е���ɫ�ĸ��ʼ���
	for (int iy = 0; iy < picture_height; iy++) {
		for (int ix = 0; ix < picture_width; ix++) {
			int srcX = ix + iy * picture_width; // ���Դ������صĽǱ�
			int sa = ((src[srcX] & 0xff000000) >> 24); // 0xAArrggbb;AA��͸����
			int sr = ((src[srcX] & 0xff0000) >> 16); // ��ȡRGB���R
			int sg = ((src[srcX] & 0xff00) >> 8); // ��ȡRGB���G
			int sb = src[srcX] & 0xff; // ��ȡRGB���B
			if (ix >= 0 && ix <= graphWidth && iy >= 0 && iy <= graphHeight && dstX <= graphWidth * graphHeight) {
				dstX = (ix + picture_x) + (iy + picture_y) * graphWidth; // ���Դ������صĽǱ�
				int dr = ((dst[dstX] & 0xff0000) >> 16);
				int dg = ((dst[dstX] & 0xff00) >> 8);
				int db = dst[dstX] & 0xff;
				draw[dstX] = ((sr * sa / 255 + dr * (255 - sa) / 255) << 16) //��ʽ�� Cp=��p*FP+(1-��p)*BP����p=sa/255 , FP=sr , BP=dr
					| ((sg * sa / 255 + dg * (255 - sa) / 255) << 8) //��p=sa/255 , FP=sg , BP=dg
					| (sb * sa / 255 + db * (255 - sa) / 255); //��p=sa/255 , FP=sb , BP=db
			}
		}
	}
}

// ���ƿ�ʼ����
void Model::welcome(IMAGE& bk1, IMAGE& w1) {
	cleardevice();
	putimage(0, -22, &bk1);
	setbkmode(TRANSPARENT);
	SetWindowText(GetHWnd(), "C++�������");

	// ���������Ϣ
	settextcolor(WHITE);
	settextstyle(28, 0, "����", 0, 0, 1000, 0, 0, 0);
	outtextxy(15, 10, "�������");
	settextcolor(WHITE);
	settextstyle(23, 0, "����", 0, 0, 1000, 0, 0, 0);
	outtextxy(10, 50, "���ߣ�����");
	drawAlpha(&w1, 10, 285);

	// �����������
	while (!_kbhit()) {
	}
}

// �������α߿�
void Model::drawBorder(const int x0, const int y0, const int size, COLORREF C, const int borderwidth) {
	double m, n;
	double x, y;
	for (double i = 0; i <= 6.29; i = i + 0.01) {
		// �����������
		m = i;
		n = -size * (((sin(i) * sqrt(fabs(cos(i)))) / (sin(i) + 1.4142)) - 2 * sin(i) + 2);
		// ת��Ϊ�ѿ�������
		x = n * cos(m) + x0;
		y = n * sin(m) + y0;
		// �������α߿�
		setfillcolor(C);
		solidcircle(x, y, borderwidth);
	}
}

// �ڻ������α߿򴦻����ֶ�
void Model::drawText(const double i, const int x0, const int y0, const int size, const char* s) {
	double m, n;
	double x, y;

	// �����������
	m = i;
	n = -size * (((sin(i) * sqrt(fabs(cos(i)))) / (sin(i) + 1.4142)) - 2 * sin(i) + 2);
	// ת��Ϊ�ѿ�������
	x = n * cos(m) + x0;
	y = n * sin(m) + y0;

	outtextxy(x - 11, y - 11, s); // ȷ���������е��ڸ�����·�ƶ�
}

// ��������ͼ��
void Model::drawHeart(IMAGE& bk2, vector<IMAGE>& img, vector<int>& pic_x, vector<int>& pic_y, const int x, const int y, const int size, COLORREF C, const char s[][5], const double move, const double interval, const int time) {
	settextcolor(C);
	settextstyle(22, 0, "����", 0, 0, 1000, 0, 0, 0);

	for (double i = 0 + move; i <= 6.29 + move; i = i + 0.01) {
		cleardevice(); // ���ֳ���֮������
		putimage(pic_x[this->count % pic_num], pic_y[this->count % pic_num], &img[this->count % pic_num]);
		drawAlphabk(&bk2, 0, 0);

		// ���������Ϣ
		settextcolor(WHITE);
		settextstyle(28, 0, "����", 0, 0, 1000, 0, 0, 0);
		outtextxy(15, 10, "�������");
		settextcolor(WHITE);
		settextstyle(23, 0, "����", 0, 0, 1000, 0, 0, 0);
		outtextxy(10, 50, "���ߣ�����");

		// ���ռ��ʱ�䣬������������
		this->drawText(i, x, y, size, s[0]);
		this->drawText(i + interval, x, y, size, s[1]);
		this->drawText(i + interval * 2, x, y, size, s[2]);
		this->drawText(i + interval * 3, x, y, size, s[3]);
		this->drawText(i + interval * 4, x, y, size, s[4]);
		this->drawText(i + interval * 5, x, y, size, s[5]);
		this->drawText(i + interval * 6, x, y, size, s[6]);
		FlushBatchDraw();

		// ��ʱ����λ�Ǻ��룩
		Sleep(time);
	}
}