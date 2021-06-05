#include "Model.h"
using namespace std;

Model::Model() :flag(0), start(0), finish(0), duration(0.0), row(0), col(0), mine_num(0), top(0), left(0), down(0), right(0) {}

// 获取start
clock_t& Model::getStart() {
	return this->start;
}

// 获取finish
clock_t& Model::getFinish() {
	return this->finish;
}

// 加载资源
void Model::load(IMAGE& bk, IMAGE& w1, IMAGE& w2, IMAGE& w3, IMAGE& mine, vector<IMAGE>& img, vector<int>& imgIndex) {
	// 批量加载图片
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

// 设置背景音乐
void Model::bgm() {
	mciSendString("open ./resource/music/bk.mp3 alias BGM", nullptr, 0, nullptr);
	mciSendString("play BGM repeat", nullptr, 0, nullptr);
}

// 载入PNG图并去透明部分
void Model::drawAlpha(IMAGE* picture, const int  picture_x, const int picture_y) {
	// 变量初始化
	DWORD* dst = GetImageBuffer(); // GetImageBuffer()函数，用于获取绘图设备的显存指针，EASYX自带
	DWORD* draw = GetImageBuffer();
	DWORD* src = GetImageBuffer(picture); // 获取picture的显存指针
	int picture_width = picture->getwidth(); // 获取picture的宽度，EASYX自带
	int picture_height = picture->getheight(); // 获取picture的高度，EASYX自带
	int graphWidth = getwidth(); // 获取绘图区的宽度，EASYX自带
	int graphHeight = getheight(); // 获取绘图区的高度，EASYX自带
	int dstX = 0; // 在显存里像素的角标

	// 实现透明贴图 公式：Cp=αp*FP+(1-αp)*BP，贝叶斯定理来进行点颜色的概率计算
	for (int iy = 0; iy < picture_height; iy++) {
		for (int ix = 0; ix < picture_width; ix++) {
			int srcX = ix + iy * picture_width; // 在显存里像素的角标
			int sa = ((src[srcX] & 0xff000000) >> 24); // 0xAArrggbb;AA是透明度
			int sr = ((src[srcX] & 0xff0000) >> 16); // 获取RGB里的R
			int sg = ((src[srcX] & 0xff00) >> 8); // 获取RGB里的G
			int sb = src[srcX] & 0xff; // 获取RGB里的B
			if (ix >= 0 && ix <= graphWidth && iy >= 0 && iy <= graphHeight && dstX <= graphWidth * graphHeight) {
				dstX = (ix + picture_x) + (iy + picture_y) * graphWidth; // 在显存里像素的角标
				int dr = ((dst[dstX] & 0xff0000) >> 16);
				int dg = ((dst[dstX] & 0xff00) >> 8);
				int db = dst[dstX] & 0xff;
				draw[dstX] = ((sr * sa / 255 + dr * (255 - sa) / 255) << 16) // 公式：Cp=αp*FP+(1-αp)*BP；αp=sa/255 , FP=sr , BP=dr
					| ((sg * sa / 255 + dg * (255 - sa) / 255) << 8) // αp=sa/255 , FP=sg , BP=dg
					| (sb * sa / 255 + db * (255 - sa) / 255); // αp=sa/255 , FP=sb , BP=db
			}
		}
	}
}

// 初始化数据
void Model::init(const int row, const int col, const int mine_num) {
	// 设置随机数种子
	srand((unsigned)time(nullptr) * rand());

	// 初始化游戏行列及雷的数量
	this->row = row;
	this->col = col;
	this->mine_num = mine_num;

	// 初始化数据
	this->left = width / 2 - (this->col * pic_size) / 2; // 游戏区域左上角x坐标
	this->top = height / 2 - (this->row * pic_size) / 2; // 游戏区域左上角y坐标
	this->right = this->left + this->col * pic_size; // 游戏区域右下角x坐标
	this->down = this->top + this->row * pic_size; // 游戏区域右下角y坐标

	// 初始化vector容器
	this->gameMapVec.clear();
	for (int i = 0; i < this->row; i++) {
		vector<int> lineGrid;
		for (int j = 0; j < this->col; j++) {
			lineGrid.push_back(0);
		}
		this->gameMapVec.push_back(lineGrid);
	}

	// 埋雷（这里用-1表示雷）
	for (int i = 0; i < this->mine_num;) {
		// 在游戏地图区域随机埋雷
		int cur_row = rand() % this->row;
		int cur_col = rand() % this->col;
		if (this->gameMapVec[cur_row][cur_col] == 0) { // 此处如果没有埋过雷，那么就在此处埋雷
			this->gameMapVec[cur_row][cur_col] = -1;
			i++; // 每埋雷一次，就加1
		}
	}
	// 把以雷为中心的九宫格数据加1，雷除外
	for (int i = 0; i < this->row; i++) {
		for (int j = 0; j < this->col; j++) {
			// 判断此位置上是不是雷
			if (this->gameMapVec[i][j] == -1) {
				// 遍历雷周围的九宫格，让数据加1
				for (int m = i - 1; m <= i + 1; m++) {
					for (int n = j - 1; n <= j + 1; n++) {
						// 判断是否为雷，避开雷，同时保证二维数组不要越界
						if ((m >= 0 && m < this->row && n >= 0 && n < this->col) && this->gameMapVec[m][n] != -1) {
							this->gameMapVec[m][n] += 1;
						}
					}
				}
			}
		}
	}
	// 加密格子
	for (int i = 0; i < this->row; i++) {
		for (int j = 0; j < this->col; j++) {
			this->gameMapVec[i][j] += 20;
		}
	}
}

// 绘制开始界面
void Model::welcome(IMAGE& bk, IMAGE& w1, IMAGE& w2, IMAGE& w3) {
	putimage(0, -105, &bk);
	setbkmode(TRANSPARENT);
	SetWindowText(GetHWnd(), "C++扫雷游戏");

	this->drawAlpha(&w1, 195, 210);
	this->drawAlpha(&w2, 212, 300);
	this->drawAlpha(&w3, 80, 362);

	// 输出作者信息
	settextcolor(RGB(230, 230, 230));
	settextstyle(31, 0, "楷体", 0, 0, 1000, 0, 0, 0);
	outtextxy(16, 12, "扫雷游戏");

	settextcolor(RGB(225, 225, 225));
	settextstyle(26, 0, "楷体", 0, 0, 1000, 0, 0, 0);
	outtextxy(13, 52, "作者：初云");

	while (!_kbhit()) {
	}
}

// 绘制游戏界面
void Model::draw(IMAGE& bk, IMAGE& mine, vector<IMAGE>& img) {
	cleardevice();
	putimage(0, -105, &bk);

	// 输出作者名称
	settextcolor(RGB(230, 230, 230));
	settextstyle(31, 0, "楷体", 0, 0, 1000, 0, 0, 0);
	outtextxy(16, 12, "扫雷游戏");

	settextcolor(RGB(225, 225, 225));
	settextstyle(26, 0, "楷体", 0, 0, 1000, 0, 0, 0);
	outtextxy(13, 52, "作者：初云");

	// 绘制外部边框
	for (int x = 30; x > 15; x--) {
		setlinecolor(RGB(110, 136, 100));
		rectangle(this->left - x, this->top - x, this->right + x, this->down + x);
	}

	// 绘制游戏区域
	for (int i = 0; i < this->row; i++) {
		for (int j = 0; j < this->col; j++) {
			if (this->gameMapVec[i][j] == -1) {
				putimage(this->left + j * pic_size, this->top + i * pic_size, &img[9]); // 绘制雷
			}
			else if (this->gameMapVec[i][j] == -2) {
				putimage(this->left + j * pic_size, this->top + i * pic_size, &mine); // 绘制红雷
			}
			else if (this->gameMapVec[i][j] >= 0 && this->gameMapVec[i][j] <= 8) {
				putimage(this->left + j * pic_size, this->top + i * pic_size, &img[this->gameMapVec[i][j]]); // 绘制数字
			}
			else if (this->gameMapVec[i][j] >= 19 && this->gameMapVec[i][j] <= 28) {
				putimage(this->left + j * pic_size, this->top + i * pic_size, &img[10]); // 绘制覆盖图片
			}
			else if (this->gameMapVec[i][j] > 30) {
				putimage(this->left + j * pic_size, this->top + i * pic_size, &img[11]); // 绘制标记
			}
		}
	}
}

// 打开所有以空格为中心的周围的九宫格
void Model::openNull(const int cur_row, const int cur_col) {
	if (this->gameMapVec[cur_row][cur_col] == 0) { // 如果点击的是空白
		for (int m = cur_row - 1; m <= cur_row + 1; m++) {
			for (int n = cur_col - 1; n <= cur_col + 1; n++) {
				if ((m >= 0 && m < this->row && n >= 0 && n < this->col) &&
					(this->gameMapVec[m][n] > 19 && this->gameMapVec[m][n] <= 28) && this->gameMapVec[m][n] > 8) { // 没有越界，并且为空或者数字，并且没有被打开
					this->gameMapVec[m][n] -= 20;
					this->openNull(m, n); // 递归的去打开空白
					this->flag++;
				}
			}
		}
	}
}

// 鼠标消息响应
int Model::mouseControl() {
	// 判断是否有鼠标消息
	if (MouseHit()) {
		// 获取鼠标消息
		MOUSEMSG msg = GetMouseMsg();
		// 判断用户是否点击的是游戏区域
		if (msg.x >= this->left && msg.x <= this->right &&
			msg.y >= this->top && msg.y <= this->down) {
			// 把鼠标坐标转换成数组下标
			int cur_row = (msg.y - this->top) / pic_size;
			int cur_col = (msg.x - this->left) / pic_size;
			// 对鼠标消息进行分发
			switch (msg.uMsg) {
			case WM_LBUTTONDOWN:
				if (this->gameMapVec[cur_row][cur_col] > 8) { // 如果没有打开就打开
					mciSendString("close click", nullptr, 0, nullptr);
					mciSendString("open ./resource/music/click.wav alias click", nullptr, 0, nullptr);
					mciSendString("play click", nullptr, 0, nullptr);
					this->gameMapVec[cur_row][cur_col] -= 20; // 打开
					openNull(cur_row, cur_col); // 弹开空白
					flag++;
				}
				break;
			case WM_RBUTTONDOWN:
				if (this->gameMapVec[cur_row][cur_col] > 8 && this->gameMapVec[cur_row][cur_col] <= 28) { // 如果没打开且并未被标记
					mciSendString("close rightclick", nullptr, 0, nullptr);
					mciSendString("open ./resource/music/rightclick.wav alias rightclick", nullptr, 0, nullptr);
					mciSendString("play rightclick", nullptr, 0, nullptr);
					this->gameMapVec[cur_row][cur_col] += 20;
				}
				else if (this->gameMapVec[cur_row][cur_col] > 30) { // 如果没打开但已被标记
					this->gameMapVec[cur_row][cur_col] -= 20;
				}
				break;
			case WM_MOUSEWHEEL:
				int count = 0; // 计数器
				for (int m = cur_row - 1; m <= cur_row + 1; m++) { // 对九宫格进行遍历
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
								if (this->gameMapVec[m][n] == -1) { // 如果打开的是雷，则返回此位置的值
									this->gameMapVec[m][n] -= 1; // 将爆雷位置变为红雷
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
				this->gameMapVec[cur_row][cur_col] -= 1; // 将爆雷位置变为红雷
			}
			return this->gameMapVec[cur_row][cur_col];
		}
	}
	return -1;
out:;
}

// 判断游戏输赢
void Model::over(IMAGE& bk, IMAGE& mine, vector<IMAGE>& img, const int judgeflag) {
	if (judgeflag == -2) { // 如果点击到雷则游戏结束
		// 结束
		for (int i = 0; i < this->row; i++) { // 游戏失败时显示所有的雷
			for (int j = 0; j < this->col; j++) {
				if (this->gameMapVec[i][j] == 19) {
					this->gameMapVec[i][j] -= 20; // 雷未被标记时
				}
				else if (this->gameMapVec[i][j] == 39) {
					this->gameMapVec[i][j] -= 40; // 雷已被标记时
				}
			}
		}
		this->draw(bk, mine, img); // 重新绘制地图
		FlushBatchDraw();
		this->finish = clock(); // 记录程序结束时间点
		this->duration = ((double)this->finish - (double)this->start) / CLOCKS_PER_SEC; // 常量CLOCKS_PER_SEC用于标识每秒钟有多少个记时单元
		char timeName[45];
		sprintf(timeName, "你输了！你想再玩一次吗？\n""游戏时间为%.1lf秒。", duration);
		int isok = MessageBox(GetHWnd(), timeName, "提示", MB_OKCANCEL);
		if (IDOK == isok) { // 如果选择确定，则再玩一次
			mciSendString("close search", nullptr, 0, nullptr);
			mciSendString("open ./resource/music/search.wav alias search", nullptr, 0, nullptr);
			mciSendString("play search", nullptr, 0, nullptr);
			this->init(this->row, this->col, this->mine_num); // 重新对数据进行初始化
			this->flag = 0;
			this->start = clock(); // 记录开始时间点
		}
		else { // 如果选择其他，则关闭程序
			exit(0);
		}
	}
	if (this->flag == this->row * this->col - this->mine_num) { // 如果点击完除了雷之外的所有格子则游戏胜利
		// 赢了
		for (int i = 0; i < this->row; i++) { // 游戏胜利时将所有的雷标记
			for (int j = 0; j < this->col; j++) {
				if (this->gameMapVec[i][j] == 19) {
					this->gameMapVec[i][j] += 20; // 雷未被标记时
				}
			}
		}
		this->draw(bk, mine, img); // 重新绘制地图
		FlushBatchDraw();
		this->finish = clock(); // 记录程序结束时间点
		this->duration = ((double)this->finish - (double)this->start) / CLOCKS_PER_SEC; // 常量CLOCKS_PER_SEC用于标识每秒钟有多少个记时单元
		char timeName[45];
		sprintf(timeName, "你赢了！你想再玩一次吗？\n""游戏时间为%.1lf秒。", duration);
		int isok = MessageBox(GetHWnd(), timeName, "提示", MB_OKCANCEL);
		if (IDOK == isok) { // 如果选择确定，则再玩一次
			mciSendString("close search", nullptr, 0, nullptr);
			mciSendString("open ./resource/music/search.wav alias search", nullptr, 0, nullptr);
			mciSendString("play search", nullptr, 0, nullptr);
			this->init(this->row, this->col, this->mine_num); // 重新对数据进行初始化
			this->flag = 0;
			this->start = clock(); // 记录开始时间点
		}
		else { // 如果选择其他，则关闭程序
			exit(0);
		}
	}
}

// 按键响应，接收用户按键输入
void Model::keyDown() {
	char key = _getch(); // 用来接收用户按键
	switch (key) {
	case '2': // 设置高难度
		this->init(map_size3, map_size3, mine_num3);
		break;
	case '1': // 设置中难度
		this->init(map_size2, map_size2, mine_num2);
		break;
	case '0': // 设置低难度
		this->init(map_size1, map_size1, mine_num1);
		break;
	default:
		this->init(map_size1, map_size1, mine_num1);
		break;
	}
}