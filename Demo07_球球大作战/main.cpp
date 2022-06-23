#include "Model.h"
using namespace std;

IMAGE bk; // 存放背景图片
IMAGE w1; // 存放文字图片
IMAGE w2; // 存放文字图片
IMAGE w3; // 存放文字图片
IMAGE map(MAP_WIDTH, MAP_HEIGHT); // 存放地图

//-------The main method--------//

int main(int argc, char* argv)
{
	unique_ptr<Model> pModel(new Model()); // Model对象

	pModel->load(bk, w1, w2, w3);
	pModel->bgm();
	initgraph(WIDTH, HEIGHT);
	pModel->welcome(bk, w1, w2, w3);
	pModel->setMap(map);
	pModel->init();

	FlushMouseMsgBuffer(); // 清空鼠标消息缓冲区
	fflush(stdin); // 清空键盘消息缓冲区

	BeginBatchDraw();
	while (1)
	{
		pModel->mouseControl();
		pModel->keyDown1();
		pModel->keyDown2();
		pModel->actRole();
		pModel->draw(map);
		pModel->moveBullet();
		pModel->moveRole();
	}
	EndBatchDraw();

	closegraph();
	return 0;
}
