#include "Model.h"
using namespace std;

auto const width = 1280; // 窗口宽度
auto const height = 720; // 窗口高度
auto const map_width = width * 4; // 地图宽度
auto const map_height = height * 4; // 地图高度
auto const food_num = 480; // 食物数量
auto const bullet_num = 100; // 子弹数量
auto const derived_num = 16; // 分身数量
auto const pi = 3.1415926; // π

IMAGE bk; // 存放背景图片
IMAGE w1; // 存放文字图片
IMAGE w2; // 存放文字图片
IMAGE w3; // 存放文字图片
IMAGE map(map_width, map_height); // 存放地图

int main(int argc, char* argv) {
	unique_ptr<Model> pModel(new Model()); // Model对象

	pModel->load(bk, w1, w2, w3, width, height);
	pModel->bgm();
	initgraph(width, height);
	pModel->welcome(bk, w1, w2, w3);
	pModel->setMap(map);
	pModel->init(map_width, map_height, pi, food_num, bullet_num, derived_num);

	FlushMouseMsgBuffer(); // 清空鼠标消息缓冲区
	fflush(stdin); // 清空键盘消息缓冲区

	BeginBatchDraw();
	while (1) {
		pModel->mouseControl(width, height, bullet_num, derived_num);
		pModel->keyDown1(map_width, map_height);
		pModel->keyDown2(bullet_num, derived_num);
		pModel->actRole(map_width, map_height, food_num, bullet_num, derived_num);
		pModel->draw(map, width, height, pi, food_num, bullet_num, derived_num);
		pModel->moveBullet(map_width, map_height, bullet_num);
		pModel->moveRole(map_width, map_height, derived_num);
	}
	EndBatchDraw();

	closegraph();
	return 0;
}