#include "Model.h"
using namespace std;

Model::Model() :top(0), left(0), down(0), right(0), cas(0) {
	this->beginGame = creatButton(width / 2 + 80, 500, 150, 50, "START", 38, 12, RGB(252, 252, 252), RGB(98, 147, 206));
	this->endGame = creatButton(width / 2 + 420, 500, 150, 50, "END", 52, 12, RGB(252, 252, 252), RGB(98, 147, 206));
}

Model::~Model() {
	delete this->beginGame;
	this->beginGame = nullptr;
	delete this->endGame;
	this->endGame = nullptr;
}

int& Model::getCas() {
	return this->cas;
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
void Model::load(IMAGE& bk, IMAGE& w1, IMAGE& w2, IMAGE& man, IMAGE& brick, IMAGE& replace, IMAGE fire[], IMAGE door[], IMAGE ghost[], IMAGE character[]) {
	// ���ر���ͼƬ
	loadimage(&bk, "./resource/picture/bk.jpg", width, height);
	// ��������ͼƬ
	loadimage(&man, "./resource/picture/man.png");
	// ����ש��ͼƬ
	loadimage(&brick, "./resource/picture/brick.jpg", grid_size, grid_size);
	// �������ͼƬ
	loadimage(&replace, "./resource/picture/replace.jpg", grid_size, grid_size);
	// ��������ͼƬ
	loadimage(&w1, "./resource/picture/w1.png");
	loadimage(&w2, "./resource/picture/w2.png");
	// ���ػ���ͼƬ
	loadimage(fire, "./resource/picture/fire2.jpg", grid_size, grid_size);
	loadimage(fire + 1, "./resource/picture/fire1.jpg", grid_size, grid_size);
	// �����յ�ͼƬ
	loadimage(door, "./resource/picture/door2.jpg", grid_size, grid_size);
	loadimage(door + 1, "./resource/picture/door1.jpg", grid_size, grid_size);
	// ��������ͼƬ
	loadimage(ghost, "./resource/picture/ghost2.jpg", grid_size, grid_size);
	loadimage(ghost + 1, "./resource/picture/ghost1.jpg", grid_size, grid_size);
	// ��������ͼƬ
	loadimage(character, "./resource/picture/character2.jpg", grid_size, grid_size);
	loadimage(character + 1, "./resource/picture/character1.jpg", grid_size, grid_size);
}

// ��������
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
	setlinecolor(RGB(79, 133, 200));
	setlinestyle(PS_SOLID, 5);
	setfillcolor(pButton->buttoncolor);
	fillrectangle(pButton->x, pButton->y, pButton->x + pButton->width, pButton->y + pButton->height);
	settextcolor(pButton->textcolor);
	settextstyle(28, 0, "����", 0, 0, 1000, 0, 0, 0);
	outtextxy(pButton->x + pButton->dx, pButton->y + pButton->dy, pButton->text);
}

// ��ʼ����ͼ
void Model::map1() {
	// ��ʼ�����ܵ�ǽ��
	for (int i = 11; i <= 12; i++) {
		this->gameMapVec[0][i][5] = WALL;
	}
	for (int i = 6; i <= 8; i++) {
		this->gameMapVec[0][i][30] = WALL;
	}
	for (int i = 6; i <= 7; i++) {
		this->gameMapVec[0][2][i] = WALL;
	}
	for (int i = 19; i <= 22; i++) {
		this->gameMapVec[0][13][i] = WALL;
	}
	for (int i = 28; i <= 30; i++) {
		this->gameMapVec[0][13][i] = WALL;
	}
	this->gameMapVec[0][3][25] = WALL;
	this->gameMapVec[0][6][11] = WALL;
	this->gameMapVec[0][20][24] = WALL;
	this->gameMapVec[0][19][10] = WALL;
	this->gameMapVec[0][12][21] = WALL;

	// ��ʼ�������Լ��յ�
	this->gameMapVec[0][12][22] = DOOR;
	this->gameMapVec[0][12][20] = ROLE;
}

// ��ʼ����ͼ
void Model::map2() {
	// ��ʼ�����ܵ�ǽ��
	for (int i = 3; i <= 5; i++) {
		this->gameMapVec[1][i][3] = WALL;
	}
	for (int i = 8; i <= 11; i++) {
		this->gameMapVec[1][i][13] = WALL;
	}
	for (int i = 3; i <= 7; i++) {
		this->gameMapVec[1][2][i] = WALL;
	}
	this->gameMapVec[1][4][6] = WALL;
	this->gameMapVec[1][3][9] = WALL;
	this->gameMapVec[1][1][29] = WALL;
	this->gameMapVec[1][1][33] = WALL;
	this->gameMapVec[1][5][34] = WALL;
	this->gameMapVec[1][8][12] = WALL;
	this->gameMapVec[1][13][7] = WALL;
	this->gameMapVec[1][10][8] = WALL;
	this->gameMapVec[1][19][8] = WALL;
	this->gameMapVec[1][14][9] = WALL;
	this->gameMapVec[1][20][7] = WALL;
	this->gameMapVec[1][11][12] = WALL;
	this->gameMapVec[1][13][26] = WALL;
	this->gameMapVec[1][17][25] = WALL;
	this->gameMapVec[1][14][30] = WALL;
	this->gameMapVec[1][18][33] = WALL;
	this->gameMapVec[1][21][29] = WALL;

	// ��ʼ������
	for (int i = 4; i <= 6; i++) {
		this->gameMapVec[1][i][5] = FIRE;
	}
	for (int i = 17; i <= 19; i++) {
		this->gameMapVec[1][i][7] = FIRE;
	}
	for (int i = 32; i <= 34; i++) {
		this->gameMapVec[1][3][i] = FIRE;
	}
	for (int i = 31; i <= 32; i++) {
		this->gameMapVec[1][14][i] = FIRE;
	}
	for (int i = 34; i <= 35; i++) {
		this->gameMapVec[1][14][i] = FIRE;
	}
	for (int i = 25; i <= 30; i++) {
		this->gameMapVec[1][19][i] = FIRE;
	}

	// ��ʼ������
	this->gameMapVec[1][10][30] = GHOST;
	this->gameMapVec[1][6][10] = GHOST;

	// ��ʼ�������Լ��յ�
	this->gameMapVec[1][18][8] = DOOR;
	this->gameMapVec[1][18][6] = ROLE;
}

// ��ʼ����ͼ
void Model::map3() {
	// ��ʼ�����ܵ�ǽ��
	for (int i = 11; i <= 13; i++) {
		this->gameMapVec[2][i][2] = WALL;
	}
	for (int i = 20; i <= 22; i++) {
		this->gameMapVec[2][i][5] = WALL;
	}
	for (int i = 15; i <= 17; i++) {
		this->gameMapVec[2][i][10] = WALL;
	}
	for (int i = 1; i <= 2; i++) {
		this->gameMapVec[2][i][12] = WALL;
	}
	for (int i = 19; i <= 21; i++) {
		this->gameMapVec[2][i][15] = WALL;
	}
	for (int i = 3; i <= 5; i++) {
		this->gameMapVec[2][i][18] = WALL;
	}
	for (int i = 21; i <= 23; i++) {
		this->gameMapVec[2][i][20] = WALL;
	}
	for (int i = 10; i <= 12; i++) {
		this->gameMapVec[2][i][31] = WALL;
	}
	for (int i = 27; i <= 29; i++) {
		this->gameMapVec[2][3][i] = WALL;
	}
	for (int i = 31; i <= 33; i++) {
		this->gameMapVec[2][4][i] = WALL;
	}
	for (int i = 9; i <= 11; i++) {
		this->gameMapVec[2][5][i] = WALL;
	}
	for (int i = 7; i <= 8; i++) {
		this->gameMapVec[2][10][i] = WALL;
	}
	for (int i = 20; i <= 21; i++) {
		this->gameMapVec[2][13][i] = WALL;
	}
	for (int i = 27; i <= 29; i++) {
		this->gameMapVec[2][17][i] = WALL;
	}
	for (int i = 25; i <= 27; i++) {
		this->gameMapVec[2][21][i] = WALL;
	}
	this->gameMapVec[2][9][8] = WALL;
	this->gameMapVec[2][12][1] = WALL;
	this->gameMapVec[2][12][3] = WALL;
	this->gameMapVec[2][11][8] = WALL;
	this->gameMapVec[2][10][9] = WALL;
	this->gameMapVec[2][6][10] = WALL;
	this->gameMapVec[2][4][10] = WALL;
	this->gameMapVec[2][1][11] = WALL;
	this->gameMapVec[2][0][12] = WALL;
	this->gameMapVec[2][1][13] = WALL;
	this->gameMapVec[2][4][17] = WALL;
	this->gameMapVec[2][4][19] = WALL;
	this->gameMapVec[2][21][4] = WALL;
	this->gameMapVec[2][21][6] = WALL;
	this->gameMapVec[2][16][9] = WALL;
	this->gameMapVec[2][2][28] = WALL;
	this->gameMapVec[2][4][28] = WALL;
	this->gameMapVec[2][3][32] = WALL;
	this->gameMapVec[2][5][32] = WALL;
	this->gameMapVec[2][12][21] = WALL;
	this->gameMapVec[2][14][21] = WALL;
	this->gameMapVec[2][13][22] = WALL;
	this->gameMapVec[2][16][11] = WALL;
	this->gameMapVec[2][20][14] = WALL;
	this->gameMapVec[2][20][16] = WALL;
	this->gameMapVec[2][11][17] = WALL;
	this->gameMapVec[2][22][19] = WALL;
	this->gameMapVec[2][22][21] = WALL;
	this->gameMapVec[2][20][26] = WALL;
	this->gameMapVec[2][22][26] = WALL;
	this->gameMapVec[2][16][28] = WALL;
	this->gameMapVec[2][18][28] = WALL;
	this->gameMapVec[2][11][30] = WALL;
	this->gameMapVec[2][11][32] = WALL;

	// ��ʼ�������Լ��յ�
	this->gameMapVec[2][20][4] = DOOR;
	this->gameMapVec[2][11][19] = ROLE;
}

// ��Ϸ���ݳ�ʼ��
void Model::init() {
	// ��Ϸ�����ʼ��
	this->left = width / 2 - (col * grid_size) / 2;
	this->top = height / 2 - (row * grid_size) / 2;
	this->right = this->left + col * grid_size;
	this->down = this->top + row * grid_size;

	// ��Ϸ��ͼ���ݳ�ʼ��
	this->gameMapVec.clear();
	for (int k = 0; k < maxcas; k++) {
		vector<vector<int>> MapGrid;
		for (int i = 0; i < row; i++) {
			vector<int> lineGrid;
			for (int j = 0; j < col; j++) {
				lineGrid.push_back(0);
			}
			MapGrid.push_back(lineGrid);
		}
		this->gameMapVec.push_back(MapGrid);
	}

	// ��ͼ���ݳ�ʼ��
	this->map1();
	this->map2();
	this->map3();

	// ������ݳ�ʼ��
	this->searchRole();
	this->role.step = 1.4;
	this->role.dir = NONE;
}

// ��ʼ�������
void Model::welcome(IMAGE& bk, IMAGE& w1, IMAGE& w2, IMAGE& man) {
	SetWindowText(GetHWnd(), "C++�ӳ�����");
	cleardevice();
	putimage(0, 0, &bk);
	putimage(width / 4 - 512 / 2 + 30, height / 2 - 512 / 2, &man);
	this->drawAlpha(&w1, width / 2 + 125, 120);
	this->drawAlpha(&w2, width / 2 + 160, 300);
	this->drawButton(this->beginGame);
	this->drawButton(this->endGame);
}

// ��Ϸ�������
void Model::draw(IMAGE& bk, IMAGE& brick, IMAGE fire[], IMAGE door[], IMAGE ghost[], IMAGE character[]) {
	cleardevice();
	putimage(0, 0, &bk);

	// ������Ϸ�����ͼ
	for (int i = 0; i < row; i++) {
		for (int j = 0; j < col; j++) {
			// �������±�ת��Ϊ����
			int x = this->left + grid_size * j;
			int y = this->top + grid_size * i;
			// ����Ϣ���зַ�
			switch (this->gameMapVec[this->cas][i][j]) {
			case BLANK:
				break;
			case ROLE:
				setlinecolor(RGB(128, 64, 0));
				setlinestyle(PS_SOLID, 4);
				setfillcolor(RGB(255, 138, 176));
				fillrectangle(x + 2, y + 2, x + grid_size - 2, y + grid_size - 2);
				break;
			case WALL:
				putimage(x, y, &brick);
				break;
			case FIRE:
				this->drawFire(x, y, fire);
				break;
			case GHOST:
				this->drawGhost(x, y, ghost);
				break;
			case DOOR:
			case (DOOR + 2):
				this->drawDoor(x, y, door);
				break;
			default:
				break;
			}
		}
	}
	// ��������
	this->drawRole(character);

	// �����ⲿ�߿�
	for (int x = 25; x >= 0; x--) {
		setlinecolor(RGB(128, 64, 0));
		rectangle(this->left - x, this->top - x, this->right + x, this->down + x);
	}
	for (int x = 50; x >= 25; x--) {
		setlinecolor(RGB(165, 103, 69));
		rectangle(this->left - x, this->top - x, this->right + x, this->down + x);
	}
	for (int x = 70; x >= 50; x--) {
		setlinecolor(RGB(128, 64, 0));
		rectangle(this->left - x, this->top - x, this->right + x, this->down + x);
	}
}

// ������ͼƬ
void Model::drawFire(const int x, const int y, IMAGE fire[]) {
	// Դ��ͼ��SRCPAINT��ʽȥ��ͼ
	putimage(x, y, fire + 1, SRCPAINT);
	// ����ͼ��SRCAND��ʽȥ��ͼ
	putimage(x, y, fire, SRCAND);
}

// ���յ�ͼƬ
void Model::drawDoor(const int x, const int y, IMAGE door[]) {
	// Դ��ͼ��SRCPAINT��ʽȥ��ͼ
	putimage(x, y, door + 1, SRCPAINT);
	// ����ͼ��SRCAND��ʽȥ��ͼ
	putimage(x, y, door, SRCAND);
}

// ������ͼƬ
void Model::drawGhost(const int x, const int y, IMAGE ghost[]) {
	// Դ��ͼ��SRCPAINT��ʽȥ��ͼ
	putimage(x, y, ghost + 1, SRCPAINT);
	// ����ͼ��SRCAND��ʽȥ��ͼ
	putimage(x, y, ghost, SRCAND);
}

// ������ͼƬ
void Model::drawRole(IMAGE character[]) {
	//Դ��ͼ��SRCPAINT��ʽȥ��ͼ
	putimage(this->role.x, this->role.y, character + 1, SRCPAINT);
	//����ͼ��SRCAND��ʽȥ��ͼ
	putimage(this->role.x, this->role.y, character, SRCAND);
}

// ��Ѱ���λ��
void Model::searchRole() {
	for (int i = 0; i < row; i++) {
		for (int j = 0; j < col; j++) {
			if (this->gameMapVec[this->cas][i][j] == ROLE) {
				this->role.x = j * grid_size + this->left;
				this->role.y = i * grid_size + this->top;
				break;
			}
		}
	}
}

// ����Զ��ƶ�
void Model::moveRole(IMAGE& bk, IMAGE& brick, IMAGE& replace, IMAGE fire[], IMAGE door[], IMAGE ghost[], IMAGE character[]) {
	// ��¼����ٶȳ�ʼֵ
	static double tempstep = this->role.step;
	// ��¼�������ǽ������ֵ
	double x = 0, y = 0;
	// ������е�����ת��Ϊ�����±�
	int r = (this->role.y + grid_size / 2 - this->top) / grid_size;
	int c = (this->role.x + grid_size / 2 - this->left) / grid_size;
	// �ж��ƶ�����
	if (this->role.dir == UP) { // �����ƶ�
		this->role.y -= this->role.step;
		this->role.step += 0.1;
		if (r - 1 >= 0 && this->gameMapVec[this->cas][r - 1][c] == WALL && this->role.y <= this->top + grid_size * (r - 1) + grid_size) {
			this->role.y = this->top + grid_size * (r - 1) + grid_size;
			if (this->gameMapVec[this->cas][r][c] != ROLE) {
				this->gameMapVec[this->cas][r][c] += 2;
			}
			this->role.dir = NONE;
			this->role.step = tempstep;

			// �Ƚ���ͼ����һ��
			this->draw(bk, brick, fire, door, ghost, character);
			FlushBatchDraw();

			// ��ȡ����ϱ�ǽ�ڵ�����
			x = c * grid_size + this->left;
			y = (r - 1) * grid_size + this->top;

			// ����ײ������
			mciSendString("close collision", nullptr, 0, nullptr);
			mciSendString("open ./resource/music/collision.mp3 alias collision", nullptr, 0, nullptr);
			mciSendString("play collision", nullptr, 0, nullptr);

			// ײ��ǽ�ڶ���Ч������
			for (int i = 0; i < 4; i++) {
				putimage(x, y, &replace);
				y--;
				putimage(x, y, &brick);
				Sleep(30);
				FlushBatchDraw();
			}
			for (int i = 0; i < 4; i++) {
				putimage(x, y, &replace);
				y++;
				putimage(x, y, &brick);
				Sleep(30);
				FlushBatchDraw();
			}
		}
	}
	else if (this->role.dir == DOWN) { // �����ƶ�
		this->role.y += this->role.step;
		this->role.step += 0.1;
		if (r + 1 < row && this->gameMapVec[this->cas][r + 1][c] == WALL && this->role.y >= this->top + grid_size * (r + 1) - grid_size) {
			this->role.y = this->top + grid_size * (r + 1) - grid_size;
			if (this->gameMapVec[this->cas][r][c] != ROLE) {
				this->gameMapVec[this->cas][r][c] += 2;
			}
			this->role.dir = NONE;
			this->role.step = tempstep;

			// �Ƚ���ͼ����һ��
			this->draw(bk, brick, fire, door, ghost, character);
			FlushBatchDraw();

			// ��ȡ����ϱ�ǽ�ڵ�����
			x = c * grid_size + this->left;
			y = (r + 1) * grid_size + this->top;

			// ����ײ������
			mciSendString("close collision", nullptr, 0, nullptr);
			mciSendString("open ./resource/music/collision.mp3 alias collision", nullptr, 0, nullptr);
			mciSendString("play collision", nullptr, 0, nullptr);

			// ײ��ǽ�ڶ���Ч������
			for (int i = 0; i < 4; i++) {
				putimage(x, y, &replace);
				y++;
				putimage(x, y, &brick);
				Sleep(30);
				FlushBatchDraw();
			}
			for (int i = 0; i < 4; i++) {
				putimage(x, y, &replace);
				y--;
				putimage(x, y, &brick);
				Sleep(30);
				FlushBatchDraw();
			}
		}
	}
	else if (this->role.dir == LEFT) { // �����ƶ�
		this->role.x -= this->role.step;
		this->role.step += 0.1;
		if (c - 1 >= 0 && this->gameMapVec[this->cas][r][c - 1] == WALL && this->role.x <= this->left + grid_size * (c - 1) + grid_size) {
			this->role.x = this->left + grid_size * (c - 1) + grid_size;
			if (this->gameMapVec[this->cas][r][c] != ROLE) {
				this->gameMapVec[this->cas][r][c] += 2;
			}
			this->role.dir = NONE;
			this->role.step = tempstep;

			// �Ƚ���ͼ����һ��
			this->draw(bk, brick, fire, door, ghost, character);
			FlushBatchDraw();

			// ��ȡ������ǽ�ڵ�����
			x = (c - 1) * grid_size + this->left;
			y = r * grid_size + this->top;

			//����ײ������
			mciSendString("close collision", nullptr, 0, nullptr);
			mciSendString("open ./resource/music/collision.mp3 alias collision", nullptr, 0, nullptr);
			mciSendString("play collision", nullptr, 0, nullptr);

			// ײ��ǽ�ڶ���Ч������
			for (int i = 0; i < 4; i++) {
				putimage(x, y, &replace);
				x--;
				putimage(x, y, &brick);
				Sleep(30);
				FlushBatchDraw();
			}
			for (int i = 0; i < 4; i++) {
				putimage(x, y, &replace);
				x++;
				putimage(x, y, &brick);
				Sleep(30);
				FlushBatchDraw();
			}
		}
	}
	else if (this->role.dir == RIGHT) { // �����ƶ�
		this->role.x += this->role.step;
		this->role.step += 0.1;
		if (c + 1 < col && this->gameMapVec[this->cas][r][c + 1] == WALL && this->role.x >= this->left + grid_size * (c + 1) - grid_size) {
			this->role.x = this->left + grid_size * (c + 1) - grid_size;
			if (this->gameMapVec[this->cas][r][c] != ROLE) {
				this->gameMapVec[this->cas][r][c] += 2;
			}
			this->role.dir = NONE;
			this->role.step = tempstep;

			// �Ƚ���ͼ����һ��
			this->draw(bk, brick, fire, door, ghost, character);
			FlushBatchDraw();

			// ��ȡ����ұ�ǽ�ڵ�����
			x = (c + 1) * grid_size + this->left;
			y = r * grid_size + this->top;

			// ����ײ������
			mciSendString("close collision", nullptr, 0, nullptr);
			mciSendString("open ./resource/music/collision.mp3 alias collision", nullptr, 0, nullptr);
			mciSendString("play collision", nullptr, 0, nullptr);

			// ײ��ǽ�ڶ���Ч������
			for (int i = 0; i < 4; i++) {
				putimage(x, y, &replace);
				x++;
				putimage(x, y, &brick);
				Sleep(30);
				FlushBatchDraw();
			}
			for (int i = 0; i < 4; i++) {
				putimage(x, y, &replace);
				x--;
				putimage(x, y, &brick);
				Sleep(30);
				FlushBatchDraw();
			}
		}
	}
}

// �ж�����Ƿ��ڰ�ť��
int Model::mouseInButton(struct Button* pButton, MOUSEMSG* msg) {
	if (msg->x >= pButton->x && msg->x <= pButton->x + pButton->width &&
		msg->y >= pButton->y && msg->y <= pButton->y + pButton->height) {
		pButton->buttoncolor = RGB(235, 235, 235);
		pButton->textcolor = RGB(94, 143, 204);
		return 1; // ����ֵΪ1����������ڰ�ť��
	}
	pButton->buttoncolor = RGB(252, 252, 252);
	pButton->textcolor = RGB(98, 143, 204);
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

// ������Ӧ��ͬ��������Ӧ��
void Model::keyDown() {
	// �ж��Ƿ��а�����Ϣ
	while (_kbhit()) {
		// ���ڽ��հ�����Ϣ
		char userKey = _getch();
		fflush(stdin); // ��ռ�����Ϣ������

		switch (userKey) {
		case 'W':
		case 'w':
		case 72:
			if (this->role.dir == NONE) {
				this->role.dir = UP;
			}
			break;
		case 'S':
		case 's':
		case 80:
			if (this->role.dir == NONE) {
				this->role.dir = DOWN;
			}
			break;
		case 'A':
		case 'a':
		case 75:
			if (this->role.dir == NONE) {
				this->role.dir = LEFT;
			}
			break;
		case 'D':
		case 'd':
		case 77:
			if (this->role.dir == NONE) {
				this->role.dir = RIGHT;
			}
			break;
		default:
			break;
		}
	}
}

// ��Ϸ��������1
int Model::condition1() {
	// ��ҵ����յ�
	int r = (this->role.y + grid_size / 2 - this->top) / grid_size;
	int c = (this->role.x + grid_size / 2 - this->left) / grid_size;
	if (this->gameMapVec[this->cas][r][c] == DOOR + 2) {
		return 1; // ����ֵΪ1��˵����������
	}
	return 0; // ����ֵΪ0��˵������������
}

// ��Ϸ��������2
int Model::condition2() {
	// ����뿪��Ϸ������������������
	int r = (this->role.y + grid_size / 2 - this->top) / grid_size;
	int c = (this->role.x + grid_size / 2 - this->left) / grid_size;
	if (this->role.x <= this->left || this->role.x >= this->right * 1.0 - grid_size ||
		this->role.y <= this->top || this->role.y >= this->down * 1.0 - grid_size ||
		this->gameMapVec[this->cas][r][c] == GHOST ||
		this->gameMapVec[this->cas][r][c] == FIRE) {
		return 1; // ����ֵΪ1��˵����������
	}
	return 0; // ����ֵΪ0��˵������������
}

// ��Ϸ�����ж�
bool Model::over() {
	if (this->condition1() == 1 || this->condition2() == 1) {
		return true; // ����ֵΪtrue,˵����Ϸ����
	}
	return false; // ����ֵΪfalse,˵����Ϸδ����
}