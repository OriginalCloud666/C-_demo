#include "Model.h"
using namespace std;

auto const width = 640; // 窗口宽度
auto const height = 480; // 窗口高度
auto const star_num = 100; // 星星总数

IMAGE bk; // 存放背景图片

//-------The main method--------//

int main(int argc, char* argv) {
	unique_ptr<Model> pModel(new Model()); // Model对象

	pModel->load(bk, width, height);
	pModel->bgm();
	initgraph(width, height);
	pModel->draw(bk);
	pModel->init(star_num, height, width);
	fflush(stdin); // 清空键盘消息缓冲区
	FlushMouseMsgBuffer(); // 清空鼠标消息缓冲区

	BeginBatchDraw();
	while (!_kbhit()) {
		cleardevice();
		pModel->draw(bk);

		for (int i = 0; i < star_num; i++) {
			pModel->moveStar(i, width, height);
		}
		Sleep(10);
		FlushBatchDraw();
	}
	EndBatchDraw();

	closegraph();
	return 0;
}