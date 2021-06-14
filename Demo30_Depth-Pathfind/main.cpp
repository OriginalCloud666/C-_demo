#include "Model.h"
using namespace std;

IMAGE w1; // 存放文字图片
IMAGE ren; // 存放角色图片
IMAGE wall; // 存放角色图片
IMAGE road; // 存放道路图片

//-------The main method--------//

int main(int argc, char* argv) {
	unique_ptr<Model> pModel(new Model()); // Model对象

	pModel->load(w1, ren, road, wall);
	pModel->bgm();
	initgraph(width, height);
	pModel->init();

	BeginBatchDraw();
	while (1) {
		pModel->welcome(w1);
		FlushBatchDraw();
		if (pModel->mouseControl(pModel->getButton1(), pModel->getButton1()) == 1) {
			break;
		}
	}
	fflush(stdin); // 清空键盘消息缓冲区
	FlushMouseMsgBuffer(); // 清空鼠标消息缓冲区

	pModel->draw(road, wall, ren);
	FlushBatchDraw();
	pModel->depthFinding(road, wall, ren);
	while (1) {
		pModel->draw(road, wall, ren);
		FlushBatchDraw();
	}
	EndBatchDraw();

	closegraph();
	return 0;
}