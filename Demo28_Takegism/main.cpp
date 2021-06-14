#include "Model.h"
using namespace std;

IMAGE bk; // 存放背景图片
IMAGE w1; // 存放文字图片
IMAGE w2; // 存放文字图片
IMAGE play[2]; // 存放玩家图片
IMAGE enem[2]; // 存放敌军图片

//-------The main method--------//

int main(int argc, char* argv) {
	unique_ptr<Model> pModel(new Model()); // Model对象

	pModel->load(bk, w1, w2, enem, play);
	pModel->bgm();
	initgraph(width, height);
	pModel->init();

	BeginBatchDraw();//双缓冲
	while (1) {
		pModel->welcome(bk, w1, w2);
		FlushBatchDraw();
		if (pModel->mouseControl(pModel->getButton1(), pModel->getButton2()) == 1) {
			break;
		}
	}
	fflush(stdin); // 清空键盘消息缓冲区
	FlushMouseMsgBuffer(); // 清空鼠标消息缓冲区

	while (1) {
		pModel->draw(play, enem);
		FlushBatchDraw();
		pModel->keyDown();
		pModel->moveRole();
		pModel->moveEnemy();
		if (pModel->over() == 1) {
			if (pModel->condition1() == 1) {
				mciSendString("close explode", nullptr, 0, nullptr);
				mciSendString("open ./resource/music/explode.mp3 alias explode", nullptr, 0, nullptr);
				mciSendString("play explode", nullptr, 0, nullptr);
				MessageBox(GetHWnd(), "You Lose!", "Game Over!", MB_OK);
			}
			else if (pModel->condition2() == 1) {
				mciSendString("close win", nullptr, 0, nullptr);
				mciSendString("open ./resource/music/win.wav alias win", nullptr, 0, nullptr);
				mciSendString("play win", nullptr, 0, nullptr);
				MessageBox(GetHWnd(), "You Win!", "Game Over!", MB_OK);
			}
			break;
		}
	}
	EndBatchDraw();

	closegraph();
	return 0;
}