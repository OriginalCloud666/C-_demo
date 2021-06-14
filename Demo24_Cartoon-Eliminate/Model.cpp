#include "Model.h"
using namespace std;

Model::Model() :top(0), left(0), down(0), right(0), state(true) {
	begin.x = -1; begin.y = -1;
	end.x = -1; end.y = -1;
	this->beginGame = creatButton(80, 500, 150, 50, "START", 38, 12, RGB(252, 252, 252), RGB(150, 150, 150));
	this->endGame = creatButton(420, 500, 150, 50, "END", 52, 12, RGB(252, 252, 252), RGB(150, 150, 150));
}

Model::~Model() {
	delete this->beginGame;
	this->beginGame = nullptr;
	delete this->endGame;
	this->endGame = nullptr;
}

// ��ȡ��ʼ��
POINT& Model::getBegin() {
	return this->begin;
}

// ��ȡ������
POINT& Model::getEnd() {
	return this->end;
}

// ��ȡMapVec
vector<vector<int>>& Model::getMapVec() {
	return this->gameMapVec;
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
void Model::load(IMAGE& bk, IMAGE& w1, IMAGE& w2, vector<IMAGE>& img, vector<int>& imgIndex) {
	// ��������ͼƬ
	for (int i = 0; i < pic_num; i++) {
		char fileName[25];
		sprintf(fileName, "./resource/picture/%d.png", imgIndex[i]);
		loadimage(&img[i], fileName, pic_size - 10, pic_size - 10);
	}
	loadimage(&bk, "./resource/picture/bk.png", width, height);
	loadimage(&w1, "./resource/picture/w1.png");
	loadimage(&w2, "./resource/picture/w2.png");
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
	setlinecolor(RGB(160, 160, 160));
	setlinestyle(PS_SOLID, 5);
	setfillcolor(pButton->buttoncolor);
	fillrectangle(pButton->x, pButton->y, pButton->x + pButton->width, pButton->y + pButton->height);
	settextcolor(pButton->textcolor);
	settextstyle(28, 0, "����", 0, 0, 1000, 0, 0, 0);
	outtextxy(pButton->x + pButton->dx, pButton->y + pButton->dy, pButton->text);
}

// ��ʼ������
void Model::init() {
	// �������������
	srand((unsigned)time(nullptr) * rand());

	// ��ʼ����Ϸ�����Լ���Ϸ��������
	this->left = width / 2 - (grid_num * pic_size) / 2;
	this->top = height / 2 - (grid_num * pic_size) / 2;
	this->right = this->left + grid_num * pic_size;
	this->down = this->top + grid_num * pic_size;

	// ��ʼ��vector����
	this->gameMapVec.clear();
	for (int i = 0; i < grid_num; i++) {
		vector<int> lineGrid;
		for (int j = 0; j < grid_num; j++) {
			lineGrid.push_back(j % 9);
		}
		this->gameMapVec.push_back(lineGrid);
	}

	// ����vector��������
	for (int i = 0; i < grid_num; i++) {
		for (int j = 0; j < grid_num; j++) {
			int r1 = rand() % grid_num;
			int c1 = rand() % grid_num;
			int r2 = rand() % grid_num;
			int c2 = rand() % grid_num;
			int temp = this->gameMapVec[r1][c1];
			this->gameMapVec[r1][c1] = this->gameMapVec[r2][c2];
			this->gameMapVec[r2][c2] = temp;
		}
	}
}

// ���ƿ�ʼ����
void Model::welcome(IMAGE& bk, IMAGE& w1, IMAGE& w2) {
	SetWindowText(GetHWnd(), "C++��ͨ����");
	setbkcolor(RGB(240, 240, 240));
	cleardevice();

	this->drawAlpha(&bk, 0, 0);
	this->drawAlpha(&w1, 131, 100);
	this->drawAlpha(&w2, 161, 290);
	this->drawButton(this->beginGame);
	this->drawButton(this->endGame);
}

// ������Ϸ����
void Model::draw(IMAGE& bk, vector<IMAGE>& img) {
	setbkcolor(RGB(240, 240, 240));
	cleardevice();
	drawAlpha(&bk, 0, 0);

	// ������Ϸ����߿�
	setfillcolor(RGB(255, 255, 255));
	setlinecolor(RGB(210, 210, 210));
	setlinestyle(PS_SOLID, 10);
	fillroundrect(this->left - 10, this->top - 10, this->right + 10, this->down + 10, 1, 1);

	// ������Ϸ�����ͼ
	for (int i = 0; i < grid_num; i++) {
		for (int j = 0; j < grid_num; j++) {
			// �±�ת��Ϊ����
			int x = j * pic_size + this->left;
			int y = i * pic_size + this->top;
			// ��Ϣ���зַ�
			switch (this->gameMapVec[i][j]) {
			case 0:
				this->drawAlpha(&img[0], x + 6, y + 6);
				this->drawFim(x, y);
				break;
			case 1:
				this->drawAlpha(&img[1], x + 6, y + 6);
				this->drawFim(x, y);
				break;
			case 2:
				this->drawAlpha(&img[2], x + 6, y + 6);
				this->drawFim(x, y);
				break;
			case 3:
				this->drawAlpha(&img[3], x + 6, y + 6);
				this->drawFim(x, y);
				break;
			case 4:
				this->drawAlpha(&img[4], x + 6, y + 6);
				this->drawFim(x, y);
				break;
			case 5:
				this->drawAlpha(&img[5], x + 6, y + 6);
				this->drawFim(x, y);
				break;
			case 6:
				this->drawAlpha(&img[6], x + 6, y + 6);
				this->drawFim(x, y);
				break;
			case 7:
				this->drawAlpha(&img[7], x + 6, y + 6);
				this->drawFim(x, y);
				break;
			case 8:
				this->drawAlpha(&img[8], x + 6, y + 6);
				this->drawFim(x, y);
				break;
			default:
				break;
			}
		}
	}

	// ����ѡ��߿�
	this->drawChoseFim();
}

// ���Ʊ߿�
void Model::drawFim(const int x, const int y) {
	setlinecolor(RGB(250, 250, 250));
	setlinestyle(PS_SOLID, 5);
	roundrect(x, y, x + pic_size, y + pic_size, 1, 1);
	setlinecolor(RGB(200, 200, 200));
	setlinestyle(PS_SOLID, 5);
	roundrect(x + 5, y + 5, x + pic_size - 5, y + pic_size - 5, 1, 1);
	setlinecolor(RGB(250, 250, 250));
	setlinestyle(PS_SOLID, 3);
	roundrect(x + 7, y + 7, x + pic_size - 7, y + pic_size - 7, 1, 1);
}

// ����ѡ���
void Model::drawChoseFim() {
	// �±�ת��Ϊ����
	int begin_x = this->begin.y * pic_size + this->left;
	int begin_y = this->begin.x * pic_size + this->top;
	int end_x = this->end.y * pic_size + this->left;
	int end_y = this->end.x * pic_size + this->top;
	setlinecolor(RGB(120, 120, 120));
	setlinestyle(PS_SOLID, 5);
	if (this->begin.x != -1 && this->begin.y != -1) {
		roundrect(begin_x + 5, begin_y + 5, begin_x + pic_size - 5, begin_y + pic_size - 5, 1, 1);
	}
	if (this->end.x != -1 && this->end.y != -1) {
		roundrect(end_x + 5, end_y + 5, end_x + pic_size - 5, end_y + pic_size - 5, 1, 1);
	}
}

// �ж��Ƿ��ǿհ�
bool Model::isBlock(const int r, const int c) {
	if (this->gameMapVec[r][c] == -1) {
		return false; // �ǿհ��򷵻�false
	}
	return true; // ���谭�򷵻�true
}

// �ж�����ͼƬ�Ƿ���ͬ
bool Model::isSeem() {
	if (this->gameMapVec[this->begin.x][this->begin.y] != this->gameMapVec[this->end.x][this->end.y]) {
		return false; // ��ͬ�򷵻�false
	}
	return true; // ��ͬ�򷵻�true
}

// �ж�ˮƽ�����Ƿ��ܹ�����
bool Model::horizon(POINT begin_, POINT end_) {
	// �ж��Ƿ��Ѿ�ѡ��
	if ((begin_.x == -1 && begin_.y == -1) || (end_.x == -1 && end_.y == -1)) {
		return false;
	}
	// �ж��Ƿ���ͬһˮƽ����
	if (begin_.x != end_.x) {
		return false;
	}
	// �ж��Ƿ�����ͬһ��
	if (begin_.x == end_.x && begin_.y == end_.y) {
		return false;
	}
	// �ж�������ͨͼƬ֮���Ƿ����谭
	int maxc = max(begin_.y, end_.y);
	int minc = min(begin_.y, end_.y);
	// ��С���������������ж�
	for (int i = minc + 1; i < maxc; i++) {
		if (this->isBlock(begin_.x, i) == true) { // ������谭��ֱ���˳�
			return false;
		}
	}
	return true;
}

// �ж���ֱ�����Ƿ��ܹ�����
bool Model::vertical(POINT begin_, POINT end_) {
	// �ж��Ƿ��Ѿ�ѡ��
	if ((begin_.x == -1 && begin_.y == -1) || (end_.x == -1 && end_.y == -1)) {
		return false;
	}
	// �ж��Ƿ���ͬһ��ֱ����
	if (begin_.y != end_.y) {
		return false;
	}
	// �ж��Ƿ�����ͬһ��
	if (begin_.x == end_.x && begin_.y == end_.y) {
		return false;
	}
	// �ж�������ͨͼƬ֮���Ƿ����谭
	int maxc = max(begin_.x, end_.x);
	int minc = min(begin_.x, end_.x);
	// ��С���������������ж�
	for (int i = minc + 1; i < maxc; i++) {
		if (this->isBlock(i, begin_.y) == true) { // ������谭��ֱ���˳�
			return false;
		}
	}
	return true;
}

// �ж�һ���ս��Ƿ���ɾ��
bool Model::turn_once(POINT begin_, POINT end_) {
	// һ���յ�����Էֽ�Ϊһ��ˮƽ����һ����ֱ���
	// A�㵽B���ܷ����ӿ�ת��Ϊ����һ����������������һ��:
	// 1.A����C���ˮƽ����Լ�C����B��Ĵ�ֱ���ͨ��
	// 2.A����D��Ĵ�ֱ����Լ�D����B���ˮƽ���ͨ��

	// �ж��Ƿ��Ѿ�ѡ��
	if ((begin_.x == -1 && begin_.y == -1) || (end_.x == -1 && end_.y == -1)) {
		return false;
	}
	// �ж��Ƿ�����ͬһ��
	if (begin_.x == end_.x && begin_.y == end_.y) {
		return false;
	}
	POINT c, d;
	c.x = begin_.x; c.y = end_.y;
	d.x = end_.x; d.y = begin_.y;
	// �жϹյ����Ƿ�ͨ��
	if (this->isBlock(c.x, c.y) == false && this->horizon(begin_, c) == true && this->vertical(c, end_) == true) {
		return true;
	}
	if (this->isBlock(d.x, d.y) == false && this->vertical(begin_, d) == true && this->horizon(d, end_) == true) {
		return true;
	}
	return false;
}

// �ж������ս��Ƿ���ɾ��
bool Model::turn_twice(POINT begin_, POINT end_) {
	// �����յ�����Էֽ�Ϊһ���սǼ���һ��ˮƽ��ֱ���
	// A�㵽B���ܷ����ӿ�ת��Ϊ����һ����������������һ�㣺
	// 1.A����C���ͨ��һ���ս����ӣ�C����B��ͨ��ˮƽ��ֱ����
	// 2.A����C���ͨ��ˮƽ��ֱ���ӣ�C����B��ͨ��һ���ս�����

	// �ж��Ƿ��Ѿ�ѡ��
	if ((begin_.x == -1 && begin_.y == -1) || (end_.x == -1 && end_.y == -1)) {
		return false;
	}
	// �ж��Ƿ�����ͬһ��
	if (begin_.x == end_.x && begin_.y == end_.y) {
		return false;
	}

	for (int i = 0; i < grid_num; i++) {
		for (int j = 0; j < grid_num; j++) {
			POINT m;
			m.x = i; m.y = j;
			if (i != begin_.x && i != end_.x && j != begin_.y && j != end_.y) {
				continue;
			}
			if ((i == begin_.x && j == begin_.y) || (i == end_.x && j == end_.y)) {
				continue;
			}
			if (this->isBlock(i, j) == true) {
				continue;
			}
			if (this->turn_once(begin_, m) == true && (this->vertical(m, end_) == true || this->horizon(m, end_) == true)) {
				return true;
			}
			if (this->turn_once(m, end_) == true && (this->vertical(begin_, m) == true || this->horizon(begin_, m) == true)) {
				return true;
			}
		}
	}
	return false;
}

// ���������ж�
bool Model::reMove() {
	bool ret = false;
	// ˮƽ������
	ret = this->horizon(this->begin, this->end);
	if (ret == true) {
		return true;
	}
	// ��ֱ������
	ret = this->vertical(this->begin, this->end);
	if (ret == true) {
		return true;
	}
	// һ�ιսǼ��
	ret = this->turn_once(this->begin, this->end);
	if (ret == true) {
		return true;
	}
	// ���ιսǼ��
	ret = this->turn_twice(this->begin, this->end);
	if (ret == true) {
		return true;
	}
	return false;
}

// �ж�����Ƿ��ڰ�ť��
int Model::mouseInButton(struct Button* pButton, MOUSEMSG* msg) {
	if (msg->x >= pButton->x && msg->x <= pButton->x + pButton->width &&
		msg->y >= pButton->y && msg->y <= pButton->y + pButton->height) {
		pButton->buttoncolor = RGB(235, 235, 235);
		pButton->textcolor = RGB(135, 135, 135);
		return 1; // ����ֵΪ1����������ڰ�ť��
	}
	pButton->buttoncolor = RGB(252, 252, 252);
	pButton->textcolor = RGB(150, 150, 150);
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

// ������2
void Model::mouseControl2() {
	// �ж��Ƿ��������Ϣ
	if (MouseHit()) {
		MOUSEMSG msg = GetMouseMsg();
		// �ж��û��Ƿ�����Ϸ����
		if (msg.x >= this->left && msg.x <= this->right &&
			msg.y >= this->top && msg.y <= this->down) {
			// �������Ϣת��Ϊ�����±�
			int row = (msg.y - this->top) / pic_size;
			int col = (msg.x - this->left) / pic_size;
			// �������Ϣ���зַ�
			switch (msg.uMsg) {
			case WM_LBUTTONDOWN: // ���������
				mciSendString("close click", nullptr, 0, nullptr);
				mciSendString("open ./resource/music/click.wav alias click", nullptr, 0, nullptr);
				mciSendString("play click", nullptr, 0, nullptr);
				if (this->state == true && this->gameMapVec[row][col] != -1) {
					this->begin.x = row;
					this->begin.y = col;
					this->state = false;
					// �����ظ����һ��ͼƬ
					if (this->begin.x == this->end.x && this->begin.y == this->end.y) {
						this->begin.x = -1; this->begin.y = -1;
						this->end.x = -1; this->end.y = -1;
					}
				}
				else if (this->state == false && this->gameMapVec[row][col] != -1) {
					this->end.x = row;
					this->end.y = col;
					this->state = true;
					// �����ظ����һ��ͼƬ
					if (this->begin.x == this->end.x && this->begin.y == this->end.y) {
						this->begin.x = -1; this->begin.y = -1;
						this->end.x = -1; this->end.y = -1;
					}
				}
				break;
			}
		}
	}
}

// ��Ϸʤ���ж�
bool Model::over() {
	int count = 0;
	// ��ͼƬȫ����ʧ����Ϸ����
	for (int i = 0; i < grid_num; i++) {
		for (int j = 0; j < grid_num; j++) {
			if (this->gameMapVec[i][j] == -1) {
				count++;
			}
		}
	}
	if (count == grid_num * grid_num) {
		return true; // ����ֵΪ1ʱ����Ϸ����
	}
	return false; // ����ֵΪ0ʱ����Ϸδ����
}