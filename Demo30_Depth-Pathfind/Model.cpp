#include "Model.h"
using namespace std;

Model::Model() :top(0), left(0), down(0), right(0) {
	this->gameMapVec = {
		{ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
		{ 1, 3, 1, 0, 1, 1, 1, 1, 1, 4, 1, 1 },
		{ 1, 0, 1, 0, 1, 0, 0, 0, 1, 0, 1, 1 },
		{ 1, 0, 1, 0, 0, 0, 1, 1, 1, 0, 1, 1 },
		{ 1, 0, 1, 0, 1, 0, 1, 1, 1, 0, 1, 1 },
		{ 1, 0, 0, 0, 1, 0, 1, 1, 1, 0, 1, 1 },
		{ 1, 0, 1, 1, 1, 0, 1, 1, 1, 0, 1, 1 },
		{ 1, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 1 },
		{ 1, 0, 1, 1, 1, 0, 1, 1, 0, 1, 1, 1 },
		{ 1, 0, 1, 0, 0, 0, 1, 1, 0, 1, 1, 1 },
		{ 1, 0, 1, 1, 1, 0, 1, 1, 0, 1, 1, 1 },
		{ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
	};
	this->beginPoint = { 1, 1 };
	this->endPoint = { 9, 1 };
	this->beginGame = creatButton(80, 500, 150, 50, "START", 38, 12, RGB(252, 252, 252), RGB(189, 134, 130));
	this->endGame = creatButton(420, 500, 150, 50, "END", 52, 12, RGB(252, 252, 252), RGB(189, 134, 130));
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
void Model::load(IMAGE& w1, IMAGE& ren, IMAGE& road, IMAGE& wall) {
	// ����ͼƬ
	loadimage(&w1, "./resource/picture/w1.png");
	loadimage(&ren, "./resource/picture/ren.bmp", grid_width, grid_height);
	loadimage(&road, "./resource/picture/road.bmp", grid_width, grid_height);
	loadimage(&wall, "./resource/picture/wall.bmp", grid_width, grid_height);
}

// ��������
void Model::bgm() {
	mciSendString("open ./resource/music/bk.mp3 alias BGM", nullptr, 0, nullptr);
	mciSendString("play BGM repeat", nullptr, 0, nullptr);
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
	setlinecolor(RGB(210, 118, 119));
	setlinestyle(PS_SOLID, 5);
	setfillcolor(pButton->buttoncolor);
	fillrectangle(pButton->x, pButton->y, pButton->x + pButton->width, pButton->y + pButton->height);
	settextcolor(pButton->textcolor);
	settextstyle(28, 0, "����", 0, 0, 1000, 0, 0, 0);
	outtextxy(pButton->x + pButton->dx, pButton->y + pButton->dy, pButton->text);
}

// ��Ϸ���ݳ�ʼ��
void Model::init() {
	// ���ñ�����ɫ
	setbkcolor(RGB(245, 215, 215));

	// ��Ϸ��ͼ���ݳ�ʼ��
	this->left = width / 2 - (col * grid_width) / 2;
	this->top = height / 2 - (row * grid_height) / 2;
	this->right = this->left + col * grid_width;
	this->down = this->top + row * grid_height;

	// ��Ϸ������̽�����ʼ��
	this->gameNodeVec.clear();
	for (int i = 0; i < row; i++) {
		vector<PathNode> lineGrid;
		for (int j = 0; j < col; j++) {
			struct PathNode newPoint = { 0, 0 };
			lineGrid.push_back(newPoint);
		}
		this->gameNodeVec.push_back(lineGrid);
	}

	// �����Ϊ�߹�
	this->gameNodeVec[this->beginPoint.y][this->beginPoint.x].isFind = true;
	// ��ʼ����£������ջ
	this->mystack.push(this->beginPoint);
}

// ���ƿ�ʼ����
void Model::welcome(IMAGE& w1) {
	SetWindowText(GetHWnd(), "C++���ӻ����Ѱ·");
	cleardevice();
	putimage(-40, 140, &w1);
	this->drawButton(this->beginGame);
	this->drawButton(this->endGame);
}

// ������Ϸ����
void Model::draw(IMAGE& road, IMAGE& wall, IMAGE& ren) {
	cleardevice();

	// ������Ϸ��ͼ
	for (int i = 0; i < row; i++) {
		for (int j = 0; j < col; j++) {
			// ������ͼλ������
			int x = this->left + grid_width * j;
			int y = this->top + grid_height * i;
			// ��ͼƬ��Ϣ���зַ�
			switch (this->gameMapVec[i][j]) {
			case 0://�յ�
				putimage(x, y, &road);
				break;
			case 1: // ש��
				putimage(x, y, &wall);
				break;
			case 2: // ·��
				setfillcolor(RGB(107, 146, 189));
				putimage(x, y, &road);
				solidcircle(x + grid_width / 2, y + grid_height / 2, 6);
				break;
			case 3: // ��ɫ
				putimage(x, y, &ren);
				break;
			case 4: // �յ�
				setfillcolor(RGB(210, 118, 119));
				putimage(x, y, &road);
				solidcircle(x + grid_width / 2, y + grid_height / 2, 10);
				break;
			default:
				break;
			}
		}
	}

	// �����ⲿ�߿�
	for (int x = 25; x >= 15; x--) {
		setlinecolor(RGB(210, 118, 119));
		rectangle(this->left - x, this->top - x, this->right + x, this->down + x);
	}
}

// ���Ѱ·�㷨
void Model::depthFinding(IMAGE& road, IMAGE& wall, IMAGE& ren) {
	Point currentPoint = beginPoint; // ��ǰ��
	Point searchPoint; // ��̽��

	while (1) {
		searchPoint = currentPoint;
		// �鿴��ǰ��ĵ�ǰ��̽����
		switch (this->gameNodeVec[currentPoint.y][searchPoint.x].dir) {
		case UP:
			// ��ǰ��̽��λ��ǰ���Ϸ���
			searchPoint.y--;
			// �ı䵱ǰ����̽����
			this->gameNodeVec[currentPoint.y][currentPoint.x].dir++;

			// �жϵ�ǰ���Ƿ��߹����Ƿ�Ϊ�յض�����ǽ��
			if (this->gameNodeVec[searchPoint.y][searchPoint.x].isFind == 0 &&
				this->gameMapVec[searchPoint.y][searchPoint.x] != 1) {
				// �������ǵ��߹�����ǰ��̽����ջ����ǰ���ƶ�����̽��λ��
				this->gameNodeVec[searchPoint.y][searchPoint.x].isFind = 1;
				this->mystack.push(searchPoint);
				this->gameMapVec[searchPoint.y][searchPoint.x] = 3;
				this->gameMapVec[currentPoint.y][currentPoint.x] = 2;
				currentPoint = searchPoint;
				Sleep(1000);
				this->draw(road, wall, ren);
				FlushBatchDraw();
			}
			break;
		case RIGHT:
			// ��ǰ��̽��λ��ǰ���ҷ���
			searchPoint.x++;
			// �ı䵱ǰ����̽����
			this->gameNodeVec[currentPoint.y][currentPoint.x].dir++;

			// �жϵ�ǰ���Ƿ��߹����Ƿ�Ϊ·��������ǽ��
			if (this->gameNodeVec[searchPoint.y][searchPoint.x].isFind == 0 &&
				this->gameMapVec[searchPoint.y][searchPoint.x] != 1) {
				// �������ǵ��߹�����ǰ��̽����ջ����ǰ���ƶ�����̽��λ��
				this->gameNodeVec[searchPoint.y][searchPoint.x].isFind = 1;
				this->mystack.push(searchPoint);
				this->gameMapVec[searchPoint.y][searchPoint.x] = 3;
				this->gameMapVec[currentPoint.y][currentPoint.x] = 2;
				currentPoint = searchPoint;
				Sleep(1000);
				this->draw(road, wall, ren);
				FlushBatchDraw();
			}
			break;
		case DOWN:
			// ��ǰ��̽��λ��ǰ���·���
			searchPoint.y++;
			// �ı䵱ǰ����̽����
			this->gameNodeVec[currentPoint.y][currentPoint.x].dir++;

			// �жϵ�ǰ���Ƿ��߹����Ƿ�Ϊ·��������ǽ��
			if (this->gameNodeVec[searchPoint.y][searchPoint.x].isFind == 0 &&
				this->gameMapVec[searchPoint.y][searchPoint.x] != 1) {
				// �������ǵ��߹�����ǰ��̽����ջ����ǰ���ƶ�����̽��λ��
				this->gameNodeVec[searchPoint.y][searchPoint.x].isFind = 1;
				this->mystack.push(searchPoint);
				this->gameMapVec[searchPoint.y][searchPoint.x] = 3;
				this->gameMapVec[currentPoint.y][currentPoint.x] = 2;
				currentPoint = searchPoint;
				Sleep(1000);
				this->draw(road, wall, ren);
				FlushBatchDraw();
			}
			break;
		case LEFT:
			// ��ǰ��̽��λ��ǰ���󷽵�
			searchPoint.x--;

			// �жϵ�ǰ���Ƿ��߹����Ƿ�Ϊ·��������ǽ��
			if (this->gameNodeVec[searchPoint.y][searchPoint.x].isFind == 0 &&
				this->gameMapVec[searchPoint.y][searchPoint.x] != 1) {
				// �������ǵ��߹�����ǰ��̽����ջ����ǰ���ƶ�����̽��λ��
				this->gameNodeVec[searchPoint.y][searchPoint.x].isFind = 1;
				this->mystack.push(searchPoint);
				this->gameMapVec[searchPoint.y][searchPoint.x] = 3;
				this->gameMapVec[currentPoint.y][currentPoint.x] = 2;
				currentPoint = searchPoint;
				Sleep(1000);
				this->draw(road, wall, ren);
				FlushBatchDraw();
			}
			else { // ˳ʱ����̽�����������򣬴�ʱ���������������
				// ���ջ��Ԫ��
				this->mystack.pop();
				this->gameMapVec[currentPoint.y][currentPoint.x] = 2;
				// ������µ�ջ��Ԫ�ظ���currentPoint
				currentPoint = mystack.top();
				this->gameMapVec[currentPoint.y][currentPoint.x] = 3;
				Sleep(1000);
				this->draw(road, wall, ren);
				FlushBatchDraw();
			}
			break;
		}

		// ���Ѱ·�����յ㣬������ѭ��
		if (currentPoint.y == endPoint.y &&
			currentPoint.x == endPoint.x) {
			mciSendString("close win", nullptr, 0, nullptr);
			mciSendString("open ./resource/music/win.wav alias win", NULL, 0, NULL);
			mciSendString("play win", nullptr, 0, nullptr);
			Sleep(1000);
			this->draw(road, wall, ren);
			FlushBatchDraw();
			break;
		}

		// ���ջ���ˣ�������ѭ��
		if (mystack.empty()) {
			break;
		}
	}
}

// �ж�����Ƿ��ڰ�ť��
int Model::mouseInButton(struct Button* pButton, MOUSEMSG* msg) {
	if (msg->x >= pButton->x && msg->x <= pButton->x + pButton->width &&
		msg->y >= pButton->y && msg->y <= pButton->y + pButton->height) {
		pButton->buttoncolor = RGB(235, 235, 235);
		pButton->textcolor = RGB(189, 134, 130);
		return 1; // ����ֵΪ1����������ڰ�ť��
	}
	pButton->buttoncolor = RGB(252, 252, 252);
	pButton->textcolor = RGB(189, 134, 130);
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