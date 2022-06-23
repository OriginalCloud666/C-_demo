#include "Model.h"
using namespace std;

IMAGE bk; // 存放背景图片
IMAGE w1; // 存放文字图片
IMAGE w2; // 存放文字图片
vector<IMAGE> img(PIC_NUM); // 存放图片
vector<int> imgIndex = { 0, 1, 2, 3, 4, 5, 6, 7 ,8 };

//-------The main method--------//

int main(int argc, char* argv)
{
	unique_ptr<Model> pModel(new Model()); // Model对象

	pModel->load(bk, w1, w2, img, imgIndex);
	pModel->bgm();
	initgraph(WIDTH, HEIGHT);
	pModel->init();

	BeginBatchDraw();
	while (1)
	{
		pModel->welcome(bk, w1, w2);
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
		pModel->mouseControl2();
		pModel->draw(bk, img);
		FlushBatchDraw();
		if (pModel->reMove() == true && pModel->isSeem() == true)
		{
			pModel->getMapVec()[pModel->getBegin().x][pModel->getBegin().y] = -1;
			pModel->getMapVec()[pModel->getEnd().x][pModel->getEnd().y] = -1;
			pModel->getBegin().x = -1; pModel->getBegin().y = -1;
			pModel->getEnd().x = -1; pModel->getEnd().y = -1;
		}
		if (pModel->over() == 1)
		{
			pModel->draw(bk, img);
			FlushBatchDraw();
			mciSendString("close win", nullptr, 0, nullptr);
			mciSendString("open ./resource/music/win.wav alias win", nullptr, 0, nullptr);
			mciSendString("play win", nullptr, 0, nullptr);
			MessageBox(GetHWnd(), "You Win!", "Game Over!", MB_OK);
			break;
		}
	}
	EndBatchDraw();

	closegraph();
	return 0;
}
