#pragma once 
#include <iostream>
#include <math.h>
#include <vector>
#include "Common.hpp"
#include "AABBTree.hpp"

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


class BaseObject
{
public:
	//BaseObject(){}
	BaseObject():pos(-1,-1), v(0, 0), baseSpeed(-1), type(ENUM_UNINIT), typeId(-1), uniqueId(-1), shapeType(-1), radius(-1), width(-1), height(-1), isDeleted(0){}

	Position GetPos();//����Position�ṹ��
	void SetPos(double x,double y);
	void SetPos(const Position& p);
	//��������λ��
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

	void set_width(double t) { width = t; }
	void set_height(double t) { height = t; }

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

	virtual std::vector<BaseObject*> Update(int now, int period) = 0;//�������
	virtual void OnCollision(BaseObject* obj) = 0;//������ײ
	void UpdateAABB();
	AABB* get_AABB() { return &aabb; }

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

	AABB aabb;

};

class Character : public BaseObject
{
public:
	//Character(){}
	Character():BaseObject(), bulletType(-1), bulletPeriod(-1), lastFireTime(0), bulletPeriodRate(1.0), attackRate(1.0), DEF(1.0), HPmaximum(-1), HP(-1), HPIncreasement(0), lastTime(0), velocityRate(1.0){}

	//virtual void Update(int now, int period);//�������
	//virtual void OnCollision(BaseObject* obj);//������ײ
	void IncreaseHP(double dHP);//�ı�HP
	std::vector<BaseObject*> Fire(Position direction);
	
	double get_HPmaximum(){return HPmaximum;}
	void set_HPmaximum(double t){ HPmaximum = t;}
	int get_bulletType(){return bulletType;}
	void set_bulletType(int t){ bulletType = t;}
	int get_bulletPeriod(){return bulletPeriod;}
	void set_bulletPeriod(int t){ bulletPeriod = t;}
	void set_bulletPeriodRate(double t) { bulletPeriodRate = t; }
	void set_attackRate(double t) { attackRate = t; }

	void set_DEF(double t) { DEF = t; }

	void set_HP(double t) { HP = t; }
	double get_HP() { return HP; }

	void add_HPIncreasement(double t) { HPIncreasement += t; }
	void set_lastTime(int t) { lastTime = t; }
	void set_velocityRate(double t) { velocityRate = t; }

	//TODU:�����������˼����µ����ԣ�������Ӧ�Ĺ��캯��Ҫ�ĵ�
protected://�ȼ��赯ϻ�ӵ�����
	int bulletType, bulletPeriod, lastFireTime;//�ӵ����͡��ӵ��������ڡ��ϴο���ʱ��
	double bulletPeriodRate, attackRate;//�ӵ����������ٶȱ���(�����ӵ������ٶ�)���˺�����
	double DEF;//��������һ������0��1֮���ʵ������ʾ�ܵ��˺��ı���
	double HPmaximum, HP, HPIncreasement;//Ѫ�����ޡ�Ѫ����Ѫ������(�����ɸ�)
	int lastTime;//buff����ʱ��
	double velocityRate;//�ٶȱ���(ʵ������ļ��١�����buff)
};

class Player : public Character
{
public:
	//Player(){}
	Player():Character(){}

	/*
	bool GetMouseStatus(MouseButton key);//��õ�ǰ��갴����Ϣ:��������,�򷵻�1;��û�а���,�򷵻�0;
	Position GetMousePosition();//��õ�ǰ����ڵ�ͼ�ϵ�λ�ã�ע�⣡���ǵ�ͼ�ϵ�λ�ö�������Ļ�ϵ�λ�ã�����
	bool GetKeyboardStatus(char key);//��õ�ǰ���̰�������Ϣ:��������,�򷵻�1;��û�а���,�򷵻�0;
	*/

//TODO:�����ã�������������For Debugging
	//bool GetMouseStatus(MouseButton key) { return false; }
	//Position GetMousePosition();//return Engine::get_Instance()->get_player()->GetPos();
	//bool GetKeyboardStatus(char key) { return 0; }
	
	bool GetMouseStatus(MouseButton key);
	Position GetMousePosition();
	bool GetKeyboardStatus(char key);

	virtual std::vector<BaseObject*> Update(int now, int period) override;//�������
	virtual void OnCollision(BaseObject* obj) override;//������ײ
};

class Monster : public Character
{
public:
	//Monster(){}
	Monster():Character(){}

	virtual std::vector<BaseObject*> Update(int now, int period) override;//�������
	virtual void OnCollision(BaseObject* obj) override;//������ײ
};

class Item : public BaseObject
{
public:
	//Item(){}
	Item():BaseObject()
	{
		baseSpeed = 0;
		shapeType = 0;
		radius = 30;
	}

	void Use(Character* user);//ʹ����Ʒ

	virtual std::vector<BaseObject*> Update(int now, int period) override;//�������
	virtual void OnCollision(BaseObject* obj) override;//������ײ
};

class Bullet : public BaseObject
{
public:
	//Bullet(){}
	Bullet():BaseObject(), attack(-1){}

	double GetAttack();
	virtual std::vector<BaseObject*> Update(int now, int period) override;//�������
	virtual void OnCollision(BaseObject* obj) override;//������ײ
	
		
	double get_attack(){return attack;}
	void set_attack(double t){ attack = t;}
	
protected:
	double attack;//������
};

class Wall : public BaseObject
{
public:
	//Wall(){}
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

class Map : public BaseObject
{
public:
	//Wall(){}
	Map():BaseObject() {}

	virtual std::vector<BaseObject*> Update(int now, int period) override//�������
	{
		return std::vector<BaseObject*>();
	}
	virtual void OnCollision(BaseObject* obj) override//������ײ
	{
		return;
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
	Event():occurTime(-1), type(ENUM_UNINIT), typeId(-1){}
	Event(Position _pos, int _occurTime, ObjType _type, int _typeId):
		pos(_pos), occurTime(_occurTime), type(_type), typeId(_typeId){}
	friend bool operator < (const Event &a, const Event &b)
	{
		return a.occurTime > b.occurTime;
	}
	friend std::istream& operator >> (std::istream& is, Event &t)
	{
		//TODO: deal with failure
		is >> t.occurTime >> t.pos.x >> t.pos.y >> t.typeId;
		t.type = ENUM_MONSTOR;
		return is;
	}
	BaseObject* Occur();
};