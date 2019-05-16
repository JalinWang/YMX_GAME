// AttrGenerator.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <string>
#include "..\Libs\inifile.h"

using namespace std;

using namespace inifile;


int _tmain(int argc, _TCHAR* argv[])
{
	string filepath = "test.ini";
 
	IniFile ini;
 
	ini.Load(filepath);
 
	ini.SetIntValue("","NAME",500);
	//保存到文件
	ini.Save();
	return 0;
}

