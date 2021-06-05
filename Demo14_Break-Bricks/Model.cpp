#include "Model.h"
using namespace std;

Model::Model() :top(0), left(0), down(0), right(0) {
	this->gameMapVec = {
		{ 0, 1, 0, 1, 0, 1, 0, 1 },
		{ 1, 0, 1, 0, 1, 0, 1, 0 },
		{ 0, 2, 0, 2, 0, 2, 0, 2 },
		{ 2, 0, 2, 0, 2, 0, 2, 0 },
		{ 0, 3, 0, 3, 0, 3, 0, 3 },
		{ 3, 0, 3, 0, 3, 0, 3, 0 },
		{ 0, 4, 0, 4, 0, 4, 0, 4 },
		{ 4, 0, 4, 0, 4, 0, 4, 0 },
	};
}

// ��ȡball
struct Ball& Model::getBall() {
	return this->ball;
}

// ������Դ
void Model::load(IMAGE& bk, const int width, const int height) {
	loadimage(&bk, "./resource/picture/bk.jpg", width, height * 1.58);
}

// ���ñ�������
void Model::bgm() {
	mciSendString("open ./resource/music/bk.mp3 alias BGM", nullptr, 0, nullptr);
	mciSendString("play BGM repeat", nullptr, 0, nullptr);
}

//��Ϸ���ݳ�ʼ��
void Model::init(const int brick_width, const int brick_height, const int brick_row, const int brick_col, const int width, const int height) {
	// ש�����ݳ�ʼ��
	this->left = width / 2 - (brick_width * brick_col) / 2;
	this->top = height / 2 - (brick_height * (brick_row + 16)) / 2;
	this->right = this->left + brick_width * brick_col;
	this->down = this->top + brick_height * brick_row;

	// ľ�����ݳ�ʼ��
	this->board.width = 200;
	this->board.height = 30;
	this->board.step = 3.5;
	this->board.x = (double)width / 2 - this->board.width / 2;
	this->board.y = (double)height - this->board.height;

	// С�����ݳ�ʼ��
	this->ball.r = 15;
	this->ball.x = this->board.x + this->board.width / 2;
	this->ball.y = this->board.y - this->ball.r;
	this->ball.dx = 2.2;
	this->ball.dy = 2.2;
	this->ball.step = sqrt(this->ball.dx * this->ball.dx + this->ball.dy * this->ball.dy);
	this->ball.radian = atan2(this->ball.dy, -this->ball.dx);
}

// ���ƿ�ʼ����
void Model::welcome(IMAGE& bk) {
	putimage(0, -290, &bk);
	setbkcolor(RGB(200, 191, 231));
	setbkmode(TRANSPARENT);
	SetWindowText(GetHWnd(), "C++��ש����Ϸ");

	// ���������Ϣ
	settextcolor(RGB(215, 215, 215));
	settextstyle(28, 0, "����", 0, 0, 1000, 0, 0, 0);
	outtextxy(13, 16, "��ש����Ϸ");

	settextcolor(RGB(215, 215, 215));
	settextstyle(27, 0, "����", 0, 0, 1000, 0, 0, 0);
	outtextxy(15, 55, "���ߣ�����");

	while (!_kbhit()) {
	}
}

// ������Ϸ����
void Model::draw(const int brick_width, const int brick_height, const int brick_row, const int brick_col, const int height) {
	cleardevice();

	// ���ƻҰ�ɫ�ֽ���
	setfillcolor(RGB(237, 234, 247));
	solidrectangle(this->left, this->top, this->right, this->down / 2);

	// ���ƻҵ�������ɫ�ֽ���
	setfillcolor(RGB(232, 227, 244));
	solidrectangle(this->left, this->down / 2, this->right, this->down);

	// ���Ƶ�����ɫ�ֽ�
	setfillcolor(RGB(225, 220, 241));
	solidrectangle(this->left, this->down, this->right, this->down + (height - brick_row * brick_height) / 3 - 10);

	// ���Ƶ���ɫ�ֽ�
	setfillcolor(RGB(214, 207, 237));
	solidrectangle(this->left, this->down + (height - brick_row * brick_height) / 3 - 10, this->right, this->down + (height - brick_row * brick_height) * 2 / 3 - 20);

	// ����ש��
	for (int i = 0; i < brick_row; i++) {
		for (int j = 0; j < brick_col; j++) {
			// �������±�ת��Ϊ����
			int x = this->left + brick_width * j;
			int y = this->top + brick_height * i;
			// ��������Ϣ���зַ�
			switch (this->gameMapVec[i][j]) {
			case -1:
				break;
			case 0:
				setlinecolor(RGB(255, 224, 90));
				setlinestyle(PS_SOLID, 2);
				setfillcolor(RGB(255, 204, 0));
				fillrectangle(x, y, x + brick_width, y + brick_height);
				break;
			case 1:
				setlinecolor(RGB(255, 183, 218));
				setlinestyle(PS_SOLID, 2);
				setfillcolor(RGB(255, 153, 204));
				fillrectangle(x, y, x + brick_width, y + brick_height);
				break;
			case 2:
				setlinecolor(RGB(120, 215, 255));
				setlinestyle(PS_SOLID, 2);
				setfillcolor(RGB(30, 167, 225));
				fillrectangle(x, y, x + brick_width, y + brick_height);
				break;
			case 3:
				setlinecolor(RGB(190, 229, 127));
				setlinestyle(PS_SOLID, 2);
				setfillcolor(RGB(128, 190, 31));
				fillrectangle(x, y, x + brick_width, y + brick_height);
				break;
			case 4:
				setlinecolor(RGB(255, 160, 98));
				setlinestyle(PS_SOLID, 2);
				setfillcolor(RGB(232, 106, 23));
				fillrectangle(x, y, x + brick_width, y + brick_height);
				break;
			}
		}
	}

	// ����ľ��
	setlinecolor(RGB(100, 50, 0));
	setlinestyle(PS_SOLID, 2);
	setfillcolor(RGB(115, 57, 0));
	fillrectangle(this->board.x, this->board.y, this->board.x + this->board.width, this->board.y + this->board.height);

	// ����С��
	setlinecolor(RGB(220, 220, 220));
	setlinestyle(PS_SOLID, 2);
	setfillcolor(RGB(110, 110, 110));
	solidcircle(this->ball.x, this->ball.y, this->ball.r);
}

// ����������
double Model::distance(const double x1, const double y1, const double x2, const double y2) {
	// distance=������((x2-x1)^2+(y2-y1)^2)
	return sqrt((x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2));
}

// ľ���ƶ����첽�������ƣ�
void Model::keyDown1(const int width) {
	if (GetAsyncKeyState(VK_LEFT) || GetAsyncKeyState('A')) { // �����ƶ�
		this->board.x -= this->board.step;
		if (this->board.x <= 0) { // �������ƽ���
			this->board.x = 0;
		}
	}
	if (GetAsyncKeyState(VK_RIGHT) || GetAsyncKeyState('D')) { // �����ƶ�
		this->board.x += this->board.step;
		if (this->board.x >= (double)width - this->board.width) { // �������ƽ���
			this->board.x = (double)width - this->board.width;
		}
	}
}

// С������ƣ�ͬ���������ƣ�
void Model::keyDown2() {
	while (_kbhit()) {
		// ���ڽ����û�����
		char userKey = _getch();
		fflush(stdin); // ��ռ��̻�����

		switch (userKey) {
		case' ': // �ո���Ʒ��俪��
			this->ball.flag = 1;
			mciSendString("close click", nullptr, 0, nullptr);
			mciSendString("open ./resource/music/click.wav alias click", nullptr, 0, nullptr);
			mciSendString("play click", nullptr, 0, nullptr);
			break;
		}
	}
}

// С����ƶ���ײ
void Model::ballMove(const int width, const int height, const double pi) {
	// ��С���������ұ�ʱ
	if (((this->ball.x <= this->ball.r) || (this->ball.x >= width - this->ball.r)) && (this->ball.y <= (double)height - this->board.height)) {
		this->ball.dx = -this->ball.dx; // ������
		if (this->ball.x <= this->ball.r) {
			this->ball.x = this->ball.r;
		}
		if (this->ball.x >= width - this->ball.r) {
			this->ball.x = width - this->ball.r;
		}
	}
	// ��С�������ϱ�ʱ
	if (this->ball.y <= this->ball.r) {
		this->ball.dy = -this->ball.dy; // ������
		if (this->ball.y <= this->ball.r) {
			this->ball.y = this->ball.r;
		}
	}

	// ��С������ľ�����ұ�ʱ
	if (this->ball.y > this->board.y + 2 && this->ball.y < this->board.y + this->board.height - 2 &&
		this->ball.x > this->board.x - this->ball.r + 2 && this->ball.x < this->board.x + this->board.width + this->ball.r - 2) {
		this->ball.dx = -this->ball.dx; // ������
		if (fabs(this->board.x - this->ball.x) < this->ball.r && this->board.x - this->ball.x > 0) {
			this->ball.x = this->board.x - this->ball.r;
		}
		if (fabs(this->ball.x - (this->board.x + this->board.width)) < this->ball.r && this->ball.x - (this->board.x + this->board.width) > 0) {
			this->ball.x = this->board.x + this->board.width + this->ball.r;
		}
	}
	// ��С������ľ�����±�ʱ
	if (this->ball.x > this->board.x + 2 && this->ball.x < this->board.x + this->board.width - 2 &&
		this->ball.y > this->board.y - this->ball.r + 2 && this->ball.y < this->board.y + this->board.height + this->ball.r - 2) {
		this->ball.dy = -this->ball.dy; // ������
		if (fabs(this->board.y - this->ball.y) < this->ball.r && this->board.y - this->ball.y > 0) {
			this->ball.y = this->board.y - this->ball.r;
		}
		if (fabs(this->ball.y - (this->board.y + this->board.height)) < this->ball.r && this->ball.y - (this->board.y + this->board.height) > 0) {
			this->ball.y = this->board.y + this->board.height + this->ball.r;
		}
	}

	// ��С������ľ���ĸ���ʱ
	if (this->distance(this->ball.x, this->ball.y, this->board.x, this->board.y) <= this->ball.r - 1) { // ���Ͻ�
		this->ball.radian = atan2(this->ball.y - this->board.y, this->ball.x - this->board.x);
		// ��ײ���ǶȽ�������
		if (this->ball.radian <= -pi * 7 / 8) {
			this->ball.radian = -pi * 7 / 8;
		}
		else if (this->ball.radian >= -pi * 5 / 8) {
			this->ball.radian = -pi * 5 / 8;
		}
		this->ball.dx = cos(this->ball.radian) * this->ball.step;
		this->ball.dy = sin(this->ball.radian) * this->ball.step;
		this->ball.dx = -this->ball.dx; // ������
		this->ball.dy = -this->ball.dy; // ������
		this->ball.x = this->board.x + cos(this->ball.radian) * this->ball.r;
		this->ball.y = this->board.y + sin(this->ball.radian) * this->ball.r;
	}
	else if (this->distance(this->ball.x, this->ball.y, this->board.x + this->board.width, this->board.y) <= this->ball.r - 1) { // ���Ͻ�
		this->ball.radian = atan2(this->ball.y - this->board.y, this->ball.x - (this->board.x + this->board.width));
		// ��ײ���ǶȽ�������
		if (this->ball.radian <= -pi * 3 / 8) {
			this->ball.radian = -pi * 3 / 8;
		}
		else if (this->ball.radian >= -pi * 1 / 8) {
			this->ball.radian = -pi * 1 / 8;
		}
		this->ball.dx = cos(this->ball.radian) * this->ball.step;
		this->ball.dy = sin(this->ball.radian) * this->ball.step;
		this->ball.dx = -this->ball.dx; // ������
		this->ball.dy = -this->ball.dy; // ������
		this->ball.x = this->board.x + this->board.width + cos(this->ball.radian) * this->ball.r;
		this->ball.y = this->board.y + sin(this->ball.radian) * this->ball.r;
	}
	else if (this->distance(this->ball.x, this->ball.y, this->board.x, this->board.y + this->board.height) <= this->ball.r - 1) { // ���½�
		this->ball.radian = atan2(this->ball.y - (this->board.y + this->board.height), this->ball.x - this->board.x);
		// ��ײ���ǶȽ�������
		if (this->ball.radian <= pi * 5 / 8) {
			this->ball.radian = pi * 5 / 8;
		}
		else if (this->ball.radian >= pi * 7 / 8) {
			this->ball.radian = pi * 7 / 8;
		}
		this->ball.dx = cos(this->ball.radian) * this->ball.step;
		this->ball.dy = sin(this->ball.radian) * this->ball.step;
		this->ball.dx = -this->ball.dx; // ������
		this->ball.dy = -this->ball.dy; // ������
		this->ball.x = this->board.x + cos(this->ball.radian) * this->ball.r;
		this->ball.y = this->board.y + this->board.height + sin(this->ball.radian) * this->ball.r;
	}
	else if (this->distance(this->ball.x, this->ball.y, this->board.x + this->board.width, this->board.y + this->board.height) <= this->ball.r - 1) { // ���½�
		this->ball.radian = atan2(this->ball.y - (this->board.y + this->board.height), this->ball.x - (this->board.x + this->board.width));
		// ��ײ���ǶȽ�������
		if (this->ball.radian <= pi * 1 / 8) {
			this->ball.radian = pi * 1 / 8;
		}
		else if (this->ball.radian >= pi * 3 / 8) {
			this->ball.radian = pi * 3 / 8;
		}
		this->ball.dx = cos(this->ball.radian) * this->ball.step;
		this->ball.dy = sin(this->ball.radian) * this->ball.step;
		this->ball.dx = -this->ball.dx; // ������
		this->ball.dy = -this->ball.dy; // ������
		this->ball.x = this->board.x + this->board.width + cos(this->ball.radian) * this->ball.r;
		this->ball.y = this->board.y + this->board.height + sin(this->ball.radian) * this->ball.r;
	}

	if (this->ball.flag == 1) { // С����ľ�������
		if (this->ball.y >= height + this->ball.r || this->ball.x <= -this->ball.r || this->ball.x >= width + this->ball.r) {
			// С��ɳ���������»ص�ľ����
			this->ball.flag = 0;
		}
		this->ball.x -= this->ball.dx;
		this->ball.y -= this->ball.dy;
	}
	if (ball.flag == 0) { // С��δ����ľ��ʱ
		this->ball.x = this->board.x + this->board.width / 2;
		this->ball.y = this->board.y - this->ball.r - 2;
		this->ball.dx = 2.2;
		this->ball.dy = 2.2;
		this->ball.step = sqrt(this->ball.dx * this->ball.dx + this->ball.dy * this->ball.dy);
		this->ball.radian = atan2(this->ball.dy, -this->ball.dx);
	}
}

// С����ש����ƶ���ײ
void Model::ballCrash(const int brick_row, const int brick_col, const int brick_width, const int brick_height, const double pi) {
	for (int i = 0; i < brick_row; i++) {
		for (int j = 0; j < brick_col; j++) {
			// �������±�ת��Ϊ����
			int x = this->left + brick_width * j;
			int y = this->top + brick_height * i;
			// ��ש�鲻����ʱ��������ѭ��
			if (this->gameMapVec[i][j] == -1) {
				continue;
			}
			// ��С������ש�����±�ʱ
			if (this->ball.x > x + 2.0 && this->ball.x < x + (double)brick_width - 2 &&
				this->ball.y > y - this->ball.r + 2 && this->ball.y < y + (double)brick_height + this->ball.r - 2) {
				this->ball.dy = -this->ball.dy; // ������
				if (fabs(y - this->ball.y) < this->ball.r && y - this->ball.y > 0) {
					this->ball.y = y - this->ball.r;
				}
				if (fabs(this->ball.y - (y + (double)brick_height)) < this->ball.r && this->ball.y - (y + (double)brick_height) > 0) {
					this->ball.y = y + (double)brick_height + this->ball.r;
				}
				this->gameMapVec[i][j] = -1;
				mciSendString("close crash", nullptr, 0, nullptr);
				mciSendString("open ./resource/music/crash.wav alias crash", nullptr, 0, nullptr);
				mciSendString("play crash", nullptr, 0, nullptr);
			}
			// ��С������ש�����ұ�ʱ
			if (this->ball.y > y + 2.0 && this->ball.y < y + (double)brick_height - 2 &&
				this->ball.x > x - this->ball.r + 2 && this->ball.x < x + (double)brick_width + this->ball.r - 2) {
				this->ball.dx = -ball.dx;//������
				if (fabs(x - this->ball.x) < this->ball.r && x - this->ball.x > 0) {
					this->ball.x = x - this->ball.r;
				}
				if (fabs(this->ball.x - (x + (double)brick_width)) < this->ball.r && this->ball.x - (x + (double)brick_width) > 0) {
					this->ball.x = x + (double)brick_width + this->ball.r;
				}
				this->gameMapVec[i][j] = -1;
				mciSendString("close crash", nullptr, 0, nullptr);
				mciSendString("open ./resource/music/crash.wav alias crash", nullptr, 0, nullptr);
				mciSendString("play crash", nullptr, 0, nullptr);
			}
			// ��С������ש���ĸ���ʱ��ײ���ı߽ǲ�ͬ���Ƕȵļ��㷽ʽ��ͬ
			if (this->distance(this->ball.x, this->ball.y, x, y) <= this->ball.r - 1 && this->ball.x < x && this->ball.y < y &&
				(this->gameMapVec[i][j - 1] == -1 || j == 0) && (this->gameMapVec[i - 1][j] == -1 || i == 0)) { // ���Ͻ�
				this->ball.radian = atan2(this->ball.y - y, this->ball.x - x);
				// ��ײ���ǶȽ�������
				if (this->ball.radian <= -pi * 7 / 8) {
					this->ball.radian = -pi * 7 / 8;
				}
				else if (this->ball.radian >= -pi * 5 / 8) {
					this->ball.radian = -pi * 5 / 8;
				}
				this->ball.dx = cos(this->ball.radian) * this->ball.step;
				this->ball.dy = sin(this->ball.radian) * this->ball.step;
				this->ball.dx = -this->ball.dx;//������
				this->ball.dy = -this->ball.dy;//������
				this->ball.x = x + cos(this->ball.radian) * this->ball.r;
				this->ball.y = y + sin(this->ball.radian) * this->ball.r;
				this->gameMapVec[i][j] = -1;
				mciSendString("close crash", nullptr, 0, nullptr);
				mciSendString("open ./resource/music/crash.wav alias crash", nullptr, 0, nullptr);
				mciSendString("play crash", nullptr, 0, nullptr);
			}
			else if (this->distance(this->ball.x, this->ball.y, x + (double)brick_width, y) <= this->ball.r - 1 &&
				this->ball.x > x + (double)brick_width && this->ball.y < y &&
				(this->gameMapVec[i][j + 1] == -1 || j == brick_col - 1) &&
				(this->gameMapVec[i - 1][j] == -1 || i == 0)) { // ���Ͻ�
				this->ball.radian = atan2(this->ball.y - y, this->ball.x - (x + (double)brick_width));
				// ��ײ���ǶȽ�������
				if (this->ball.radian <= -pi * 3 / 8) {
					this->ball.radian = -pi * 3 / 8;
				}
				else if (this->ball.radian >= -pi * 1 / 8) {
					this->ball.radian = -pi * 1 / 8;
				}
				this->ball.dx = cos(this->ball.radian) * this->ball.step;
				this->ball.dy = sin(this->ball.radian) * this->ball.step;
				this->ball.dx = -this->ball.dx;//������
				this->ball.dy = -this->ball.dy;//������
				this->ball.x = x + (double)brick_width + cos(this->ball.radian) * this->ball.r;
				this->ball.y = y + sin(this->ball.radian) * this->ball.r;
				this->gameMapVec[i][j] = -1;
				mciSendString("close crash", nullptr, 0, nullptr);
				mciSendString("open ./resource/music/crash.wav alias crash", nullptr, 0, nullptr);
				mciSendString("play crash", nullptr, 0, nullptr);
			}
			else if (this->distance(this->ball.x, this->ball.y, x, y + (double)brick_height) <= this->ball.r - 1 &&
				this->ball.x < x && this->ball.y > y + (double)brick_height &&
				(this->gameMapVec[i][j - 1] == -1 || j == 0) &&
				(this->gameMapVec[i + 1][j] == -1 || i == brick_row - 1)) { // ���½�
				this->ball.radian = atan2(this->ball.y - (y + (double)brick_height), this->ball.x - x);
				// ��ײ���ǶȽ�������
				if (this->ball.radian <= pi * 5 / 8) {
					this->ball.radian = pi * 5 / 8;
				}
				else if (this->ball.radian >= pi * 7 / 8) {
					this->ball.radian = pi * 7 / 8;
				}
				this->ball.dx = cos(this->ball.radian) * this->ball.step;
				this->ball.dy = sin(this->ball.radian) * this->ball.step;
				this->ball.dx = -this->ball.dx;//������
				this->ball.dy = -this->ball.dy;//������
				this->ball.x = x + cos(this->ball.radian) * this->ball.r;
				this->ball.y = y + (double)brick_height + sin(this->ball.radian) * this->ball.r;
				this->gameMapVec[i][j] = -1;
				mciSendString("close crash", nullptr, 0, nullptr);
				mciSendString("open ./resource/music/crash.wav alias crash", nullptr, 0, nullptr);
				mciSendString("play crash", nullptr, 0, nullptr);
			}
			else if (this->distance(this->ball.x, this->ball.y, x + (double)brick_width, y + (double)brick_height) <= this->ball.r - 1 &&
				this->ball.x > x + (double)brick_width && this->ball.y > y + (double)brick_height &&
				(this->gameMapVec[i][j + 1] == -1 || j == brick_col - 1) &&
				(this->gameMapVec[i + 1][j] == -1 || i == brick_row - 1)) { // ���½�
				this->ball.radian = atan2(this->ball.y - (y + (double)brick_height), this->ball.x - (x + (double)brick_width));
				// ��ײ���ǶȽ�������
				if (this->ball.radian <= pi * 1 / 8) {
					this->ball.radian = pi * 1 / 8;
				}
				else if (this->ball.radian >= pi * 3 / 8) {
					this->ball.radian = pi * 3 / 8;
				}
				this->ball.dx = cos(this->ball.radian) * this->ball.step;
				this->ball.dy = sin(this->ball.radian) * this->ball.step;
				this->ball.dx = -this->ball.dx;//������
				this->ball.dy = -this->ball.dy;//������
				this->ball.x = x + (double)brick_width + cos(this->ball.radian) * this->ball.r;
				this->ball.y = y + (double)brick_height + sin(this->ball.radian) * this->ball.r;
				this->gameMapVec[i][j] = -1;
				mciSendString("close crash", nullptr, 0, nullptr);
				mciSendString("open ./resource/music/crash.wav alias crash", nullptr, 0, nullptr);
				mciSendString("play crash", nullptr, 0, nullptr);
			}
		}
	}
}

// ���������ж�
bool Model::over(const int brick_row, const int brick_col) {
	// �ж��������Ƿ�ȫ��Ϊ-1
	for (int i = 0; i < brick_row; i++) {
		for (int j = 0; j < brick_col; j++) {
			if (this->gameMapVec[i][j] != -1) {
				return false; // ��Ϸδ����
			}
		}
	}
	return true; // ��Ϸ����
}