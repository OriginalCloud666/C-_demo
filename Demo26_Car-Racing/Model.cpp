#include "Model.h"
using namespace std;

Model::Model() {
	this->beginGame = creatButton(2 * pic_size - 95, 500, 150, 50, "START", 38, 12, RGB(252, 252, 252), RGB(110, 110, 110));
	this->endGame = creatButton(6 * pic_size - 55, 500, 150, 50, "END", 52, 12, RGB(252, 252, 252), RGB(110, 110, 110));
}

Model::~Model() {
	delete this->beginGame;
	this->beginGame = nullptr;
	delete this->endGame;
	this->endGame = nullptr;
}

// ��ȡbeginGame
struct Button*& Model::getButton1() {
	return this->beginGame;
}

// ��ȡendGame
struct Button*& Model::getButton2() {
	return this->endGame;
}

// ������Դ
void Model::load(IMAGE& bk, IMAGE& w1, IMAGE& w2, IMAGE& track, vector<IMAGE>& img, vector<int>& imgIndex) {
	// ��������ͼƬ
	for (int i = 0; i < pic_num; i++) {
		char fileName[25];
		sprintf(fileName, "./resource/picture/%d.png", imgIndex[i]);
		loadimage(&img[i], fileName);
	}
	// ���ر���ͼƬ
	loadimage(&bk, "./resource/picture/bk.png", pic_size, pic_size);
	// ��������ͼƬ
	loadimage(&track, "./resource/picture/track.png", pic_size, pic_size);
	// ��������ͼƬ
	loadimage(&w1, "./resource/picture/w1.png");
	loadimage(&w2, "./resource/picture/w2.png");
}

// ��������
void Model::bgm() {
	mciSendString("open ./resource/music/bk.mp3 alias BGM", nullptr, 0, nullptr);
	mciSendString("play BGM repeat", nullptr, 0, nullptr);
}

// ������֮��ľ���
double Model::distance(const double x1, const double y1, const double x2, const double y2) {
	// distance=������(x2-x1)^2+(y2-y1)^2
	return sqrt((x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2));
}

// ����PNGͼ��ȥ͸������
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
				draw[dstX] = ((sr * sa / 255 + dr * (255 - sa) / 255) << 16) // ��ʽ��Cp=��p*FP+(1-��p)*BP����p=sa/255 , FP=sr , BP=dr
					| ((sg * sa / 255 + dg * (255 - sa) / 255) << 8) // ��p=sa/255 , FP=sg , BP=dg
					| (sb * sa / 255 + db * (255 - sa) / 255); // ��p=sa/255 , FP=sb , BP=db
			}
		}
	}
}

// ������ť
struct Button* Model::creatButton(const int x, const int y, const int width, const int height,
	const char* text, const int dx, const int dy, COLORREF buttoncolor, COLORREF textcolor) {
	struct Button* pButton = new struct Button;
	pButton->x = x;
	pButton->y = y;
	pButton->width = width;
	pButton->height = height;
	pButton->text = (char*)malloc(strlen(text) + 1);
	strcpy(pButton->text, text);
	pButton->dx = dx;
	pButton->dy = dy;
	pButton->buttoncolor = buttoncolor;
	pButton->textcolor = textcolor;
	return pButton;
}

// ���ư�ť
void Model::drawButton(struct Button* pButton) {
	setbkmode(TRANSPARENT);
	setlinecolor(RGB(60, 60, 60));
	setlinestyle(PS_SOLID, 5);
	setfillcolor(pButton->buttoncolor);
	fillrectangle(pButton->x, pButton->y, pButton->x + pButton->width, pButton->y + pButton->height);
	settextcolor(pButton->textcolor);
	settextstyle(28, 0, "����", 0, 0, 1000, 0, 0, 0);
	outtextxy(pButton->x + pButton->dx, pButton->y + pButton->dy, pButton->text);
}

// ��ʼ����Ϸ����
void Model::init() {
	// ��ʼ�����������
	srand((unsigned)time(nullptr) * rand());

	// ��ʼ������
	for (int i = 0; i < line_num; i++) {
		struct Line newline;
		newline.FL_x = pic_size * 3.0;
		newline.SL_x = pic_size * 5.0;
		newline.FL_y = pic_size * 1.5 * i - pic_size;
		newline.SL_y = pic_size * 1.5 * i - pic_size;
		newline.width = 5;
		newline.height = 30;
		newline.step = 0.8;
		this->lines.push_back(newline);
	}

	// ��ʼ����ɫ
	this->role.x = pic_size * 2.0 + 20.0;
	this->role.y = height - 80.0;
	this->role.step = 0.8;

	// ��ʼ������
	this->cars = {
		{ pic_size * 3.0 + 20.0, 100, 0.8, 1, 2 },
		{ pic_size * 5.0 + 20.0, 500, 0.8, 2, 4 },
		{ pic_size * 4.0 + 20.0, 250, 0.8, 3, 3 },
	};
	for (int i = 0; i < car_num; i++) {
		this->cars[i].step = (rand() % 250) / 1000.0 + 0.20;
	}
}

// ��ʼ�������
void Model::welcome(IMAGE& bk, IMAGE& w1, IMAGE& w2, IMAGE& track, vector<IMAGE>& img) {
	SetWindowText(GetHWnd(), "C++��������");
	cleardevice();
	this->drawLayout(bk, track);
	this->drawLine();
	// ������������
	for (int i = 0; i < car_num; i++) {
		putimage(this->cars[i].x, this->cars[i].y, &img[this->cars[i].kind]);
	}
	this->drawAlpha(&w1, 130, 115);
	this->drawAlpha(&w2, 160, 305);
	this->drawButton(this->beginGame);
	this->drawButton(this->endGame);
}

// ��Ϸ�������
void Model::draw(IMAGE& bk, IMAGE& track, vector<IMAGE>& img) {
	cleardevice();
	this->drawLayout(bk, track);
	this->drawLine();
	// ������������
	for (int i = 0; i < car_num; i++) {
		putimage(this->cars[i].x, this->cars[i].y, &img[this->cars[i].kind]);
	}
	// ���ƽ�ɫ����
	this->drawAlpha(&img[0], this->role.x, this->role.y);
}

// ��������
void Model::drawLayout(IMAGE& bk, IMAGE& track) {
	for (int i = 0; i < row; i++) {
		for (int j = 0; j < col; j++) {
			// �������±�ת��Ϊ����
			int x = pic_size * j;
			int y = pic_size * i;
			// �жϷַ�
			if (j >= 2 && j <= 5) {
				// ����
				putimage(x, y, &track);
			}
			else {
				// ����
				putimage(x, y, &bk);
			}
		}
	}
	// ��ͼ����
	setlinestyle(PS_SOLID, 5);
	setlinecolor(RGB(40, 40, 40));
	line(pic_size * 2, 0, pic_size * 2, height);
	line(pic_size * 6, 0, pic_size * 6, height);
	setlinecolor(RGB(245, 245, 245));
	line(pic_size * 4, 0, pic_size * 4, height);
}

// ��������
void Model::drawLine() {
	for (int i = 0; i < line_num; i++) {
		setlinestyle(PS_SOLID, this->lines[i].width);
		setlinecolor(RGB(245, 245, 245));
		line(this->lines[i].FL_x, this->lines[i].FL_y, this->lines[i].FL_x, this->lines[i].FL_y + this->lines[i].height);
		line(this->lines[i].SL_x, this->lines[i].SL_y, this->lines[i].SL_x, this->lines[i].SL_y + this->lines[i].height);
	}
}

// �߶ε��ƶ�
void Model::moveLine() {
	for (int i = line_num - 1; i >= 0; i--) {
		this->lines[i].FL_y += this->lines[i].step;
		this->lines[i].SL_y += this->lines[i].step;
		if (this->lines[i].FL_y >= height + pic_size / 2.0 || this->lines[i].SL_y >= height + pic_size / 2.0) {
			this->lines[i].FL_y = -pic_size;
			this->lines[i].SL_y = -pic_size;
		}
	}
}

// �������ƶ�
void Model::moveCar() {
	for (int i = 0; i < car_num; i++) {
		this->cars[i].y += this->cars[i].step;
		if (this->cars[i].y >= height) {
			this->cars[i].step = (rand() % 250) / 1000.0 + 0.20;
			this->cars[i].road = rand() % 4 + 1;
			while (this->cars[i].road == this->cars[(i + 1) % 3].road || this->cars[i].road == this->cars[(i + 2) % 3].road) {
				this->cars[i].road = rand() % 4 + 1;
			}
			this->cars[i].x = pic_size * (this->cars[i].road + 1.0) + 20;
			this->cars[i].y = -65;
		}
	}
}

// �ж�����Ƿ��ڰ�ť��
int Model::mouseInButton(struct Button* pButton, MOUSEMSG* msg) {
	if (msg->x >= pButton->x && msg->x <= pButton->x + pButton->width &&
		msg->y >= pButton->y && msg->y <= pButton->y + pButton->height) {
		pButton->buttoncolor = RGB(235, 235, 235);
		pButton->textcolor = RGB(90, 90, 90);
		return 1; // ����ֵΪ1����������ڰ�ť��
	}
	pButton->buttoncolor = RGB(252, 252, 252);
	pButton->textcolor = RGB(110, 110, 110);
	return 0; // ����ֵΪ0��˵�����ڰ�ť��
}

// ������
int Model::mouseControl(struct Button* pButton1, struct Button* pButton2) {
	// �ж��Ƿ��������Ϣ
	if (MouseHit()) {
		MOUSEMSG msg = GetMouseMsg();
		// �ж��û��Ƿ�����ť
		if (this->mouseInButton(pButton1, &msg) == 1 && msg.uMsg == WM_LBUTTONDOWN) {
			mciSendString("close click", nullptr, 0, nullptr);
			mciSendString("open ./resource/music/click.wav alias click", nullptr, 0, nullptr);
			mciSendString("play click", nullptr, 0, nullptr);
			return 1; // ����ֵΪ1��������Ϸ
		}
		else if (this->mouseInButton(pButton2, &msg) == 1 && msg.uMsg == WM_LBUTTONDOWN) {
			mciSendString("close click", nullptr, 0, nullptr);
			mciSendString("open ./resource/music/click.wav alias click", nullptr, 0, nullptr);
			mciSendString("play click", nullptr, 0, nullptr);
			exit(0); // ��Ϸ����
		}
	}
	return 0;
}

// �������ƣ��첽�������ƣ�
void Model::keyDown() {
	// �жϵ�ǰWindowsǰ̨Ӧ�ó��������Ƿ�Ϊ��ǰeasyx����
	if (GetForegroundWindow() == GetHWnd()) {
		if (GetAsyncKeyState(VK_UP) || GetAsyncKeyState('W')) {
			this->role.y -= this->role.step;
			if (this->role.y <= 3) {
				this->role.y = 3;
			}
		}
		if (GetAsyncKeyState(VK_DOWN) || GetAsyncKeyState('S')) {
			this->role.y += this->role.step;
			if (this->role.y >= height - 65.0) {
				this->role.y = height - 65.0;
			}
		}
		if (GetAsyncKeyState(VK_LEFT) || GetAsyncKeyState('A')) {
			this->role.x -= this->role.step;
			if (this->role.x <= pic_size * 2.0 + 3.0) {
				this->role.x = pic_size * 2.0 + 3.0;
			}
		}
		if (GetAsyncKeyState(VK_RIGHT) || GetAsyncKeyState('D')) {
			this->role.x += this->role.step;
			if (this->role.x >= width - 42.0 - pic_size * 2.0) {
				this->role.x = width - 42.0 - pic_size * 2.0;
			}
		}
	}
}

// ��Ϸ����ж�
bool Model::over() {
	// �����ɫ������������ײ������Ϸ����
	for (int i = 0; i < car_num; i++) {
		if (this->distance(0, this->cars[i].y + 31, 0, this->role.y + 31) <= 62 - 2.0 &&
			this->distance(this->cars[i].x + 20, 0, this->role.x + 20, 0) <= 40 - 2.0) {
			return true; // ����ֵΪtrue��˵����Ϸ����
		}
	}
	return false; // ����ֵΪfalse��˵����Ϸδ����
}