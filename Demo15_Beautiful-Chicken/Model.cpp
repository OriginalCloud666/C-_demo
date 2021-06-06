#include "Model.h"
using namespace std;

Model::Model() :top(0), left(0), down(0), right(0), flag(0) {
	this->beginGame = creatButton(562, 642, pic_size, pic_size, RGB(218, 224, 219), RGB(196, 204, 198));
	this->gameMapVec = {
		{
			{ 1, 1, 1, 1, 1, 1, 1, 1, },
			{ 1, 2, 3, 0, 0, 3, 2, 1, },
			{ 1, 0, 1, 0, 1, 1, 0, 1, },
			{ 1, 0, 0, 4, 0, 0, 0, 1, },
			{ 1, 0, 0, 0, 0, 0, 0, 1, },
			{ 1, 0, 1, 0, 1, 1, 0, 1, },
			{ 1, 2, 3, 0, 0, 3, 2, 1, },
			{ 1, 1, 1, 1, 1, 1, 1, 1, },
		},
		{
			{ 1, 1, 1, 1, 1, 1, 1, 1, },
			{ 1, 2, 3, 0, 0, 3, 2, 1, },
			{ 1, 0, 1, 0, 1, 1, 0, 1, },
			{ 1, 2, 3, 4, 0, 3, 2, 1, },
			{ 1, 0, 0, 0, 0, 0, 0, 1, },
			{ 1, 0, 1, 0, 1, 1, 0, 1, },
			{ 1, 2, 3, 0, 0, 3, 2, 1, },
			{ 1, 1, 1, 1, 1, 1, 1, 1, },
		},
		{
			{ 1, 1, 1, 1, 1, 1, 1, 1, },
			{ 1, 2, 0, 0, 0, 0, 2, 1, },
			{ 1, 3, 1, 0, 1, 1, 3, 1, },
			{ 1, 0, 0, 4, 0, 0, 0, 1, },
			{ 1, 0, 0, 0, 0, 0, 0, 1, },
			{ 1, 3, 1, 0, 1, 1, 3, 1, },
			{ 1, 2, 0, 0, 0, 0, 2, 1, },
			{ 1, 1, 1, 1, 1, 1, 1, 1, },
		},
	};
}

// ��ȡflag
int& Model::getFlag() {
	return this->flag;
}

// ��ȡbeginGame
Button*& Model::getBeginGame() {
	return this->beginGame;
}

// ������Դ
void Model::load(IMAGE& bk, IMAGE& button, vector<IMAGE>& img, vector<int>& imgIndex) {
	// ����������ͼ
	for (int i = 0; i < pic_num; i++) {
		char fileName[25];
		sprintf(fileName, "./resource/picture/%d.bmp", imgIndex[i]);
		loadimage(&img[i], fileName, pic_size, pic_size);
	}
	loadimage(&bk, "./resource/picture/bk.jpg", width, height * 1.256);
	loadimage(&button, "./resource/picture/7.bmp", beginGame->width, beginGame->height);
}

// ���ñ�������
void Model::bgm() {
	// ��������
	mciSendString("open ./resource/music/bk.mp3 alias BGM", nullptr, 0, nullptr);
	mciSendString("play BGM repeat", nullptr, 0, nullptr);
}

// ������ť
struct Button* Model::creatButton(int x, int y, int width, int height, COLORREF color1, COLORREF color2) {
	struct Button* pButton = (struct Button*)malloc(sizeof(struct Button));
	pButton->x = x;
	pButton->y = y;
	pButton->width = width;
	pButton->height = height;
	pButton->color1 = color1;
	pButton->color2 = color2;
	return pButton;
}

// ���ư�ť
void Model::drawButton(struct Button* pButton, IMAGE& button) {
	// �����ťͼƬ
	putimage(pButton->x, pButton->y, &button);
	// ���ư�ť�߿�
	for (int x = 8; x >= 0; x--) {
		setlinecolor(pButton->color1);
		rectangle(pButton->x - x, pButton->y - x, pButton->x + pButton->width + x, pButton->y + pButton->height + x);
	}
	for (int x = 16; x >= 8; x--) {
		setlinecolor(pButton->color2);
		rectangle(pButton->x - x, pButton->y - x, pButton->x + pButton->width + x, pButton->y + pButton->height + x);
	}
}

// ��ʼ����Ϸ����
void Model::init() {
	// ��Ϸ��ͼ���ݳ�ʼ��
	this->left = width / 2 - (grid_num * pic_size) / 2;
	this->top = height / 2 - (grid_num * pic_size) / 2;
	this->right = this->left + grid_num * pic_size;
	this->down = this->top + grid_num * pic_size;

	// ��vector�������г�ʼ��
	this->gameMapVec = {
		{
			{ 1, 1, 1, 1, 1, 1, 1, 1, },
			{ 1, 2, 3, 0, 0, 3, 2, 1, },
			{ 1, 0, 1, 0, 1, 1, 0, 1, },
			{ 1, 0, 0, 4, 0, 0, 0, 1, },
			{ 1, 0, 0, 0, 0, 0, 0, 1, },
			{ 1, 0, 1, 0, 1, 1, 0, 1, },
			{ 1, 2, 3, 0, 0, 3, 2, 1, },
			{ 1, 1, 1, 1, 1, 1, 1, 1, },
		},
		{
			{ 1, 1, 1, 1, 1, 1, 1, 1, },
			{ 1, 2, 3, 0, 0, 3, 2, 1, },
			{ 1, 0, 1, 0, 1, 1, 0, 1, },
			{ 1, 2, 3, 4, 0, 3, 2, 1, },
			{ 1, 0, 0, 0, 0, 0, 0, 1, },
			{ 1, 0, 1, 0, 1, 1, 0, 1, },
			{ 1, 2, 3, 0, 0, 3, 2, 1, },
			{ 1, 1, 1, 1, 1, 1, 1, 1, },
		},
		{
			{ 1, 1, 1, 1, 1, 1, 1, 1, },
			{ 1, 2, 0, 0, 0, 0, 2, 1, },
			{ 1, 3, 1, 0, 1, 1, 3, 1, },
			{ 1, 0, 0, 4, 0, 0, 0, 1, },
			{ 1, 0, 0, 0, 0, 0, 0, 1, },
			{ 1, 3, 1, 0, 1, 1, 3, 1, },
			{ 1, 2, 0, 0, 0, 0, 2, 1, },
			{ 1, 1, 1, 1, 1, 1, 1, 1, },
		},
	};
}

// ���ƿ�ʼ����
void Model::welcome(IMAGE& bk) {
	putimage(0, -2, &bk);
	setbkmode(TRANSPARENT);
	SetWindowText(GetHWnd(), "C++����̫��");

	// ���������Ϣ
	settextcolor(RGB(240, 240, 240));
	settextstyle(31, 0, "����", 0, 0, 1000, 0, 0, 0);
	outtextxy(16, 12, "����̫��");

	settextcolor(RGB(240, 240, 240));
	settextstyle(26, 0, "����", 0, 0, 1000, 0, 0, 0);
	outtextxy(13, 52, "���ߣ�����");

	while (!_kbhit()) {
	}
	int temp = _getch(); // ��ȡ��ǰ����������Ժ�ߵİ�����Ӧ����Ӱ��
}

// ������Ϸ����
void Model::draw(IMAGE& bk, IMAGE& button, vector<IMAGE>& img) {
	cleardevice();
	putimage(0, -2, &bk);
	this->drawButton(beginGame, button);

	// ����������Ϣ
	setbkmode(TRANSPARENT);
	settextcolor(RGB(235, 235, 235));
	settextstyle(31, 0, "����", 0, 0, 1000, 0, 0, 0);
	outtextxy(16, 12, "����̫��");

	settextcolor(RGB(240, 240, 240));
	settextstyle(26, 0, "����", 0, 0, 1000, 0, 0, 0);
	outtextxy(13, 52, "���ߣ�����");

	// �����ⲿ�߿�
	for (int x = 25; x >= 0; x--) {
		setlinecolor(RGB(218, 224, 219));
		rectangle(this->left - x, this->top - x, this->right + x, this->down + x);
	}
	for (int x = 50; x >= 25; x--) {
		setlinecolor(RGB(196, 204, 198));
		rectangle(this->left - x, this->top - x, this->right + x, this->down + x);
	}

	// ������Ϸ��ͼ
	for (int i = 0; i < grid_num; i++) {
		for (int j = 0; j < grid_num; j++) {
			// ������ͼλ������
			int x = this->left + pic_size * j;
			int y = this->top + pic_size * i;
			// ��ͼƬ��Ϣ���зַ�
			switch (this->gameMapVec[this->flag][i][j]) {
			case 0: // �յ�
				putimage(x, y, &img[0]);
				break;
			case 1: // ǽ��
				putimage(x, y, &img[1]);
				break;
			case 2: // �����
				putimage(x, y, &img[2]);
				break;
			case 3: // ����
				putimage(x, y, &img[3]);
				break;
			case 4: // ������յ��غ�
			case 6: // ������������غ�
				putimage(x, y, &img[4]);
				break;
			case 5: // ������������غ�
				putimage(x, y, &img[5]);
				break;
			}
		}
	}
}

// �ҵ����������������
int Model::search_i() {
	for (int i = 0; i < grid_num; i++) {
		for (int j = 0; j < grid_num; j++) {
			if (this->gameMapVec[this->flag][i][j] == 4 || this->gameMapVec[this->flag][i][j] == 6) {
				return i; // �ҵ�����
			}
		}
	}
	return -1; // δ�ҵ�����
}

// �ҵ������������������
int Model::search_j() {
	for (int i = 0; i < grid_num; i++) {
		for (int j = 0; j < grid_num; j++) {
			if (this->gameMapVec[this->flag][i][j] == 4 || this->gameMapVec[this->flag][i][j] == 6) {
				return j; // �ҵ�����
			}
		}
	}
	return -1; // δ�ҵ�����
}

// ��������
void Model::keyDown() {
	// �ж��Ƿ��а�����Ϣ
	while (_kbhit()) {
		// ���ڽ����û�������Ϣ
		char userKey = _getch();
		fflush(stdin); // ��ռ�����Ϣ������

		// �ҵ����λ��
		int i = this->search_i();
		int j = this->search_j();

		switch (userKey) {
		case 'W': // ��
		case 'w':
		case 72:
			mciSendString("close click", nullptr, 0, nullptr);
			mciSendString("open ./resource/music/click.wav alias click", nullptr, 0, nullptr);
			mciSendString("play click", nullptr, 0, nullptr);
			if (this->gameMapVec[this->flag][i - 1][j] == 0 || this->gameMapVec[this->flag][i - 1][j] == 2) {
				// ����Ϸ����������߿յ�
				this->gameMapVec[this->flag][i][j] -= 4; // ԭ��λ������뿪
				this->gameMapVec[this->flag][i - 1][j] += 4; // �µ�λ����ҵ���
			}
			if (this->gameMapVec[this->flag][i - 1][j] == 3 || this->gameMapVec[this->flag][i - 1][j] == 5) {
				// ����Ϸ������ӻ���������Ŀ�ĵ�ʱ
				if (this->gameMapVec[this->flag][i - 2][j] == 0 || this->gameMapVec[this->flag][i - 2][j] == 2) {
					// ��������Ա��ǿյػ��������ʱ
					this->gameMapVec[this->flag][i][j] -= 4; // ԭ��λ������뿪
					this->gameMapVec[this->flag][i - 1][j] += 1; // ��ҵ�������ԭλ��
					this->gameMapVec[this->flag][i - 2][j] += 3; // ���ӵ�����λ��
				}
			}
			break;
		case 'S': // ��
		case 's':
		case 80:
			mciSendString("close click", nullptr, 0, nullptr);
			mciSendString("open ./resource/music/click.wav alias click", nullptr, 0, nullptr);
			mciSendString("play click", nullptr, 0, nullptr);
			if (this->gameMapVec[this->flag][i + 1][j] == 0 || this->gameMapVec[this->flag][i + 1][j] == 2) {
				// ����·����������߿յ�
				this->gameMapVec[this->flag][i][j] -= 4; // ԭ��λ������뿪
				this->gameMapVec[this->flag][i + 1][j] += 4; // �µ�λ����ҵ���
			}
			if (this->gameMapVec[this->flag][i + 1][j] == 3 || this->gameMapVec[this->flag][i + 1][j] == 5) {
				// ����·������ӻ���������Ŀ�ĵ�ʱ
				if (this->gameMapVec[this->flag][i + 2][j] == 0 || this->gameMapVec[this->flag][i + 2][j] == 2) {
					//��������Ա��ǿյػ��������ʱ
					this->gameMapVec[this->flag][i][j] -= 4; // ԭ��λ������뿪
					this->gameMapVec[this->flag][i + 1][j] += 1; // ��ҵ�������ԭλ��
					this->gameMapVec[this->flag][i + 2][j] += 3; // ���ӵ�����λ��
				}
			}
			break;
		case 'A': // ��
		case 'a':
		case 75:
			mciSendString("close click", nullptr, 0, nullptr);
			mciSendString("open ./resource/music/click.wav alias click", nullptr, 0, nullptr);
			mciSendString("play click", nullptr, 0, nullptr);
			if (this->gameMapVec[this->flag][i][j - 1] == 0 || this->gameMapVec[this->flag][i][j - 1] == 2) {
				// ��������������߿յ�
				this->gameMapVec[this->flag][i][j] -= 4; // ԭ��λ������뿪
				this->gameMapVec[this->flag][i][j - 1] += 4; // �µ�λ����ҵ���
			}
			if (this->gameMapVec[this->flag][i][j - 1] == 3 || this->gameMapVec[this->flag][i][j - 1] == 5) {
				// ����·������ӻ���������Ŀ�ĵ�ʱ
				if (this->gameMapVec[this->flag][i][j - 2] == 0 || this->gameMapVec[this->flag][i][j - 2] == 2) {
					// ��������Ա��ǿյػ��������ʱ
					this->gameMapVec[this->flag][i][j] -= 4; // ԭ��λ������뿪
					this->gameMapVec[this->flag][i][j - 1] += 1; // ��ҵ�������ԭλ��
					this->gameMapVec[this->flag][i][j - 2] += 3; // ���ӵ�����λ��
				}
			}
			break;
		case 'D': // ��
		case 'd':
		case 77:
			mciSendString("close click", nullptr, 0, nullptr);
			mciSendString("open ./resource/music/click.wav alias click", nullptr, 0, nullptr);
			mciSendString("play click", nullptr, 0, nullptr);
			if (this->gameMapVec[this->flag][i][j + 1] == 0 || this->gameMapVec[this->flag][i][j + 1] == 2) {
				// ����ҷ����������߿յ�
				this->gameMapVec[this->flag][i][j] -= 4; // ԭ��λ������뿪
				this->gameMapVec[this->flag][i][j + 1] += 4; // �µ�λ����ҵ���
			}
			if (this->gameMapVec[this->flag][i][j + 1] == 3 || this->gameMapVec[this->flag][i][j + 1] == 5) {
				// ����·������ӻ���������Ŀ�ĵ�ʱ
				if (this->gameMapVec[this->flag][i][j + 2] == 0 || this->gameMapVec[this->flag][i][j + 2] == 2) {
					// ��������Ա��ǿյػ��������ʱ
					this->gameMapVec[this->flag][i][j] -= 4; // ԭ��λ������뿪
					this->gameMapVec[this->flag][i][j + 1] += 1; // ��ҵ�������ԭλ��
					this->gameMapVec[this->flag][i][j + 2] += 3; // ���ӵ�����λ��
				}
			}
			break;
		}
	}
}

// �ж�����Ƿ��ڰ�ť��
int Model::mouseInButton(struct Button* pButton, MOUSEMSG* msg) {
	if (msg->x >= pButton->x && msg->x <= pButton->x + pButton->width &&
		msg->y >= pButton->y && msg->y <= pButton->y + pButton->height) {
		pButton->color1 = RGB(185, 196, 187);
		pButton->color2 = RGB(165, 177, 167);
		return 1; // ����ֵΪ1��������ڰ�ť��
	}
	pButton->color1 = RGB(218, 224, 219);
	pButton->color2 = RGB(196, 204, 198);
	return 0; // ����ֵΪ0������겻�ڰ�ť��
}

// ������
void Model::mouseControl(struct Button* pButton) {
	// �ж��Ƿ��������Ϣ
	if (MouseHit()) {
		MOUSEMSG msg = GetMouseMsg();
		// �ж��û��Ƿ�����ť����
		if (this->mouseInButton(pButton, &msg) == 1 && msg.uMsg == WM_LBUTTONDOWN) {
			mciSendString("close reset", nullptr, 0, nullptr);
			mciSendString("open ./resource/music/reset.mp3 alias reset", nullptr, 0, nullptr);
			mciSendString("play reset", nullptr, 0, nullptr);
			this->init();
		}
	}
}

// ��Ϸ���������ж�
bool Model::over() {
	// ���һ�����Ӷ�û������ô��Ϸ����
	for (int i = 0; i < grid_num; i++) {
		for (int j = 0; j < grid_num; j++) {
			if (this->gameMapVec[this->flag][i][j] == 3) {
				return false; // ��Ϸδ����
			}
		}
	}
	return true; // ��Ϸ����
}