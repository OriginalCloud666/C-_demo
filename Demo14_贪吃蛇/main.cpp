#include "Model.h"
using namespace std;

IMAGE bk; // 存放背景图片

//-------The main method--------//

int main(int argc, char* argv)
{
	unique_ptr<Model> pModel(new Model()); // Model对象

	pModel->load(bk);
	pModel->bgm();
	initgraph(WIDTH, HEIGHT);
	pModel->init();

	BeginBatchDraw();
	while (1)
	{
		pModel->welcome(bk);;
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
		pModel->keyDown();
		pModel->draw(bk);
		FlushBatchDraw();
		pModel->snackMove();
		pModel->eatFood();
		if (pModel->getFood().flag == 0) // 如果食物不存在
		{
			pModel->creatFood();
		}
		if (pModel->over() == 1)
		{
			mciSendString("close end", nullptr, 0, nullptr);
			mciSendString("open ./resource/music/end.wav alias end", nullptr, 0, nullptr);
			mciSendString("play end", nullptr, 0, nullptr);
			char fileName[25];
			sprintf(fileName, "Game scores：%d", pModel->getSnake().size);
			MessageBox(GetHWnd(), fileName, "Game Over!", MB_OK);
			break;
		}
	}
	EndBatchDraw();

	closegraph();
	return 0;
}
