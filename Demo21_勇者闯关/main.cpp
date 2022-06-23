#include "Model.h"
using namespace std;

IMAGE bk; // 存放背景图片
IMAGE w1; // 存放文字图片
IMAGE w2; // 存放文字图片
IMAGE stage1; // 存放通常木板
IMAGE stage2[2]; // 存放带刺木板
IMAGE apron; // 存放墙体图片
IMAGE nail; // 存放尖刺图片
IMAGE hp; // 存放生命图片
IMAGE player[2]; // 存放人物图片

//-------The main method--------//

int main(int argc, char* argv)
{
	unique_ptr<Model> pModel(new Model()); // Model对象

	pModel->load(bk, w1, w2, stage1, stage2, apron, nail, hp, player);
	pModel->bgm();
	initgraph(WIDTH, HEIGHT);
	pModel->init();

	BeginBatchDraw();
	while (1)
	{
		pModel->welcome(bk, w1, w2, nail, apron);
		FlushBatchDraw();
		if (pModel->mouseControl(pModel->getButton1(), pModel->getButton2()) == 1)
		{
			break;
		}
	}
	fflush(stdin); // 清空键盘消息缓冲区
	FlushMouseMsgBuffer(); // 清空鼠标消息缓冲区

	while (1)
	{
		pModel->draw(bk, nail, apron, stage1, stage2, player);
		FlushBatchDraw();
		pModel->keyDown();
		pModel->moveBoard();
		pModel->sumScore();
		if (pModel->over() == 1)
		{
			mciSendString("close end", nullptr, 0, nullptr);
			mciSendString("open ./resource/music/end.wav alias end", nullptr, 0, nullptr);
			mciSendString("play end", nullptr, 0, nullptr);
			pModel->overAction(bk, nail, apron, stage1, stage2, player);
			char fileName[25];
			sprintf(fileName, "勇者来到了第%d层!", pModel->getScore());
			MessageBox(GetHWnd(), fileName, "Game Over!", MB_OK);
			break;
		}
	}
	EndBatchDraw();

	closegraph();
	return 0;
}
