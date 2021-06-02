#include "Model.h"
using namespace std;

Model::Model() :score(0), top(0), left(0), down(0), right(0) {}

// ��ȡscore
int& Model::getScore() {
	return this->score;
}

// ������Դ
void Model::load(IMAGE& bk, vector<IMAGE>& img, vector<int>& imgIndex, const int grid_length, const int width, const int height, const int photo_num) {
	// ��������ͼƬ
	for (int i = 0; i < photo_num; i++) {
		// ��������
		char fileName[25]; // �����ַ����飬������Ŵ�����չ����ͼƬ����
		sprintf(fileName, "./resource/picture/%d.bmp", imgIndex[i]);
		loadimage(&img[i], fileName, grid_length, grid_length);
	}
	// ���ر���ͼƬ
	loadimage(&bk, "./resource/picture/bk.jpg", width, height);
}

// ��������
void Model::bgm() {
	mciSendString("open ./resource/music/bk.mp3 alias BGM", nullptr, 0, nullptr);
	mciSendString("play BGM repeat", nullptr, 0, nullptr);
}

// ��ʼ������
void Model::init(const int grid_num, const int grid_length, const int width, const int height) {
	// ��ʼ����Ϸ��������
	this->left = width / 2 - (grid_num * grid_length) / 2; // ��Ϸ�������Ͻ�x����
	this->top = height / 2 - (grid_num * grid_length) / 2; // ��Ϸ�������Ͻ�y����
	this->right = this->left + (grid_num * grid_length); // ��Ϸ�������½�x����
	this->down = this->top + (grid_num * grid_length); // ��Ϸ�������½�y����

	// ��ʼ��vector����
	this->gameMapVec.clear();
	for (int i = 0; i < grid_num; i++) {
		vector<int> lineGrid;
		for (int j = 0; j < grid_num; j++) {
			lineGrid.push_back(0);
		}
		this->gameMapVec.push_back(lineGrid);
	}
}

// ��ʼ�������
void Model::welcome(IMAGE& bk) {
	putimage(0, 0, &bk);
	setbkmode(TRANSPARENT);
	SetWindowText(GetHWnd(), "C++2048��Ϸ");

	// ���������Ϣ
	setbkcolor(WHITE);
	settextstyle(52, 0, "����", 0, 0, FW_LIGHT, 0, 0, 0);
	settextcolor(RGB(130, 130, 130));
	outtextxy(218, 440, "2048��Ϸ");

	while (!_kbhit()) {
		settextstyle(26, 0, "����", 0, 0, FW_EXTRABOLD, 0, 0, 0);
		settextcolor(RGB(245, 245, 245));
		outtextxy(232, 500, "�������������");
	}
}

// ��Ϸ�������
void Model::draw(IMAGE& bk, vector<IMAGE>& img, vector<int>& imgIndex, const int grid_num, const int grid_length, const int photo_num) {
	cleardevice();
	putimage(0, 0, &bk);

	// ���������Ϣ
	settextcolor(WHITE);
	settextstyle(35, 0, "����", 0, 0, FW_NORMAL, 0, 0, 0);
	outtextxy(28, 15, "2048��Ϸ");
	settextstyle(30, 0, "����", 0, 0, 1000, 0, 0, 0);
	outtextxy(20, 60, "���ߣ�����");

	// ����Ϸ�������������м���
	for (int x = 3; x > -2; x--) {
		setlinecolor(RGB(244, 215, 215));
		rectangle(this->left - x, this->top - x, this->right + x, this->down + x);
	}

	// ������߿�
	for (int x = 22; x > 12; x--) {
		setlinecolor(RGB(244, 215, 215)); // ������߿���ɫ
		rectangle(this->left - x, this->top - x, this->right + x, this->down + x);
	}

	// ����ͷ���߿�
	for (int x = 10; x > 0; x--) {
		setlinecolor(RGB(244, 215, 215)); // ����ͷ���߿���ɫ
		rectangle(this->left - 12 - x, this->top - 122 - x, this->right + 12 + x, this->top - 42 + x);
	}

	// ����ͷ���߿��ڲ�
	setfillcolor(WHITE); // �Թ��峤���ν����ڲ���ɫ�����
	solidrectangle(this->left - 12 + 13, this->top - 122 + 13, this->right + 12 - 13, this->top - 42 - 13);

	// ��ͷ����ɫ�߿��������м���
	for (int x = 3; x > -5; x--) {
		setlinecolor(RGB(244, 215, 215)); // ������߿���ɫ
		rectangle(this->left - 12 + 10 - x, this->top - 122 + 10 - x, this->right + 12 - 10 + x, this->top - 42 - 10 + x);
	}

	int k;
	// ���ݶ�ά�����е�ֵȥ��ͼ
	for (int i = 0; i < grid_num; i++) {
		for (int j = 0; j < grid_num; j++) {
			// �������±�ת��Ϊ����
			int x = this->left + j * grid_length;
			int y = this->top + i * grid_length;
			for (k = 0; k < photo_num; k++) {
				// �����ǰԪ�ض�Ӧ��ͼƬ���
				if (imgIndex[k] == this->gameMapVec[i][j]) {
					break;
				}
			}
			// ����Ӧ��ŵ�ͼƬ
			putimage(x, y, &img[k]);
		}
	}

	// ����ͷ������
	settextcolor(RGB(244, 215, 215));
	settextstyle(35, 0, "����", 0, 0, 1000, 0, 0, 0);
	outtextxy((this->left + this->right) / 2 - 87, 142, "�÷֣�");

	settextcolor(RGB(244, 215, 215));
	settextstyle(45, 0, "Times New Roman", 0, 0, FW_BOLD, 0, 0, 0);
	char scoreName[20]; // �����ַ����飬�������ַ�����ʽ��ŵ÷�
	sprintf(scoreName, "%d ", this->score); // �÷�д��scoreName����
	outtextxy((this->left + this->right) / 2 + 20, 137, scoreName);
}

// ��Ϸ��������1��û�и���Ϊ0����Ϸ����
bool Model::condition1(const int grid_num) {
	int count = 0;
	for (int i = 0; i < grid_num; i++) {
		for (int j = 0; j < grid_num; j++) {
			if (this->gameMapVec[i][j] != 0) {
				count++;
			}
		}
	}
	if (count == grid_num * grid_num) {
		return true; // ��Ϸ����
	}
	else {
		return false; // ��Ϸ����
	}
}

// ��Ϸ��������2�����ڣ����£����ң���ֵ�����
bool Model::condition2(const int grid_num) {
	int count = 0;
	for (int i = 0; i < grid_num; i++) {
		for (int j = 0; j < grid_num - 1; j++) {
			if ((this->gameMapVec[i][j] == this->gameMapVec[i][j + 1] || this->gameMapVec[j][i] == this->gameMapVec[j + 1][i])) {
				count++;
			}
		}
	}
	if (count == 0) {
		return true; // ��Ϸ����
	}
	else {
		return false; // ��Ϸ����
	}
}

// �ж���Ϸ�Ƿ����
bool Model::over(const int grid_num) {
	if (this->condition1(grid_num) && this->condition2(grid_num)) {
		return true;
	}
	else {
		return false;
	}
}

// �������һ��������
int Model::randIntNum(const int grid_num) {
	srand((unsigned)(time(nullptr) + rand()));

	while (!this->over(grid_num)) {
		int i = rand() % 4;
		int j = rand() % 4;
		if (this->gameMapVec[i][j] == 0 || condition1(grid_num)) {
		out:
			this->gameMapVec[i][j] = (rand() % 3) * 2; // ���������0,2,4
			// ����0�Ļ����ôβ�����ֵ����
			if (this->gameMapVec[i][j] == 0) {
				goto out;
			}
			break;
		}
	}
	return 0; // ����ֵΪ0������һ�����������ص������Ϊ2����4
}

// �����ƶ�
int Model::moveLeft(const int grid_num) {
	int flag = 0; // ��Ϸ�������
	for (int i = 0; i < grid_num; i++) {
		for (int j = 0; j < grid_num; j++) {
			int curKey = this->gameMapVec[i][j]; // ��ǰԪ�ص�ֵ
			if (curKey != 0) {
				int k = j + 1;
				while (k < grid_num) {
					int curKeyNext = this->gameMapVec[i][k]; // ��ǰԪ�ص���һ��
					if (curKeyNext != 0) {
						// ������ڵ�����ͬ����ô�ͺϲ�
						if (curKey == curKeyNext) {
							flag = 1;
							this->gameMapVec[i][j] += this->gameMapVec[i][k];
							this->gameMapVec[i][k] = 0;
							this->score += this->gameMapVec[i][j]; // ��¼�÷�
						}
						k = 4; // �˳�ѭ��
						break;
					}
					k++; // �ж�������λ��
				}
			}
		}
	}
	for (int i = 0; i < grid_num; i++) {
		for (int j = 0; j < grid_num - 1; j++) {
			int curKey = this->gameMapVec[i][j];
			if (curKey == 0) {
				int k = j + 1;
				while (k < grid_num) {
					int curKeyNext = this->gameMapVec[i][k];
					if (curKeyNext != 0) {
						flag = 1;
						this->gameMapVec[i][j] = curKeyNext; // �ѷ�0Ԫ���Ƶ�0��λ��
						this->gameMapVec[i][k] = 0; // �ƶ����Ϊ0
						k = 4;
					}
					k++; // ����λ��Ҳ��Ҫ�ж�
				}
			}
		}
	}
	if (flag)
		return 0;
	else
		return 4;
}

// �����ƶ�
int Model::moveRight(const int grid_num) {
	int flag = 0; // ��Ϸ������־
	for (int i = 0; i < grid_num; i++) {
		for (int j = grid_num - 1; j >= 0; j--) {
			int curkey = this->gameMapVec[i][j]; // ��ǰԪ�ص�ֵ
			if (curkey != 0) {
				int k = j - 1;
				while (k >= 0) {
					int curkeyNext = this->gameMapVec[i][k]; // ��ǰԪ�ص������һ��Ԫ�ص�ֵ
					if (curkeyNext != 0) {
						// ������ڵ�����ͬ����ô���Ǿͺϲ�
						if (curkey == curkeyNext) {
							flag = 1;
							this->gameMapVec[i][j] += this->gameMapVec[i][k];
							this->gameMapVec[i][k] = 0;
							this->score += this->gameMapVec[i][j]; // ��¼�÷�
						}
						k = -1; // �˳�ѭ��
						break;
					}
					k--; // �ж�������λ��
				}
			}
		}
	}
	for (int i = 0; i < grid_num; i++) {
		for (int j = grid_num - 1; j > 0; j--) {
			int curkey = this->gameMapVec[i][j]; // ��ǰԪ�ص�ֵ
			if (curkey == 0) {
				int k = j - 1;
				while (k >= 0) {
					int curkeyNext = this->gameMapVec[i][k]; // ��ǰԪ�ص������һ��Ԫ�ص�ֵ
					if (curkeyNext != 0) {
						flag = 1;
						this->gameMapVec[i][j] = curkeyNext; // �ѷ���Ԫ���Ƶ�0��λ��
						this->gameMapVec[i][k] = 0; // �ƶ����Ϊ0
						k = -1;
					}
					k--; // ����λ��Ҳ��Ҫ�ж�
				}
			}
		}
	}
	if (flag) {
		return 0;
	}
	else {
		return 4;
	}
}

// �����ƶ�
int Model::moveUp(const int grid_num) {
	int flag = 0;
	for (int i = 0; i < grid_num; i++) {
		for (int j = 0; j < grid_num; j++) {
			int curKey = this->gameMapVec[i][j];
			if (curKey != 0) {
				int k = i + 1;
				while (k < grid_num) {
					int curKeyNext = this->gameMapVec[k][j];
					if (curKeyNext != 0) {
						if (curKey == curKeyNext) {
							flag = 1;
							this->gameMapVec[i][j] += this->gameMapVec[k][j];
							this->gameMapVec[k][j] = 0;
							this->score += this->gameMapVec[i][j]; // ��¼�÷�
						}
						k = 4;
						break;
					}
					k++;
				}
			}
		}
	}
	for (int i = 0; i < grid_num - 1; i++) {
		for (int j = 0; j < grid_num; j++) {
			int curKey = this->gameMapVec[i][j];
			if (curKey == 0) {
				int k = i + 1;
				while (k < grid_num) {
					int curKeyNext = this->gameMapVec[k][j];
					if (curKeyNext != 0) {
						flag = 1;
						this->gameMapVec[i][j] = curKeyNext;
						this->gameMapVec[k][j] = 0;
						k = 4;
					}
					k++;
				}
			}
		}
	}
	if (flag)
		return 0;
	else
		return 4;
}

// �����ƶ�
int Model::moveDown(const int grid_num) {
	int flag = 0;
	for (int i = grid_num - 1; i >= 0; i--) {
		for (int j = 0; j < grid_num; j++) {
			int curKey = this->gameMapVec[i][j];
			if (curKey != 0) {
				int k = i - 1;
				while (k >= 0) {
					int curKeyNext = this->gameMapVec[k][j];
					if (curKeyNext != 0) {
						if (this->gameMapVec[i][j] == this->gameMapVec[k][j]) {
							flag = 1;
							this->gameMapVec[i][j] += this->gameMapVec[k][j];
							this->gameMapVec[k][j] = 0;
							this->score += this->gameMapVec[i][j];//��¼�÷�
						}
						k = -1;
						break;
					}
					k--;
				}
			}
		}
	}
	for (int i = grid_num - 1; i > 0; i--) {
		for (int j = 0; j < grid_num; j++) {
			int curKey = this->gameMapVec[i][j];
			if (curKey == 0) {
				int k = i - 1;
				while (k >= 0) {
					int curKeyNext = this->gameMapVec[k][j];
					if (curKeyNext != 0) {
						flag = 1;
						this->gameMapVec[i][j] = curKeyNext;
						this->gameMapVec[k][j] = 0;
						k = -1;
					}
					k--;
				}
			}
		}
	}
	if (flag)
		return 0;
	else
		return 4;
}

// ������Ӧ
void Model::keyDown(const int grid_num) {
	// �ж����ް�����Ϣ
	while (_kbhit()) {
		// ���ڽ��հ�����Ϣ
		char key = _getch();
		switch (key) {
		case 'W':
		case 'w':
		case 72: // �Ϸ����С����
			this->moveUp(grid_num);
			this->randIntNum(grid_num);
			break;
		case 'S':
		case 's':
		case 80: // �·����С����
			this->moveDown(grid_num);
			this->randIntNum(grid_num);
			break;
		case 'A':
		case 'a':
		case 75: // �����С����
			this->moveLeft(grid_num);
			this->randIntNum(grid_num);
			break;
		case 'D':
		case 'd':
		case 77: // �ҷ����С����
			this->moveRight(grid_num);
			this->randIntNum(grid_num);
			break;
		}
	}
}