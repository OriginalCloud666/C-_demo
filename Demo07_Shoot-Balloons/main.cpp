#include "Model.h"
using namespace std;

auto const width = 640; // 窗口宽度
auto const height = 720; // 窗口高度
auto const bull_num = 50; // 子弹数目
auto const balloon_num = 12; // 气球数量
auto const barrel_speed = 0.01; // 炮管移动速度

IMAGE bk; // 存放背景图片
IMAGE w1; // 存放文字图片
IMAGE w2; // 存放文字图片
IMAGE w3; // 存放文字图片
IMAGE w4; // 存放文字图片

//-------The main method--------//

int main(int argc, char* argv) {
	unique_ptr<Model> pModel(new Model()); // Model对象

	pModel->load(bk, w1, w2, w3, w4, width, height);
	pModel->bgm();
	initgraph(width, height);
	pModel->welcome(bk, w1, w2, w3, w4);
	pModel->init(bull_num, balloon_num, width, height);

	FlushMouseMsgBuffer(); // 清空鼠标消息缓冲区
	fflush(stdin); // 清空键盘消息缓冲区

	BeginBatchDraw();
	while (1) {
		pModel->draw(bk, bull_num, balloon_num, height);
		pModel->mouseControl(bull_num, width, height);
		pModel->keyDown(bull_num, width, height);
		pModel->keyDown2(barrel_speed);
		pModel->moveBullet(bull_num);
		pModel->crashBalloon(bull_num, balloon_num, width, height);
		pModel->moveBalloon(balloon_num, width, height);
		FlushBatchDraw();
	}
	EndBatchDraw();

	closegraph();
	return 0;
}