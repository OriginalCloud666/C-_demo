#include "Model.h"
using namespace std;

IMAGE bk; // 存放背景图片
IMAGE mybird[3][2]; // 存放鸟的图片
IMAGE guid[2]; // 存放引导图片
IMAGE down[2]; // 朝下的柱子
IMAGE up[2]; // 朝上的柱子
IMAGE start_[2]; // 开始动画设置
IMAGE end_[2]; // 结束动画设置

//-------The main method--------//

int main(int argc, char* argv)
{
	unique_ptr<Model> pModel(new Model()); // Model对象

	pModel->load(bk, mybird, guid, up, down, start_, end_);
	pModel->bgm();
	initgraph(WIDTH, HEIGHT);
	pModel->welcome(bk, guid);
	pModel->init();

	BeginBatchDraw();
	pModel->beginAction(bk, guid, start_);

	fflush(stdin); // 清空键盘消息缓冲区
	FlushMouseMsgBuffer(); // 清空鼠标消息缓冲区

	while (1)
	{
		pModel->keyDown();
		pModel->moveBird();
		pModel->movePillar();
		pModel->moveBkDown();
		pModel->draw(bk, up, down, mybird);
		pModel->flyAction();
		FlushBatchDraw();
		if (pModel->over() == 1)
		{
			mciSendString("close end", nullptr, 0, nullptr);
			mciSendString("open ./resource/music/end.wav alias end", nullptr, 0, nullptr);
			mciSendString("play end", nullptr, 0, nullptr);
			pModel->hitAction(bk, up, down, mybird);
			pModel->endAction(bk, up, down, mybird, end_);
			char scoreName[25];
			sprintf(scoreName, "Game scores:%d", pModel->getScore());
			MessageBox(GetHWnd(), scoreName, "Game Over！", MB_OK);
			break;
		}
	}
	EndBatchDraw();

	closegraph();
	return 0;
}
