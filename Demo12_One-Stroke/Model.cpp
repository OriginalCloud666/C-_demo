#include "Model.h"
using namespace std;

Model::Model() :top(0), left(0), down(0), right(0), flag(0) {
	this->gameMapVec = {
		{
			{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
			{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
			{ 0, 2, 1, 1, 1, 1, 1, 1, 1, 0 },
			{ 0, 0, 0, 0, 0, 1, 1, 1, 1, 0 },
			{ 0, 0, 0, 0, 0, 1, 1, 1, 1, 0 },
			{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
			{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
			{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
			{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
			{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
		},

		{
			{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
			{ 0, 0, 1, 1, 1, 1, 1, 1, 0, 0 },
			{ 0, 0, 1, 1, 1, 1, 1, 1, 0, 0 },
			{ 0, 0, 1, 1, 1, 1, 1, 1, 0, 0 },
			{ 0, 0, 1, 1, 2, 1, 1, 1, 0, 0 },
			{ 0, 0, 1, 1, 1, 1, 1, 1, 0, 0 },
			{ 0, 0, 1, 1, 1, 1, 1, 1, 0, 0 },
			{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
			{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
			{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
		},

		{
			{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
			{ 0, 0, 1, 1, 1, 1, 1, 1, 0, 0 },
			{ 0, 0, 1, 0, 0, 0, 0, 1, 0, 0 },
			{ 0, 0, 1, 0, 1, 1, 1, 1, 0, 0 },
			{ 0, 0, 1, 0, 1, 1, 0, 0, 0, 0 },
			{ 0, 0, 1, 1, 1, 1, 0, 0, 0, 0 },
			{ 0, 0, 1, 1, 2, 1, 0, 0, 0, 0 },
			{ 0, 0, 1, 1, 1, 1, 0, 0, 0, 0 },
			{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
			{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
		}
	};
}

// ��ȡcount
int& Model::getCount() {
	return this->count;
}

// ��ȡflag
int& Model::getFlag() {
	return this->flag;
}

// ��ȡrole
struct Role& Model::getRole() {
	return this->role;
}

// ��ȡpoint
vector<Point>& Model::getPoint() {
	return this->point;
}

//������Դ
void Model::load(IMAGE& bk, IMAGE& w1, IMAGE& w2) {
	// ���ر���ͼƬ
	loadimage(&bk, "./resource/picture/bk.jpg");
	// ��������ͼƬ
	loadimage(&w1, "./resource/picture/w1.png", 900, 150);
	loadimage(&w2, "./resource/picture/w2.png", 680, 113);
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

// ��ʼ����Ϸ����
void Model::init() {
	// ��ʼ����Ϸ��������
	this->left = width / 2 - (grid_num * grid_size) / 2;
	this->top = height / 2 - (grid_num * grid_size) / 2;
	this->right = this->left + grid_num * grid_size;
	this->down = this->top + grid_num * grid_size;

	// ��ʼ��vector����
	this->point.clear();
	for (int i = 0; i < point_num; i++) {
		this->point.push_back({ 0, 0 });
	}
}

// �����ں���ɫ���ε������Σ���ɫ��
void Model::drawRole(const double x, const double y, const int border_size) {
	// ����ⲿ��ɫ
	setfillcolor(RGB(40, 39, 96));
	solidrectangle(x, y, x + grid_size, y + grid_size);
	// ����ڲ���ɫ����
	setfillcolor(RGB(245, 245, 245));
	solidrectangle(x + (grid_size / 2 - border_size / 2), y + (grid_size / 2 - border_size / 2),
		x + (grid_size / 2 + border_size / 2), y + (grid_size / 2 + border_size / 2));
}

// ���Ƴ�ɫ���Σ�ǽ�ڣ�
void Model::drawWall(const double x, const double y) {
	// ���ƴ��߿�ľ���
	setlinecolor(RGB(240, 240, 240));
	setlinestyle(PS_SOLID, 1);
	setfillcolor(RGB(200, 106, 55));
	fillrectangle(x, y, x + grid_size, y + grid_size);
}

// ���ߵĹ���
void Model::drawLine(struct Point* begin, struct Point* end) {
	// �������±�ת��Ϊ����
	const int bx = this->left + begin->col * grid_size + grid_size / 2;
	const int by = this->top + begin->row * grid_size + grid_size / 2;
	const int ex = this->left + end->col * grid_size + grid_size / 2;
	const int ey = this->top + end->row * grid_size + grid_size / 2;
	setlinecolor(WHITE);
	setlinestyle(PS_SOLID, 5);
	line(bx, by, ex, ey);
}

// ���ƿ�ʼ����
void Model::welcome(IMAGE& bk, IMAGE& w1, IMAGE& w2) {
	putimage(-25, -83, &bk);
	setbkmode(TRANSPARENT);
	SetWindowText(GetHWnd(), "C++һ�ʻ�Ϳ����");

	setbkcolor(RGB(31, 61, 110));
	drawAlpha(&w1, 95, 475);
	drawAlpha(&w2, 175, 580);

	while (!_kbhit()) {
	}
}

// ������Ϸ����
void Model::draw() {
	cleardevice();

	// ���������Ϣ
	settextcolor(RGB(215, 215, 215));
	settextstyle(25, 0, "����", 0, 0, 1000, 0, 0, 0);
	outtextxy(12, 17, "һ�ʻ���Ϸ");

	settextcolor(RGB(215, 215, 215));
	settextstyle(23, 0, "����", 0, 0, 1000, 0, 0, 0);
	outtextxy(16, 54, "���ߣ�����");

	// ���Ƶ�ǰ�ؿ���Ϣ
	char casName[25];
	sprintf(casName, "��ǰ�ؿ�����%d��", this->flag + 1);
	settextcolor(RGB(215, 215, 215));
	settextstyle(29, 0, "����", 0, 0, 1000, 0, 0, 0);
	outtextxy(205, 658, casName);

	// ������Ϸ�����ⲿ�߿�
	for (int x = 15; x >= 0; x--) {
		setlinecolor(RGB(40, 39, 96));
		rectangle(this->left - x, this->top - x, this->right + x, this->down + x);
	}

	// ������Ϸ�����ͼ
	for (int i = 0; i < grid_num; i++) {
		for (int j = 0; j < grid_num; j++) {
			// �������±�ת��Ϊ����
			double x = this->left + (double)grid_size * j;
			double y = this->top + (double)grid_size * i;
			// ����ͼ��Ϣ���зַ�
			switch (this->gameMapVec[this->flag][i][j]) {
			case BLANK:
				break;
			case WALL:
				this->drawWall(x, y);
				break;
			case ROLE:
				this->drawRole(x, y, grid_size / 5);
				break;
			case BOARD:
				this->drawRole(x, y, 1);
				break;
			}
		}
	}
}

// ��Ѱ��ɫλ��
void Model::searchRolePos() {
	for (int i = 0; i < grid_num; i++) {
		for (int j = 0; j < grid_num; j++) {
			if (this->gameMapVec[this->flag][i][j] == ROLE) {
				// ��ȡ��ɫ����λ��
				this->role.row = i;
				this->role.col = j;
			}
		}
	}
	return;
}

// ��¼λ����Ϣ�ĵ�ĳ�ʼ��
void Model::pointInit() {
	// ��ʼ��vector����
	for (vector<Point>::iterator iter = this->point.begin(); iter != this->point.end(); iter++) {
		*iter = { 0, 0 };
	}
	// ��ȡ��ҵ�����λ��
	this->searchRolePos();
	// ��¼��ʼ���λ��
	this->point[0].row = this->role.row;
	this->point[0].col = this->role.col;
	// �Լ�������ʼ��
	this->count = 1;
	// �Է�����г�ʼ��
	this->role.flag = -1;
}

// �ƶ���׼�ж�
int Model::roleStop(const int dir) {
	// �����ƶ��������ǽ��
	if (dir == LEFT && this->gameMapVec[this->flag][this->role.row][this->role.col - 1] == WALL) {
		return 1; // ����1����ʾ�ܹ���
	}
	// �����ƶ����Ҳ���ǽ��
	if (dir == RIGHT && this->gameMapVec[this->flag][this->role.row][this->role.col + 1] == WALL) {
		return 1; // ����1����ʾ�ܹ���
	}
	// �����ƶ����ϲ���ǽ��
	if (dir == UP && this->gameMapVec[this->flag][this->role.row - 1][this->role.col] == WALL) {
		return 1; // ����1����ʾ�ܹ���
	}
	// �����ƶ����²���ǽ��
	if (dir == DOWN && this->gameMapVec[this->flag][this->role.row + 1][this->role.col] == WALL) {
		return 1; // ����1����ʾ�ܹ���
	}
	return -1; // ����-1����ʾ�����ƶ�
}

// ��ɫ�ƶ�����
void Model::moveRole(const int dir) {
	switch (dir) {
	case UP:
		if (this->role.row == this->point[0].row && this->role.col == this->point[0].col) {
			this->gameMapVec[this->flag][this->role.row][this->role.col] = ROLE;
			this->role.row--;
			break;
		}
		this->gameMapVec[this->flag][this->role.row][this->role.col] = BOARD; // ԭ�����λ�ñ�Ϊ����״̬
		this->role.row--;
		break;
	case DOWN:
		if (this->role.row == this->point[0].row && this->role.col == this->point[0].col) {
			this->gameMapVec[this->flag][this->role.row][this->role.col] = ROLE;
			this->role.row++;
			break;
		}
		this->gameMapVec[this->flag][this->role.row][this->role.col] = BOARD; // ԭ�����λ�ñ�Ϊ����״̬
		this->role.row++;
		break;
	case LEFT:
		if (this->role.row == this->point[0].row && this->role.col == this->point[0].col) {
			this->gameMapVec[this->flag][this->role.row][this->role.col] = ROLE;
			this->role.col--;
			break;
		}
		this->gameMapVec[this->flag][this->role.row][this->role.col] = BOARD; // ԭ�����λ�ñ�Ϊ����״̬
		this->role.col--;
		break;
	case RIGHT:
		if (this->role.row == this->point[0].row && this->role.col == this->point[0].col) {
			this->gameMapVec[this->flag][this->role.row][this->role.col] = ROLE;
			this->role.col++;
			break;
		}
		this->gameMapVec[this->flag][this->role.row][this->role.col] = BOARD; // ԭ�����λ�ñ�Ϊ����״̬
		this->role.col++;
		break;
	}
	this->gameMapVec[this->flag][this->role.row][this->role.col] = ROLE; // �µĵط�����˵�λ��
}

// ������Ӧ
void Model::keyDown() {
	// �ж��Ƿ��м�����Ϣ
	while (_kbhit()) {
		// ���ڽ����û�����
		char userKey = _getch();
		fflush(stdin); // ��ռ��̻�����

		switch (userKey) {
		case'W': // ��
		case 'w':
		case 72:
			mciSendString("close click", nullptr, 0, nullptr);
			mciSendString("open ./resource/music/click.wav alias click", nullptr, 0, nullptr);
			mciSendString("play click", nullptr, 0, nullptr);
			this->role.flag = UP;
			break;
		case'S': // ��
		case 's':
		case 80:
			mciSendString("close click", nullptr, 0, nullptr);
			mciSendString("open ./resource/music/click.wav alias click", nullptr, 0, nullptr);
			mciSendString("play click", nullptr, 0, nullptr);
			this->role.flag = DOWN;
			break;
		case'A': // ��
		case 'a':
		case 75:
			mciSendString("close click", nullptr, 0, nullptr);
			mciSendString("open ./resource/music/click.wav alias click", nullptr, 0, nullptr);
			mciSendString("play click", nullptr, 0, nullptr);
			this->role.flag = LEFT;
			break;
		case'D': // ��
		case 'd':
		case 77:
			mciSendString("close click", nullptr, 0, nullptr);
			mciSendString("open ./resource/music/click.wav alias click", nullptr, 0, nullptr);
			mciSendString("play click", nullptr, 0, nullptr);
			this->role.flag = RIGHT;
			break;
		}
	}
}

// ���������ж�
bool Model::over() {
	// �ж������л���û��ǽ
	for (int i = 0; i < grid_num; i++) {
		for (int j = 0; j < grid_num; j++) {
			if (this->gameMapVec[this->flag][i][j] == WALL) {
				return false; // ��Ϸδ����
			}
		}
	}
	return true; // ��Ϸ����
}