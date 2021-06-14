#include "Model.h"
using namespace std;

Model::Model() :needleNum(0), finaltime(0.0) {
	this->beginGame = creatButton(80, 500, 150, 50, "START", 38, 12, RGB(252, 252, 252), RGB(112, 177, 205));
	this->endGame = creatButton(420, 500, 150, 50, "END", 52, 12, RGB(252, 252, 252), RGB(112, 177, 205));
}

Model::~Model() {
	delete this->beginGame;
	this->beginGame = nullptr;
	delete this->endGame;
	this->endGame = nullptr;
}

// ��ȡneedleNum
int& Model::getNeedleNum() {
	return this->needleNum;
}

// ��ȡfinaltime
double& Model::getFinaltime() {
	return this->finaltime;
}

// ��ȡrole1
struct Role& Model::getRole1() {
	return this->role1;
}

// ��ȡrole2
struct Role& Model::getRole2() {
	return this->role2;
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
void Model::load(IMAGE& bk, IMAGE& w1, IMAGE& w2, vector<IMAGE>& img, vector<int>& imgIndex) {
	//��������ͼƬ
	for (int i = 0; i < pic_num; i++) {
		char fileName[25];
		sprintf(fileName, "./resource/picture/%d.jpg", imgIndex[i]);
		loadimage(&img[i], fileName);
	}
	// ���ر���ͼƬ
	loadimage(&bk, "./resource/picture/bk.jpg", width, height);
	// ��������ͼƬ
	loadimage(&w1, "./resource/picture/w1.png");
	loadimage(&w2, "./resource/picture/w2.png");
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
	setlinecolor(RGB(130, 188, 213));
	setlinestyle(PS_SOLID, 5);
	setfillcolor(pButton->buttoncolor);
	fillrectangle(pButton->x, pButton->y, pButton->x + pButton->width, pButton->y + pButton->height);
	settextcolor(pButton->textcolor);
	settextstyle(28, 0, "����", 0, 0, 1000, 0, 0, 0);
	outtextxy(pButton->x + pButton->dx, pButton->y + pButton->dy, pButton->text);
}

// ��Ϸ���ݳ�ʼ��
void Model::init() {
	this->finaltime = 30;
	this->needleNum = 0;

	// ������ݳ�ʼ��
	for (int i = 0; i < needle_num; i++) {
		struct Needle newneedle;
		newneedle.x = -width / 2;
		newneedle.y = -height / 2;
		newneedle.len = 120;
		newneedle.angle = 0;
		newneedle.rotateSpeed = (pi / 360) / 4 * 3;
		newneedle.flag = -1;
		this->needleVec.push_back(newneedle);
	}

	// ������ݳ�ʼ��
	this->role1.x = width / 2 - 62.5 + 3;
	this->role1.y = 0;
	this->role1.ball.x = width / 2;
	this->role1.ball.y = 13;
	this->role1.ball.r = 13;
	this->role1.step = 5;
	this->role1.score = 0;
	this->role1.count = 0;
	this->role2.x = width / 2 - 62.5 + 3;
	this->role2.y = height - 125.0;
	this->role2.ball.x = width / 2;
	this->role2.ball.y = height - 13.0;
	this->role2.ball.r = 13;
	this->role2.step = 5;
	this->role2.score = 0;
	this->role2.count = 0;

	// �в������ݳ�ʼ��
	this->ball.x = width / 2;
	this->ball.y = height / 2;
	this->ball.r = 60;
}

// ��ʼ�������
void Model::welcome(IMAGE& bk, IMAGE& w1, IMAGE& w2) {
	SetWindowText(GetHWnd(), "C++�������");
	setbkcolor(RGB(240, 240, 240));
	cleardevice();

	putimage(0, 0, &bk);
	this->drawAlpha(&w1, 127, 150);
	this->drawAlpha(&w2, 160, 320);
	this->drawButton(beginGame);
	this->drawButton(endGame);
}

// ������Ϸ����
void Model::draw(IMAGE& bk, vector<IMAGE>& img) {
	cleardevice();
	putimage(0, 0, &bk);
	this->drawRole1(img);
	this->drawRole2(img);

	// �������β����Բ��
	settextstyle(20, 0, "����", 0, 0, 1000, 0, 0, 0);
	setfillcolor(RGB(123, 206, 227));

	char countName1[10];
	sprintf(countName1, "%02d", this->role1.count + 1);
	static double x1 = this->role1.ball.x;
	static double y1 = this->role1.ball.y;
	solidcircle(x1, y1, this->role1.ball.r + 10);
	clearcircle(x1, y1, this->role1.ball.r + 5);
	solidcircle(this->role1.ball.x, this->role1.ball.y, this->role1.ball.r);
	outtextxy(x1 - 10, y1 - 9, countName1);

	char countName2[10];
	sprintf(countName2, "%02d", this->role2.count + 1);
	static double x2 = this->role2.ball.x;
	static double y2 = this->role2.ball.y;
	solidcircle(x2, y2, this->role2.ball.r + 10);
	clearcircle(x2, y2, this->role2.ball.r + 5);
	solidcircle(this->role2.ball.x, this->role2.ball.y, this->role2.ball.r);
	outtextxy(x2 - 10, y2 - 9, countName2);

	// ���������Լ��÷�
	settextstyle(22, 0, "����", 0, 0, 1000, 0, ANTIALIASED_QUALITY, 0); // �����
	settextcolor(RGB(46, 110, 168));

	char scoreName1[10];
	sprintf(scoreName1, "%02d", this->role1.score);
	outtextxy(60, 20, "�÷֣�");
	outtextxy(130, 20, scoreName1);
	sprintf(countName1, "%02d", this->role1.count);
	outtextxy(60, 60, "������");
	outtextxy(130, 60, countName1);

	char scoreName2[10];
	sprintf(scoreName2, "%02d", this->role2.score);
	outtextxy(width - 160, height - 90, "�÷֣�");
	outtextxy(width - 90, height - 90, scoreName2);
	sprintf(countName2, "%02d", this->role2.count);
	outtextxy(width - 160, height - 50, "������");
	outtextxy(width - 90, height - 50, countName2);

	// �����м��Բ��
	setfillcolor(RGB(123, 206, 227));
	solidcircle(this->ball.x, this->ball.y, this->ball.r);
	clearcircle(this->ball.x, this->ball.y, this->ball.r - 10);
	solidcircle(this->ball.x, this->ball.y, this->ball.r - 20);

	// �����м�Բ���ڵĵ���ʱ
	settextstyle(22, 0, "����", 0, 0, 1000, 0, 0, 0);
	settextcolor(RGB(245, 245, 245));
	char fileName[10];
	sprintf(fileName, "%05.2f", finaltime);
	outtextxy(this->ball.x - 34, this->ball.y - 22, "����ʱ");
	outtextxy(this->ball.x - 30, this->ball.y + 4, fileName);

	// ������ת��
	for (int i = 0; i < needleNum; i++) {
		setlinecolor(RGB(123, 206, 227));
		setlinestyle(PS_SOLID, 6);
		int x = this->ball.x + cos(this->needleVec[i].angle) * this->ball.r;
		int y = this->ball.y + sin(this->needleVec[i].angle) * this->ball.r;
		line(x, y, this->needleVec[i].x, this->needleVec[i].y);
		solidcircle(this->needleVec[i].x, this->needleVec[i].y, this->role1.ball.r);
	}
}

// ���Ϸ����ͼƬ
void Model::drawRole1(vector<IMAGE>& img) {
	// Դ��ͼ��SRCPAINT��ʽȥ��ͼ
	putimage(this->role1.x, this->role1.y, &img[2], SRCPAINT);
	// ����ͼ��SRCAND��ʽȥ��ͼ
	putimage(this->role1.x, this->role1.y, &img[3], SRCAND);
}

// ���·����ͼƬ
void Model::drawRole2(vector<IMAGE>& img) {
	// Դ��ͼ��SRCPAINT��ʽȥ��ͼ
	putimage(this->role2.x, this->role2.y, &img[0], SRCPAINT);
	// ����ͼ��SRCAND��ʽȥ��ͼ
	putimage(this->role2.x, this->role2.y, &img[1], SRCAND);
}

// �����ת
void Model::rotateNeedle() {
	for (int i = 0; i < needle_num; i++) {
		this->needleVec[i].angle += this->needleVec[i].rotateSpeed;
		//����ĽǶ�����һ������
		if (this->needleVec[i].angle > pi) {
			this->needleVec[i].angle -= 2 * pi;
		}
		this->needleVec[i].x = ball.x + cos(this->needleVec[i].angle) * (ball.r + this->needleVec[i].len);
		this->needleVec[i].y = ball.y + sin(this->needleVec[i].angle) * (ball.r + this->needleVec[i].len);
	}
}

// �ж�����Ƿ��ڰ�ť��
int Model::mouseInButton(struct Button* pButton, MOUSEMSG* msg) {
	if (msg->x >= pButton->x && msg->x <= pButton->x + pButton->width &&
		msg->y >= pButton->y && msg->y <= pButton->y + pButton->height) {
		pButton->buttoncolor = RGB(235, 235, 235);
		pButton->textcolor = RGB(96, 169, 200);
		return 1; // ����ֵΪ1����������ڰ�ť��
	}
	pButton->buttoncolor = RGB(252, 252, 252);
	pButton->textcolor = RGB(112, 177, 205);
	return 0; // ����ֵΪ0��������겻�ڰ�ť��
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

// ������Ӧ��ͬ��������Ӧ��
void Model::keyDown(IMAGE& bk, vector<IMAGE>& img) {
	// �ж��Ƿ��а�����Ϣ
	while (_kbhit()) {
		// ���ڽ����û�������Ϣ
		char userKey = _getch();
		fflush(stdin); // ��ռ�����Ϣ������

		switch (userKey) {
		case ' ':
			mciSendString("close shoot2", nullptr, 0, nullptr);
			mciSendString("open ./resource/music/shoot2.mp3 alias shoot2", nullptr, 0, nullptr);
			mciSendString("play shoot2", nullptr, 0, nullptr);
			// ���β�����������
			while (this->role1.ball.y <= this->ball.y - (this->ball.r + this->needleVec[0].len)) {
				this->role1.ball.y += this->role1.step;
				this->rotateNeedle();
				this->draw(bk, img);
				FlushBatchDraw();
			}
			this->role1.ball.y = 12;
			this->needleNum++;
			this->needleVec[this->needleNum - 1].angle = -pi / 2;
			this->needleVec[this->needleNum - 1].x = this->ball.x + cos(this->needleVec[this->needleNum - 1].angle) * (this->ball.r + this->needleVec[this->needleNum - 1].len);
			this->needleVec[this->needleNum - 1].y = this->ball.y + sin(this->needleVec[this->needleNum - 1].angle) * (this->ball.r + this->needleVec[this->needleNum - 1].len);
			this->role1.count++;
			this->role1.score++;
			for (int i = 0; i < this->needleNum - 1; i++) {
				// �������֮����úܽ�����Ϊ��ײ������ʧ
				if (fabs(this->needleVec[this->needleNum - 1].angle - this->needleVec[i].angle) < pi / 25) {
					mciSendString("close shoot1", nullptr, 0, nullptr);
					mciSendString("open ./resource/music/shoot1.wav alias shoot1", nullptr, 0, nullptr);
					mciSendString("play shoot1", nullptr, 0, nullptr);
					this->needleVec[this->needleNum - 1].x = -width / 2;
					this->needleVec[this->needleNum - 1].y = -height / 2;
					this->needleVec[this->needleNum - 1].angle = 0;
					this->needleNum--;
					this->role1.score--;
					break;
				}
			}
			break;
		case '0':
			mciSendString("close shoot2", nullptr, 0, nullptr);
			mciSendString("open ./resource/music/shoot2.mp3 alias shoot2", nullptr, 0, nullptr);
			mciSendString("play shoot2", nullptr, 0, nullptr);
			// ���β�����������
			while (this->role2.ball.y >= this->ball.y + (this->ball.r + this->needleVec[0].len)) {
				this->role2.ball.y -= this->role2.step;
				this->rotateNeedle();
				this->draw(bk, img);
				FlushBatchDraw();
			}
			this->role2.ball.y = height - 12.0;
			this->needleNum++;
			this->needleVec[this->needleNum - 1].angle = pi / 2;
			this->needleVec[this->needleNum - 1].x = this->ball.x + cos(this->needleVec[this->needleNum - 1].angle) * (this->ball.r + this->needleVec[this->needleNum - 1].len);
			this->needleVec[this->needleNum - 1].y = this->ball.y + sin(this->needleVec[this->needleNum - 1].angle) * (this->ball.r + this->needleVec[this->needleNum - 1].len);
			this->role2.count++;
			this->role2.score++;
			for (int i = 0; i < this->needleNum - 1; i++) {
				// �������֮����úܽ�����Ϊ��ײ������ʧ
				if (fabs(this->needleVec[this->needleNum - 1].angle - this->needleVec[i].angle) < pi / 25) {
					mciSendString("close shoot1", nullptr, 0, nullptr);
					mciSendString("open ./resource/music/shoot1.wav alias shoot1", nullptr, 0, nullptr);
					mciSendString("play shoot1", nullptr, 0, nullptr);
					this->needleVec[this->needleNum - 1].x = -width / 2;
					this->needleVec[this->needleNum - 1].y = -height / 2;
					this->needleVec[this->needleNum - 1].angle = 0;
					this->needleNum--;
					this->role2.score--;
					break;
				}
			}
			break;
		default:
			break;
		}
	}
}

// ��Ϸʤ���ж�
bool Model::over() {
	if (this->finaltime <= 0) {
		this->finaltime = 0;
		return true; // ����1ʱ����Ϸ����
	}
	return false; // ����0ʱ����Ϸδ����
}