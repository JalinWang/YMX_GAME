#pragma once 
#include <iostream>
#include <math.h>
#include <vector>
/*
����˵����
1.���������й���ʱ�������now, period������int����������λ�Ǻ���
2.�����е��ٶ�ʸ���ĵ�λΪ������ÿ����
3.Item��Ч����Ӳ����
4.
*/

/*
TODO:
1.����͹�����Էֳɺö���Ӫ����������
2.�����ֺ����ӿںͷ���ֵ������ֱ�Ӵ����������ȫ���ĳɴ�ָ�룬���Ч�ʣ�������ͣ�Update������
*/

//�������ԣ��������������������
/*
ENUM_MAP, ENUM_WALL��BaseObject��
ENUM_PLAYER��Player��
ENUM_MONSTOR��Monstor��
ENUM_ITEM��Item��
ENUM_BULLET��Bullet��
*/
enum ObjType
{
	ENUM_MAP, ENUM_WALL, ENUM_PLAYER, ENUM_MONSTOR, ENUM_ITEM, ENUM_BULLET
};

//ENUM_LBUTTON��ʾ������, ENUM_MBUTTON��ʾ����м�, ENUM_RBUTTON��ʾ����Ҽ�
enum MouseButton
{
	ENUM_LBUTTON, ENUM_MBUTTON, ENUM_RBUTTON
};

//λ��ʸ���ṹ��
struct Position
{
	double x, y;
	Position(){}
	Position(double _x, double _y):x(_x), y(_y){}
	Position operator - (Position& a)
	{
		return Position(x-a.x, y-a.y);
	}

	double Norm()//����������ģ��������L2������
	{
		return sqrt(x*x + y*y);
	}

	void Standardize()//������׼��������ɵ�λ������
	{
		x /= Norm();
		y /= Norm();
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

//�ٶ�ʸ���ṹ��
struct MainVelocity
{
	double x, y;
	MainVelocity(){}
	MainVelocity(double _x, double _y):x(_x), y(_y){}
};

class BaseObject
{
public:
	BaseObject(){}
	BaseObject():pos(-1,-1), v(-1, -1), baseSpeed(-1), type(-1), typeId(-1), uniqueId(-1), shapeType(-1), radius(-1), width(-1), height(-1), isDeleted(0){}

	Position GetPos();//����Position�ṹ��
	void SetPos(double x,double y);//��������λ��
	void IncreasePos(double dx, double dy);//��������λ��Ϊdx, dy

	MainVelocity GetVelocity();//����MainVelocity�ṹ��
	void SetVelocity(double x,double y);//���������ٶ�
	void IncreaseVelocity(double dx, double dy);//���������ٶ�����Ϊdx, dy

	double GetBaseSpeed();//���ظö����ܹ������ƶ��Ļ����ٶ�
	
	void set_baseSpeed(double t){baseSpeed = t;}

	double get_radius(){return radius;}
	void set_radius(double t){radius = t;}
	int get_shapeType(){return shapeType;}
	void set_shapeType(int t){shapeType = t;}

	int get_uniqueId(){return uniqueId;}
	void set_uniqueId(int t){uniqueId = t;}
	int get_typeId(){return typeId;}
	void set_typeId(int t){typeId = t;}
	ObjType get_type(){return type;}
	void set_type(ObjType t){type = t;}

	ObjType GetType();//���ض������ԣ�Player,Monster...��
	int GetTypeId();//������������ID
	int GetUniqueId();//���ض�һ�޶���ID

	int GetShapeType();
	double GetRadius();
	double GetWidth();
	double GetHeight();

	void Delete();//ɾ��������
	bool IsDeleted();//�������Ƿ�ɾ��������Ƿ���1��

	virtual std::vector<BaseObject*> Update(int now, int period);//�������
	virtual void OnCollision(BaseObject* obj);//������ײ

protected:
	Position pos;//λ�úᡢ������
	MainVelocity v;//���������ٶ�
	double baseSpeed;//�������ܹ������ƶ��Ļ����ٶ�
	ObjType type;//�������ԣ�Player,Monster...��
	int typeId;//��������ID
	int uniqueId;//ÿ�������һ�޶���ID
	int shapeType;//������״�����ࣺ0ΪԲ�Σ�1Ϊ����
	double radius;//Բ�ΰ뾶
	double width;//���ο��
	double height;//���θ߶�
	bool isDeleted;//ɾ�����
};

class Character : public BaseObject
{
public:
	Character(){}
	Character():BaseObject(), bulletType(-1), bulletPeriod(-1), lastFireTime(0), bulletPeriodRate(1.0), HPmaximum(-1), HP(-1), HPIncrement(0), lastTime(0), velocityRate(1.0){}

	//virtual void Update(int now, int period);//�������
	//virtual void OnCollision(BaseObject* obj);//������ײ
	void IncreaseHP(double dHP);//�ı�HP
	std::vector<BaseObject*> Fire(Position direction);
	
	double get_HPmaximum(){return radius;}
	void set_HPmaximum(double t){radius = t;}
	int get_bulletType(){return shapeType;}
	void set_bulletType(int t){shapeType = t;}
	int get_bulletPeriod(){return shapeType;}
	void set_bulletPeriod(int t){shapeType = t;}

protected://�ȼ��赯ϻ�ӵ�����
	int bulletType, bulletPeriod, lastFireTime;//�ӵ����͡��ӵ��������ڡ��ϴο���ʱ��
	double bulletPeriodRate;//�ӵ����������ٶȱ���(�����ӵ������ٶ�)
	double HPmaximum, HP, HPIncrement;//Ѫ�����ޡ�Ѫ����Ѫ������(�����ɸ�)
	int lastTime;//buff����ʱ��
	double velocityRate;//�ٶȱ���(ʵ������ļ��١�����buff)
};

class Player : public Character
{
public:
	Player(){}
	Player():Character(){}

	bool GetMouseStatus(MouseButton key);//��õ�ǰ��갴����Ϣ:��������,�򷵻�1;��û�а���,�򷵻�0;
	Position GetMousePosition();//��õ�ǰ����ڵ�ͼ�ϵ�λ�ã�ע�⣡���ǵ�ͼ�ϵ�λ�ö�������Ļ�ϵ�λ�ã�����
	bool GetKeyboardStatus(char key);//��õ�ǰ���̰�������Ϣ:��������,�򷵻�1;��û�а���,�򷵻�0;

	virtual std::vector<BaseObject*> Update(int now, int period) override;//�������
	virtual void OnCollision(BaseObject* obj) override;//������ײ
};

class Monster : public Character
{
public:
	Monster(){}
	Monster():Character(){}

	virtual std::vector<BaseObject*> Update(int now, int period) override;//�������
	virtual void OnCollision(BaseObject* obj) override;//������ײ
};

class Item : public BaseObject
{
public:
	Item(){}
	Item():BaseObject(){}

	void Use(Character* user);//ʹ����Ʒ

	virtual std::vector<BaseObject*> Update(int now, int period) override;//�������
	virtual void OnCollision(BaseObject* obj) override;//������ײ
};

class Bullet : public BaseObject
{
public:
	Bullet(){}
	Bullet():BaseObject(), attack(-1){}

	double GetAttack();
	virtual std::vector<BaseObject*> Update(int now, int period) override;//�������
	virtual void OnCollision(BaseObject* obj) override;//������ײ
	
		
	double get_attack(){return radius;}
	void set_attack(double t){radius = t;}
	
protected:
	double attack;//������
};

class Wall : public BaseObject
{
public:
	Wall(){}
	Wall():BaseObject(){}

	virtual std::vector<BaseObject*> Update(int now, int period) override//�������
	{
		return std::vector<BaseObject*>();
	}
	virtual void OnCollision(BaseObject* obj) override//������ײ
	{
		return ;
	}
};

//map Event
class Event
{
public:
	int occurTime; //����ʱ��
	int typeId;
	ObjType type;

	int get_typeId(){return typeId;}
	void set_typeId(int t){typeId = t;}
	ObjType get_type(){return type;}
	void set_type(ObjType t){type = t;}

	Position pos;
	Event(){}
	Event(Position _pos, int _occurTime, ObjType _type, int _typeId):
		pos(_pos), occurTime(_occurTime), type(_type), typeId(_typeId){}
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
	BaseObject *Occur()
	{
		auto mRes = Engine::get_Instance()->get_mRes();
		return mRes->Load(type, typeId);
	}
};