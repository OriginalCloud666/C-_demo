#include "Model.h"
using namespace std;

// ������Դ
void Model::load(IMAGE& bk, int width, int height) {
	loadimage(&bk, "./resource/picture/bk.jpg", width * 1.035, height * 1.02);
}

// ��������
void Model::bgm() {
	mciSendString("open ./resource/music/bk.mp3 alias BGM", nullptr, 0, nullptr);
	mciSendString("play BGM repeat", nullptr, 0, nullptr);
}

// ��ʼ������
struct Star Model::initStar(const int height) {
	struct Star newstar;
	newstar.x = 0;
	newstar.y = rand() % height; // ���ǵı任�������������
	newstar.radius = (rand() % 3000) / 1000.0 + 1;
	newstar.step = (rand() % 5000) / 1000.0 + 1;
	newstar.color = (int)(newstar.step * 255 / 6.0 + 0.5); // �ٶ�Խ�죬��ɫԽ��
	if (newstar.color <= 160) { // ������������̫��
		newstar.color = 160;
	}
	return newstar;
}

// �ƶ�����
void Model::moveStar(const int i, const int width, const int height) {
	this->startVec[i].x += this->startVec[i].step;
	if (this->startVec[i].x > width) {
		this->startVec[i] = Model::initStar(height);
	}

	// �����µ�����
	setfillcolor(RGB(this->startVec[i].color, this->startVec[i].color, this->startVec[i].color));
	solidcircle((int)this->startVec[i].x, this->startVec[i].y, this->startVec[i].radius);
}

// ��ʼ������
void Model::init(const int star_num, const int width, const int height) {
	// �������������
	srand((unsigned)time(nullptr));

	// ��ʼ����������
	for (int i = 0; i < star_num; i++) {
		this->startVec.push_back(initStar(height));
		this->startVec[i].x = rand() % width;
	}
}

// ���Ƴ������
void Model::draw(IMAGE& bk) {
	putimage(0, 0, &bk);
	setbkmode(TRANSPARENT);
	SetWindowText(GetHWnd(), "C++��˸����");

	// ���������Ϣ
	settextcolor(RGB(175, 175, 175));
	settextstyle(22, 0, "����", 0, 0, 1000, 0, 0, 0);
	outtextxy(30, 20, "��ά����");

	settextcolor(RGB(170, 170, 170));
	settextstyle(18, 0, "����", 0, 0, 900, 0, 0, 0);
	outtextxy(27, 50, "���ߣ�����");
}