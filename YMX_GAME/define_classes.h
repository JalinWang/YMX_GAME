#pragma once 
#include <iostream>

/*
TODO:
����͹�����Էֳɺö���Ӫ����������
*/

enum ObjType
{
	ENUM_MAP, ENUM_MONSTOR, ENUM_PLAYER, ENUM_WALL, ENUM_BULLET
};

//ENUM_LBUTTON��ʾ������, ENUM_MBUTTON��ʾ����м�, ENUM_RBUTTON��ʾ����Ҽ�
enum MouseButton
{
	ENUM_LBUTTON, ENUM_MBUTTON, ENUM_RBUTTON
};

struct Position
{
	int x, y;
	Position(){}
	Position(int _x, int _y):x(_x), y(_y){}
	Position operator - (Position& a)
	{
		return Position(x-a.x, y-a.y);
	}
	friend inline std::ostream & operator << (std::ostream & f, Position &p)
	{
	    f << p.x << " " << p.y;
	    return f;
	}

	friend inline std::istream & operator >> (std::istream & f, Position &p)
	{
	    f >> p.x >> p.y;
	    return f;
	}
};

struct MainVelocity
{
	int x, y;
};

//TODO: give to shiyang
class Mouse
{
public:
	bool GetMouseStatus(MouseButton key);//��õ�ǰ��갴����Ϣ:��������,�򷵻�1;��û�а���,�򷵻�0;
	Position GetMousePosition();//��õ�ǰ����ڵ�ͼ�ϵ�λ�ã�ע�⣡���ǵ�ͼ�ϵ�λ�ö�������Ļ�ϵ�λ�ã�����

};
//TODO: give to shiyang
class Keyboard
{
public:
	bool GetKeyboardStatus(char key);//��õ�ǰ���̰�������Ϣ:��������,�򷵻�1;��û�а���,�򷵻�0;
};

class BaseObject
{
public:
	Position GetPos();//����Position�ṹ��
	MainVelocity GetVelocity();//����MainVelocity�ṹ��
	void IncreaseVelocity(int dx, int dy);//���������ٶ�����Ϊdx, dy
	ObjType GetType();//���ض������ԣ�Player,Monster...��
	int GetTypeId();//������������ID
	int GetUniqueId();//���ض�һ�޶���ID

	virtual void Update(int now, int period);//�������
	virtual void OnCollision(BaseObject* obj);//������ײ

protected:
	Position pos;//λ�úᡢ������
	MainVelocity v;//���������ٶ�
	int baseSpeed;//�������ܹ������ƶ��Ļ����ٶ�
	ObjType type;//�������ԣ�Player,Monster...��
	int typeId;//��������ID
	int uniqueId;//ÿ�������һ�޶���ID
	int shapeType;//������״�����ࣺ0ΪԲ�Σ�1Ϊ����
	int radius;//Բ�ΰ뾶
	int width;//���ο��
	int height;//���θ߶�
};

class Charater : public BaseObject
{
public:
	//virtual void Update(int now, int period);//�������
	//virtual void OnCollision(BaseObject* obj);//������ײ

	//virtual void Fire();//����

protected://�ȼ��赯ϻ�ӵ�����
	int bulletType, bulletPeriod, lastFireTime;//�ӵ����͡��ӵ��������ڡ��ϴο���ʱ��
	float bulletPeriodRate;//�ӵ����������ٶȱ���(�����ӵ������ٶ�)
	int HP, HPIncrement, lastTime;//Ѫ����Ѫ������(�����ɸ�)��buff����ʱ��
	float velocityRate;//�ٶȱ���(����������ƶ��ٶȴ�С)
};

class Player : public Charater
{
public:
	virtual void Update(int now, int period) override;//�������
	virtual void OnCollision(BaseObject* obj) override;//������ײ

	void Fire();//����
};

class Monster : public Charater
{
public:
	virtual void Update(int now, int period) override;//�������
	virtual void OnCollision(BaseObject* obj) override;//������ײ

	void Fire();//����
};

class Item : public BaseObject
{
public:
	virtual void Use(Player* user) const;//ʹ����Ʒ

	virtual void Update(int now, int period) override;//�������
	virtual void OnCollision(BaseObject* obj) override;//������ײ
};

class Bullet : BaseObject
{
public:
	virtual void Update(int now, int period) override;//�������
	virtual void OnCollision(BaseObject* obj) override;//������ײ
	
protected:
	int attack;//������
};

//map Event
class Event{
public:
	int occurTime; //����ʱ��
	int typeId;
	ObjType type;
	int posX, posY;
	bool operator < (const Event &t)
	{
		return occurTime < t.occurTime;
	}
	BaseObject *Occur()
	{
		BaseObject *ret;
		switch (type)
		{
		case ENUM_MAP:
			break;
		case ENUM_MONSTOR:
			break;
		case ENUM_PLAYER:
			ret = new Player();
			break;
		case ENUM_WALL:
			break;
		case ENUM_BULLET:
			break;
		default:
			break;
		}
		return ret;
	}
};

//map Event
class Event
{
public:
	int occurTime; //����ʱ��
	int typeId;
	ObjType type;
	Position pos;
	Event(){}
	Event(Position _pos, int _occurTime):pos(_pos), occurTime(_occurTime){}
	bool operator < (const Event &t)
	{
		return occurTime < t.occurTime;
	}
	friend istream& operator >> (istream& is, Event &t)
	{
		//TODO: deal with failure
		is >> t.occurTime >> t.pos.x >> t.pos.y >> t.typeId;
		t.type = ENUM_MONSTOR;
		return is;
	}
	BaseObject *Occur() const
	{
		BaseObject *ret;
		switch (type)
		{
		case ENUM_MAP:
			break;
		case ENUM_MONSTOR:
			break;
		case ENUM_PLAYER:
			ret = new Player();
			break;
		case ENUM_WALL:
			break;
		case ENUM_BULLET:
			break;
		default:
			break;
		}
		return ret;
	}
};