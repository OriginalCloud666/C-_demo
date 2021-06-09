#include "Model.h"
using namespace std;

Model::Model() :score(0), cas(0), tempcas(0) {
	for (int i = 0; i < pillar_num; i++) {
		struct Pillar newpillar;
		this->pillar.push_back(newpillar);
	}
	for (int i = 0; i < 2; i++) {
		struct BkDown newbkdown;
		this->bkdown.push_back(newbkdown);
	}
	this->beginGame = creatButton(80, 500, 150, 50, "START", 38, 12, RGB(234, 139, 83), RGB(240, 240, 240));
	this->endGame = creatButton(420, 500, 150, 50, "END", 52, 12, RGB(234, 139, 83), RGB(240, 240, 240));
}

Model::~Model() {
	delete this->beginGame;
	this->beginGame = nullptr;
	delete this->endGame;
	this->endGame = nullptr;
}

// 获取score
int& Model::getScore() {
	return this->score;
}

// 获取beginGame
struct Button*& Model::getButton1() {
	return this->beginGame;
}

// 获取endGame
struct Button*& Model::getButton2() {
	return this->endGame;
}

// 加载资源
void Model::load(IMAGE& bk, IMAGE mybird[][2], IMAGE guid[], IMAGE up[], IMAGE down[], IMAGE start[], IMAGE end[]) {
	// 加载背景图片
	loadimage(&bk, "./resource/picture/bk.bmp", width, height);
	// 加载引导图片
	loadimage(guid, "./resource/picture/guid.bmp");
	loadimage(guid + 1, "./resource/picture/guidy.bmp");
	// 加载鸟的图片
	loadimage(mybird[0], "./resource/picture/bird0.bmp", bird_size, bird_size);
	loadimage(mybird[0] + 1, "./resource/picture/birdy0.bmp", bird_size, bird_size);
	loadimage(mybird[1], "./resource/picture/bird1.bmp", bird_size, bird_size);
	loadimage(mybird[1] + 1, "./resource/picture/birdy1.bmp", bird_size, bird_size);
	loadimage(mybird[2], "./resource/picture/bird2.bmp", bird_size, bird_size);
	loadimage(mybird[2] + 1, "./resource/picture/birdy2.bmp", bird_size, bird_size);
	// 加载下方向柱子图片
	loadimage(down, "./resource/picture/down.bmp");
	loadimage(down + 1, "./resource/picture/downy.bmp");
	// 加载上方向柱子图片
	loadimage(up, "./resource/picture/up.bmp");
	loadimage(up + 1, "./resource/picture/upy.bmp");
	// 加载游戏开始图片
	loadimage(start, "./resource/picture/start.bmp");
	loadimage(start + 1, "./resource/picture/starty.bmp");
	// 加载游戏结束图片
	loadimage(end, "./resource/picture/end.bmp");
	loadimage(end + 1, "./resource/picture/endy.bmp");
}

// 加载音乐
void Model::bgm() {
	mciSendString("open ./resource/music/bk.mp3 alias BGM", nullptr, 0, nullptr);
	mciSendString("play BGM repeat", nullptr, 0, nullptr);
}

// 创建按钮
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

// 绘制按钮
void Model::drawButton(struct Button* pButton) {
	setbkmode(TRANSPARENT);
	setlinecolor(RGB(243, 192, 161));
	setlinestyle(PS_SOLID, 5);
	setfillcolor(pButton->buttoncolor);
	fillrectangle(pButton->x, pButton->y, pButton->x + pButton->width, pButton->y + pButton->height);
	settextcolor(pButton->textcolor);
	settextstyle(28, 0, "楷体", 0, 0, 1000, 0, 0, 0);
	outtextxy(pButton->x + pButton->dx, pButton->y + pButton->dy, pButton->text);
}

// 贴引导图片
void Model::drawGuid(IMAGE guid[], const int x, const int y) {
	// 掩码图：SRCAND格式去贴图
	putimage(x, y, guid + 1, SRCAND);
	// 源码图：SRCPAINT格式去贴图
	putimage(x, y, guid, SRCPAINT);
}

// 贴鸟的图片
void Model::drawBird(IMAGE mybird[][2], const int x, const int y) {
	// 掩码图：SRCAND格式去贴图
	putimage(x, y, mybird[this->cas] + 1, SRCAND);
	// 源码图：SRCPAINT格式去贴图
	putimage(x, y, mybird[this->cas], SRCPAINT);
}

// 贴柱子的图片
void Model::drawPillar(IMAGE up[], IMAGE down[], struct Pillar mypillar) {
	// 上边柱子绘制
	// 掩码图：SRCAND格式去贴图
	putimage(mypillar.x, 0, 65, mypillar.height, down + 1, 0, 400 - mypillar.height, SRCAND);
	// 源码图：SRCPAINT格式去贴图
	putimage(mypillar.x, 0, 65, mypillar.height, down, 0, 400 - mypillar.height, SRCPAINT);

	// 下边柱子绘制
	// 掩码图：SRCAND格式去贴图
	putimage(mypillar.x, 607 - (400 - mypillar.height), 65, 400 - mypillar.height, up + 1, 0, 0, SRCAND);
	// 源码图：SRCPAINT格式去贴图
	putimage(mypillar.x, 607 - (400 - mypillar.height), 65, 400 - mypillar.height, up, 0, 0, SRCPAINT);
}

// 贴下部地面背景图片
void Model::drawBackground(IMAGE& bk, struct BkDown mybkdown) {
	// 贴下部地面背景图片
	putimage(mybkdown.x, 607, width - 1, height - 607, &bk, 0, 607);
}

// 绘制开始界面
void Model::welcome(IMAGE& bk, IMAGE guid[]) {
	cleardevice();
	putimage(0, 0, &bk);
	SetWindowText(GetHWnd(), "C++飞翔的小鸟");

	this->drawGuid(guid, 10, 10); // 绘制引导图片
	this->drawButton(beginGame); // 绘制开始按钮
	this->drawButton(endGame); // 绘制结束按钮

	// 绘制背景图片
	for (int i = 0; i < 2; i++) {
		if (this->bkdown[i].flag == 1) { // 当地面存在时，对地面进行绘制
			this->drawBackground(bk, this->bkdown[i]);
		}
	}
}

// 绘制游戏界面
void Model::draw(IMAGE& bk, IMAGE up[], IMAGE down[], IMAGE mybird[][2]) {
	cleardevice();
	putimage(0, 0, &bk);

	// 绘制背景图片
	for (int i = 0; i < 2; i++) {
		if (this->bkdown[i].flag == 1) { // 当地面存在时，对地面进行绘制
			this->drawBackground(bk, this->bkdown[i]);
		}
	}

	// 绘制柱子图片
	for (int i = 0; i < pillar_num; i++) {
		if (this->pillar[i].flag == 1) { // 当柱子存在时，对柱子进行绘制
			this->drawPillar(up, down, this->pillar[i]);
		}
	}

	// 绘制鸟的图片
	this->drawBird(mybird, this->bird.x, this->bird.y);
}

// 柱子的产生
void Model::createPillar(const int i) {
	pillar[i].height = rand() % 240 + 80;
	// 当生成的柱子高度与其他柱子相同时，重新生成高度
	while (pillar[i].height == pillar[(i + 1) % 3].height || pillar[i].height == pillar[(i + 2) % 3].height) {
		pillar[i].height = rand() % 240 + 80;
	}
	// 设置柱子产生时的x坐标
	pillar[i].x = width;
	pillar[i].flag = 1;
}

// 地面的产生
void Model::createBkDown(const int i) {
	// 设置地面产生时的x坐标
	this->bkdown[i].x = width - 1.0;
	this->bkdown[i].flag = 1;
}

// 初始化游戏数据
void Model::init() {
	// 设置随机数种子
	srand((unsigned)time(nullptr) * rand());

	this->score = 0;
	this->cas = 0;
	this->tempcas = 0;

	// 对鸟的数据进行初始化
	this->bird.x = width / 5 - 50.0;
	this->bird.y = height * 2 / 5;
	this->bird.speed = 0.6;

	// 初始情况下，先产生三对柱子
	for (int i = 0; i < pillar_num; i++) {
		this->createPillar(i);
		pillar[i].x = width + 235.0 * i;
	}

	// 初始情况下，先产生两个地面
	for (int i = 0; i < 2; i++) {
		this->createBkDown(i);
		bkdown[i].x = width * 1.0 * i;
	}
}

// 鸟的移动
void Model::moveBird() {
	// 鸟自己向下移动
	this->bird.y += this->bird.speed;
	// 对鸟的移动边界进行限制
	if (this->bird.y >= 607.0 - bird_size + 15.0) {
		this->bird.y = 607.0 - bird_size + 15.0;
	}
	else if (this->bird.y <= -15) {
		this->bird.y = -15;
	}
}

// 柱子的移动消失
void Model::movePillar() {
	for (int i = 0; i < pillar_num; i++) {
		this->pillar[i].x -= move_step;
		if (this->pillar[i].x <= -65) {
			this->pillar[i].flag = 0;
			this->createPillar(i);
			this->score += 1;
		}
	}
}

// 地面的移动消失
void Model::moveBkDown() {
	for (int i = 0; i < 2; i++) {
		this->bkdown[i].x -= move_step;
		if (this->bkdown[i].x <= -width) {
			this->bkdown[i].flag = 0;
			this->createBkDown(i);
		}
	}
}

// 柱子与鸟的碰撞
int Model::hitPillar() {
	for (int i = 0; i < pillar_num; i++) {
		if (this->pillar[i].x <= this->bird.x + bird_size - 15 && this->pillar[i].x >= this->bird.x - 65 + 10) {
			// 判断上柱是否发生碰撞
			if (this->bird.y + 15 <= this->pillar[i].height) {
				return 1; // 返回值为1说明有碰撞发生
			}
			// 判断下柱是否发生碰撞
			if (this->bird.y + bird_size - 15 >= 607 - (400.0 - this->pillar[i].height)) {
				return 1; // 返回值为1说明有碰撞发生
			}
		}
	}
	return 0; // 返回值为0说明没有碰撞
}

// 判断鼠标是否在按钮中
int Model::mouseInButton(struct Button* pButton, MOUSEMSG* msg) {
	if (msg->x >= pButton->x && msg->x <= pButton->x + pButton->width &&
		msg->y >= pButton->y && msg->y <= pButton->y + pButton->height) {
		pButton->buttoncolor = RGB(230, 121, 55);
		pButton->textcolor = RGB(135, 135, 135);
		return 1; // 返回值为1，表明鼠标在按钮中
	}
	pButton->buttoncolor = RGB(234, 139, 83);
	pButton->textcolor = RGB(240, 240, 240);
	return 0; // 返回值为0，表明鼠标不在按钮中
}

// 鼠标控制
int Model::mouseControl(struct Button* pButton1, struct Button* pButton2) {
	// 判断是否有鼠标消息
	if (MouseHit()) {
		MOUSEMSG msg = GetMouseMsg();
		// 判断用户是否点击按钮
		if (this->mouseInButton(pButton1, &msg) == 1 && msg.uMsg == WM_LBUTTONDOWN) {
			mciSendString("close click", nullptr, 0, nullptr);
			mciSendString("open ./resource/music/click.wav alias click", nullptr, 0, nullptr);
			mciSendString("play click", nullptr, 0, nullptr);
			return 1; // 返回值为1，进入游戏
		}
		else if (this->mouseInButton(pButton2, &msg) == 1 && msg.uMsg == WM_LBUTTONDOWN) {
			mciSendString("close click", nullptr, 0, nullptr);
			mciSendString("open ./resource/music/click.wav alias click", nullptr, 0, nullptr);
			mciSendString("play click", nullptr, 0, nullptr);
			exit(0); // 游戏结束
		}
	}
	return 0;
}

// 按键响应
void Model::keyDown() {
	// 判断是否有按键消息
	while (_kbhit()) {
		// 用于接收按键消息
		char userKey = _getch();
		fflush(stdin); // 清空键盘消息缓冲区

		switch (userKey) {
		case ' ': // 上
			mciSendString("close fly", nullptr, 0, nullptr);
			mciSendString("open ./resource/music/fly.mp3 alias fly", nullptr, 0, nullptr);
			mciSendString("play fly", nullptr, 0, nullptr);
			this->bird.y -= (this->bird.speed + 130);
		}
	}
}

// 游戏结束条件1
int Model::condition1() {
	// 当小鸟撞到地板时，游戏结束
	if (this->bird.y >= 607.0 - bird_size + 15.0) {
		return 1; // 返回值为1时游戏结束
	}
	// 小鸟撞到天花板时，游戏结束
	else if (this->bird.y <= 15) {
		return 1; // 返回值为1时游戏结束
	}
	return 0; // 返回值为0时游戏未结束
}

// 游戏结束条件2
int Model::condition2() {
	// 当小鸟撞到柱子时游戏结束
	return this->hitPillar();
}

// 游戏结束条件判断
bool Model::over() {
	if (this->condition1() == 1 || this->condition2() == 1) {
		return true; // 返回值为1时游戏结束
	}
	return false; // 返回值为0时游戏未结束
}

// 游戏开始动画
void Model::beginAction(IMAGE& bk, IMAGE guid[], IMAGE start[]) {
	// 设置开始动画初始坐标
	double startX = 121;
	double startY = 87;
	// 设置动画漂移幅度以及状态标识
	double step = 0.38;
	int flag = 0;
	// 设置局部变量用于记录位移处值
	double tempstep = step;

	while (1) {
		this->welcome(bk, guid);

		// 设置浮动动画效果
		if (flag == 0) { // 此时动画向下移动
			step -= 0.0035;
			if (step <= -tempstep) {
				step = -tempstep;
				flag = 1; // 此后动画开始向上移动
			}
		}
		else if (flag == 1) { // 此时动画向上移动
			step += 0.0035;
			if (step >= tempstep) {
				step = tempstep;
				flag = 0; // 此后动画开始向下移动
			}
		}
		startY += step;

		// 绘制动画效果
		putimage(startX, startY, start + 1, SRCAND); // 贴掩码图
		putimage(startX, startY, start, SRCPAINT); // 贴源码图

		this->moveBkDown();
		FlushBatchDraw();
		// 鼠标是否点击按钮判断
		if (this->mouseControl(this->beginGame, this->endGame) == 1) {
			break;
		}
	}
}

// 游戏扇翅膀动画
void Model::flyAction() {
	this->tempcas += 0.025;
	if (this->tempcas >= 3) {
		this->tempcas = 0;
	}
	this->cas = (int)this->tempcas;
}

// 游戏撞击动画
void Model::hitAction(IMAGE& bk, IMAGE up[], IMAGE down[], IMAGE mybird[][2]) {
	// 设置下落速度
	double step = -1.85;

	// 绘制动画效果
	while (this->bird.y + bird_size - 15 <= 607) {
		this->bird.y += step;
		step += 0.025;
		this->draw(bk, up, down, mybird);
		FlushBatchDraw();
	}
}

// 游戏结束动画
void Model::endAction(IMAGE& bk, IMAGE up[], IMAGE down[], IMAGE mybird[][2], IMAGE end[]) {
	// 起始坐标设置
	double startX = width / 2 - 204.0 / 2;
	double startY = height;

	// 绘制动画效果
	while (startY >= height / 2 - 54.0 / 2) {
		this->draw(bk, up, down, mybird);
		putimage(startX, startY, end + 1, SRCAND); // 贴掩码图
		putimage(startX, startY, end, SRCPAINT); // 贴源码图
		FlushBatchDraw();
		startY -= 0.68; // 控制动画速度
		Sleep(0); // 控制动画速度
	}
}