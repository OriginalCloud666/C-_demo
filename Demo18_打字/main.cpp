#include "Model.h"
using namespace std;

IMAGE kk; // 存放字符图片
IMAGE w1; // 存放文字图片
IMAGE w2; // 存放文字图片
vector<IMAGE> img(PIC_NUM); // 存放封面图片

//-------The main method--------//

int main(int argc, char* argv)
{
	unique_ptr<Model> pModel(new Model()); // Model对象

	pModel->load(kk, w1, w2, img);
	pModel->bgm();
	initgraph(WIDTH, HEIGHT);
	pModel->init();

	BeginBatchDraw();
	while (1)
	{
		pModel->welcome(w1, w2, img);
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
		pModel->draw(kk);
		FlushBatchDraw();
		if (pModel->over() == true)
		{
			MessageBox(GetHWnd(), "You Win!", "Game Over!", MB_OK);
			break;
		}
		pModel->keyDown();
		pModel->targetMove();
		pModel->errorStr();
	}
	EndBatchDraw();

	closegraph();
	return 0;
}
