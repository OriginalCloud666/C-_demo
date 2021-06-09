#include "Model.h"
using namespace std;

Model::Model() :score(0), cas(0), tempcas(0) {
	for (int i = 0; i < pillar_num; i++) {
		struct Pillar newpillar;
		this->pillar.push_back(newpillar);
	}
	for (int i = 0; i < 2; i++) {
		struct BkDown newbkdown;
		this->bkdown.push_back(newbkdown);
	}
	this->beginGame = creatButton(80, 500, 150, 50, "START", 38, 12, RGB(234, 139, 83), RGB(240, 240, 240));
	this->endGame = creatButton(420, 500, 150, 50, "END", 52, 12, RGB(234, 139, 83), RGB(240, 240, 240));
}

Model::~Model() {
	delete this->beginGame;
	this->beginGame = nullptr;
	delete this->endGame;
	this->endGame = nullptr;
}

// ��ȡscore
int& Model::getScore() {
	return this->score;
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
void Model::load(IMAGE& bk, IMAGE mybird[][2], IMAGE guid[], IMAGE up[], IMAGE down[], IMAGE start[], IMAGE end[]) {
	// ���ر���ͼƬ
	loadimage(&bk, "./resource/picture/bk.bmp", width, height);
	// ��������ͼƬ
	loadimage(guid, "./resource/picture/guid.bmp");
	loadimage(guid + 1, "./resource/picture/guidy.bmp");
	// �������ͼƬ
	loadimage(mybird[0], "./resource/picture/bird0.bmp", bird_size, bird_size);
	loadimage(mybird[0] + 1, "./resource/picture/birdy0.bmp", bird_size, bird_size);
	loadimage(mybird[1], "./resource/picture/bird1.bmp", bird_size, bird_size);
	loadimage(mybird[1] + 1, "./resource/picture/birdy1.bmp", bird_size, bird_size);
	loadimage(mybird[2], "./resource/picture/bird2.bmp", bird_size, bird_size);
	loadimage(mybird[2] + 1, "./resource/picture/birdy2.bmp", bird_size, bird_size);
	// �����·�������ͼƬ
	loadimage(down, "./resource/picture/down.bmp");
	loadimage(down + 1, "./resource/picture/downy.bmp");
	// �����Ϸ�������ͼƬ
	loadimage(up, "./resource/picture/up.bmp");
	loadimage(up + 1, "./resource/picture/upy.bmp");
	// ������Ϸ��ʼͼƬ
	loadimage(start, "./resource/picture/start.bmp");
	loadimage(start + 1, "./resource/picture/starty.bmp");
	// ������Ϸ����ͼƬ
	loadimage(end, "./resource/picture/end.bmp");
	loadimage(end + 1, "./resource/picture/endy.bmp");
}

// ��������
void Model::bgm() {
	mciSendString("open ./resource/music/bk.mp3 alias BGM", nullptr, 0, nullptr);
	mciSendString("play BGM repeat", nullptr, 0, nullptr);
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
	setlinecolor(RGB(243, 192, 161));
	setlinestyle(PS_SOLID, 5);
	setfillcolor(pButton->buttoncolor);
	fillrectangle(pButton->x, pButton->y, pButton->x + pButton->width, pButton->y + pButton->height);
	settextcolor(pButton->textcolor);
	settextstyle(28, 0, "����", 0, 0, 1000, 0, 0, 0);
	outtextxy(pButton->x + pButton->dx, pButton->y + pButton->dy, pButton->text);
}

// ������ͼƬ
void Model::drawGuid(IMAGE guid[], const int x, const int y) {
	// ����ͼ��SRCAND��ʽȥ��ͼ
	putimage(x, y, guid + 1, SRCAND);
	// Դ��ͼ��SRCPAINT��ʽȥ��ͼ
	putimage(x, y, guid, SRCPAINT);
}

// �����ͼƬ
void Model::drawBird(IMAGE mybird[][2], const int x, const int y) {
	// ����ͼ��SRCAND��ʽȥ��ͼ
	putimage(x, y, mybird[this->cas] + 1, SRCAND);
	// Դ��ͼ��SRCPAINT��ʽȥ��ͼ
	putimage(x, y, mybird[this->cas], SRCPAINT);
}

// �����ӵ�ͼƬ
void Model::drawPillar(IMAGE up[], IMAGE down[], struct Pillar mypillar) {
	// �ϱ����ӻ���
	// ����ͼ��SRCAND��ʽȥ��ͼ
	putimage(mypillar.x, 0, 65, mypillar.height, down + 1, 0, 400 - mypillar.height, SRCAND);
	// Դ��ͼ��SRCPAINT��ʽȥ��ͼ
	putimage(mypillar.x, 0, 65, mypillar.height, down, 0, 400 - mypillar.height, SRCPAINT);

	// �±����ӻ���
	// ����ͼ��SRCAND��ʽȥ��ͼ
	putimage(mypillar.x, 607 - (400 - mypillar.height), 65, 400 - mypillar.height, up + 1, 0, 0, SRCAND);
	// Դ��ͼ��SRCPAINT��ʽȥ��ͼ
	putimage(mypillar.x, 607 - (400 - mypillar.height), 65, 400 - mypillar.height, up, 0, 0, SRCPAINT);
}

// ���²����汳��ͼƬ
void Model::drawBackground(IMAGE& bk, struct BkDown mybkdown) {
	// ���²����汳��ͼƬ
	putimage(mybkdown.x, 607, width - 1, height - 607, &bk, 0, 607);
}

// ���ƿ�ʼ����
void Model::welcome(IMAGE& bk, IMAGE guid[]) {
	cleardevice();
	putimage(0, 0, &bk);
	SetWindowText(GetHWnd(), "C++�����С��");

	this->drawGuid(guid, 10, 10); // ��������ͼƬ
	this->drawButton(beginGame); // ���ƿ�ʼ��ť
	this->drawButton(endGame); // ���ƽ�����ť

	// ���Ʊ���ͼƬ
	for (int i = 0; i < 2; i++) {
		if (this->bkdown[i].flag == 1) { // ���������ʱ���Ե�����л���
			this->drawBackground(bk, this->bkdown[i]);
		}
	}
}

// ������Ϸ����
void Model::draw(IMAGE& bk, IMAGE up[], IMAGE down[], IMAGE mybird[][2]) {
	cleardevice();
	putimage(0, 0, &bk);

	// ���Ʊ���ͼƬ
	for (int i = 0; i < 2; i++) {
		if (this->bkdown[i].flag == 1) { // ���������ʱ���Ե�����л���
			this->drawBackground(bk, this->bkdown[i]);
		}
	}

	// ��������ͼƬ
	for (int i = 0; i < pillar_num; i++) {
		if (this->pillar[i].flag == 1) { // �����Ӵ���ʱ�������ӽ��л���
			this->drawPillar(up, down, this->pillar[i]);
		}
	}

	// �������ͼƬ
	this->drawBird(mybird, this->bird.x, this->bird.y);
}

// ���ӵĲ���
void Model::createPillar(const int i) {
	pillar[i].height = rand() % 240 + 80;
	// �����ɵ����Ӹ߶�������������ͬʱ���������ɸ߶�
	while (pillar[i].height == pillar[(i + 1) % 3].height || pillar[i].height == pillar[(i + 2) % 3].height) {
		pillar[i].height = rand() % 240 + 80;
	}
	// �������Ӳ���ʱ��x����
	pillar[i].x = width;
	pillar[i].flag = 1;
}

// ����Ĳ���
void Model::createBkDown(const int i) {
	// ���õ������ʱ��x����
	this->bkdown[i].x = width - 1.0;
	this->bkdown[i].flag = 1;
}

// ��ʼ����Ϸ����
void Model::init() {
	// �������������
	srand((unsigned)time(nullptr) * rand());

	this->score = 0;
	this->cas = 0;
	this->tempcas = 0;

	// ��������ݽ��г�ʼ��
	this->bird.x = width / 5 - 50.0;
	this->bird.y = height * 2 / 5;
	this->bird.speed = 0.6;

	// ��ʼ����£��Ȳ�����������
	for (int i = 0; i < pillar_num; i++) {
		this->createPillar(i);
		pillar[i].x = width + 235.0 * i;
	}

	// ��ʼ����£��Ȳ�����������
	for (int i = 0; i < 2; i++) {
		this->createBkDown(i);
		bkdown[i].x = width * 1.0 * i;
	}
}

// ����ƶ�
void Model::moveBird() {
	// ���Լ������ƶ�
	this->bird.y += this->bird.speed;
	// ������ƶ��߽��������
	if (this->bird.y >= 607.0 - bird_size + 15.0) {
		this->bird.y = 607.0 - bird_size + 15.0;
	}
	else if (this->bird.y <= -15) {
		this->bird.y = -15;
	}
}

// ���ӵ��ƶ���ʧ
void Model::movePillar() {
	for (int i = 0; i < pillar_num; i++) {
		this->pillar[i].x -= move_step;
		if (this->pillar[i].x <= -65) {
			this->pillar[i].flag = 0;
			this->createPillar(i);
			this->score += 1;
		}
	}
}

// ������ƶ���ʧ
void Model::moveBkDown() {
	for (int i = 0; i < 2; i++) {
		this->bkdown[i].x -= move_step;
		if (this->bkdown[i].x <= -width) {
			this->bkdown[i].flag = 0;
			this->createBkDown(i);
		}
	}
}

// �����������ײ
int Model::hitPillar() {
	for (int i = 0; i < pillar_num; i++) {
		if (this->pillar[i].x <= this->bird.x + bird_size - 15 && this->pillar[i].x >= this->bird.x - 65 + 10) {
			// �ж������Ƿ�����ײ
			if (this->bird.y + 15 <= this->pillar[i].height) {
				return 1; // ����ֵΪ1˵������ײ����
			}
			// �ж������Ƿ�����ײ
			if (this->bird.y + bird_size - 15 >= 607 - (400.0 - this->pillar[i].height)) {
				return 1; // ����ֵΪ1˵������ײ����
			}
		}
	}
	return 0; // ����ֵΪ0˵��û����ײ
}

// �ж�����Ƿ��ڰ�ť��
int Model::mouseInButton(struct Button* pButton, MOUSEMSG* msg) {
	if (msg->x >= pButton->x && msg->x <= pButton->x + pButton->width &&
		msg->y >= pButton->y && msg->y <= pButton->y + pButton->height) {
		pButton->buttoncolor = RGB(230, 121, 55);
		pButton->textcolor = RGB(135, 135, 135);
		return 1; // ����ֵΪ1����������ڰ�ť��
	}
	pButton->buttoncolor = RGB(234, 139, 83);
	pButton->textcolor = RGB(240, 240, 240);
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

// ������Ӧ
void Model::keyDown() {
	// �ж��Ƿ��а�����Ϣ
	while (_kbhit()) {
		// ���ڽ��հ�����Ϣ
		char userKey = _getch();
		fflush(stdin); // ��ռ�����Ϣ������

		switch (userKey) {
		case ' ': // ��
			mciSendString("close fly", nullptr, 0, nullptr);
			mciSendString("open ./resource/music/fly.mp3 alias fly", nullptr, 0, nullptr);
			mciSendString("play fly", nullptr, 0, nullptr);
			this->bird.y -= (this->bird.speed + 130);
		}
	}
}

// ��Ϸ��������1
int Model::condition1() {
	// ��С��ײ���ذ�ʱ����Ϸ����
	if (this->bird.y >= 607.0 - bird_size + 15.0) {
		return 1; // ����ֵΪ1ʱ��Ϸ����
	}
	// С��ײ���컨��ʱ����Ϸ����
	else if (this->bird.y <= 15) {
		return 1; // ����ֵΪ1ʱ��Ϸ����
	}
	return 0; // ����ֵΪ0ʱ��Ϸδ����
}

// ��Ϸ��������2
int Model::condition2() {
	// ��С��ײ������ʱ��Ϸ����
	return this->hitPillar();
}

// ��Ϸ���������ж�
bool Model::over() {
	if (this->condition1() == 1 || this->condition2() == 1) {
		return true; // ����ֵΪ1ʱ��Ϸ����
	}
	return false; // ����ֵΪ0ʱ��Ϸδ����
}

// ��Ϸ��ʼ����
void Model::beginAction(IMAGE& bk, IMAGE guid[], IMAGE start[]) {
	// ���ÿ�ʼ������ʼ����
	double startX = 121;
	double startY = 87;
	// ���ö���Ư�Ʒ����Լ�״̬��ʶ
	double step = 0.38;
	int flag = 0;
	// ���þֲ��������ڼ�¼λ�ƴ�ֵ
	double tempstep = step;

	while (1) {
		this->welcome(bk, guid);

		// ���ø�������Ч��
		if (flag == 0) { // ��ʱ���������ƶ�
			step -= 0.0035;
			if (step <= -tempstep) {
				step = -tempstep;
				flag = 1; // �˺󶯻���ʼ�����ƶ�
			}
		}
		else if (flag == 1) { // ��ʱ���������ƶ�
			step += 0.0035;
			if (step >= tempstep) {
				step = tempstep;
				flag = 0; // �˺󶯻���ʼ�����ƶ�
			}
		}
		startY += step;

		// ���ƶ���Ч��
		putimage(startX, startY, start + 1, SRCAND); // ������ͼ
		putimage(startX, startY, start, SRCPAINT); // ��Դ��ͼ

		this->moveBkDown();
		FlushBatchDraw();
		// ����Ƿ�����ť�ж�
		if (this->mouseControl(this->beginGame, this->endGame) == 1) {
			break;
		}
	}
}

// ��Ϸ�ȳ�򶯻�
void Model::flyAction() {
	this->tempcas += 0.025;
	if (this->tempcas >= 3) {
		this->tempcas = 0;
	}
	this->cas = (int)this->tempcas;
}

// ��Ϸײ������
void Model::hitAction(IMAGE& bk, IMAGE up[], IMAGE down[], IMAGE mybird[][2]) {
	// ���������ٶ�
	double step = -1.85;

	// ���ƶ���Ч��
	while (this->bird.y + bird_size - 15 <= 607) {
		this->bird.y += step;
		step += 0.025;
		this->draw(bk, up, down, mybird);
		FlushBatchDraw();
	}
}

// ��Ϸ��������
void Model::endAction(IMAGE& bk, IMAGE up[], IMAGE down[], IMAGE mybird[][2], IMAGE end[]) {
	// ��ʼ��������
	double startX = width / 2 - 204.0 / 2;
	double startY = height;

	// ���ƶ���Ч��
	while (startY >= height / 2 - 54.0 / 2) {
		this->draw(bk, up, down, mybird);
		putimage(startX, startY, end + 1, SRCAND); // ������ͼ
		putimage(startX, startY, end, SRCPAINT); // ��Դ��ͼ
		FlushBatchDraw();
		startY -= 0.68; // ���ƶ����ٶ�
		Sleep(0); // ���ƶ����ٶ�
	}
}