#include "Model.h"
using namespace std;

IMAGE bk; // 存放背景图片
IMAGE w1; // 存放文字图片
IMAGE w2; // 存放文字图片
IMAGE track; // 存放赛道图片
vector<IMAGE> img(PIC_NUM); // 存放图片
vector<int> imgIndex = { 0, 1, 2, 3 };

//-------The main method--------//

int main(int argc, char* argv)
{
	unique_ptr<Model> pModel(new Model()); // Model对象

	pModel->load(bk, w1, w2, track, img, imgIndex);
	pModel->bgm();
	initgraph(WIDTH, HEIGHT);
	pModel->init();

	BeginBatchDraw();
	while (1)
	{
		pModel->welcome(bk, w1, w2, track, img);
		FlushBatchDraw();
		pModel->moveLine();
		pModel->moveCar();
		if (pModel->mouseControl(pModel->getButton1(), pModel->getButton2()) == 1)
		{
			break;
		}
	}
	fflush(stdin); // 清空键盘消息缓冲区
	FlushMouseMsgBuffer(); // 清空鼠标消息缓冲区

	while (1)
	{
		pModel->draw(bk, track, img);
		FlushBatchDraw();
		pModel->moveLine();
		pModel->moveCar();
		if (pModel->over() == true)
		{
			mciSendString("close explode", nullptr, 0, nullptr);
			mciSendString("open ./resource/music/explode.mp3 alias explode", nullptr, 0, nullptr);
			mciSendString("play explode", nullptr, 0, nullptr);
			MessageBox(GetHWnd(), "You Lose!", "Game Over!", MB_OK);
			break;
		}
		pModel->keyDown();
	}
	EndBatchDraw();

	closegraph();
	return 0;
}
