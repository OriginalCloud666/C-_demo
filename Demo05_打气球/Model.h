#pragma once // 避免头文件重复定义
#pragma comment(lib,"winmm.lib") // 加载静态库

#include <iostream>
#include <vector>
#include <conio.h>
#include <math.h>
#include <time.h>
#include <graphics.h>
#include <mmsystem.h>
using namespace std;

constexpr auto WIDTH = 640; // 窗口宽度
constexpr auto HEIGHT = 720; // 窗口高度
constexpr auto BULL_NUM = 50; // 子弹数目
constexpr auto BALLOON_NUM = 12; // 气球数量
constexpr auto BARREL_SPEED = 0.01; // 炮管移动速度

struct Battery // 炮台
{
    double x; // 末端横坐标
    double y; // 末端纵坐标
    int len; // 长度
    double cx; // 中心x坐标
    double cy; // 中心y坐标
    double radian; // 与水平面弧度
};

struct Bullet // 子弹
{
    double x; // x坐标
    double y; // y坐标
    double vx; // x轴的速度分量
    double vy; // y轴的速度分量
    double step; // 每次移动的距离
    bool flag; // 状态标识，标识状态
    DWORD color; // 颜色
};

struct Balloon // 气球
{
    double x; // x坐标
    double y; // y坐标
    double step; // 每次移动的距离
    bool flag; // 状态标识，判断是否被击中
    DWORD color; // 颜色
};

//-------打气球游戏类模型--------//

class Model
{
public:
    explicit Model();
    ~Model() = default;

    void load(IMAGE& bk, IMAGE& w1, IMAGE& w2, IMAGE& w3, IMAGE& w4); // 加载资源
    void bgm(); // 加载音乐
    void init(); // 初始化数据
    void drawAlpha(IMAGE* picture, const int  picture_x, const int picture_y); // 载入PNG透明图片
    void welcome(IMAGE& bk, IMAGE& w1, IMAGE& w2, IMAGE& w3, IMAGE& w4); // 绘制开始界面
    void draw(IMAGE& bk); // 绘制游戏界面
    void creatBullet(); // 产生子弹
    void speedBullet(); // 加速子弹
    void fillBullet(); // 填充子弹
    void moveBullet(); // 子弹移动
    void creatBalloon(const int i); // 产生气球
    void crashBalloon(); // 气球碰撞
    void moveBalloon(); // 气球移动
    void mouseControl(); // 鼠标消息响应
    void keyDown(); // 键盘消息响应（同步）
    void keyDown2(); // 键盘消息响应（异步）

private:
    int score; // 记录得分
    int speed; // 记录子弹速度能级
    int bullet_num; // 记录子弹数量
    struct Battery bat; // 炮台
    vector<Bullet> bullVec; // 子弹
    vector<Balloon> ballVec; // 气球
};