// AttrGenerator.cpp : �������̨Ӧ�ó������ڵ㡣
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
	//Position pos;//λ�úᡢ������
	//MainVelocity v;//���������ٶ�
	double baseSpeed;//�������ܹ������ƶ��Ļ����ٶ�
	//ObjType type;//�������ԣ�Player,Monster...��
	//int typeId;//��������ID
	//int uniqueId;//ÿ�������һ�޶���ID
	int shapeType;//������״�����ࣺ0ΪԲ�Σ�1Ϊ����
	double radius;//Բ�ΰ뾶
	double width;//���ο��
	double height;//���θ߶�
};

class Character : public BaseObject
{
protected://�ȼ��赯ϻ�ӵ�����
	int bulletType, bulletPeriod;//, lastFireTime;//�ӵ����͡��ӵ��������ڡ��ϴο���ʱ��
	//float bulletPeriodRate;//�ӵ����������ٶȱ���(�����ӵ������ٶ�)
	//���Ѫ����10000 
	double HPMaximum;//HP, HPIncrement, lastTime;//Ѫ����Ѫ������(�����ɸ�)��buff����ʱ��
	//float velocityRate;//�ٶȱ���(����������ƶ��ٶȴ�С)
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
	int attack;//������
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

