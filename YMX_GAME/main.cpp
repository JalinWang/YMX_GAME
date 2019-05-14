#include <queue>
#include <string>
#include <vector>
#include <thread>
#include <chrono>
#include <functional>
#include <iostream>
#include "define_classes.h"

using namespace std;

int now;	//当前时间

constexpr int ROUND_0 = 0;

void Welcome()// 界面、欢迎信息等
{
	cout << "Hellow world!" << endl;
}

int main()
{
	Welcome();
	mRes.Init();
	mScene.Init();
	//开个新线程后台运行
	thread backgoundThread(bind(mScene.Run, &mScene));
	backgoundThread.detach();
	
}