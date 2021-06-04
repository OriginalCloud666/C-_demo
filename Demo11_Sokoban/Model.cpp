#include "Model.h"
using namespace std;

Model::Model() :top(0), left(0), down(0), right(0), flag(0) {
	this->gameMapVec = {
		{
			{ 2, 2, 1, 1, 1, 2, 2, 2 },
			{ 2, 2, 1, 4, 1, 2, 2, 2 },
			{ 2, 2, 1, 2, 1, 1, 1, 1 },
			{ 1, 1, 1, 3, 2, 3, 4, 1 },
			{ 1, 4, 2, 3, 6, 1, 1, 1 },
			{ 1, 1, 1, 1, 3, 1, 2, 2 },
			{ 2, 2, 2, 1, 4, 1, 2, 2 },
			{ 2, 2, 2, 1, 1, 1, 2, 2 },
		},

		{
			{ 1, 1, 1, 1, 1, 1, 1, 1 },
			{ 1, 1, 6, 2, 1, 1, 1, 1 },
			{ 1, 1, 2, 3, 2, 2, 1, 1 },
			{ 1, 1, 1, 2, 1, 2, 1, 1 },
			{ 1, 4, 1, 2, 1, 2, 2, 1 },
			{ 1, 4, 3, 2, 2, 1, 2, 1 },
			{ 1, 4, 2, 2, 2, 3, 2, 1 },
			{ 1, 1, 1, 1, 1, 1, 1, 1 },
		},

		{
			{ 2, 2, 1, 1, 1, 1, 2, 2 },
			{ 2, 2, 1, 4, 4, 1, 2, 2 },
			{ 2, 1, 1, 2, 4, 1, 1, 2 },
			{ 2, 1, 2, 2, 3, 4, 1, 2 },
			{ 1, 1, 2, 3, 2, 2, 1, 1 },
			{ 1, 2, 2, 1, 3, 3, 2, 1 },
			{ 1, 2, 2, 6, 2, 2, 2, 1 },
			{ 1, 1, 1, 1, 1, 1, 1, 1 },
		}
	};
}

// ��ȡflag
int& Model::getFlag() {
	return this->flag;
}

// ������Դ
void Model::load(IMAGE& bk, vector<IMAGE>& img, vector<int>& imgIndex, const int pic_num, const int pic_size, const int width, const int height) {
	// ��������ͼƬ
	for (int i = 0; i < pic_num; i++) {
		char fileName[25];
		sprintf(fileName, "./resource/picture/%d.png", imgIndex[i]);
		loadimage(&img[i], fileName, pic_size, pic_size);
	}
	loadimage(&bk, "./resource/picture/bk.jpg", width, height * 1.3);
}

// ���ñ�������
void Model::bgm() {
	mciSendString("open ./resource/music/bk.mp3 alias BGM", nullptr, 0, nullptr);
	mciSendString("play BGM repeat", nullptr, 0, nullptr);
}

// ��ʼ����Ϸ����
void Model::init(const int width, const int height, const int grid_num, const int pic_size, const int cas) {
	// ��ʼ����Ϸ��������
	this->left = width / 2 - (grid_num * pic_size) / 2;
	this->top = height / 2 - (grid_num * pic_size) / 2;
	this->right = this->left + grid_num * pic_size;
	this->down = this->top + grid_num * pic_size;
}

// ���ƿ�ʼ����
void Model::welcome(IMAGE& bk) {
	putimage(0, 0, &bk);
	setbkmode(TRANSPARENT);
	SetWindowText(GetHWnd(), "C++��������Ϸ");

	// ���������Ϣ
	settextcolor(RGB(120, 120, 120));
	settextstyle(28, 0, "����", 0, 0, 1000, 0, 0, 0);
	outtextxy(478, 23, "��������Ϸ");

	settextcolor(RGB(115, 115, 115));
	settextstyle(26, 0, "����", 0, 0, 1000, 0, 0, 0);
	outtextxy(484, 63, "���ߣ�����");

	while (!_kbhit()) {
	}
	int temp = _getch(); // ��ȡ��ʱ�İ����������֮��İ�����Ӧ����Ӱ��
}

// ������Ϸ����
void Model::draw(IMAGE& bk, vector<IMAGE>& img, const int grid_num, const int pic_size) {
	cleardevice();
	putimage(0, 0, &bk);

	// ���������Ϣ
	settextcolor(RGB(120, 120, 120));
	settextstyle(28, 0, "����", 0, 0, 1000, 0, 0, 0);
	outtextxy(478, 23, "��������Ϸ");

	settextcolor(RGB(115, 115, 115));
	settextstyle(26, 0, "����", 0, 0, 1000, 0, 0, 0);
	outtextxy(484, 63, "���ߣ�����");

	// �����ⲿ�߿�
	for (int x = 25; x >= 0; x--) {
		setlinecolor(RGB(160, 100, 67));
		rectangle(this->left - x, this->top - x, this->right + x, this->down + x);
	}
	for (int x = 45; x >= 25; x--) {
		setlinecolor(RGB(128, 64, 0));
		rectangle(this->left - x, this->top - x, this->right + x, this->down + x);
	}

	// ������Ϸ��ͼ
	for (int i = 0; i < grid_num; i++) {
		for (int j = 0; j < grid_num; j++) {
			// ������ͼλ������
			int x = this->left + pic_size * j;
			int y = this->top + pic_size * i;
			// ��ͼƬ���зַ�
			switch (this->gameMapVec[flag][i][j]) {
			case 0:
				putimage(x, y, &img[0]);
				break;
			case 1:
				putimage(x, y, &img[1]);
				break;
			case 2:
				putimage(x, y, &img[2]);
				break;
			case 3:
				putimage(x, y, &img[3]);
				break;
			case 4:
				putimage(x, y, &img[4]);
				break;
			case 5:
				putimage(x, y, &img[5]);
				break;
			case 6: // �����յ��غ�
			case 8: // ���������ͣ�ŵ��غ�
				putimage(x, y, &img[6]);
				break;
			case 7: // �����յ��غ�
			case 9: // ���������ͣ�ŵ��غ�
				putimage(x, y, &img[7]);
				break;
			case 10: // �����յ��غ�
			case 12: // ���������ͣ�ŵ��غ�
				putimage(x, y, &img[8]);
				break;
			case 11: // �����յ��غ�
			case 13: // ���������ͣ�ŵ��غ�
				putimage(x, y, &img[9]);
				break;
			}
		}
	}
}

// �����յػ�����Ŀ�ĵ�ʱ�������ԭλ�ý��и���
void Model::changOne(const int i, const int j) {
	// ������ƶ�ǰ��λ������Ϊ�յػ�������Ŀ�ĵ�
	if (this->gameMapVec[flag][i][j] == 7 || this->gameMapVec[flag][i][j] == 9) { // ��
		this->gameMapVec[flag][i][j] -= 5;
	}
	if (this->gameMapVec[flag][i][j] == 6 || this->gameMapVec[flag][i][j] == 8) { // ��
		this->gameMapVec[flag][i][j] -= 4;
	}
	else if (this->gameMapVec[flag][i][j] == 10 || this->gameMapVec[flag][i][j] == 12) { // ��
		this->gameMapVec[flag][i][j] -= 8;
	}
	else if (this->gameMapVec[flag][i][j] == 11 || this->gameMapVec[flag][i][j] == 13) { // ��
		this->gameMapVec[flag][i][j] -= 9;
	}
}

// ������Ӧ
void Model::keyDown(const int grid_num) {
	// �ж��Ƿ��м�����Ϣ
	while (_kbhit()) {
		// ���ڽ����û�����
		char userKey = _getch();
		fflush(stdin); // ��ռ��̻�����

		// �ҵ����λ��
		int i, j = 0;
		for (i = 0; i < grid_num; i++) {
			for (j = 0; j < grid_num; j++) {
				// ��ҵĲ�ͬ����
				if (this->gameMapVec[flag][i][j] >= 6 && this->gameMapVec[flag][i][j] <= 13) {
					break;
				}
			}
			if (this->gameMapVec[flag][i][j] >= 6 && this->gameMapVec[flag][i][j] <= 13) {
				break;
			}
		}

		// ������Ӧ
		switch (userKey) {
		case'W': // ��
		case 'w':
		case 72:
			mciSendString("close click", nullptr, 0, nullptr);
			mciSendString("open ./resource/music/click.wav alias click", nullptr, 0, nullptr);
			mciSendString("play click", nullptr, 0, nullptr);
			if (this->gameMapVec[flag][i - 1][j] == 2 || this->gameMapVec[flag][i - 1][j] == 4) {
				// ����������ǿյػ�����Ŀ�ĵ�
				this->changOne(i, j); // ����뿪ԭλ��
				this->gameMapVec[flag][i - 1][j] += 5;
			}
			if (this->gameMapVec[flag][i - 1][j] == 3 || this->gameMapVec[flag][i - 1][j] == 5) {
				// ��������������ӻ�������Ŀ�ĵ�ʱ
				if (this->gameMapVec[flag][i - 2][j] == 2 || this->gameMapVec[flag][i - 2][j] == 4) {
					// ��������Ե��ǿյػ�����Ŀ�ĵ�
					this->changOne(i, j); // ����뿪ԭλ��
					this->gameMapVec[flag][i - 1][j] += (5 - 1); // ��ҵ�������λ��
					this->gameMapVec[flag][i - 2][j] += 1; // ���ӵ�����λ��
				}
			}
			break;
		case'S': // ��
		case 's':
		case 80:
			mciSendString("close click", nullptr, 0, nullptr);
			mciSendString("open ./resource/music/click.wav alias click", nullptr, 0, nullptr);
			mciSendString("play click", nullptr, 0, nullptr);
			if (this->gameMapVec[flag][i + 1][j] == 2 || this->gameMapVec[flag][i + 1][j] == 4) {
				// ����������ǿյػ�����Ŀ�ĵ�
				this->changOne(i, j); // ����뿪ԭλ��
				this->gameMapVec[flag][i + 1][j] += 4;
			}
			if (this->gameMapVec[flag][i + 1][j] == 3 || this->gameMapVec[flag][i + 1][j] == 5) {
				// ��������������ӻ�������Ŀ�ĵ�ʱ
				if (this->gameMapVec[flag][i + 2][j] == 2 || this->gameMapVec[flag][i + 2][j] == 4) {
					// ��������Ե��ǿյػ�����Ŀ�ĵ�
					this->changOne(i, j); // ����뿪ԭλ��
					this->gameMapVec[flag][i + 1][j] += (4 - 1); // ��ҵ�������λ��
					this->gameMapVec[flag][i + 2][j] += 1; // ���ӵ�����λ��
				}
			}
			break;
		case'A': // ��
		case 'a':
		case 75:
			mciSendString("close click", nullptr, 0, nullptr);
			mciSendString("open ./resource/music/click.wav alias click", nullptr, 0, nullptr);
			mciSendString("play click", nullptr, 0, nullptr);
			if (this->gameMapVec[flag][i][j - 1] == 2 || this->gameMapVec[flag][i][j - 1] == 4) {
				// ����������ǿյػ�����Ŀ�ĵ�
				this->changOne(i, j); // ����뿪ԭλ��
				this->gameMapVec[flag][i][j - 1] += 8;
			}
			if (this->gameMapVec[flag][i][j - 1] == 3 || this->gameMapVec[flag][i][j - 1] == 5) {
				// ��������������ӻ�������Ŀ�ĵ�ʱ
				if (this->gameMapVec[flag][i][j - 2] == 2 || this->gameMapVec[flag][i][j - 2] == 4) {
					// ��������Ե��ǿյػ�����Ŀ�ĵ�
					this->changOne(i, j); // ����뿪ԭλ��
					this->gameMapVec[flag][i][j - 1] += (8 - 1); // ��ҵ�������λ��
					this->gameMapVec[flag][i][j - 2] += 1; // ���ӵ�����λ��
				}
			}
			break;
		case'D': // ��
		case 'd':
		case 77:
			mciSendString("close click", nullptr, 0, nullptr);
			mciSendString("open ./resource/music/click.wav alias click", nullptr, 0, nullptr);
			mciSendString("play click", nullptr, 0, nullptr);
			if (this->gameMapVec[flag][i][j + 1] == 2 || this->gameMapVec[flag][i][j + 1] == 4) {
				// ����������ǿյػ�����Ŀ�ĵ�
				this->changOne(i, j); // ����뿪ԭλ��
				this->gameMapVec[flag][i][j + 1] += 9;
			}
			if (this->gameMapVec[flag][i][j + 1] == 3 || this->gameMapVec[flag][i][j + 1] == 5) {
				// ��������������ӻ�������Ŀ�ĵ�ʱ
				if (this->gameMapVec[flag][i][j + 2] == 2 || this->gameMapVec[flag][i][j + 2] == 4) {
					// ��������Ե��ǿյػ�����Ŀ�ĵ�
					this->changOne(i, j); // ����뿪ԭλ��
					this->gameMapVec[flag][i][j + 1] += (9 - 1); // ��ҵ�������λ��
					this->gameMapVec[flag][i][j + 2] += 1; // ���ӵ�����λ��
				}
			}
			break;
		}
	}
}

// ���������ж�
int Model::over(const int grid_num) {
	// ״̬��ʶ�������ж���Ϸ�Ƿ����
	int countFlag = 1;
	for (int i = 0; i < grid_num; i++) {
		for (int j = 0; j < grid_num; j++) {
			if (this->gameMapVec[flag][i][j] == 3) {
				// �����Ϸ���Դ���δ��Ŀ�ĵص���������Ϸ��δ����
				countFlag = 0;
				return countFlag;
			}
		}
	}
	// �����Ϸ�в�����δ��Ŀ�ĵص���������Ϸ����
	return countFlag;
}

// ��Ϸ������������
void Model::action(IMAGE& bk, vector<IMAGE>& img, const int grid_num, const int pic_size, const int width, const int height) {
	IMAGE end, endy;
	// ����ͼƬ
	loadimage(&end, "./resource/picture/end.bmp");
	loadimage(&endy, "./resource/picture/endy.bmp");

	// ��ʼ��������
	double startX = width / 2 - 204.0 / 2;
	double startY = height;

	while (startY >= height / 2 - 54.0 / 2) {
		this->draw(bk, img, grid_num, pic_size);
		putimage(startX, startY, &endy, SRCAND); // ������ͼ
		putimage(startX, startY, &end, SRCPAINT); // ��Դ��ͼ
		FlushBatchDraw();
		startY -= 0.25; // ���ƶ����ٶ�
		Sleep(0); // ���ƶ����ٶ�
	}
}