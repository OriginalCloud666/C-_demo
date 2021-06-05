#include "Model.h"
using namespace std;

Model::Model() :top(0), left(0), down(0), right(0) {}

// ������Դ
void Model::load(IMAGE& bk, IMAGE& white) {
	// ����ͼƬ
	loadimage(&bk, "./resource/picture/bk.jpg", width, height);
	loadimage(&white, "./resource/picture/white.jpg", 533, 300);
}

// ���ñ�������
void Model::bgm() {
	mciSendString("open ./resource/music/bk.mp3 alias BGM", nullptr, 0, nullptr);
	mciSendString("play BGM repeat", nullptr, 0, nullptr);
}

// ��ʼ����Ϸ����
void Model::init() {
	// �������������
	srand((unsigned)time(nullptr) * rand());

	// ��ʼ����Ϸ��������
	this->left = width / 2 - (grid_num * grid_width) / 2;
	this->top = height / 2 - (grid_num * grid_height) / 2;
	this->right = this->left + grid_num * grid_width;
	this->down = this->top + grid_num * grid_height;

	// ��ʼ��vector����
	this->gameMapVec.clear();
	for (int i = 0; i < grid_num; i++) {
		vector<int> lineGrid;
		for (int j = 0; j < grid_num; j++) {
			lineGrid.push_back(i * grid_num + j);
		}
		this->gameMapVec.push_back(lineGrid);
	}

	// �������vector������ֵ
	for (int i = 0; i < grid_num * grid_num; i++) {
		int m = rand() % grid_num;
		int n = rand() % grid_num;
		int temp = this->gameMapVec[i / grid_num][i % grid_num];
		this->gameMapVec[i / grid_num][i % grid_num] = this->gameMapVec[m][n];
		this->gameMapVec[m][n] = temp;
	}
}

// ���ƿ�ʼ����
void Model::welcome(IMAGE& bk) {
	putimage(0, 0, &bk);
	setbkmode(TRANSPARENT);
	SetWindowText(GetHWnd(), "C++����ƴͼ��Ϸ");

	// ���������Ϣ
	settextcolor(RGB(220, 220, 220));
	settextstyle(31, 0, "����", 0, 0, 1000, 0, 0, 0);
	outtextxy(16, 12, "ƴͼ��Ϸ");

	settextcolor(RGB(215, 215, 215));
	settextstyle(26, 0, "����", 0, 0, 1000, 0, 0, 0);
	outtextxy(13, 52, "���ߣ�����");

	while (!_kbhit()) {
	}
}

// ������Ϸ����
void Model::draw(IMAGE& bk, IMAGE& white) {
	cleardevice();

	// ��Ϸ���������ͼ
	for (int i = 0; i < grid_num; i++) {
		for (int j = 0; j < grid_num; j++) {
			int x = this->left + grid_width * j;
			int y = this->top + grid_height * i;
			if (this->gameMapVec[i][j] == 15) {
				putimage(x, y, grid_width, grid_height, &white, 105, 95);
			}
			else {
				putimage(x, y, grid_width, grid_height, &bk, (this->gameMapVec[i][j] % grid_num) * grid_width, (this->gameMapVec[i][j] / grid_num) * grid_height);
			}
		}
	}
}

// ��λ�׿�i
int Model::searchArray_i() {
	for (int i = 0; i < grid_num; i++) {
		for (int j = 0; j < grid_num; j++) {
			if (this->gameMapVec[i][j] == 15) {
				return i;
			}
		}
	}
	return -1; // ���û���ҵ����򷵻�-1
}

// ��λ�׿�j
int Model::searchArray_j() {
	for (int i = 0; i < grid_num; i++) {
		for (int j = 0; j < grid_num; j++) {
			if (this->gameMapVec[i][j] == 15) {
				return j;
			}
		}
	}
	return -1; // ���û���ҵ����򷵻�-1
}

// ������
void Model::mouseControl() {
	if (MouseHit()) { // �ж��Ƿ��������Ϣ
		MOUSEMSG msg = GetMouseMsg();
		// �ж��û�������Ƿ�����Ϸ����
		if (msg.x >= this->left && msg.x <= this->right &&
			msg.y >= this->top && msg.y <= this->down) {
			// ���������ת��Ϊ�����±�
			int row = (msg.y - this->top) / grid_height; // ��
			int col = (msg.x - this->left) / grid_width; // ��
			// ��Ѱ��ȡ�׿�ĺ�������
			int white_i = this->searchArray_i();
			int white_j = this->searchArray_j();
			// �������Ϣ���зַ�
			switch (msg.uMsg) {
			case WM_LBUTTONDOWN: // ���������
				mciSendString("close click", nullptr, 0, nullptr);
				mciSendString("open ./resource/music/click.wav alias click", nullptr, 0, nullptr);
				mciSendString("play click", nullptr, 0, nullptr);
				// �ж��ĸ�������ƶ�
				if (row == white_i + 1 && col == white_j) {//��
					this->gameMapVec[white_i][white_j] = this->gameMapVec[row][col];
					this->gameMapVec[row][col] = 15;
				}
				if (row == white_i - 1 && col == white_j) {//��
					this->gameMapVec[white_i][white_j] = this->gameMapVec[row][col];
					this->gameMapVec[row][col] = 15;
				}
				if (row == white_i && col == white_j + 1) {//��
					this->gameMapVec[white_i][white_j] = this->gameMapVec[row][col];
					this->gameMapVec[row][col] = 15;
				}
				if (row == white_i && col == white_j - 1) {//��
					this->gameMapVec[white_i][white_j] = this->gameMapVec[row][col];
					this->gameMapVec[row][col] = 15;
				}
			}
		}
	}
}

// ��Ϸ�����ж�
bool Model::over() {
	int count = 0;
	for (int i = 0; i < grid_num; i++) {
		for (int j = 0; j < grid_num; j++) {
			if (this->gameMapVec[i][j] != count) {
				return false; // ��Ϸδ�����򷵻�false
			}
			count++;
		}
	}
	return true; // ��Ϸ�����򷵻�true
}