// AttrGenerator.cpp : �������̨Ӧ�ó������ڵ㡣
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
	//���浽�ļ�
	ini.Save();
	return 0;
}

