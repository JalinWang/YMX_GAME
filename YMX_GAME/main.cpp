#include <queue>
#include <string>
#include <vector>
#include <thread>
#include <chrono>
#include <functional>
#include <iostream>
#include "define_classes.h"
#include "Modules.h"
#include "Setting.h"

using namespace std;

void Welcome()// 界面、欢迎信息等
{
	cout << "Hellow world!" << endl;
}

int main()
{
	Welcome();
	Engine *eng  = Engine::get_Instance();
	eng->Init();
	//开个新线程后台运行
	eng->Run();
}