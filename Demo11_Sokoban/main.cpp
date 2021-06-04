#include "Model.h"
using namespace std;

auto const width = 640; // 窗口宽度
auto const height = 720; // 窗口高度
auto const pic_num = 10; // 图片数量
auto const pic_size = 50; // 图片大小
auto const grid_num = 8; // 地图行列
auto const cas = 3; // 关卡数量

IMAGE bk;
vector<IMAGE> img(pic_num); // 存放图片
vector<int> imgIndex = { 0, 1, 2, 3, 4, 5, 6, 7, 10, 11 };

//-------The main method--------//

int main(int argc, char* argv) {
	unique_ptr<Model> pModel(new Model()); // Model对象

	pModel->load(bk, img, imgIndex, pic_num, pic_size, width, height);
	pModel->bgm();
	initgraph(width, height);
	pModel->welcome(bk);
	pModel->init(width, height, grid_num, pic_size, cas);

	fflush(stdin); // 清空键盘消息缓存区
	FlushMouseMsgBuffer(); // 清空鼠标消息缓冲区

	BeginBatchDraw();
	while (1) {
		pModel->draw(bk, img, grid_num, pic_size);
		FlushBatchDraw();
		if (pModel->over(grid_num) == 1) {
			pModel->getFlag()++;
			if (pModel->getFlag() < 3) {
				mciSendString("close littlewin", nullptr, 0, nullptr);
				mciSendString("open ./resource/music/littlewin.wav alias littlewin", nullptr, 0, nullptr);
				mciSendString("play littlewin", nullptr, 0, nullptr);
				char fileName[25];
				sprintf(fileName, "Welcome to the %d level！", pModel->getFlag() + 1);
				MessageBox(GetHWnd(), fileName, "游戏继续！", MB_OK);
			}
			else if (pModel->getFlag() == 3) {
				pModel->getFlag()--;
				mciSendString("close win", nullptr, 0, nullptr);
				mciSendString("open ./resource/music/win.wav alias win", nullptr, 0, nullptr);
				mciSendString("play win", nullptr, 0, nullptr);
				pModel->action(bk, img, grid_num, pic_size, width, height);
				MessageBox(GetHWnd(), "You Win!", "游戏结束！", MB_OK);
				break;
			}
		}
		pModel->keyDown(grid_num);
	}
	EndBatchDraw();

	closegraph();
	return 0;
}