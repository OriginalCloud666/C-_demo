#include "Model.h"
using namespace std;

auto const width = 640; // 窗口宽度
auto const height = 720; // 窗口高度
auto const brick_width = 80; // 砖块宽度
auto const brick_height = 30; // 砖块高度
auto const brick_row = (height / brick_height - 16); // 砖块行数
auto const brick_col = (width / brick_width); // 砖块列数
auto const pi = 3.1415926; // π

IMAGE bk; // 存放背景图片

//-------The main method--------//

int main(int argc, char* argv) {
	unique_ptr<Model> pModel(new Model()); // Model对象

	pModel->load(bk, width, height);
	pModel->bgm();
	initgraph(width, height);
	pModel->welcome(bk);
	pModel->init(brick_width, brick_height, brick_row, brick_col, width, height);

	fflush(stdin); // 清空键盘消息缓冲区
	FlushMouseMsgBuffer(); // 清空鼠标消息缓冲区

	BeginBatchDraw();
	while (1) {
		pModel->keyDown1(width);
		pModel->keyDown2();
		pModel->ballMove(width, height, pi);
		pModel->ballCrash(brick_row, brick_col, brick_width, brick_height, pi);
		pModel->draw(brick_width, brick_height, brick_row, brick_col, height);
		FlushBatchDraw();
		if (pModel->over(brick_row, brick_col) == 1 && pModel->getBall().flag == 0) {
			mciSendString("close win", nullptr, 0, nullptr);
			mciSendString("open ./resource/music/win.wav alias win", nullptr, 0, nullptr);
			mciSendString("play win", nullptr, 0, nullptr);
			MessageBox(GetHWnd(), "You Win!", "游戏结束！", MB_OK);
			break;
		}
	}
	EndBatchDraw();

	closegraph();
	return 0;
}