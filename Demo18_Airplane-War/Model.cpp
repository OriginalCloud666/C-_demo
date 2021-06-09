#include "Model.h"
using namespace std;

Model::Model() :score(0), cas(0) {
	this->beginGame = creatButton(80, 465, 150, 50, "START", 38, 12, RGB(252, 252, 252), RGB(150, 150, 150));
	this->endGame = creatButton(420, 465, 150, 50, "END", 52, 12, RGB(252, 252, 252), RGB(150, 150, 150));
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

// ��ȡrole
Plane& Model::getRole() {
	return this->role;
}

// ��ȡbeginGame
struct Button*& Model::getButton1() {
	return this->beginGame;
}

// ��ȡendGame
struct Button*& Model::getButton2() {
	return this->endGame;
}

// ��ȡenemy1
vector<Plane>& Model::getEnemy1() {
	return this->enemy1;
}

// ��ȡenemy2
vector<Plane>& Model::getEnemy2() {
	return this->enemy2;
}

// ������Դ
void Model::load(IMAGE& bk, IMAGE& begin, IMAGE& author, IMAGE plane[][2], IMAGE enemy_big[], IMAGE enemy_small[], IMAGE bullet[]) {
	// ���ر���ͼƬ
	loadimage(&bk, "./resource/picture/bk.jpg", width, height * 1.299);
	// ���ؿ�ʼͼƬ
	loadimage(&begin, "./resource/picture/begin.png");
	// ����������ϢͼƬ
	loadimage(&author, "./resource/picture/author.png");
	// ���طɻ�����ͼƬ
	loadimage(plane[0], "./resource/picture/planeNormal_1.jpg", 110, 112);
	loadimage(plane[0] + 1, "./resource/picture/planeNormal_2.jpg", 110, 112);
	// ���طɻ�������ͼƬ
	loadimage(plane[1], "./resource/picture/planeExplode_1.jpg", 110, 112);
	loadimage(plane[1] + 1, "./resource/picture/planeExplode_2.jpg", 110, 112);
	// ���صо����ͷɻ�
	loadimage(enemy_big, "./resource/picture/enemyPlane_1.jpg", 95, 135);
	loadimage(enemy_big + 1, "./resource/picture/enemyPlane_2.jpg", 95, 135);
	// ���صо�С�ͷɻ�
	loadimage(enemy_small, "./resource/picture/enemy_1.jpg", 48, 36);
	loadimage(enemy_small + 1, "./resource/picture/enemy_2.jpg", 48, 36);
	// ���طɻ��ӵ�ͼƬ
	loadimage(bullet, "./resource/picture/bullet_1.jpg");
	loadimage(bullet + 1, "./resource/picture/bullet_2.jpg");
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

// Timer��ʱ��
bool Model::timer(const int ms, const int id) {
	// ����Ϊ��̬������Ϊ����t1,t2ֻ��ʼ��һ��
	static DWORD t[10];
	if (clock() - t[id] > ms) {
		t[id] = clock();
		return true;
	}
	return false;
}

// ������֮��ľ���
double Model::distance(const double x1, const double y1, const double x2, const double y2) {
	// distance=������(x2-x1)^2+(y2-y1)^2
	return sqrt((x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2));
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

// ��ʼ����Ϸ����
void Model::init() {
	// �������������
	srand((unsigned)time(nullptr) * rand());

	this->score = 0;
	this->cas = 0;

	// ���Ҿ��ɻ����г�ʼ��
	this->role.set(width / 2 - 48.0, height - 105.0, 1, 110, 112, 1, true);

	// �Եо��ɻ����г�ʼ��
	for (int i = 0; i < enemy1_num; i++) {
		Plane newplane1;
		newplane1.set(rand() % (width - newplane1.getWidth()), height, (rand() % 100 / 1000.0) + 0.4, 95, 135, 6, true);
		this->enemy1.push_back(newplane1);
	}
	for (int i = 0; i < enemy2_num; i++) {
		Plane newplane2;
		newplane2.set(rand() % (width - newplane2.getWidth()), height, (rand() % 100 / 1000.0) + 0.8, 48, 36, 1, true);
		this->enemy2.push_back(newplane2);
	}
}

// ���ƿ�ʼ����
void Model::welcome(IMAGE& bk, IMAGE& begin, IMAGE& author) {
	cleardevice();
	putimage(0, -132, &bk);
	SetWindowText(GetHWnd(), "C++�ɻ���ս");

	this->drawButton(this->beginGame); // ���ƿ�ʼ��ť
	this->drawButton(this->endGame); // ���ƽ�����ť
	this->drawAlpha(&begin, 25, 130); // ����ʼͼƬ
	this->drawAlpha(&author, 160, 300); // ��������Ϣ
}

// ������Ϸ����
void Model::draw(IMAGE& bk, IMAGE enemy_big[], IMAGE enemy_small[], IMAGE plane[][2], IMAGE bullet[]) {
	cleardevice();
	putimage(0, -132, &bk);

	// ���Ƶо��ɻ�
	for (int i = 0; i < enemy1_num; i++) {
		if (this->enemy1[i].getFlag() == true) {
			this->enemy1[i].draw(enemy_big);
		}
	}
	for (int i = 0; i < enemy2_num; i++) {
		if (this->enemy2[i].getFlag() == true) {
			this->enemy2[i].draw(enemy_small);
		}
	}

	// �����Ҿ��ɻ�
	if (this->role.getFlag() == true) {
		this->role.draw(plane[this->cas]);
		this->role.drawBullet(bullet);
	}
}

// �ƶ��л�
void Model::moveEnemy() {
	for (int i = 0; i < enemy1_num; i++) {
		if (this->enemy1[i].getFlag() == true) {
			this->enemy1[i].get_y() += this->enemy1[i].getStep();
			if (this->enemy1[i].get_y() > height && this->timer(10000, 1) == true) {
				this->enemy1[i].getFlag() = false;
				this->enemy1[i].get_x() = rand() % (width - this->enemy1[i].getWidth());
				this->enemy1[i].get_y() = -height / 6;
				this->enemy1[i].getFlag() = true;
			}
		}
	}
	for (int i = 0; i < enemy2_num; i++) {
		if (this->enemy2[i].getFlag() == true) {
			this->enemy2[i].get_y() += this->enemy2[i].getStep();
			if (this->enemy2[i].get_y() > height && timer(300, 2) == true) {
				this->enemy2[i].getFlag() = false;
				this->enemy2[i].get_x() = rand() % (width - this->enemy2[i].getWidth());
				this->enemy2[i].get_y() = -height / 6;
				this->enemy2[i].getFlag() = true;
			}
		}
	}
}

// �л���ײ��ʧ
void Model::hitEnemy(vector<Plane>& enemy, const int enemy_num, const int hp) {
	for (int i = 0; i < enemy_num; i++) {
		if (enemy[i].getFlag() == true) {
			// ���ӵ����б���
			for (list<Bullet>::iterator iter = this->role.getBullet().begin(); iter != this->role.getBullet().end(); iter++) {
				if (iter->x + 9 >= enemy[i].get_x() &&
					iter->x + 9 <= enemy[i].get_x() + enemy[i].getWidth() &&
					iter->y + 24 >= enemy[i].get_y() &&
					iter->y + 24 <= enemy[i].get_y() + enemy[i].getHeight()) {
					// ɾ����ǰ�ڵ�
					this->role.getBullet().pop_front();
					// �����зɻ�Ѫ������
					enemy[i].getHp()--;
					// Ѫ��Ϊ0ʱ���ɻ�������
					if (enemy[i].getHp() <= 0) {
						enemy[i].get_y() = height * 1.2;
						enemy[i].getHp() = hp;
						this->score += hp;
					}
					break;
				}
			}
		}
	}
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

// ������Ӧ���첽�������ƣ�
void Model::keyDown1() {
	// �жϵ�ǰWindowsǰ̨Ӧ�ó��������Ƿ�Ϊ��ǰeasyx��ͼ����
	if (GetForegroundWindow() == GetHWnd()) {
		if (GetAsyncKeyState(VK_UP) || GetAsyncKeyState('W')) { // ����ϼ�
			this->role.get_y() -= this->role.getStep();
			if (this->role.get_y() <= 0) {
				this->role.get_y() = 0;
			}
		}
		if (GetAsyncKeyState(VK_DOWN) || GetAsyncKeyState('S')) { // ����¼�
			this->role.get_y() += this->role.getStep();
			if (this->role.get_y() >= height - 55.0) {
				this->role.get_y() = height - 55.0;
			}
		}
		if (GetAsyncKeyState(VK_LEFT) || GetAsyncKeyState('A')) { // ������
			this->role.get_x() -= this->role.getStep();
			if (this->role.get_x() <= -44) {
				this->role.get_x() = -44;
			}
		}
		if (GetAsyncKeyState(VK_RIGHT) || GetAsyncKeyState('D')) { // ����Ҽ�
			this->role.get_x() += this->role.getStep();
			if (this->role.get_x() >= width - 54.0) {
				this->role.get_x() = width - 54.0;
			}
		}
	}
}

// ������Ӧ��ͬ���������ƣ�
void Model::keyDown2() {
	// �ж��Ƿ��а�����Ϣ
	while (_kbhit()) {
		// ���ڽ��հ�����Ϣ
		char userKey = _getch();
		fflush(stdin); // ��ռ�����Ϣ������

		// �԰�����Ϣ���зַ�
		switch (userKey) {
		case ' ':
			// ���¿ո������ӵ�
			this->role.createBullet(bullet_step);
			break;
		}
	}
}

// ��Ϸ��������
bool Model::condition(vector<Plane>& enemy) {
	// ���л����Ҿ��ɻ�������ײʱ����Ϸ����
	for (int i = 0; i < enemy.size(); i++) {
		if (enemy[i].getFlag() == true) {
			if (this->distance(enemy[i].get_x() + enemy[i].getWidth() / 2, enemy[i].get_y() + enemy[i].getHeight() / 2, this->role.get_x() + this->role.getWidth() / 2, this->role.get_y() + this->role.getHeight() / 2) <= 55) {
				this->cas++;
				return true; // ���������򷵻�true
			}
		}
	}
	return false; // �����������򷵻�false
}

// ��Ϸ�����ж�
bool Model::over() {
	if (this->condition(this->enemy1) == true || this->condition(this->enemy2) == true) {
		return true; // ����ֵΪtrueʱ��Ϸ����
	}
	return false; // ����ֵΪfalseʱ��Ϸδ����
}