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
	int baseSpeed;//该物体能够主动移动的基础速度
	//ObjType type;//对象属性（Player,Monster...）
	//int typeId;//物体种类ID
	//int uniqueId;//每个物体独一无二的ID
	int shapeType;//几何形状的种类：0为圆形，1为矩形
	int radius;//圆形半径
	int width;//矩形宽度
	int height;//矩形高度
};

class Charater : public BaseObject
{
protected://先假设弹匣子弹无限
	int bulletType, bulletPeriod;//, lastFireTime;//子弹类型、子弹发射周期、上次开火时间
	//float bulletPeriodRate;//子弹发射周期速度倍率(控制子弹发射速度)
	//最高血量：10000 
	int HPMaximum;//HP, HPIncrement, lastTime;//血量、血量增速(可正可负)、buff持续时间
	//float velocityRate;//速度倍率(控制人物的移动速度大小)
};

class Player : public Charater
{
public:
};

class Monster : public Charater
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

int _tmain(int argc, _TCHAR* argv[])
{
	string filepath = "..\\YMX_GAME\\Res\\Monsters.ini";
 
	IniFile ini;
 
	ini.Load(filepath);
 
	ini.SetIntValue("0","baseSpeed",1000);
	ini.SetIntValue("0","shapeType",0);
	ini.SetIntValue("0","radius",2);
	ini.SetIntValue("0","bulletType",0);
	ini.SetIntValue("0","bulletPeriod",100);
	ini.SetIntValue("0","baseSpeed",10);

	ini.Save();
	return 0;
}

