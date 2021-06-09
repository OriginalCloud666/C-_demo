#include "Model.h"
using namespace std;

Model::Model() :score(0), error(0), count(0), rate(0.0) {
	this->userkey = { 220, 630, "" };
	this->beginGame = creatButton(80, 465, 150, 50, "START", 38, 12, RGB(252, 252, 252), RGB(150, 150, 150));
	this->endGame = creatButton(420, 465, 150, 50, "END", 52, 12, RGB(252, 252, 252), RGB(150, 150, 150));
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
void Model::load(IMAGE& kk, IMAGE& w1, IMAGE& w2, vector<IMAGE>& img) {
	// ��������ͼƬ
	loadimage(&img[0], "./resource/picture/0.png", 90, 90);
	loadimage(&img[1], "./resource/picture/1.png", 230, 230);
	loadimage(&img[2], "./resource/picture/2.png", 100, 95);
	// ��������ͼƬ
	loadimage(&w1, "./resource/picture/w1.png");
	loadimage(&w2, "./resource/picture/w2.png");
	// �����ַ�ͼƬ
	loadimage(&kk, "./resource/picture/m.png", pic_size, pic_size);
}

// ��������
void Model::bgm() {
	mciSendString("open ./resource/music/bk.mp3 alias BGM", nullptr, 0, nullptr);
	mciSendString("play BGM repeat", nullptr, 0, nullptr);
}

// Timer��ʱ��
int Model::timer(time_t sec, int id) {
	static time_t start[3] = { clock(), clock(), clock() };
	time_t end = clock();
	if (end - start[id] >= sec) {
		start[id] = end;
		return 1;
	}
	return 0;
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
	setlinecolor(RGB(160, 160, 160));
	setlinestyle(PS_SOLID, 5);
	setfillcolor(pButton->buttoncolor);
	fillrectangle(pButton->x, pButton->y, pButton->x + pButton->width, pButton->y + pButton->height);
	settextcolor(pButton->textcolor);
	settextstyle(28, 0, "����", 0, 0, 1000, 0, 0, 0);
	outtextxy(pButton->x + pButton->dx, pButton->y + pButton->dy, pButton->text);
}

// ��ʼ������
void Model::init() {
	// �������������
	srand((unsigned)time(nullptr) * rand());

	this->score = 0;
	this->error = 0;
	this->count = 0;

	//�ַ������ݳ�ʼ��
	for (int i = 0; i < str_num; i++) {
		this->targetCreat(this->target, i);
		this->target[i].y = height - pic_size;
	}
}

// ���ƿ�ʼ����
void Model::welcome(IMAGE& w1, IMAGE& w2, vector<IMAGE>& img) {
	SetWindowText(GetHWnd(), "C++������Ϸ");
	setbkcolor(RGB(240, 240, 240));
	cleardevice();

	this->drawAlpha(&w1, 20, 85);
	this->drawAlpha(&w2, 165, 265);
	this->drawAlpha(&img[0], 85, 280);
	this->drawAlpha(&img[0], 475, 280);
	this->drawAlpha(&img[1], 200, 450);
	this->drawAlpha(&img[2], 427, 85);
	this->drawButton(this->beginGame);
	this->drawButton(this->endGame);
}

// ������Ϸ����
void Model::draw(IMAGE& kk) {
	cleardevice();

	// ������ȷ������ȷ��
	this->rate = this->score * 1.0 / (this->score + this->error);
	if (this->score == 0) {
		this->rate = 0;
	}
	settextcolor(RGB(80, 80, 80));
	settextstyle(30, 0, "����", 0, 0, 1000, 0, 0, 0);
	this->outtextxy_int(210, 210, "��ȷ����%d", this->score);
	this->outtextxy_double(210, 310, "��ȷ�ʣ�%%\t%.2lf", this->rate * 100);

	// �����ַ���
	for (int i = 0; i < str_num; i++) {
		if (this->target[i].flag == true && this->target[i].y >= 0 && this->target[i].y <= height - pic_size) {
			drawAlpha(&kk, this->target[i].x, this->target[i].y);
			settextcolor(RGB(80, 80, 80));
			settextstyle(24, 0, "Times New Roman", 0, 0, 1000, 0, 0, 0);
			outtextxy(this->target[i].dx, this->target[i].dy, this->target[i].str);
		}
	}

	// �����ڱ��ɰ�
	setfillcolor(RGB(240, 240, 240));
	solidrectangle(0, 0, width, pic_size);
	solidrectangle(0, height - pic_size, width, height);

	// ���Ƹ�ʽ����
	setlinecolor(RGB(80, 80, 80));
	setlinestyle(PS_SOLID, 10);
	line(0, pic_size - 5, width, pic_size - 5);
	line(0, height - pic_size + 5, width, height - pic_size + 5);

	// ���ƽ�������
	settextcolor(RGB(80, 80, 80));
	settextstyle(28, 0, "����", 0, 0, 1000, 0, 0, 0);
	outtextxy(60, 30, "����Ա������Ϸ");
	outtextxy(60, 80, "���ֵ��ʾ�ΪC������32���ؼ���");

	// �û��������
	settextcolor(RGB(80, 80, 80));
	settextstyle(36, 0, "Times New Roman", 0, 0, 1000, 0, 0, 0);
	outtextxy(this->userkey.x, this->userkey.y, this->userkey.str);
	outtextxy(this->userkey.x - 100, this->userkey.y, "Input:");
}

// ��ָ��λ���������
void Model::outtextxy_int(int x, int y, const char* format, int num) {
	char str[20] = "";
	sprintf(str, format, num);
	outtextxy(x, y, str);
}

// ��ָ��λ�����������
void Model::outtextxy_double(int x, int y, const  char* format, double num) {
	char str[20] = "";
	sprintf(str, format, num);
	outtextxy(x, y, str);
}

// �ַ�������
void Model::targetCreat(struct Target words[], int n) {
	static char str[32][10] = { "   char", "     int", "    long", "   short", "  signed", "unsigned", "  double", "   float", "   void", "   union",
		"  extern", " register", "    auto", "   const", "   static", "  struct", "  typdef", "     for", "   while", "     do", "      if", "    else", "  switch", " default", "   case",
		"continue", "  return", "   break", "   sizeof", "    goto", "   enum", " volatile" };
	// �������
	words[n].str = str[rand() % 32];
	// �ж��Ƿ������ظ��������������ظ�����������
	while (words[n].str == words[(n + 1) % 5].str || words[n].str == words[(n + 2) % 5].str ||
		words[n].str == words[(n + 3) % 5].str || words[n].str == words[(n + 4) % 5].str) {
		words[n].str = str[rand() % 32];
	}
	words[n].x = rand() % (width - pic_size);
	words[n].y = 0;
	words[n].step = (rand() % 10) / 1000.0 + 0.22;
	words[n].dx = words[n].x + 40;
	words[n].dy = words[n].y + 62;
	words[n].flag = true;
}

// �������������ַ���
void Model::targetMove() {
	for (int i = 0; i < str_num; i++) {
		if (this->target[i].flag == true) {
			this->target[i].y += this->target[i].step;
			if (this->target[i].y >= height - pic_size && this->timer(1800, 0) == 1) {
				this->target[i].flag = false;
				this->targetCreat(this->target, i);
				this->count++;
			}
		}
		this->target[i].dx = this->target[i].x + 40;
		this->target[i].dy = this->target[i].y + 62;
	}
}

// ���ո���ַ����Ƚϴ�С
int Model::mystrcmp(const char* s1, const char* s2) {
	while (*s1 == ' ') {
		s1++;
	}
	while (*s1 == *s2 && *s1 != '\0') {
		s1++;
		s2++;
	}

	int flag = 0;
	if (*s1 - *s2 == 0) {
		flag = 0;
	}
	else if (*s1 - *s2 > 0) {
		flag = 1;
	}
	else if (*s1 - *s2 < 0) {
		flag = -1;
	}
	return flag;
}

// ����������ַ�������
void Model::errorStr() {
	int k = 0;
	for (int i = 0; i < str_num; i++) {
		if (this->target[i].y < height - pic_size) {
			k++;
		}
	}
	this->error = this->count - this->score - k;
}

// �ж�����Ƿ��ڰ�ť��
int Model::mouseInButton(struct Button* pButton, MOUSEMSG* msg) {
	if (msg->x >= pButton->x && msg->x <= pButton->x + pButton->width &&
		msg->y >= pButton->y && msg->y <= pButton->y + pButton->height) {
		pButton->buttoncolor = RGB(235, 235, 235);
		pButton->textcolor = RGB(135, 135, 135);
		return 1; // ����ֵΪ1����������ڰ�ť��
	}
	pButton->buttoncolor = RGB(252, 252, 252);
	pButton->textcolor = RGB(150, 150, 150);
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

// ������Ӧ(ͬ����Ӧ)
void Model::keyDown() {
	// �ж����ް�����Ϣ
	while (_kbhit()) {
		// ���ڽ��հ�����Ϣ
		char userKey = _getch();
		fflush(stdin); // ��ռ�����Ϣ������

		static int n = 0;
		if (userKey != '\r') {
			if (userKey == 0x8 && n > 0) {
				this->userkey.str[--n] = '\0';
			}
			else if (userKey != 0x8) {
				this->userkey.str[n++] = userKey;
			}
		}
		else { // ���һ�����������
		   // �����ַ������ж�
			for (int i = 0; i < str_num; i++) {
				if (this->mystrcmp(target[i].str, userkey.str) == 0) {
					mciSendString("close click", nullptr, 0, nullptr);
					mciSendString("open ./resource/music/click.wav alias click", nullptr, 0, nullptr);
					mciSendString("play click", nullptr, 0, nullptr);
					this->target[i].y = height - pic_size;
					this->score++;
				}
			}
			n = 0;
			memset(this->userkey.str, 0, sizeof(char) * 20);
		}
	}
}

// ��Ϸ�����ж�
bool Model::over() {
	if (this->rate >= 0.8 && this->score >= 1000) {
		return true; // ����ֵΪtrueʱ����Ϸ����
	}
	return false; // ����ֵΪfalseʱ����Ϸδ����
}