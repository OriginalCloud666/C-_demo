#include "Model.h"
using namespace std;

Model::Model() {
	this->beginGame = creatButton(80, 220, 150, 50, "��ʼ��Ϸ", 18, 12, WHITE, RGB(150, 150, 150));
	this->endGame = creatButton(395, 220, 150, 50, "������Ϸ", 18, 12, WHITE, RGB(150, 150, 150));
}

// ��ȡfood
struct Food& Model::getFood() {
	return this->food;
}

// ��ȡsnake
struct Snake& Model::getSnake() {
	return this->snake;
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
void Model::load(IMAGE& bk) {
	loadimage(&bk, "./resource/picture/bk.jpg", width * 1.094, height * 1.728);
}

// ���ñ�������
void Model::bgm() {
	mciSendString("open ./resource/music/bk.mp3 alias BGM", nullptr, 0, nullptr);
	mciSendString("play BGM repeat", nullptr, 0, nullptr);
}

// ������ť
struct Button* Model::creatButton(const int x, const int y, const int width, const int height,
	const char* text, const int dx, const int dy, COLORREF buttoncolor, COLORREF textcolor) {
	struct Button* pButton = (struct Button*)malloc(sizeof(struct Button));
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
	setlinecolor(RGB(240, 240, 240));
	setlinestyle(PS_SOLID, 5);
	setfillcolor(pButton->buttoncolor);
	fillrectangle(pButton->x, pButton->y, pButton->x + pButton->width, pButton->y + pButton->height);
	settextcolor(pButton->textcolor);
	settextstyle(28, 0, "����", 0, 0, 1000, 0, 0, 0);
	outtextxy(pButton->x + pButton->dx, pButton->y + pButton->dy, pButton->text);
}

// ��ʼ����Ϸ����
void Model::init() {
	// �������������
	srand((unsigned)time(nullptr) * rand());

	// ���ߵ����ݽ��г�ʼ��
	this->snake.size = 5; // �����ֻ�����
	this->snake.dir = RIGHT; // ����ͷ��ʼ��������
	struct Point newpoint = { 0, 0 };
	this->snake.point.push_back(newpoint);
	// ���������ݽ��г�ʼ��
	for (int i = 5; i < snake_size; i++) {
		struct Point newpoint = { -100, -100 };
		this->snake.point.push_back(newpoint);
	}
}

// ���ƿ�ʼ����
void Model::welcome(IMAGE& bk) {
	cleardevice();
	putimage(-30, -235, &bk);
	setbkmode(TRANSPARENT);
	SetWindowText(GetHWnd(), "C++̰����");

	this->drawButton(beginGame);
	this->drawButton(endGame);

	// ���������Ϣ
	settextcolor(RGB(175, 175, 175));
	settextstyle(25, 0, "����", 0, 0, 1000, 0, 0, 0);
	outtextxy(12, 17, "̰������Ϸ");

	settextcolor(RGB(175, 175, 175));
	settextstyle(23, 0, "����", 0, 0, 1000, 0, 0, 0);
	outtextxy(16, 54, "���ߣ�����");
}

// ������Ϸ����
void Model::draw(IMAGE& bk) {
	cleardevice();
	putimage(-30, -235, &bk);

	// ���������Ϣ
	settextcolor(RGB(175, 175, 175));
	settextstyle(25, 0, "����", 0, 0, 1000, 0, 0, 0);
	outtextxy(12, 17, "̰������Ϸ");

	settextcolor(RGB(175, 175, 175));
	settextstyle(23, 0, "����", 0, 0, 1000, 0, 0, 0);
	outtextxy(16, 54, "���ߣ�����");

	// ����������ͼ
	for (int i = 0; i < grid_row; i++) {
		for (int j = 0; j < grid_col; j++) {
			// ������ͼλ������
			int x = grid_width * j;
			int y = grid_height * i;
			// �Ե�ͼ���ӽ��л���
			setlinecolor(RGB(240, 240, 240));
			setlinestyle(PS_SOLID, 3);
			setfillcolor(WHITE);
			fillrectangle(x, y, x + grid_width, y + grid_height);
		}
	}

	// ����������
	for (int i = 1; i < this->snake.size; i++) {
		setlinecolor(RGB(240, 240, 240));
		setlinestyle(PS_SOLID, 3);
		setfillcolor(RGB(252, 208, 199));
		fillrectangle(this->snake.point[i].x, this->snake.point[i].y, this->snake.point[i].x + grid_width, this->snake.point[i].y + grid_height);
	}
	// �����ߵ�ͷ��
	setlinecolor(RGB(240, 240, 240));
	setlinestyle(PS_SOLID, 3);
	setfillcolor(RGB(244, 165, 215));
	fillrectangle(this->snake.point[0].x, this->snake.point[0].y, this->snake.point[0].x + grid_width, this->snake.point[0].y + grid_height);

	// ����ʳ��
	if (this->food.flag == 1) {
		setlinecolor(RGB(240, 240, 240));
		setlinestyle(PS_SOLID, 3);
		setfillcolor(RGB(91, 173, 255));
		fillrectangle(this->food.point.x, this->food.point.y, this->food.point.x + grid_width, this->food.point.y + grid_height);
	}
}

// �ߵ��ƶ�
void Model::snackMove() {
	// ������ƶ�����
	for (int i = this->snake.size - 1; i > 0; i--) {
		this->snake.point[i].x = this->snake.point[i - 1].x;
		this->snake.point[i].y = this->snake.point[i - 1].y;
	}
	switch (this->snake.dir) {
	case UP:
		snake.point[0].y -= grid_width;
		break;
	case DOWN:
		snake.point[0].y += grid_height;
		break;
	case LEFT:
		snake.point[0].x -= grid_width;
		break;
	case RIGHT:
		snake.point[0].x += grid_width;
		break;
	}
}

// ʳ�ﱻ����
void Model::creatFood() {
	// ��ʳ������ͷ����
	this->food.point.x = rand() % (width / grid_width) * grid_width;
	this->food.point.y = rand() % (height / grid_height) * grid_height;
	// ʳ�ﲻ�ܳ�����������
	while (1) {
		int flag = 0; // ״̬��ʶ�������ж�ʳ���Ƿ������������
		for (int i = 0; i < this->snake.size; i++) {
			if (this->snake.point[i].x == this->food.point.x && this->snake.point[i].y == this->food.point.y) {
				this->food.point.x = rand() % (width / grid_width) * grid_width;
				this->food.point.y = rand() % (height / grid_height) * grid_height;
				flag = 1;
			}
		}
		if (flag == 0) { // ���ʳ��û�г������������κ�һ�ڣ�������ѭ��
			this->food.flag = 1; // ʳ�����
			break;
		}
	}
}

// ʳ�ﱻ�Ե�
void Model::eatFood() {
	if (this->snake.point[0].x == this->food.point.x && this->snake.point[0].y == this->food.point.y) {
		mciSendString("close eat", nullptr, 0, nullptr);
		mciSendString("open ./resource/music/eat.wav alias eat", nullptr, 0, nullptr);
		mciSendString("play eat", nullptr, 0, nullptr);
		this->snake.size++;
		this->food.flag = 0;//ʳ�ﱻ�Ե�����ʧ
	}
}

// ������Ӧ��ͬ����
void Model::keyDown() {
	// �ж��Ƿ��а�����Ϣ
	while (_kbhit()) {
		// ���ڽ����û�����
		char userKey = _getch();
		fflush(stdin); // ��ռ�����Ϣ������

		switch (userKey) {
		case 'W':
		case 'w':
		case 72:
			if (this->snake.dir != DOWN) {
				this->snake.dir = UP;
			}
			break;
		case 'S':
		case 's':
		case 80:
			if (this->snake.dir != UP) {
				this->snake.dir = DOWN;
			}
			break;
		case 'A':
		case 'a':
		case 75:
			if (this->snake.dir != RIGHT) {
				this->snake.dir = LEFT;
			}
			break;
		case 'D':
		case 'd':
		case 77:
			if (this->snake.dir != LEFT) {
				this->snake.dir = RIGHT;
			}
			break;
		}
	}
}

// �ж�����Ƿ��ڰ�ť��
int Model::mouseInButton(struct Button* pButton, MOUSEMSG* msg) {
	if (msg->x >= pButton->x && msg->x <= pButton->x + pButton->width &&
		msg->y >= pButton->y && msg->y <= pButton->y + pButton->height) {
		pButton->buttoncolor = RGB(220, 220, 220);
		pButton->textcolor = RGB(120, 120, 120);
		return 1; // ����ֵΪ1����������ڰ�ť��
	}
	pButton->buttoncolor = WHITE;
	pButton->textcolor = RGB(150, 150, 150);
	return 0; // ����ֵΪ0��������겻�ڰ�ť��
}

// ������
int Model::mouseControl(struct Button* pButton1, struct Button* pButton2) {
	// �ж��Ƿ��������Ϣ
	if (MouseHit()) {
		MOUSEMSG msg = GetMouseMsg();
		// �ж��û��Ƿ�����ť
		if (this->mouseInButton(pButton1, &msg) == 1 && msg.uMsg == WM_LBUTTONDOWN) {
			mciSendString("close end", nullptr, 0, nullptr);
			mciSendString("open ./resource/music/click.wav alias click", nullptr, 0, nullptr);
			mciSendString("play click", nullptr, 0, nullptr);
			return 1;
		}
		else if (this->mouseInButton(pButton2, &msg) == 1 && msg.uMsg == WM_LBUTTONDOWN) {
			mciSendString("close end", nullptr, 0, nullptr);
			mciSendString("open ./resource/music/click.wav alias click", nullptr, 0, nullptr);
			mciSendString("play click", nullptr, 0, nullptr);
			exit(0); // ��Ϸ����
		}
	}
	return 0;
}

// ��Ϸ��������1
int Model::condition1() {
	// ����ײ���Լ�ʱ��Ϸ����
	for (int i = 1; i < this->snake.size; i++) {
		if (this->snake.point[i].x == this->snake.point[0].x && this->snake.point[i].y == this->snake.point[0].y) {
			return 1; // ����ֵΪ1ʱ��Ϸ����
		}
	}
	return 0; // ����ֵΪ0ʱ��Ϸ����
}

// ��Ϸ��������2
int Model::condition2() {
	// ����ײ��ǽ��ʱ��Ϸ����
	if (this->snake.point[0].x < 0) { // ײ�����
		return 1; // ����ֵΪ1ʱ��Ϸ����
	}
	else if (this->snake.point[0].x > width - grid_width) {
		return 1; // ����ֵΪ1ʱ��Ϸ����
	}
	else if (this->snake.point[0].y < 0) {
		return 1; // ����ֵΪ1ʱ��Ϸ����
	}
	else if (this->snake.point[0].y > height - grid_height) {
		return 1; // ����ֵΪ1ʱ��Ϸ����
	}
	return 0; // ����ֵΪ0ʱ��Ϸ����
}

// ��Ϸ���������ж�
bool Model::over() {
	// ���������������һ����������Ϸ����
	if (this->condition1() == 1 || this->condition2() == 1) {
		return true; // ����ֵΪ1ʱ��Ϸ����
	}
	return false; // ����ֵΪ0ʱ��Ϸ����
}