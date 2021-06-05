#include "Model.h"
using namespace std;

// ������Դ
void Model::load(IMAGE& bk, IMAGE& w1, IMAGE& w2, IMAGE& w3) {
	// ����ͼƬ
	loadimage(&bk, "./resource/picture/bk.jpg", width, height * 1.11);
	loadimage(&w1, "./resource/picture/w1.png", 700, 151);
	loadimage(&w2, "./resource/picture/w2.png", 480, 104);
	loadimage(&w3, "./resource/picture/w3.png", 750, 125);
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

// ������֮��ľ���
double Model::distance(const double x1, const double y1, const double x2, const double y2) {
	// distance=������(x2-x1)^2+(y2-y1)^2
	return sqrt((x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2));
}

// ʹ�ô�����������ƶ�
void Model::updatePos() {
	// ʹ������ڴ������м�
	this->g_pos.x = this->roleVec[0].x - width / 2;
	this->g_pos.y = this->roleVec[0].y - height / 2;
}

// ���ô����Լ���Ϸ��ͼ�ı�����ɫ
void Model::setMap(IMAGE& map) {
	setbkcolor(RGB(244, 215, 215));
	cleardevice();

	// ��ͼĬ���ǻ��Ƶ������ϵģ�����Ҫָ����ͼ�豸
	SetWorkingImage(&map); // ָ����ͼ�豸Ϊmap

	setbkcolor(RGB(255, 255, 255));
	cleardevice();

	SetWorkingImage(); // �ָ�Ĭ�ϻ�ͼ�豸��������
}

// ��ʼ����Ϸ����
void Model::init() {
	// �������������
	srand((unsigned)time(nullptr) * rand());

	// ��ʼ���������
	struct Role newrole;
	newrole.r = 25;
	newrole.x = rand() % (int)(map_width - 2 * newrole.r) + newrole.r;
	newrole.y = rand() % (int)(map_height - 2 * newrole.r) + newrole.r;
	newrole.movestep = 8;
	newrole.radian = -pi / 2;
	newrole.cx = newrole.x;
	newrole.cy = newrole.y - newrole.r;
	newrole.flag = true; // ��ʼ״̬��Ҵ��
	newrole.color = RGB(244, 215, 215);
	this->roleVec.push_back(newrole);

	// ��ʼ����ҷ�������
	for (int i = 1; i < derived_num; i++) {
		struct Role newrole;
		newrole.x = this->roleVec[0].x;
		newrole.y = this->roleVec[0].y;
		newrole.radian = -pi / 2;
		newrole.cx = newrole.x; // �����ڹ�ĩ��x����
		newrole.cy = newrole.y - newrole.r; // �����ڹ�ĩ��y����
		newrole.shootstep = 9; // ��ʼ��������ҷ������ٶ�
		newrole.flag = false; // ��ʼ��������ҷ���ȫ������
		this->roleVec.push_back(newrole);
	}

	// ��ʼ��ʳ������
	for (int i = 0; i < food_num; i++) {
		struct Food newfood;
		newfood.r = rand() % 6 + 1.0;
		newfood.x = rand() % (int)(map_width - 2 * newfood.r) + newfood.r;
		newfood.y = rand() % (int)(map_height - 2 * newfood.r) + newfood.r;
		newfood.flag = true; // ��ʼ״̬ʳ�����
		newfood.color = RGB(rand() % 256, rand() % 256, rand() % 256);
		this->foodVec.push_back(newfood);
	}

	// ��ʼ���ӵ�����
	for (int i = 0; i < bullet_num; i++) {
		struct Bullet newbull;
		newbull.x = this->roleVec[0].x;
		newbull.y = this->roleVec[0].y;
		newbull.r = 10;
		newbull.shootstep = 10; // �����ӵ��ٶ�
		newbull.flag = false; // ��ʼ�������ӵ�ȫ������
		this->bullVec.push_back(newbull);
	}
}

// ���ƿ�ʼ����
void Model::welcome(IMAGE& bk, IMAGE& w1, IMAGE& w2, IMAGE& w3) {
	putimage(0, -78, &bk);
	setbkmode(TRANSPARENT);
	SetWindowText(GetHWnd(), "C++�������ս");

	drawAlpha(&w1, 60, 105);
	drawAlpha(&w2, 125, 334);
	drawAlpha(&w3, 85, 224);

	while (!_kbhit()) {
	}
}

// ������Ϸ����
void Model::draw(IMAGE& map) {
	cleardevice();

	// ��ͼĬ���ǻ��Ƶ������ϵģ�����Ҫָ����ͼ�豸
	SetWorkingImage(&map); // ָ����ͼ�豸Ϊmap

	cleardevice();

	// ��ʳ����л���
	for (int i = 0; i < food_num; i++) {
		if (this->foodVec[i].flag == true) {
			// �����������л���
			setfillcolor(this->foodVec[i].color);
			solidcircle(this->foodVec[i].x, this->foodVec[i].y, this->foodVec[i].r);
		}
	}

	// �Խ�ɫ���л���
	for (int i = 0; i < derived_num; i++) {
		if (this->roleVec[i].flag == true) {
			// �Խ�ɫ��Χ�ڹܽ��л���
			setfillcolor(this->roleVec[i].color);
			// ������Χ����
			solidpie(this->roleVec[i].x - this->roleVec[i].r * 1.4, this->roleVec[i].y - this->roleVec[i].r * 1.4, this->roleVec[i].x + this->roleVec[i].r * 1.4, this->roleVec[i].y + this->roleVec[i].r * 1.4, -this->roleVec[0].radian - pi / 9, -this->roleVec[0].radian + pi / 9);
			clearpie(this->roleVec[i].x - this->roleVec[i].r * 1.2, this->roleVec[i].y - this->roleVec[i].r * 1.2, this->roleVec[i].x + this->roleVec[i].r * 1.2, this->roleVec[i].y + this->roleVec[i].r * 1.2, -this->roleVec[0].radian - pi / 9 - 0.1, -this->roleVec[0].radian + pi / 9 + 0.1);
			// ������Χ������
			POINT point[3] = {
				point[0].x = this->roleVec[i].x + cos(this->roleVec[0].radian) * this->roleVec[i].r * 1.7, point[0].y = this->roleVec[i].y + sin(this->roleVec[0].radian) * this->roleVec[i].r * 1.7,
				point[1].x = this->roleVec[i].x + cos(this->roleVec[0].radian - pi / 12) * this->roleVec[i].r * 1.38, point[1].y = this->roleVec[i].y + sin(this->roleVec[0].radian - pi / 12) * this->roleVec[i].r * 1.38,
				point[2].x = this->roleVec[i].x + cos(this->roleVec[0].radian + pi / 12) * this->roleVec[i].r * 1.38, point[2].y = this->roleVec[i].y + sin(this->roleVec[0].radian + pi / 12) * this->roleVec[i].r * 1.38,
			};
			solidpolygon(point, 3);

			// �Խ�ɫ���л���
			setfillcolor(this->roleVec[i].color);
			solidcircle(this->roleVec[i].x, this->roleVec[i].y, this->roleVec[i].r);
		}
	}

	// ���ӵ����л���
	for (int i = 0; i < bullet_num; i++) {
		if (this->bullVec[i].flag == true) {
			setfillcolor(this->bullVec[i].color);
			solidcircle(this->bullVec[i].x, this->bullVec[i].y, this->bullVec[i].r);
		}
	}

	SetWorkingImage(); // �ָ�Ĭ�ϻ�ͼ�豸��������
	this->updatePos(); // ����λ��
	putimage(0, 0, width, height, &map, g_pos.x, g_pos.y); // �Ի��ƺ�ĵ�ͼ�������
	FlushBatchDraw(); // ˫����ͼ�����
}

// ʳ�ﱻ�Ե������²���ʳ��
void Model::creatFood(const int i) {
	this->foodVec[i].flag = true; // ��ʳ��״̬����Ϊ����
	this->foodVec[i].r = rand() % 8 + 1.0; // ����ʳ���С���
	this->foodVec[i].x = rand() % (int)(map_width - 2 * this->foodVec[i].r) + this->foodVec[i].r;
	this->foodVec[i].y = rand() % (int)(map_height - 2 * this->foodVec[i].r) + this->foodVec[i].r;
	this->foodVec[i].color = RGB(rand() % 256, rand() % 256, rand() % 256);
}

// �Ե�ʳ��
void Model::eatFood(struct Role* role, const int k) {
	// ʳ�ﱻ�Ե�
	for (int i = 0; i < food_num; i++) {
		if (this->foodVec[i].flag == true) { // ���ʳ�����
			if (this->distance(role->x, role->y, this->foodVec[i].x, this->foodVec[i].y) < role->r) {
				// ���ʳ����������Բ�ľ�С����Ұ뾶����ʳ�ﱻ�Ե�
				this->foodVec[i].flag = false; // ʳ�ﱻ�Ե�ʳ����ʧ
				role->r += this->foodVec[i].r / 6; // �Ե�ʳ�����
				role->movestep -= this->foodVec[i].r / 180; // �Ե�ʳ����ٶȱ���
				if (role->r > 200) { // ����ҵ����������һ������
					role->r = 200;
				}
				if (role->movestep < 2.0) { // ����ҵ���С�ٶ���һ������
					role->movestep = 2.0;
				}
			}
		}
		else { // ���ʳ�ﲻ����
			this->creatFood(i);
		}
	}

	// �ӵ����Ե�
	for (int i = 0; i < bullet_num; i++) {
		if (this->bullVec[i].flag == true) { // ����ӵ�����
			if (this->distance(role->x, role->y, this->bullVec[i].x, this->bullVec[i].y) < role->r) {
				// ����ӵ����������Բ�ľ�С����Ұ뾶����ʳ�ﱻ�Ե�
				this->bullVec[i].flag = false; // �ӵ����Ե��ӵ���ʧ
				role->r += 10.0 / 6; // �Ե�ʳ�����
				role->movestep -= 10.0 / 180; // �Ե�ʳ����ٶȱ���
				if (role->r > 200) { // ����ҵ����������һ������
					role->r = 200;
				}
				if (role->movestep < 2.0) { // ����ҵ���С�ٶ���һ������
					role->movestep = 2.0;
				}
			}
		}
	}
}

// �����ӵ�
void Model::creatBullet(struct Role* role, const double radian) {
	for (int i = 0; i < bullet_num; i++) {
		if (this->bullVec[i].flag == false) { // ����ӵ������������ӵ�
			this->bullVec[i].flag = true;
			this->bullVec[i].curradian = radian;
			this->bullVec[i].x = role->cx; // �����ӵ�x����
			this->bullVec[i].y = role->cy; // �����ӵ�y����
			this->bullVec[i].color = role->color; // �����ӵ���ɫ
			this->bullVec[i].shootstep = 10; // �����ӵ��ٶ�
			this->bullVec[i].vx = this->bullVec[i].shootstep * cos(this->bullVec[i].curradian);
			this->bullVec[i].vy = this->bullVec[i].shootstep * sin(this->bullVec[i].curradian);
			role->r -= 10.0 / 6; // �ӵ������ȥ��������ͼ�С
			role->movestep += 10.0 / 180; // �ӵ������ȥ������ٶȱ��
			break;
		}
	}
}

// �ӵ����ƶ�
void Model::moveBullet() {
	for (int i = 0; i < bullet_num; i++) {
		if (this->bullVec[i].flag == true) { // ���ڵ��ӵ��ǿ����ƶ���
			if (this->bullVec[i].shootstep <= 0) {
				continue;
			}
			if (this->bullVec[i].x <= 15 || this->bullVec[i].x >= map_width - 15.0 || this->bullVec[i].y <= 15 || this->bullVec[i].y >= map_height - 15.0) {
				continue;
			}
			this->bullVec[i].x += this->bullVec[i].vx;
			this->bullVec[i].y += this->bullVec[i].vy;
			this->bullVec[i].shootstep -= 0.35;
			this->bullVec[i].vx = this->bullVec[i].shootstep * cos(this->bullVec[i].curradian); // ����ÿ���ӵ���x�����ƶ�����
			this->bullVec[i].vy = this->bullVec[i].shootstep * sin(this->bullVec[i].curradian); // ����ÿ���ӵ���y�����ƶ�����
		}
	}
}

// ����ʵ��
void Model::creatRole(struct Role* role, const double radian) {
	for (int i = 1; i < derived_num; i++) {
		if (this->roleVec[i].flag == false) {//�����ҷ��������������
			this->roleVec[i].flag = true;
			this->roleVec[i].x = role->cx; // ������ҷ���x����
			this->roleVec[i].y = role->cy; // ������ҷ���y����
			this->roleVec[i].color = role->color; // ������ҷ�����ɫ
			this->roleVec[i].r = role->r / 2; // ������ҷ����С
			this->roleVec[i].shootstep = 9; // ������ҷ������ٶ�
			role->r /= 2; // �����������ͼ�С
			role->movestep += role->r / 30; // ���������ٶ�����
			this->roleVec[i].curradian = radian;
			this->roleVec[i].movestep = role->movestep;
			this->roleVec[i].vx = this->roleVec[i].shootstep * cos(this->roleVec[i].curradian); // ����ÿ�η�����x�����ƶ�����
			this->roleVec[i].vy = this->roleVec[i].shootstep * sin(this->roleVec[i].curradian); // ����ÿ�η�����y�����ƶ�����
			break;
		}
	}
}

// �����ƶ�
void Model::moveRole() {
	for (int i = 1; i < derived_num; i++) {
		if (this->roleVec[i].flag == true) { // ���ڵ���ҷ����ǿ����ƶ���
			if (this->roleVec[i].shootstep <= 0) {
				continue;
			}
			if (this->roleVec[i].x <= (5 + this->roleVec[i].r) ||
				this->roleVec[i].x >= map_width - (5 + this->roleVec[i].r) ||
				this->roleVec[i].y <= (5 + this->roleVec[i].r) ||
				this->roleVec[i].y >= map_height - (5 + this->roleVec[i].r)) {
				continue;
			}
			this->roleVec[i].x += this->roleVec[i].vx;
			this->roleVec[i].y += this->roleVec[i].vy;
			this->roleVec[i].shootstep -= 0.35;
			this->roleVec[i].vx = this->roleVec[i].shootstep * cos(this->roleVec[i].curradian); // ����ÿ����ҷ�����x�����ƶ�����
			this->roleVec[i].vy = this->roleVec[i].shootstep * sin(this->roleVec[i].curradian); // ����ÿ����ҷ�����y�����ƶ�����
		}
	}
}

// �����ں�
void Model::mixRole(struct Role* role) {
	// ��ҷ�������ҵ��ں�
	for (int i = 1; i < derived_num; i++) {
		if (this->roleVec[i].flag == true) { // �����һ�������
			if (this->distance(role->x, role->y, this->roleVec[i].x, this->roleVec[i].y) < role->r) {
				// �������֮���Բ�ľ�С�ڷ���뾶��������ں�
				this->roleVec[i].flag = false; // �ںϺ������ʧ
				role->r += this->roleVec[i].r; // �ںϺ����ͱ��
				role->movestep -= this->roleVec[i].r / 30; // �ںϺ��ٶȱ���
				if (role->r > 200) { // ����ҵ����������һ������
					role->r = 200;
				}
				if (role->movestep < 2.0) { // ����ҵ���С�ٶ���һ������
					role->movestep = 2.0;
				}
			}
		}
	}
}

// ��ɫ�
void Model::actRole() {
	for (int i = 0; i < derived_num; i++) {
		if (this->roleVec[i].flag == true) {//���ڵ���һ������Գ�ʳ��
			this->eatFood(&this->roleVec[i], i);
		}
	}
	this->mixRole(&this->roleVec[0]);
}

// ����ƶ����첽�������ƣ�
void Model::keyDown1() {
	// �жϵ�ǰWindowsǰ̨Ӧ�ó��������Ƿ�Ϊ��ǰeasyx��ͼ����
	if (GetForegroundWindow() == GetHWnd()) {
		if ((GetAsyncKeyState(VK_UP) || GetAsyncKeyState('W'))) { // ����ϼ�
			for (int i = 0; i < this->roleVec.size(); i++) {
				if (this->roleVec[i].flag == true) {
					if (this->roleVec[i].y <= (5 + this->roleVec[i].r)) {
						continue;
					}
					this->roleVec[i].y -= this->roleVec[i].movestep;
				}
			}
		}
		if ((GetAsyncKeyState(VK_DOWN) || GetAsyncKeyState('S'))) { // ����¼�
			for (int i = 0; i < this->roleVec.size(); i++) {
				if (this->roleVec[i].flag == true) {
					if (this->roleVec[i].y >= map_height - (5 + this->roleVec[i].r)) {
						continue;
					}
					this->roleVec[i].y += this->roleVec[i].movestep;
				}
			}
		}
		if ((GetAsyncKeyState(VK_LEFT) || GetAsyncKeyState('A'))) { // ������
			for (int i = 0; i < this->roleVec.size(); i++) {
				if (this->roleVec[i].flag == true) {
					if (this->roleVec[i].x <= (5 + this->roleVec[i].r)) {
						continue;
					}
					this->roleVec[i].x -= this->roleVec[i].movestep;
				}
			}
		}
		if ((GetAsyncKeyState(VK_RIGHT) || GetAsyncKeyState('D'))) { // ����Ҽ�
			for (int i = 0; i < this->roleVec.size(); i++) {
				if (this->roleVec[i].flag == true) {
					if (this->roleVec[i].x >= map_width - (5 + this->roleVec[i].r)) {
						continue;
					}
					this->roleVec[i].x += this->roleVec[i].movestep;
				}
			}
		}
		if (GetAsyncKeyState('J')) { // ���'J'��
			this->roleVec[0].radian -= 0.1;
		}
		if (GetAsyncKeyState('K')) { // ���'K'��
			this->roleVec[0].radian += 0.1;
		}
	}
	for (int i = 0; i < this->roleVec.size(); i++) {
		if (this->roleVec[i].flag == true) {
			this->roleVec[i].cx = this->roleVec[i].x + cos(this->roleVec[0].radian) * this->roleVec[i].r;
			this->roleVec[i].cy = this->roleVec[i].y + sin(this->roleVec[0].radian) * this->roleVec[i].r;
		}
	}
}

// ��ҹ��ܣ�ͬ���������ƣ�
void Model::keyDown2() {
	// �����û���Ϣ
	char key;
	// �ж��Ƿ��м�����Ϣ
	while (_kbhit()) {
		key = (char)_getch();
		fflush(stdin); // ��ռ�����Ϣ������
		switch (key) {
			// 'U'�������ӵ�
		case'U':
		case'u':
			for (int i = 0; i < derived_num; i++) {
				if (this->roleVec[i].flag == true && this->roleVec[i].r > 25) {
					this->creatBullet(&this->roleVec[i], this->roleVec[0].radian);
				}
			}
			break;
			// 'I'����ҷֽ�
		case'I':
		case'i':
			for (int i = derived_num - 1; i >= 0; i--) {
				if (this->roleVec[i].flag == true && this->roleVec[i].r > 60) {
					this->creatRole(&this->roleVec[i], this->roleVec[0].radian);
				}
			}
			break;
		}
	}
}

// ������
void Model::mouseControl() {
	// �ж��Ƿ��������Ϣ
	if (MouseHit()) {
		// ��ȡ�����Ϣ
		MOUSEMSG msg = GetMouseMsg();
		this->roleVec[0].radian = atan2((double)(msg.y - height / 2.0), (double)(msg.x - width / 2.0));
		// atan2()�������ڻ�ȡy/x�ķ�����ֵ���Ի��ȱ�ʾ��
		this->roleVec[0].cx = this->roleVec[0].x + cos(this->roleVec[0].radian) * this->roleVec[0].r;
		this->roleVec[0].cy = this->roleVec[0].y + sin(this->roleVec[0].radian) * this->roleVec[0].r;
		// ���������£������ӵ�
		if (msg.uMsg == WM_LBUTTONDOWN) {
			for (int i = 0; i < derived_num; i++) {
				if (this->roleVec[i].flag == true && this->roleVec[i].r > 25) {
					this->creatBullet(&this->roleVec[i], this->roleVec[0].radian);
				}
			}
		}
		// ����Ҽ����£���ҷֽ�
		if (msg.uMsg == WM_RBUTTONDOWN) {
			for (int i = derived_num - 1; i >= 0; i--) {
				if (this->roleVec[i].flag == true && this->roleVec[i].r > 60) {
					this->creatRole(&this->roleVec[i], this->roleVec[0].radian);
				}
			}
		}
	}
}