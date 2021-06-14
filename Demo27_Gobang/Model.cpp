#include "Model.h"
using namespace std;

Model::Model() :top(0), left(0), down(0), right(0), flag(0) {
	this->beginGame = creatButton(80, 500, 150, 50, "START", 38, 12, RGB(252, 252, 252), RGB(189, 164, 17));
	this->endGame = creatButton(420, 500, 150, 50, "END", 52, 12, RGB(252, 252, 252), RGB(189, 164, 17));
}

Model::~Model() {
	delete this->beginGame;
	this->beginGame = nullptr;
	delete this->endGame;
	this->endGame = nullptr;
}

// ��ȡflag
int& Model::getFlag() {
	return this->flag;
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
void Model::load(IMAGE& bk, IMAGE& w1, IMAGE& w2) {
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
bool Model::timer(const int ms, const int id) {
	// ����Ϊ��̬������Ϊ����t1,t2ֻ��ʼ��һ��
	static DWORD t[10];
	if (clock() - t[id] > ms) {
		t[id] = clock();
		return true;
	}
	return false;
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
	setlinecolor(RGB(210, 182, 19));
	setlinestyle(PS_SOLID, 5);
	setfillcolor(pButton->buttoncolor);
	fillrectangle(pButton->x, pButton->y, pButton->x + pButton->width, pButton->y + pButton->height);
	settextcolor(pButton->textcolor);
	settextstyle(28, 0, "����", 0, 0, 1000, 0, 0, 0);
	outtextxy(pButton->x + pButton->dx, pButton->y + pButton->dy, pButton->text);
}

// ��ʼ������
void Model::init() {
	// ���ñ�����ɫ
	setbkcolor(RGB(246, 232, 155));

	// ������Ϸ����
	this->left = width / 2 - (grid_num * grid_size) / 2;
	this->top = height / 2 - (grid_num * grid_size) / 2;
	this->right = this->left + grid_num * grid_size;
	this->down = this->top + grid_num * grid_size;

	// ��ʼ����������
	this->gameMapVec.clear();
	for (int i = 0; i < grid_num; i++) {
		vector<int> lineGrid;
		for (int j = 0; j < grid_num; j++) {
			lineGrid.push_back(0);
		}
		this->gameMapVec.push_back(lineGrid);
	}

	// ��ʼ���������
	this->curse1.row = grid_num / 2 - 3;
	this->curse1.col = grid_num / 2 - 3;
	this->curse2.row = grid_num / 2 - 3;
	this->curse2.col = grid_num / 2 + 3;
}

// ��ʼ�������
void Model::welcome(IMAGE& w1, IMAGE& w2) {
	SetWindowText(GetHWnd(), "C++�������ս");
	cleardevice();
	putimage(-10, 140, &w1);
	putimage(-10, 270, &w2);
	this->drawButton(this->beginGame);
	this->drawButton(this->endGame);
}

// ��Ϸ�������
void Model::draw() {
	cleardevice();

	// ������Ϸ��ͼ
	for (int i = 0; i < grid_num; i++) {
		for (int j = 0; j < grid_num; j++) {
			// ���÷������λ��
			int x = this->left + grid_size * j;
			int y = this->top + grid_size * i;

			// ���������ʽ
			settextcolor(RGB(80, 80, 80));
			settextstyle(grid_size, 0, "����", 0, 0, 500, 0, 0, 0);

			// ������������
			if (this->gameMapVec[i][j] == 1) {
				setfillcolor(RGB(40, 40, 40));
				solidcircle(x + grid_size / 2, y + grid_size / 2, grid_size / 2 - 1);
			}
			else if (this->gameMapVec[i][j] == 2) {
				setfillcolor(RGB(255, 255, 255));
				solidcircle(x + grid_size / 2, y + grid_size / 2, grid_size / 2 - 1);
			}
			else if (this->gameMapVec[i][j] == 3) {
				setfillcolor(RGB(40, 40, 40));
				solidcircle(x + grid_size / 2, y + grid_size / 2, grid_size / 2 - 1);
				setfillcolor(RGB(0, 128, 192));
				solidcircle(x + grid_size / 2, y + grid_size / 2, 6);
			}
			else if (this->gameMapVec[i][j] == 4) {
				setfillcolor(RGB(255, 255, 255));
				solidcircle(x + grid_size / 2, y + grid_size / 2, grid_size / 2 - 1);
				setfillcolor(RGB(238, 187, 187));
				solidcircle(x + grid_size / 2, y + grid_size / 2, 6);
			}
			else if (i == this->curse1.row && j == this->curse1.col) {
				settextcolor(BLUE);
				settextstyle(grid_size, 0, "����", 0, 0, 1000, 0, 0, 0);
				outtextxy(x, y, "�p");
			}
			else if (i == this->curse2.row && j == this->curse2.col) {
				settextcolor(RED);
				settextstyle(grid_size, 0, "����", 0, 0, 1000, 0, 0, 0);
				outtextxy(x, y, "�p");
			}
			// �������̸���
			else if (i == 0 && j == 0) {
				outtextxy(x, y, "��");
			}
			else if (i == 0 && j == grid_num - 1) {
				outtextxy(x, y, "��");
			}
			else if (i == grid_num - 1 && j == 0) {
				outtextxy(x, y, "��");
			}
			else if (i == grid_num - 1 && j == grid_num - 1) {
				outtextxy(x, y, "��");
			}
			else if (i == 0) {
				outtextxy(x, y, "��");
			}
			else if (i == grid_num - 1) {
				outtextxy(x, y, "��");
			}
			else if (j == 0) {
				outtextxy(x, y, "��");
			}
			else if (j == grid_num - 1) {
				outtextxy(x, y, "��");
			}
			else if (this->gameMapVec[i][j] == 0 && !(i == grid_num / 2 && j == grid_num / 2)) {
				outtextxy(x, y, "��");
			}
			else if (i == grid_num / 2 && j == grid_num / 2) {
				settextstyle(grid_size, 0, "����", 0, 0, 1000, 0, 0, 0);
				outtextxy(x, y, "�p");
				setfillcolor(RGB(40, 40, 40));
				solidcircle(x + grid_size / 2, y + grid_size / 2, 6);
			}
		}
	}
}

// �ж�����Ƿ��ڰ�ť��
int Model::mouseInButton(struct Button* pButton, MOUSEMSG* msg) {
	if (msg->x >= pButton->x && msg->x <= pButton->x + pButton->width &&
		msg->y >= pButton->y && msg->y <= pButton->y + pButton->height) {
		pButton->buttoncolor = RGB(235, 235, 235);
		pButton->textcolor = RGB(189, 164, 17);
		return 1; // ����ֵΪ1����������ڰ�ť��
	}
	pButton->buttoncolor = RGB(252, 252, 252);
	pButton->textcolor = RGB(189, 164, 17);
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

// ������Ӧ���첽������Ӧ��
void Model::keyDown() {
	// �жϵ�ǰWindows�ö������Ƿ�Ϊ��ǰ��ͼ����
	if (GetForegroundWindow() == GetHWnd()) {
		// ���1��������
		if (GetAsyncKeyState('W') && this->timer(120, 0)) {
			this->curse1.row--;
			if (this->curse1.row <= 0) {
				this->curse1.row = 0;
			}
		}
		if (GetAsyncKeyState('S') && this->timer(120, 1)) {
			this->curse1.row++;
			if (this->curse1.row >= grid_num - 1) {
				this->curse1.row = grid_num - 1;
			}
		}
		if (GetAsyncKeyState('A') && this->timer(120, 2)) {
			this->curse1.col--;
			if (this->curse1.col <= 0) {
				this->curse1.col = 0;
			}
		}
		if (GetAsyncKeyState('D') && this->timer(120, 3)) {
			this->curse1.col++;
			if (this->curse1.col >= grid_num - 1) {
				this->curse1.col = grid_num - 1;
			}
		}
		if (GetAsyncKeyState('J') && this->timer(120, 8)) {
			if (this->gameMapVec[this->curse1.row][this->curse1.col] != 2) {
				this->gameMapVec[this->curse1.row][this->curse1.col] = 1;
			}
			mciSendString("close click", nullptr, 0, nullptr);
			mciSendString("open ./resource/music/click.wav alias click", nullptr, 0, nullptr);
			mciSendString("play click", nullptr, 0, nullptr);
		}
		// ���2��������
		if (GetAsyncKeyState(VK_UP) && this->timer(120, 4)) {
			this->curse2.row--;
			if (this->curse2.row <= 0) {
				this->curse2.row = 0;
			}
		}
		if (GetAsyncKeyState(VK_DOWN) && this->timer(120, 5)) {
			this->curse2.row++;
			if (this->curse2.row >= grid_num - 1) {
				this->curse2.row = grid_num - 1;
			}
		}
		if (GetAsyncKeyState(VK_LEFT) && this->timer(120, 6)) {
			this->curse2.col--;
			if (this->curse2.col <= 0) {
				this->curse2.col = 0;
			}
		}
		if (GetAsyncKeyState(VK_RIGHT) && this->timer(120, 7)) {
			this->curse2.col++;
			if (this->curse2.col >= grid_num - 1) {
				this->curse2.col = grid_num - 1;
			}
		}
		if (GetAsyncKeyState(VK_NUMPAD0) && this->timer(120, 9)) {
			if (this->gameMapVec[this->curse2.row][this->curse2.col] != 1) {
				this->gameMapVec[this->curse2.row][this->curse2.col] = 2;
			}
			mciSendString("close click", nullptr, 0, nullptr);
			mciSendString("open ./resource/music/click.wav alias click", nullptr, 0, nullptr);
			mciSendString("play click", nullptr, 0, nullptr);
		}
	}
}

// ��Ϸ��������
int Model::condition1() {
	// �ж��Ƿ�����ͬ���������
	for (int i = 0; i < grid_num; i++) {
		for (int j = 0; j < grid_num; j++) {
			// �����ж�
			if (i + 4 < grid_num &&
				this->gameMapVec[i][j] != 0 &&
				this->gameMapVec[i][j] == this->gameMapVec[i + 1][j] &&
				this->gameMapVec[i + 1][j] == this->gameMapVec[i + 2][j] &&
				this->gameMapVec[i + 2][j] == this->gameMapVec[i + 3][j] &&
				this->gameMapVec[i + 3][j] == this->gameMapVec[i + 4][j]) {
				if (this->gameMapVec[i][j] == 1) {
					this->flag = 1;
				}
				else if (this->gameMapVec[i][j] == 2) {
					this->flag = 2;
				}
				this->gameMapVec[i][j] += 2;
				this->gameMapVec[i + 1][j] += 2;
				this->gameMapVec[i + 2][j] += 2;
				this->gameMapVec[i + 3][j] += 2;
				this->gameMapVec[i + 4][j] += 2;
				return 1; // ����ֵΪ1����������Ϸ��������
			}
			// �����ж�
			if (j + 4 < grid_num &&
				this->gameMapVec[i][j] != 0 &&
				this->gameMapVec[i][j] == this->gameMapVec[i][j + 1] &&
				this->gameMapVec[i][j + 1] == this->gameMapVec[i][j + 2] &&
				this->gameMapVec[i][j + 2] == this->gameMapVec[i][j + 3] &&
				this->gameMapVec[i][j + 3] == this->gameMapVec[i][j + 4]) {
				if (this->gameMapVec[i][j] == 1) {
					this->flag = 1;
				}
				else if (this->gameMapVec[i][j] == 2) {
					this->flag = 2;
				}
				this->gameMapVec[i][j] += 2;
				this->gameMapVec[i][j + 1] += 2;
				this->gameMapVec[i][j + 2] += 2;
				this->gameMapVec[i][j + 3] += 2;
				this->gameMapVec[i][j + 4] += 2;
				return 1; // ����ֵΪ1����������Ϸ��������
			}
			// б���ж�
			if (i + 4 < grid_num && j + 4 < grid_num &&
				this->gameMapVec[i][j] != 0 &&
				this->gameMapVec[i][j] == this->gameMapVec[i + 1][j + 1] &&
				this->gameMapVec[i + 1][j + 1] == this->gameMapVec[i + 2][j + 2] &&
				this->gameMapVec[i + 2][j + 2] == this->gameMapVec[i + 3][j + 3] &&
				this->gameMapVec[i + 3][j + 3] == this->gameMapVec[i + 4][j + 4]) {
				if (this->gameMapVec[i][j] == 1) {
					this->flag = 1;
				}
				else if (this->gameMapVec[i][j] == 2) {
					this->flag = 2;
				}
				this->gameMapVec[i][j] += 2;
				this->gameMapVec[i + 1][j + 1] += 2;
				this->gameMapVec[i + 2][j + 2] += 2;
				this->gameMapVec[i + 3][j + 3] += 2;
				this->gameMapVec[i + 4][j + 4] += 2;
				return 1; // ����ֵΪ1����������Ϸ��������
			}
			// б���ж�
			if (i + 4 < grid_num && j - 4 >= 0 &&
				this->gameMapVec[i][j] != 0 &&
				this->gameMapVec[i][j] == this->gameMapVec[i + 1][j - 1] &&
				this->gameMapVec[i + 1][j - 1] == this->gameMapVec[i + 2][j - 2] &&
				this->gameMapVec[i + 2][j - 2] == this->gameMapVec[i + 3][j - 3] &&
				this->gameMapVec[i + 3][j - 3] == this->gameMapVec[i + 4][j - 4]) {
				if (this->gameMapVec[i][j] == 1) {
					this->flag = 1;
				}
				else if (this->gameMapVec[i][j] == 2) {
					this->flag = 2;
				}
				this->gameMapVec[i][j] += 2;
				this->gameMapVec[i + 1][j - 1] += 2;
				this->gameMapVec[i + 2][j - 2] += 2;
				this->gameMapVec[i + 3][j - 3] += 2;
				this->gameMapVec[i + 4][j - 4] += 2;
				return 1; // ����ֵΪ1����������Ϸ��������
			}
		}
	}
	return 0; // ����ֵΪ0����δ������Ϸ��������
}

// ��Ϸ��������
int Model::condition2() {
	int count = 0;
	// �ж��Ƿ����
	for (int i = 0; i < grid_num; i++) {
		for (int j = 0; j < grid_num; j++) {
			if (this->gameMapVec[i][j] != 0) {
				count++;
			}
		}
	}
	if (count == grid_num * grid_num) {
		this->flag = 3;
		return 1; // ����ֵΪ1����������Ϸ��������
	}
	return 0; // ����ֵΪ0����δ������Ϸ��������
}

// ��Ϸ�����ж�
bool Model::over() {
	if (this->condition1() == 1 || this->condition2() == 1) {
		return true; // ����ֵΪtrue��˵����Ϸ����
	}
	return false; // ����ֵΪfalse��˵����Ϸδ����
}