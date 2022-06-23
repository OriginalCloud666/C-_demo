#pragma once // 避免头文件重复定义
#pragma comment(lib,"winmm.lib") // 加载静态库

#include <iostream>
#include <vector>
#include <conio.h>
#include <time.h>
#include <string.h>
#include <graphics.h>
#include <mmsystem.h>
using namespace std;

constexpr auto WIDTH = 640; // 窗口宽度
constexpr auto HEIGHT = 720; // 窗口高度
constexpr auto PIC_NUM = 9; // 图片数量
constexpr auto PIC_SIZE = 60; // 图片大小
constexpr auto GRID_NUM = 10; // 地图行列

struct Button // 按钮
{
    int x; // 左上角x坐标
    int y; // 左上角y坐标
    int width; // 宽度
    int height; // 高度
    char* text; // 文字
    int dx; // 文字相对于按钮左上角x坐标的偏移值
    int dy; // 文字相对于按钮左上角y坐标的偏移值
    COLORREF buttoncolor; // 颜色
    COLORREF textcolor; // 中文字颜色
};

//-------连连看游戏类模型--------//

class Model
{
public:
    explicit Model();
    ~Model();

    POINT& getBegin();
    POINT& getEnd();
    vector<vector<int>>& getMapVec();
    struct Button*& getButton1();
    struct Button*& getButton2();
    void load(IMAGE& bk, IMAGE& w1, IMAGE& w2, vector<IMAGE>& img, vector<int>& imgIndex); // 加载资源
    void bgm(); // 加载音乐
    void init(); // 初始化数据
    void drawAlpha(IMAGE* picture, const int  picture_x, const int picture_y); // PNG图片透明实现
    void welcome(IMAGE& bk, IMAGE& w1, IMAGE& w2); // 程序开始绘制
    void draw(IMAGE& bk, vector<IMAGE>& img); // 程序游戏绘制
    struct Button* creatButton(const int x, const int y, const int width, const int height, const char* text, const int dx, const int dy, COLORREF buttoncolor, COLORREF textcolor); // 创建按钮
    void drawButton(struct Button* pButton); // 绘制按钮
    void drawFim(const int x, const int y); //绘制边框
    void drawChoseFim(); // 绘制选择框
    bool isBlock(const int r, const int c); // 判断是否空白
    bool isSeem(); // 判断是否相同
    bool horizon(POINT begin_, POINT end_); // 水平方向判断
    bool vertical(POINT begin_, POINT end_); // 竖直方向判断
    bool turn_once(POINT begin_, POINT end_); // 一个拐点判断
    bool turn_twice(POINT begin_, POINT end_); // 两个拐点判断
    bool reMove(); // 整体消除判断
    int mouseInButton(struct Button* pButton, MOUSEMSG* msg); // 判断鼠标是否在按钮内
    int mouseControl(struct Button* pButton1, struct Button* pButton2); // 鼠标消息响应
    void mouseControl2(); // 鼠标消息响应
    bool over(); // 结束判断

private:
    int top; // 地图左上角y坐标
    int left; // 地图左上角x坐标
    int down; // 地图右下角y坐标
    int right; // 地图右下角x坐标
    vector<vector<int>> gameMapVec; // 存放地图及关卡信息
    POINT begin; // 开始点的数组下标
    POINT end; // 结束点的数组下标
    bool state; // 状态标识，记录开始和结束点
    struct Button* beginGame; // 开始按钮
    struct Button* endGame; // 结束按钮
};