#include "Model.h"
using namespace std;

Model::Model() :top(0), left(0), down(0), right(0) {
	this->beginGame = creatButton(width / 2 + 80, 500, 150, 50, "START", 38, 12, RGB(252, 252, 252), RGB(154, 208, 216));
	this->endGame = creatButton(width / 2 + 420, 500, 150, 50, "END", 52, 12, RGB(252, 252, 252), RGB(154, 208, 216));
}

Model::~Model() {
	delete this->beginGame;
	this->beginGame = nullptr;
	delete this->endGame;
	this->endGame = nullptr;
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
void Model::load(IMAGE& bk, IMAGE& w1, IMAGE& w2, IMAGE enem[], IMAGE play[]) {
	// ���ر���ͼƬ
	loadimage(&bk, "./resource/picture/bk.jpg", width, height * 1.243);
	// ��������ͼƬ
	loadimage(&w1, "./resource/picture/w1.png");
	loadimage(&w2, "./resource/picture/w2.png");
	// �������ͼƬ
	loadimage(play, "./resource/picture/role.bmp", grid_size, grid_size);
	loadimage(play + 1, "./resource/picture/role_mask.bmp", grid_size, grid_size);
	// ���ص���ͼƬ
	loadimage(enem, "./resource/picture/sound.png", grid_size, grid_size);
	loadimage(enem + 1, "./resource/picture/sound_mask.png", grid_size, grid_size);
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
	setlinecolor(RGB(154, 208, 216));
	setlinestyle(PS_SOLID, 5);
	setfillcolor(pButton->buttoncolor);
	fillrectangle(pButton->x, pButton->y, pButton->x + pButton->width, pButton->y + pButton->height);
	settextcolor(pButton->textcolor);
	settextstyle(28, 0, "����", 0, 0, 1000, 0, 0, 0);
	outtextxy(pButton->x + pButton->dx, pButton->y + pButton->dy, pButton->text);
}

// ��ʼ��ǽ��
void Model::initWall() {
	// ��ʼ�����ܵ�ǽ��
	for (int i = 0; i < row; i++) {
		this->gameMapVec[i][0] = WALL;
		this->gameMapVec[i][col - 1] = WALL;
	}
	for (int i = 0; i < col; i++) {
		this->gameMapVec[0][i] = WALL;
		this->gameMapVec[row - 1][i] = WALL;
	}
	// ��ʼ�������ǽ��
	for (int i = 1; i < row / 3 - 2; i++) {
		this->gameMapVec[i][col / 2 + 4] = WALL;
	}
	for (int i = row - 8; i < row - 1; i++) {
		this->gameMapVec[i][col - 8] = WALL;
	}
	for (int i = row - 3; i < row - 1; i++) {
		this->gameMapVec[i][col - 5] = WALL;
	}
	for (int i = row - 1; i > row - 9; i--) {
		this->gameMapVec[i][col / 3 - 10] = WALL;
	}
	for (int i = row - 12; i > row / 2 - 15; i--) {
		this->gameMapVec[i][col / 4 + 6] = WALL;
	}
	for (int i = row - 14; i > row / 2 - 1; i--) {
		this->gameMapVec[i][col / 2 + 7] = WALL;
	}
	for (int i = row - 12; i > row / 2 + 2; i--) {
		this->gameMapVec[i][col / 2 + 14] = WALL;
	}
	for (int i = row - 14; i > row / 2 - 1; i--) {
		this->gameMapVec[i][col / 2 + 21] = WALL;
	}
	for (int i = 5; i < row / 2 - 6; i++) {
		this->gameMapVec[i][col / 4 - 4] = WALL;
	}
	// ��ʼ�������ǽ��
	for (int i = col / 2; i < col - 1; i++) {
		this->gameMapVec[row / 2 - 1][i] = WALL;
	}
	for (int i = col / 2; i > 0; i--) {
		this->gameMapVec[4][i] = WALL;
	}
	for (int i = col / 4 + 6; i < col / 2 + 4; i++) {
		this->gameMapVec[row / 2 - 6][i] = WALL;
	}
	for (int i = col / 4 + 6; i < col / 2 + 4; i++) {
		this->gameMapVec[row / 2 + 4][i] = WALL;
	}
	for (int i = col / 4 + 6; i > col / 4 - 12; i--) {
		this->gameMapVec[row / 2 + 1][i] = WALL;
	}
	for (int i = col - 5; i < col - 1; i++) {
		this->gameMapVec[row - 6][i] = WALL;
	}
	for (int i = col / 3 - 8; i < col - 1; i++) {
		this->gameMapVec[row - 11][i] = WALL;
	}
	for (int i = 1; i < col / 3 - 12; i++) {
		this->gameMapVec[row - 11][i] = WALL;
	}
	for (int i = col - 2; i > col - 4; i--) {
		this->gameMapVec[row - 11][i] = BLANK;
	}
	// ����ȱ��
	this->gameMapVec[4][4] = BLANK;
	this->gameMapVec[4][5] = BLANK;
	// �����յ�
	this->gameMapVec[row - 3][col - 3] = DOOR;
}

// ��Ϸ���ݳ�ʼ��
void Model::init() {
	// ���ñ�����ɫ
	setbkcolor(RGB(245, 245, 245));

	// ��Ϸ�����ʼ��
	this->left = width / 2 - (col * grid_size) / 2;
	this->top = height / 2 - (row * grid_size) / 2;
	this->right = this->left + col * grid_size;
	this->down = this->top + row * grid_size;

	// ��Ϸ��ͼ���ݳ�ʼ��
	this->gameMapVec.clear();
	for (int i = 0; i < row; i++) {
		vector<int> lineGrid;
		for (int j = 0; j < col; j++) {
			lineGrid.push_back(0);
		}
		this->gameMapVec.push_back(lineGrid);
	}

	// ǽ�����ݳ�ʼ��
	this->initWall();

	// ������ݳ�ʼ��
	this->role.x = 2 * grid_size;
	this->role.y = 2 * grid_size;
	this->role.step = 3.2;
	this->role.dir = NONE;

	// �������ݳ�ʼ��
	for (int i = 0; i < enemy_num; i++) {
		struct Enemy newenemy;
		newenemy.x = this->left + grid_size * -10;
		newenemy.y = this->left + grid_size * -10;
		newenemy.step = 4.15;
		newenemy.tempstep = newenemy.step;
		newenemy.steprange = 0.105;
		newenemy.flag = 0;
		this->enemys.push_back(newenemy);
	}
	this->enemys[0].x = this->left + grid_size * 15;
	this->enemys[0].y = this->left + grid_size * 15;
	this->enemys[1].x = this->left + grid_size * 28;
	this->enemys[1].y = this->left + grid_size * 8;
	this->enemys[2].x = this->left + grid_size * 47;
	this->enemys[2].y = this->left + grid_size * 18;
	this->enemys[3].x = this->left + grid_size * 22;
	this->enemys[3].y = this->left + grid_size * 1;
	this->enemys[4].x = this->left + grid_size * 6;
	this->enemys[4].y = this->left + grid_size * 7;
	this->enemys[5].x = this->left + grid_size * 14;
	this->enemys[5].y = this->left + grid_size * 21;
	this->enemys[6].x = this->left + grid_size * 44;
	this->enemys[6].y = this->left + grid_size * 4;
	this->enemys[7].x = this->left + grid_size * 30;
	this->enemys[7].y = this->left + grid_size * 24;
	this->enemys[8].x = this->left + grid_size * 35;
	this->enemys[8].y = this->left + grid_size * 6;
	this->enemys[9].x = this->left + grid_size * 1;
	this->enemys[9].y = this->left + grid_size * 19;
	this->enemys[10].x = this->left + grid_size * 25;
	this->enemys[10].y = this->left + grid_size * 30;
	this->enemys[11].x = this->left + grid_size * 57;
	this->enemys[11].y = this->left + grid_size * 30;
	this->enemys[12].x = this->left + grid_size * 8;
	this->enemys[12].y = this->left + grid_size * 13;
	this->enemys[13].x = this->left + grid_size * 49;
	this->enemys[13].y = this->left + grid_size * 10;
	this->enemys[14].x = this->left + grid_size * 43;
	this->enemys[14].y = this->left + grid_size * 30;
	this->enemys[15].x = this->left + grid_size * 19;
	this->enemys[15].y = this->left + grid_size * 10;
	this->enemys[16].x = this->left + grid_size * 42;
	this->enemys[16].y = this->left + grid_size * 12;
	this->enemys[17].x = this->left + grid_size * 6;
	this->enemys[17].y = this->left + grid_size * 30;
	this->enemys[18].x = this->left + grid_size * 57;
	this->enemys[18].y = this->left + grid_size * 6;
	this->enemys[19].x = this->left + grid_size * 59;
	this->enemys[19].y = this->left + grid_size * 21;
	this->enemys[20].x = this->left + grid_size * 27;
	this->enemys[20].y = this->left + grid_size * 17;
	this->enemys[21].x = this->left + grid_size * 34;
	this->enemys[21].y = this->left + grid_size * 30;
	this->enemys[22].x = this->left + grid_size * 16;
	this->enemys[22].y = this->left + grid_size * 30;
	this->enemys[23].x = this->left + grid_size * 52;
	this->enemys[23].y = this->left + grid_size * 30;
}

// ��ʼ�������
void Model::welcome(IMAGE& bk, IMAGE& w1, IMAGE& w2) {
	SetWindowText(GetHWnd(), "C++��������");
	cleardevice();
	putimage(0, -160, &bk);
	this->drawAlpha(&w1, width / 2 + 125, 120);
	this->drawAlpha(&w2, width / 2 + 160, 300);
	this->drawButton(this->beginGame);
	this->drawButton(this->endGame);
}

// ��Ϸ�������
void Model::draw(IMAGE play[], IMAGE enem[]) {
	cleardevice();

	// ������Ϸ�����ͼ
	for (int i = 0; i < row; i++) {
		for (int j = 0; j < col; j++) {
			// �������±�ת��Ϊ����
			int x = this->left + grid_size * j;
			int y = this->top + grid_size * i;
			// ����Ϣ���зַ�
			switch (this->gameMapVec[i][j]) {
			case BLANK:
				break;
			case WALL:
				setlinestyle(PS_SOLID, 1);
				setlinecolor(RGB(255, 255, 255));
				setfillcolor(RGB(93, 107, 153));
				fillrectangle(x, y, x + grid_size, y + grid_size);
				break;
			case DOOR:
				setfillcolor(RGB(255, 128, 128));
				solidrectangle(x - grid_size / 2, y - grid_size / 2, x + grid_size + grid_size / 2, y + grid_size + grid_size / 2);
				clearrectangle(x - grid_size / 4, y - grid_size / 4, x + grid_size + grid_size / 4, y + grid_size + grid_size / 4);
				setfillcolor(RGB(255, 128, 128));
				solidrectangle(x, y, x + grid_size, y + grid_size);
				break;
			default:
				break;
			}
		}
	}

	// ���������Լ�����
	this->drawRole(&this->role, play);
	for (int i = 0; i < enemy_num; i++) {
		this->drawEnemy(&this->enemys[i], enem);
	}
}

// �����ͼƬ
void Model::drawRole(struct Role* role, IMAGE play[]) {
	// ����ͼ��SRCAND��ʽȥ��ͼ
	putimage(role->x, role->y, play + 1, SRCAND);
	// Դ��ͼ��SRCPAINT��ʽȥ��ͼ
	putimage(role->x, role->y, play, SRCPAINT);
}

// ������ͼƬ
void Model::drawEnemy(struct Enemy* enemy, IMAGE enem[]) {
	// ����ͼ��SRCAND��ʽȥ��ͼ
	putimage(enemy->x, enemy->y, enem + 1, SRCAND);
	// Դ��ͼ��SRCPAINT��ʽȥ��ͼ
	putimage(enemy->x, enemy->y, enem, SRCPAINT);
}

// ����Զ��ƶ�
void Model::moveRole() {
	// ������е�����ת��Ϊ�����±�
	int r = (this->role.y + grid_size / 2 - this->top) / grid_size;
	int c = (this->role.x + grid_size / 2 - this->left) / grid_size;
	// ������ĸ��߽�����ת��Ϊ�����±�
	int r1 = (this->role.y - this->top) / grid_size;
	int r2 = (this->role.y + grid_size - this->top) / grid_size;
	int c1 = (this->role.x - this->left) / grid_size;
	int c2 = (this->role.x + grid_size - this->left) / grid_size;
	// �ж��ƶ�����
	if (this->role.dir == UP) { // �����ƶ�
		this->role.y -= this->role.step;
		if ((this->gameMapVec[r - 1][c1] == WALL || this->gameMapVec[r - 1][c2] == WALL) && this->role.y <= this->top + grid_size * (r - 1) + grid_size) {
			this->role.y = this->top + grid_size * (r - 1) + grid_size + 2;
			this->role.dir = DOWN;
		}
	}
	else if (this->role.dir == DOWN) { // �����ƶ�
		this->role.y += this->role.step;
		if ((this->gameMapVec[r + 1][c1] == WALL || this->gameMapVec[r + 1][c2] == WALL) && this->role.y >= this->top + grid_size * (r + 1) - grid_size) {
			this->role.y = this->top + grid_size * (r + 1) - grid_size - 2;
			this->role.dir = UP;
		}
	}
	else if (this->role.dir == LEFT) { // �����ƶ�
		this->role.x -= this->role.step;
		if ((this->gameMapVec[r1][c - 1] == WALL || this->gameMapVec[r2][c - 1] == WALL) && this->role.x <= this->left + grid_size * (c - 1) + grid_size) {
			this->role.x = this->left + grid_size * (c - 1) + grid_size + 2;
			this->role.dir = RIGHT;
		}
	}
	else if (this->role.dir == RIGHT) { // �����ƶ�
		this->role.x += this->role.step;
		if ((this->gameMapVec[r1][c + 1] == WALL || this->gameMapVec[r2][c + 1] == WALL) && this->role.x >= this->left + grid_size * (c + 1) - grid_size) {
			this->role.x = this->left + grid_size * (c + 1) - grid_size - 2;
			this->role.dir = LEFT;
		}
	}
}

// ���˵��Զ��ƶ�
void Model::moveEnemy() {
	for (int i = 0; i < enemy_num; i++) {
		if (this->enemys[i].flag == 0) {
			this->enemys[i].step -= this->enemys[i].steprange;
			if (this->enemys[i].step <= -this->enemys[i].tempstep) {
				this->enemys[i].step = -this->enemys[i].tempstep;
				this->enemys[i].flag = 1;
			}
		}
		else if (this->enemys[i].flag == 1) {
			this->enemys[i].step += this->enemys[i].steprange;
			if (this->enemys[i].step >= this->enemys[i].tempstep) {
				this->enemys[i].step = this->enemys[i].tempstep;
				this->enemys[i].flag = 0;
			}
		}
		if (i < enemy_num / 6) {
			this->enemys[i].x += this->enemys[i].step * 0.93;
		}
		else if (i >= enemy_num / 6 && i < enemy_num / 3) {
			this->enemys[i].x -= this->enemys[i].step * 0.93;
		}
		else if (i >= enemy_num / 3 && i < enemy_num / 2) {
			this->enemys[i].y += this->enemys[i].step * 0.93;
		}
		else if (i >= enemy_num / 2 && i < enemy_num / 3 * 2) {
			this->enemys[i].y -= this->enemys[i].step * 0.93;
		}
		else if (i >= enemy_num / 3 * 2 && i < enemy_num / 3 * 2 + 2) {
			this->enemys[i].x += this->enemys[i].step * 2 / 3 * 0.9;
			this->enemys[i].y += this->enemys[i].step * 4 / 5 * 0.9;
		}
		else if (i >= enemy_num / 6 * 5 - 2 && i < enemy_num / 6 * 5) {
			this->enemys[i].x -= this->enemys[i].step * 2 / 3 * 0.9;
			this->enemys[i].y -= this->enemys[i].step * 4 / 5 * 0.9;
		}
		else if (i >= enemy_num / 6 * 5 && i < enemy_num / 6 * 5 + 2) {
			this->enemys[i].x -= this->enemys[i].step * 2 / 3 * 0.9;
			this->enemys[i].y += this->enemys[i].step * 4 / 5 * 0.9;
		}
		else {
			this->enemys[i].x += this->enemys[i].step * 2 / 3 * 0.9;
			this->enemys[i].y -= this->enemys[i].step * 4 / 5 * 0.9;
		}
	}
}

// �ж�����Ƿ��ڰ�ť��
int Model::mouseInButton(struct Button* pButton, MOUSEMSG* msg) {
	if (msg->x >= pButton->x && msg->x <= pButton->x + pButton->width &&
		msg->y >= pButton->y && msg->y <= pButton->y + pButton->height) {
		pButton->buttoncolor = RGB(235, 235, 235);
		pButton->textcolor = RGB(137, 201, 209);
		return 1; // ����ֵΪ1����������ڰ�ť��
	}
	pButton->buttoncolor = RGB(252, 252, 252);
	pButton->textcolor = RGB(154, 208, 216);
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

// ������Ӧ��ͬ��������Ӧ��
void Model::keyDown() {
	// �ж��Ƿ��а�����Ϣ
	while (_kbhit()) {
		// ���ڽ��հ�����Ϣ
		char userKey = _getch();
		fflush(stdin); // ��ռ�����Ϣ������
		// �԰�����Ϣ���зַ�
		switch (userKey) {
		case 'W':
		case 'w':
		case 72:
			this->role.dir = UP;
			break;
		case 'S':
		case 's':
		case 80:
			this->role.dir = DOWN;
			break;
		case 'A':
		case 'a':
		case 75:
			this->role.dir = LEFT;
			break;
		case 'D':
		case 'd':
		case 77:
			this->role.dir = RIGHT;
			break;
		default:
			break;
		}
	}
}

// ��Ϸ��������1
int Model::condition1() {
	// ��������ϰ�����ײʱ��Ϸ����
	for (int i = 0; i < enemy_num; i++) {
		if (this->distance(this->enemys[i].x, this->enemys[i].y, this->role.x, this->role.y) <= 15) {
			return 1; // ����ֵΪ1��˵����������
		}
	}
	return 0; // ����ֵΪ0��˵������δ����
}

// ��Ϸ��������2
int Model::condition2() {
	// ��ҵ����յ�ʱ��Ϸ����
	// ������е�����ת��Ϊ�����±�
	int r = (this->role.y + grid_size / 2 - this->top) / grid_size;
	int c = (this->role.x + grid_size / 2 - this->left) / grid_size;
	if (this->gameMapVec[r][c] == DOOR) {
		return 1; // ����ֵΪ1��˵����������
	}
	return 0; // ����ֵΪ0��˵������δ����
}

// ��Ϸ�����ж�
bool Model::over() {
	if (this->condition1() == 1 || this->condition2() == 1) {
		return true; // ����ֵΪtrue��˵����Ϸ����
	}
	return false; // ����ֵΪfalse��˵����Ϸδ����
}