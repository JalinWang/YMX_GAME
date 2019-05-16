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
	int baseSpeed;//�������ܹ������ƶ��Ļ����ٶ�
	//ObjType type;//�������ԣ�Player,Monster...��
	//int typeId;//��������ID
	//int uniqueId;//ÿ�������һ�޶���ID
	int shapeType;//������״�����ࣺ0ΪԲ�Σ�1Ϊ����
	int radius;//Բ�ΰ뾶
	int width;//���ο��
	int height;//���θ߶�
};

class Charater : public BaseObject
{
protected://�ȼ��赯ϻ�ӵ�����
	int bulletType, bulletPeriod;//, lastFireTime;//�ӵ����͡��ӵ��������ڡ��ϴο���ʱ��
	//float bulletPeriodRate;//�ӵ����������ٶȱ���(�����ӵ������ٶ�)
	//���Ѫ����10000 
	int HPMaximum;//HP, HPIncrement, lastTime;//Ѫ����Ѫ������(�����ɸ�)��buff����ʱ��
	//float velocityRate;//�ٶȱ���(����������ƶ��ٶȴ�С)
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
	int attack;//������
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

