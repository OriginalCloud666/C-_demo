#include "Model.h"
using namespace std;

Model::Model() :top(0), left(0), down(0), right(0) {}

// ������Դ
void Model::load(IMAGE& bk) {
	loadimage(&bk, "./resource/picture/bk.jpg", 924, 520);
}

// ��������
void Model::bgm() {
	mciSendString("open ./resource/music/bk.mp3 alias BGM", nullptr, 0, nullptr);
	mciSendString("play BGM repeat", nullptr, 0, nullptr);
}

// ��ʼ������
void Model::init() {
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
			lineGrid.push_back(1);
		}
		this->gameMapVec.push_back(lineGrid);
	}
}

// ��ʼ�������
void Model::welcome(IMAGE& bk) {
	putimage(-50, -49, &bk);
	setbkmode(TRANSPARENT);
	SetWindowText(GetHWnd(), "C++Ϳ������Ϸ");

	// ���������Ϣ
	settextcolor(RGB(100, 230, 230));
	settextstyle(50, 0, "����", 0, 0, 600, 0, 0, 0);
	outtextxy(180, 150, "Ϳ������Ϸ");
	settextstyle(30, 0, "����", 0, 0, 600, 0, 0, 0);
	outtextxy(220, 280, "���ߣ�����");

	// ��������������߿�
	for (int x = 10; x < 20; x++) {
		setlinecolor(RGB(0, 255 - 13 * (x - 10), 255 - 13 * (x - 10)));
		rectangle(x - 10, x - 10, width + 10 - x, height + 10 - x);
	}
	settextcolor(WHITE);
	settextstyle(30, 0, "����", 0, 0, 600, 0, 0, 0);
	outtextxy(200, 225, "�������������");

	while (!_kbhit()) {
	}
}

// ��Ϸ�������
void Model::draw(IMAGE& bk) {
	cleardevice();
	putimage(-50, -49, &bk);

	// ���Ƹ���
	setlinecolor(RGB(0, 200, 200));

	// ��������
	for (int x = this->left; x <= right; x += grid_length) {
		line(x, this->top, x, this->down);
	}

	// ���ƺ���
	for (int y = this->top; y <= this->down; y += grid_length) {
		line(this->left, y, this->right, y);
	}

	// ������߿�
	for (int x = 20; x > 10; x--) {
		setlinecolor(RGB(0, 255 - 13 * (x - 10), 255 - 13 * (x - 10)));
		rectangle(this->left - x, this->top - x, this->right + x, this->down + x);
	}

	// ��������������߿�
	for (int x = 10; x < 20; x++) {
		setlinecolor(RGB(0, 255 - 13 * (x - 10), 255 - 13 * (x - 10)));
		rectangle(x - 10, x - 10, width + 10 - x, height + 10 - x);
	}

	// �����������ݻ��Ƹ�����ɫ���ж��������ֵ��1����-1
	for (int i = 0; i < grid_num; i++) {
		for (int k = 0; k < grid_num; k++) {
			int x = k * grid_length + left; // ÿ���������Ͻǵ�x����
			int y = i * grid_length + top; // ÿ���������Ͻǵ�y����
			if (this->gameMapVec[i][k] == 1) {
				setfillcolor(BLACK);
				solidrectangle(x + 1, y + 1, x + grid_length - 1, y + grid_length - 1);
			}
			else {
				setfillcolor(RGB(0, 120, 120));
				solidrectangle(x + 1, y + 1, x + grid_length - 1, y + grid_length - 1);
			}
		}
	}
}

// �����Ϣ��Ӧ
void Model::mouseControl() {
	// �ж��Ƿ��������Ϣ
	if (MouseHit()) {
		// ��ȡ�����Ϣ
		MOUSEMSG msg = GetMouseMsg();

		// �ж��û��Ƿ���������Ϸ����
		if (msg.x >= this->left && msg.x <= this->right &&
			msg.y >= this->top && msg.y <= this->down &&
			msg.uMsg == WM_LBUTTONDOWN) {
			// ���������ת��Ϊ�����±�
			int row = (msg.y - this->top) / grid_length;
			int col = (msg.x - this->left) / grid_length;
			this->gameMapVec[row][col] = -this->gameMapVec[row][col];

			// �ı���Χ�ĸ�����
			if (row - 1 >= 0) {
				this->gameMapVec[row - 1][col] = -this->gameMapVec[row - 1][col];
			}
			if (row + 1 < grid_num) {
				this->gameMapVec[row + 1][col] = -this->gameMapVec[row + 1][col];
			}
			if (col - 1 >= 0) {
				this->gameMapVec[row][col - 1] = -this->gameMapVec[row][col - 1];
			}
			if (col + 1 < grid_num) {
				this->gameMapVec[row][col + 1] = -this->gameMapVec[row][col + 1];
			}
		}
	}
}

// ��Ϸ�����ж�
int Model::over() { // �ж��Ƿ�ﵽͨ������
	int count = 0; // ������
	for (int i = 0; i < grid_num; i++) {
		for (int k = 0; k < grid_num; k++) {
			if (this->gameMapVec[i][k] == -1) {
				count++;
			}
		}
	}
	return count;
}