// AttrGenerator.cpp : 定义控制台应用程序的入口点。
//

#include <stdio.h>
#include <tchar.h>
#include "inifile.h"
#include <string>

using namespace inifile;
using namespace std;


class BaseObject
{
protected:
	//Position pos;//位置横、纵坐标
	//MainVelocity v;//横向、纵向速度
	double baseSpeed;//该物体能够主动移动的基础速度
	//ObjType type;//对象属性（Player,Monster...）
	//int typeId;//物体种类ID
	//int uniqueId;//每个物体独一无二的ID
	int shapeType;//几何形状的种类：0为圆形，1为矩形
	double radius;//圆形半径
	double width;//矩形宽度
	double height;//矩形高度
};

class Character : public BaseObject
{
protected://先假设弹匣子弹无限
	int bulletType, bulletPeriod;//, lastFireTime;//子弹类型、子弹发射周期、上次开火时间
	//float bulletPeriodRate;//子弹发射周期速度倍率(控制子弹发射速度)
	//最高血量：10000 
	double HPMaximum;//HP, HPIncrement, lastTime;//血量、血量增速(可正可负)、buff持续时间
	//float velocityRate;//速度倍率(控制人物的移动速度大小)
};

class Player : public Character
{
public:
};

class Monster : public Character
{
public:
};

class Item : public BaseObject
{
public:
};

class Bullet : BaseObject
{
public:
	
protected:
	int attack;//攻击力
};

class Wall : public BaseObject
{
public:
};


void WriteMonster()
{
	string str = "0";

	string filepath = "..\\YMX_GAME\\Res\\Monsters.ini";
	IniFile ini;
 
	ini.Load(filepath);
	ini.SetDoubleValue(str,"baseSpeed",0.1);
	ini.SetIntValue(str,"shapeType",0);
	ini.SetDoubleValue(str,"radius",20);
	ini.SetIntValue(str,"bulletType",0);
	ini.SetIntValue(str,"bulletPeriod",10);
	ini.SetDoubleValue(str,"HPmaximum",10000);
	ini.Save();
}

void WritePlayer()
{
	string str = "0";

	string filepath = "..\\YMX_GAME\\Res\\Players.ini";
	IniFile ini;
 
	ini.Load(filepath);
	ini.SetDoubleValue(str,"baseSpeed",0.1);
	ini.SetIntValue(str,"shapeType",0);
	ini.SetDoubleValue(str,"radius",20);
	ini.SetIntValue(str,"bulletType",0);
	ini.SetIntValue(str,"bulletPeriod",10);
	ini.SetDoubleValue(str,"HPmaximum",10000);
	ini.Save();
}

void WriteBullet()
{
	string str = "0";

	string filepath = "..\\YMX_GAME\\Res\\Bullets.ini";
	IniFile ini;
 
	ini.Load(filepath);
	ini.SetDoubleValue(str,"baseSpeed",1);
	ini.SetIntValue(str,"shapeType",0);
	ini.SetDoubleValue(str,"radius",2);
	ini.SetDoubleValue(str,"attack",500);
	ini.Save();
}

void WriteWall()
{
	string str = "0";

	string filepath = "..\\YMX_GAME\\Res\\Walls.ini";
	IniFile ini;
 
	ini.Load(filepath);
	ini.SetDoubleValue(str,"baseSpeed",1);
	ini.SetIntValue(str,"shapeType",0);
	ini.SetDoubleValue(str,"radius",2);
	ini.Save();
}

int _tmain(int argc, _TCHAR* argv[])
{
	WriteMonster();
	WritePlayer();
	WriteBullet();
	WriteWall();
	return 0;
}

