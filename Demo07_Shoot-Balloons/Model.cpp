#include "Model.h"
using namespace std;

Model::Model() :score(0), speed(0), bullet_num(0) {
	this->bat.cx = 0;
	this->bat.cy = 0;
	this->bat.len = 0;
	this->bat.x = 0;
	this->bat.y = 0;
	this->bat.radian = 0;
}

// ������Դ
void Model::load(IMAGE& bk, IMAGE& w1, IMAGE& w2, IMAGE& w3, IMAGE& w4, const int width, const int height) {
	loadimage(&bk, "./resource/picture/bk.jpg", width, height * 1.58);
	loadimage(&w1, "./resource/picture/w1.png", 850, 142);
	loadimage(&w2, "./resource/picture/w2.png", 500, 83);
	loadimage(&w3, "./resource/picture/w3.png", 630, 136);
	loadimage(&w4, "./resource/picture/w4.png", 500, 83);
}

// ���ñ�������
void Model::bgm() {
	mciSendString("open ./resource/music/bk.mp3 alias BGM", nullptr, 0, nullptr);
	mciSendString("play BGM repeat", nullptr, 0, nullptr);
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

// ��ʼ������
void Model::init(const int bull_num, const int balloon_num, const int width, const int height) {
	// �������������
	srand((unsigned)time(nullptr) * rand());

	// ��ʼ������
	this->score = 0;
	this->speed = 1;
	this->bullet_num = bull_num;

	// ����̨���г�ʼ��
	this->bat.cx = width / 2; // ��̨����λ��x����
	this->bat.cy = height - 10.0; // ��̨����λ��y����
	this->bat.radian = -3.1415926 / 2; // �ڹܽǶ�
	this->bat.len = 90; // �ڹܳ���
	this->bat.x = this->bat.cx; // �ڹ�ĩ��x����
	this->bat.y = this->bat.cy - this->bat.len; // �ڹ�ĩ��y����

	// ���ӵ����г�ʼ��
	for (int i = 0; i < bull_num; i++) {
		struct Bullet newbullet;
		newbullet.x = -width;
		newbullet.y = -height;
		newbullet.step = 1; // �ӵ��ٶ�
		newbullet.flag = false; // ��ʼ�������ӵ�ȫ������
		this->bullVec.push_back(newbullet);
	}

	// ��������г�ʼ��
	for (int i = 0; i < balloon_num; i++) {
		struct Balloon newballoon;
		newballoon.x = rand() % width - 18.0;
		newballoon.y = rand() % height - 28.0;
		newballoon.step = (rand() % 500 / 1000.0) + 0.6; // ����ÿ�ε�λ��
		newballoon.flag = true; // ��ʼ�������������
		newballoon.color = RGB(rand() % 256, rand() % 256, rand() % 256); // ������ɫ
		this->ballVec.push_back(newballoon);
	}
}

// ���ƿ�ʼ����
void Model::welcome(IMAGE& bk, IMAGE& w1, IMAGE& w2, IMAGE& w3, IMAGE& w4) {
	putimage(0, -182, &bk);
	setbkmode(TRANSPARENT);
	SetWindowText(GetHWnd(), "C++��������Ϸ");

	// ���������Ϣ
	settextcolor(RGB(253, 253, 253));
	settextstyle(30, 0, "����", 0, 0, 1000, 0, 0, 0);
	outtextxy(17, 16, "��������Ϸ");

	settextcolor(RGB(253, 253, 253));
	settextstyle(28, 0, "����", 0, 0, 800, 0, 0, 0);
	outtextxy(20, 57, "���ߣ�����");

	this->drawAlpha(&w1, 132, 170);
	this->drawAlpha(&w2, 190, 275);
	this->drawAlpha(&w3, 17, 340);
	this->drawAlpha(&w4, 207, 415);

	while (!_kbhit()) {
	}
}

//��Ϸ�������
void Model::draw(IMAGE& bk, const int bull_num, const int balloon_num, const int height) {
	cleardevice();//����
	putimage(0, -182, &bk);

	// ���������Ϣ
	settextcolor(RGB(253, 253, 253));
	settextstyle(30, 0, "����", 0, 0, 1000, 0, 0, 0);
	outtextxy(17, 16, "��������Ϸ");

	settextcolor(RGB(253, 253, 253));
	settextstyle(28, 0, "����", 0, 0, 800, 0, 0, 0);
	outtextxy(20, 57, "���ߣ�����");

	// ��ʾ��Ϸ�÷�
	settextcolor(RGB(254, 254, 254));
	settextstyle(28, 0, "����", 0, 0, 800, 0, 0, 0);
	char scoreName[20];
	sprintf(scoreName, "��Ϸ�÷֣�%d", this->score);
	outtextxy(36, height - 55, scoreName);

	// ��ʾ�ӵ�����
	settextcolor(RGB(255, 255, 255));
	settextstyle(25, 0, "����", 0, 0, 800, 0, 0, 0);
	char bullName[25];
	sprintf(bullName, "�ӵ�������%02d/%d", this->bullet_num, bull_num);
	outtextxy(412, height - 45, bullName);

	// ��ʾ�ӵ��ٶ��ܼ�
	settextcolor(RGB(255, 255, 255));
	settextstyle(25, 0, "����", 0, 0, 800, 0, 0, 0);
	char speedName[25];
	sprintf(speedName, "�ӵ��ٶ��ܼ���%d", this->speed);
	outtextxy(413, height - 85, speedName);

	// ��������
	for (int i = 0; i < balloon_num; i++) {
		if (this->ballVec[i].flag == true) {
			// ��������ͷ����Բ
			setfillcolor(this->ballVec[i].color);
			solidellipse(this->ballVec[i].x, this->ballVec[i].y, this->ballVec[i].x + 35, this->ballVec[i].y + 55);

			// ��������β��β��
			setlinecolor(WHITE);
			setlinestyle(PS_SOLID, 2);
			arc(this->ballVec[i].x, this->ballVec[i].y + 55, this->ballVec[i].x + 25, this->ballVec[i].y + 55 + 27, 0, 1.2);

			// ���������ڲ��Ļ��ߣ����俴������������
			setlinestyle(PS_SOLID, 3);
			arc(this->ballVec[i].x + 10, this->ballVec[i].y + 10, this->ballVec[i].x + 10 + 15, this->ballVec[i].y + 10 + 37, 0, 1.2);
		}
	}

	// ������̨����
	setlinecolor(RGB(245, 245, 245));
	setlinestyle(PS_DASHDOT, 5);
	circle(this->bat.cx, this->bat.cy, 70);

	// ������̨����
	setfillcolor(RGB(45, 45, 45));
	solidcircle(this->bat.cx, this->bat.cy, 8);

	// �����ڹ�
	setlinecolor(RGB(45, 45, 45));
	setlinestyle(PS_SOLID, 5);
	line(this->bat.cx, this->bat.cy, this->bat.x, this->bat.y);

	// �����ӵ�
	for (int i = 0; i < bull_num; i++) {
		if (this->bullVec[i].flag == true) {
			setfillcolor(this->bullVec[i].color);
			solidcircle(this->bullVec[i].x, this->bullVec[i].y, 5);
		}
	}
}

// �����ӵ�
void Model::creatBullet(const int bull_num) {
	for (int i = 0; i < bull_num; i++) {
		if (this->bullVec[i].flag == false) { // ����ӵ������ڣ������ӵ�
			this->bullVec[i].flag = true;
			this->bullVec[i].x = bat.x; // �����ӵ�x����
			this->bullVec[i].y = bat.y; // �����ӵ�y����
			this->bullVec[i].color = RGB(rand() % 256, rand() % 256, rand() % 256); // �����ӵ���ɫ
			this->bullVec[i].vx = this->bullVec[i].step * cos(bat.radian); // �����ӵ�x����ÿ���ƶ��ľ���
			this->bullVec[i].vy = this->bullVec[i].step * sin(bat.radian); // �����ӵ�y����ÿ���ƶ��ľ���
			this->bullet_num--; // �ӵ�Ϊtrue�����ӵ���������
			break;
		}
	}
}

// �����ӵ�
void Model::speedBullet(const int bull_num) {
	mciSendString("close up", nullptr, 0, nullptr);
	mciSendString("open ./resource/music/up.wav alias up", nullptr, 0, nullptr);
	mciSendString("play up", nullptr, 0, nullptr);
	for (int i = 0; i < bull_num; i++) {
		this->bullVec[i].step += 1;
		if (this->bullVec[i].step > 3) {
			this->bullVec[i].step -= 3;
		}
	}
	this->speed = this->bullVec[0].step; // ȡ��һ���ӵ���ÿ��λ��Ϊ�ٶ��ܼ�
}

// ����ӵ�
void Model::fillBullet(const int bull_num, const int width, const int height) {
	mciSendString("close fill", nullptr, 0, nullptr);
	mciSendString("open ./resource/music/fillbullet.mp3 alias fill", nullptr, 0, nullptr);
	mciSendString("play fill", nullptr, 0, nullptr);
	this->bullet_num = 0;
	for (int i = 0; i < bull_num; i++) {
		// �ж��ӵ��Ƿ񳬳��˱߽�
		if (this->bullVec[i].x<0 || this->bullVec[i].x>width || this->bullVec[i].y<0 || this->bullVec[i].y>height) {
			this->bullVec[i].x = -width; // �ӵ������߽�󣬽��ӵ��鵽��ʼλ��
			this->bullVec[i].y = -height; // �ӵ������߽�󣬽��ӵ��鵽��ʼλ��
			this->bullVec[i].flag = false; // �ӵ������߽���ӵ�����false
		}
		// ���¼����ӵ�����
		if (this->bullVec[i].flag == false) {
			this->bullet_num++; // �ӵ�Ϊfalse�����ӵ���������
		}
	}
}

// �ӵ����ƶ�
void Model::moveBullet(const int bull_num) {
	for (int i = 0; i < bull_num; i++) {
		if (this->bullVec[i].flag == true) { // ���ڵ��ӵ��ǿ����ƶ���
			this->bullVec[i].x += this->bullVec[i].vx;
			this->bullVec[i].y += this->bullVec[i].vy;
		}
	}
}

// ��������
void Model::creatBalloon(const int i, const int width, const int height) {
	this->ballVec[i].x = rand() % width - 18.0; // ��������x���꣨ȷ��x�����ʾ���������е�x���꣩
	this->ballVec[i].y = height; // ��������y���꣨ȷ��y�����ʾ���������е��y���꣩
	this->ballVec[i].step = (rand() % 500) / 1000.0 + 0.6; // ��������ÿ�ε�λ��
	this->ballVec[i].flag = true; // �²�������������true
	this->ballVec[i].color = RGB(rand() % 256, rand() % 256, rand() % 256); // ����������ɫ
}

// ������ײ����
void Model::crashBalloon(const int bull_num, const int balloon_num, const int width, const int height) {
	// ��������
	for (int i = 0; i < balloon_num; i++) {
		if (this->ballVec[i].flag == true) {
			// �����ӵ�
			for (int k = 0; k < bull_num; k++) {
				// �ж������Ƿ�������ײ
				if (this->bullVec[k].x > this->ballVec[i].x && this->bullVec[k].x < this->ballVec[i].x + 35 &&
					this->bullVec[k].y > this->ballVec[i].y && this->bullVec[k].y < this->ballVec[i].y + 55) {
					mciSendString("close boom", nullptr, 0, nullptr);
					mciSendString("open ./resource/music/boom.wav alias boom", nullptr, 0, nullptr);
					mciSendString("play boom", nullptr, 0, nullptr);
					this->bullVec[k].x = -width; // ��ײ֮���ӵ���ʧ�������������������⣩
					this->bullVec[k].y = -height; // ��ײ֮���ӵ���ʧ�������������������⣩
					this->ballVec[i].flag = false; // ��ײ֮��������ʧ
					this->score++; // ����ը�󣬵÷ּ�һ
					this->creatBalloon(i, width, height); // ������ײ��ʧ������µ�����
				}
			}
		}
	}
}

// ������ƶ�
void Model::moveBalloon(const int balloon_num, const int width, const int height) {
	for (int i = 0; i < balloon_num; i++) {
		if (this->ballVec[i].flag == true) {
			this->ballVec[i].y -= this->ballVec[i].step;
			if (this->ballVec[i].y < -55) {
				this->ballVec[i].flag = false; // ����ɳ�����󣬽���������false
				this->creatBalloon(i, width, height);
			}
		}
	}
}

// �����Ϣ��Ӧ
void Model::mouseControl(const int bull_num, const int width, const int height) {
	// �ж��Ƿ��������Ϣ
	if (MouseHit()) {
		// ��ȡ�����Ϣ
		MOUSEMSG msg = GetMouseMsg();
		this->bat.radian = atan2(msg.y - this->bat.cy, msg.x - this->bat.cx); // atan2()�������ڻ�ȡy/x�ķ�����ֵ���Ի��ȱ�ʾ��
		this->bat.x = this->bat.cx + cos(bat.radian) * this->bat.len;
		this->bat.y = this->bat.cy + sin(bat.radian) * this->bat.len;
		// ���������£������ӵ�
		if (msg.uMsg == WM_LBUTTONDOWN) {
			this->creatBullet(bull_num);
		}
		// ����Ҽ����£������ӵ�
		if (msg.uMsg == WM_RBUTTONDOWN) {
			this->speedBullet(bull_num);
		}
		// ������ֹ���������ӵ�
		if (msg.uMsg == WM_MOUSEWHEEL) {
			this->fillBullet(bull_num, width, height);
		}
	}
}

// ������Ϣ��Ӧ��ͬ����Ӧ��
void Model::keyDown(const int bull_num, const int width, const int height) {
	// ���������û�����
	char key;
	// �ж��Ƿ��м�����Ϣ
	while (_kbhit()) {
		key = (char)_getch();
		fflush(stdin); // ��ռ�����Ϣ������
		switch (key) {
			// �Ϸ���������ӵ�������
		case'W':
		case'w':
		case 72:
			this->speedBullet(bull_num);
			break;
			// �·���������ӵ������
		case'S':
		case's':
		case 80:
			this->fillBullet(bull_num, width, height);
			break;
			// �ո���������ӵ��ķ���
		case' ':
			this->creatBullet(bull_num);
			break;
		}
	}
}

// �����첽��Ӧ
void Model::keyDown2(const double barrel_speed) {
	if (GetForegroundWindow() == GetHWnd()) {
		// �жϵ�ǰWindowsǰ̨�����Ƿ�Ϊ��ǰ��ͼ����
		if ((GetAsyncKeyState(VK_LEFT) || GetAsyncKeyState('A')) && 0x8000) { // ������
			// ��������ڹ������ƶ�
			this->bat.radian = this->bat.radian - barrel_speed;
			if (this->bat.radian < -3.1415926) {
				this->bat.radian = this->bat.radian + barrel_speed;
			}
		}
		if ((GetAsyncKeyState(VK_RIGHT) || GetAsyncKeyState('D')) && 0x8000) { // ����Ҽ�
			// �ҷ�������ڹ������ƶ�
			this->bat.radian = this->bat.radian + barrel_speed;
			if (this->bat.radian > 0) {
				this->bat.radian = this->bat.radian - barrel_speed;
			}
		}
		this->bat.x = this->bat.cx + cos(this->bat.radian) * this->bat.len;
		this->bat.y = this->bat.cy + sin(this->bat.radian) * this->bat.len;
	}
	fflush(stdin); // ��ռ�����Ϣ������
}