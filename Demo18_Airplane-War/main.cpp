#include "Model.h"
using namespace std;

IMAGE bk; // 存放背景图片
IMAGE title; // 存放开始图片
IMAGE author; // 存放作者信息
IMAGE plane[2][2]; // 存放我军飞机图片
IMAGE enemy_big[2]; // 存放敌军大型飞机图片
IMAGE enemy_small[2]; // 存放敌军小型飞机图片
IMAGE bullet[2]; // 存放飞机子弹图片

//-------The main method--------//

int main(int argc, char* argv) {
	unique_ptr<Model> pModel(new Model()); // Model对象

	pModel->load(bk, title, author, plane, enemy_big, enemy_small, bullet);
	pModel->bgm();
	initgraph(width, height);
	pModel->welcome(bk, title, author);
	pModel->init();

	BeginBatchDraw();
	while (1) {
		pModel->welcome(bk, title, author);
		FlushBatchDraw();
		if (pModel->mouseControl(pModel->getButton1(), pModel->getButton2()) == 1) {
			break;
		}
	}
	fflush(stdin); // 清空键盘消息缓冲区
	FlushMouseMsgBuffer(); // 清空鼠标消息缓冲区

	while (1) {
		pModel->draw(bk, enemy_big, enemy_small, plane, bullet);
		FlushBatchDraw();
		pModel->keyDown1();
		pModel->keyDown2();
		pModel->getRole().moveBullet();
		pModel->moveEnemy();
		pModel->hitEnemy(pModel->getEnemy1(), enemy1_num, 6);
		pModel->hitEnemy(pModel->getEnemy2(), enemy2_num, 1);
		if (pModel->over() == true) {
			pModel->draw(bk, enemy_big, enemy_small, plane, bullet);
			FlushBatchDraw();
			mciSendString("close explode", nullptr, 0, nullptr);
			mciSendString("open ./resource/music/explode.mp3 alias explode", nullptr, 0, nullptr);
			mciSendString("play explode", nullptr, 0, nullptr);
			char scoreName[25];
			sprintf(scoreName, "Game score:%d", pModel->getScore());
			MessageBox(GetHWnd(), scoreName, "Game Over!", MB_OK);
			break;
		}
	}
	EndBatchDraw();

	closegraph();
	return 0;
}