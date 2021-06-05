#include "Model.h"
using namespace std;

Model::Model() :flag(0), start(0), finish(0), duration(0.0), row(0), col(0), mine_num(0), top(0), left(0), down(0), right(0) {}

// ��ȡstart
clock_t& Model::getStart() {
	return this->start;
}

// ��ȡfinish
clock_t& Model::getFinish() {
	return this->finish;
}

// ������Դ
void Model::load(IMAGE& bk, IMAGE& w1, IMAGE& w2, IMAGE& w3, IMAGE& mine, vector<IMAGE>& img, vector<int>& imgIndex) {
	// ��������ͼƬ
	for (int i = 0; i < pic_num; i++) {
		char fileName[25];
		sprintf(fileName, "./resource/picture/%d.jpg", imgIndex[i]);
		loadimage(&img[i], fileName, pic_size, pic_size);
	}
	loadimage(&mine, "./resource/picture/9.bmp", pic_size, pic_size);
	loadimage(&bk, "./resource/picture/bk.jpg", width, height * 1.257);
	loadimage(&w1, "./resource/picture/w1.png", 728, 122);
	loadimage(&w2, "./resource/picture/w2.png", 450, 75);
	loadimage(&w3, "./resource/picture/w3.png", 500, 108);
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

// ��ʼ������
void Model::init(const int row, const int col, const int mine_num) {
	// �������������
	srand((unsigned)time(nullptr) * rand());

	// ��ʼ����Ϸ���м��׵�����
	this->row = row;
	this->col = col;
	this->mine_num = mine_num;

	// ��ʼ������
	this->left = width / 2 - (this->col * pic_size) / 2; // ��Ϸ�������Ͻ�x����
	this->top = height / 2 - (this->row * pic_size) / 2; // ��Ϸ�������Ͻ�y����
	this->right = this->left + this->col * pic_size; // ��Ϸ�������½�x����
	this->down = this->top + this->row * pic_size; // ��Ϸ�������½�y����

	// ��ʼ��vector����
	this->gameMapVec.clear();
	for (int i = 0; i < this->row; i++) {
		vector<int> lineGrid;
		for (int j = 0; j < this->col; j++) {
			lineGrid.push_back(0);
		}
		this->gameMapVec.push_back(lineGrid);
	}

	// ���ף�������-1��ʾ�ף�
	for (int i = 0; i < this->mine_num;) {
		// ����Ϸ��ͼ�����������
		int cur_row = rand() % this->row;
		int cur_col = rand() % this->col;
		if (this->gameMapVec[cur_row][cur_col] == 0) { // �˴����û������ף���ô���ڴ˴�����
			this->gameMapVec[cur_row][cur_col] = -1;
			i++; // ÿ����һ�Σ��ͼ�1
		}
	}
	// ������Ϊ���ĵľŹ������ݼ�1���׳���
	for (int i = 0; i < this->row; i++) {
		for (int j = 0; j < this->col; j++) {
			// �жϴ�λ�����ǲ�����
			if (this->gameMapVec[i][j] == -1) {
				// ��������Χ�ľŹ��������ݼ�1
				for (int m = i - 1; m <= i + 1; m++) {
					for (int n = j - 1; n <= j + 1; n++) {
						// �ж��Ƿ�Ϊ�ף��ܿ��ף�ͬʱ��֤��ά���鲻ҪԽ��
						if ((m >= 0 && m < this->row && n >= 0 && n < this->col) && this->gameMapVec[m][n] != -1) {
							this->gameMapVec[m][n] += 1;
						}
					}
				}
			}
		}
	}
	// ���ܸ���
	for (int i = 0; i < this->row; i++) {
		for (int j = 0; j < this->col; j++) {
			this->gameMapVec[i][j] += 20;
		}
	}
}

// ���ƿ�ʼ����
void Model::welcome(IMAGE& bk, IMAGE& w1, IMAGE& w2, IMAGE& w3) {
	putimage(0, -105, &bk);
	setbkmode(TRANSPARENT);
	SetWindowText(GetHWnd(), "C++ɨ����Ϸ");

	this->drawAlpha(&w1, 195, 210);
	this->drawAlpha(&w2, 212, 300);
	this->drawAlpha(&w3, 80, 362);

	// ���������Ϣ
	settextcolor(RGB(230, 230, 230));
	settextstyle(31, 0, "����", 0, 0, 1000, 0, 0, 0);
	outtextxy(16, 12, "ɨ����Ϸ");

	settextcolor(RGB(225, 225, 225));
	settextstyle(26, 0, "����", 0, 0, 1000, 0, 0, 0);
	outtextxy(13, 52, "���ߣ�����");

	while (!_kbhit()) {
	}
}

// ������Ϸ����
void Model::draw(IMAGE& bk, IMAGE& mine, vector<IMAGE>& img) {
	cleardevice();
	putimage(0, -105, &bk);

	// �����������
	settextcolor(RGB(230, 230, 230));
	settextstyle(31, 0, "����", 0, 0, 1000, 0, 0, 0);
	outtextxy(16, 12, "ɨ����Ϸ");

	settextcolor(RGB(225, 225, 225));
	settextstyle(26, 0, "����", 0, 0, 1000, 0, 0, 0);
	outtextxy(13, 52, "���ߣ�����");

	// �����ⲿ�߿�
	for (int x = 30; x > 15; x--) {
		setlinecolor(RGB(110, 136, 100));
		rectangle(this->left - x, this->top - x, this->right + x, this->down + x);
	}

	// ������Ϸ����
	for (int i = 0; i < this->row; i++) {
		for (int j = 0; j < this->col; j++) {
			if (this->gameMapVec[i][j] == -1) {
				putimage(this->left + j * pic_size, this->top + i * pic_size, &img[9]); // ������
			}
			else if (this->gameMapVec[i][j] == -2) {
				putimage(this->left + j * pic_size, this->top + i * pic_size, &mine); // ���ƺ���
			}
			else if (this->gameMapVec[i][j] >= 0 && this->gameMapVec[i][j] <= 8) {
				putimage(this->left + j * pic_size, this->top + i * pic_size, &img[this->gameMapVec[i][j]]); // ��������
			}
			else if (this->gameMapVec[i][j] >= 19 && this->gameMapVec[i][j] <= 28) {
				putimage(this->left + j * pic_size, this->top + i * pic_size, &img[10]); // ���Ƹ���ͼƬ
			}
			else if (this->gameMapVec[i][j] > 30) {
				putimage(this->left + j * pic_size, this->top + i * pic_size, &img[11]); // ���Ʊ��
			}
		}
	}
}

// �������Կո�Ϊ���ĵ���Χ�ľŹ���
void Model::openNull(const int cur_row, const int cur_col) {
	if (this->gameMapVec[cur_row][cur_col] == 0) { // ���������ǿհ�
		for (int m = cur_row - 1; m <= cur_row + 1; m++) {
			for (int n = cur_col - 1; n <= cur_col + 1; n++) {
				if ((m >= 0 && m < this->row && n >= 0 && n < this->col) &&
					(this->gameMapVec[m][n] > 19 && this->gameMapVec[m][n] <= 28) && this->gameMapVec[m][n] > 8) { // û��Խ�磬����Ϊ�ջ������֣�����û�б���
					this->gameMapVec[m][n] -= 20;
					this->openNull(m, n); // �ݹ��ȥ�򿪿հ�
					this->flag++;
				}
			}
		}
	}
}

// �����Ϣ��Ӧ
int Model::mouseControl() {
	// �ж��Ƿ��������Ϣ
	if (MouseHit()) {
		// ��ȡ�����Ϣ
		MOUSEMSG msg = GetMouseMsg();
		// �ж��û��Ƿ���������Ϸ����
		if (msg.x >= this->left && msg.x <= this->right &&
			msg.y >= this->top && msg.y <= this->down) {
			// ���������ת���������±�
			int cur_row = (msg.y - this->top) / pic_size;
			int cur_col = (msg.x - this->left) / pic_size;
			// �������Ϣ���зַ�
			switch (msg.uMsg) {
			case WM_LBUTTONDOWN:
				if (this->gameMapVec[cur_row][cur_col] > 8) { // ���û�д򿪾ʹ�
					mciSendString("close click", nullptr, 0, nullptr);
					mciSendString("open ./resource/music/click.wav alias click", nullptr, 0, nullptr);
					mciSendString("play click", nullptr, 0, nullptr);
					this->gameMapVec[cur_row][cur_col] -= 20; // ��
					openNull(cur_row, cur_col); // �����հ�
					flag++;
				}
				break;
			case WM_RBUTTONDOWN:
				if (this->gameMapVec[cur_row][cur_col] > 8 && this->gameMapVec[cur_row][cur_col] <= 28) { // ���û���Ҳ�δ�����
					mciSendString("close rightclick", nullptr, 0, nullptr);
					mciSendString("open ./resource/music/rightclick.wav alias rightclick", nullptr, 0, nullptr);
					mciSendString("play rightclick", nullptr, 0, nullptr);
					this->gameMapVec[cur_row][cur_col] += 20;
				}
				else if (this->gameMapVec[cur_row][cur_col] > 30) { // ���û�򿪵��ѱ����
					this->gameMapVec[cur_row][cur_col] -= 20;
				}
				break;
			case WM_MOUSEWHEEL:
				int count = 0; // ������
				for (int m = cur_row - 1; m <= cur_row + 1; m++) { // �ԾŹ�����б���
					for (int n = cur_col - 1; n <= cur_col + 1; n++) {
						if (this->gameMapVec[m][n] > 30 && m >= 0 && m < this->row && n >= 0 && n < this->col) {
							count++;
						}
					}
				}
				if (this->gameMapVec[cur_row][cur_col] > 0 && this->gameMapVec[cur_row][cur_col] <= 8 && this->gameMapVec[cur_row][cur_col] == count) {
					mciSendString("close click", nullptr, 0, nullptr);
					mciSendString("open ./resource/music/click.wav alias click", nullptr, 0, nullptr);
					mciSendString("play click", nullptr, 0, nullptr);
					for (int m = cur_row - 1; m <= cur_row + 1; m++) {
						for (int n = cur_col - 1; n <= cur_col + 1; n++) {
							if (this->gameMapVec[m][n] > 8 && this->gameMapVec[m][n] <= 28
								&& m >= 0 && m < this->row && n >= 0 && n < this->col) {
								this->gameMapVec[m][n] -= 20;
								this->flag++;
								this->openNull(m, n);
								if (this->gameMapVec[m][n] == -1) { // ����򿪵����ף��򷵻ش�λ�õ�ֵ
									this->gameMapVec[m][n] -= 1; // ������λ�ñ�Ϊ����
									return this->gameMapVec[m][n];
									goto out;
								}
							}
						}
					}
				}
				break;
			}
			if (this->gameMapVec[cur_row][cur_col] == -1) {
				this->gameMapVec[cur_row][cur_col] -= 1; // ������λ�ñ�Ϊ����
			}
			return this->gameMapVec[cur_row][cur_col];
		}
	}
	return -1;
out:;
}

// �ж���Ϸ��Ӯ
void Model::over(IMAGE& bk, IMAGE& mine, vector<IMAGE>& img, const int judgeflag) {
	if (judgeflag == -2) { // ��������������Ϸ����
		// ����
		for (int i = 0; i < this->row; i++) { // ��Ϸʧ��ʱ��ʾ���е���
			for (int j = 0; j < this->col; j++) {
				if (this->gameMapVec[i][j] == 19) {
					this->gameMapVec[i][j] -= 20; // ��δ�����ʱ
				}
				else if (this->gameMapVec[i][j] == 39) {
					this->gameMapVec[i][j] -= 40; // ���ѱ����ʱ
				}
			}
		}
		this->draw(bk, mine, img); // ���»��Ƶ�ͼ
		FlushBatchDraw();
		this->finish = clock(); // ��¼�������ʱ���
		this->duration = ((double)this->finish - (double)this->start) / CLOCKS_PER_SEC; // ����CLOCKS_PER_SEC���ڱ�ʶÿ�����ж��ٸ���ʱ��Ԫ
		char timeName[45];
		sprintf(timeName, "�����ˣ���������һ����\n""��Ϸʱ��Ϊ%.1lf�롣", duration);
		int isok = MessageBox(GetHWnd(), timeName, "��ʾ", MB_OKCANCEL);
		if (IDOK == isok) { // ���ѡ��ȷ����������һ��
			mciSendString("close search", nullptr, 0, nullptr);
			mciSendString("open ./resource/music/search.wav alias search", nullptr, 0, nullptr);
			mciSendString("play search", nullptr, 0, nullptr);
			this->init(this->row, this->col, this->mine_num); // ���¶����ݽ��г�ʼ��
			this->flag = 0;
			this->start = clock(); // ��¼��ʼʱ���
		}
		else { // ���ѡ����������رճ���
			exit(0);
		}
	}
	if (this->flag == this->row * this->col - this->mine_num) { // �������������֮������и�������Ϸʤ��
		// Ӯ��
		for (int i = 0; i < this->row; i++) { // ��Ϸʤ��ʱ�����е��ױ��
			for (int j = 0; j < this->col; j++) {
				if (this->gameMapVec[i][j] == 19) {
					this->gameMapVec[i][j] += 20; // ��δ�����ʱ
				}
			}
		}
		this->draw(bk, mine, img); // ���»��Ƶ�ͼ
		FlushBatchDraw();
		this->finish = clock(); // ��¼�������ʱ���
		this->duration = ((double)this->finish - (double)this->start) / CLOCKS_PER_SEC; // ����CLOCKS_PER_SEC���ڱ�ʶÿ�����ж��ٸ���ʱ��Ԫ
		char timeName[45];
		sprintf(timeName, "��Ӯ�ˣ���������һ����\n""��Ϸʱ��Ϊ%.1lf�롣", duration);
		int isok = MessageBox(GetHWnd(), timeName, "��ʾ", MB_OKCANCEL);
		if (IDOK == isok) { // ���ѡ��ȷ����������һ��
			mciSendString("close search", nullptr, 0, nullptr);
			mciSendString("open ./resource/music/search.wav alias search", nullptr, 0, nullptr);
			mciSendString("play search", nullptr, 0, nullptr);
			this->init(this->row, this->col, this->mine_num); // ���¶����ݽ��г�ʼ��
			this->flag = 0;
			this->start = clock(); // ��¼��ʼʱ���
		}
		else { // ���ѡ����������رճ���
			exit(0);
		}
	}
}

// ������Ӧ�������û���������
void Model::keyDown() {
	char key = _getch(); // ���������û�����
	switch (key) {
	case '2': // ���ø��Ѷ�
		this->init(map_size3, map_size3, mine_num3);
		break;
	case '1': // �������Ѷ�
		this->init(map_size2, map_size2, mine_num2);
		break;
	case '0': // ���õ��Ѷ�
		this->init(map_size1, map_size1, mine_num1);
		break;
	default:
		this->init(map_size1, map_size1, mine_num1);
		break;
	}
}