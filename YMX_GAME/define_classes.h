#pragma once 
#include <iostream>
#include <math.h>
#include <vector>

/*
TODO:
1.����͹�����Էֳɺö���Ӫ����������
2.�����ֺ����ӿںͷ���ֵ������ֱ�Ӵ����������ȫ���ĳɴ�ָ�룬���Ч�ʣ�������ͣ�Update������
*/

//�������ԣ��������������������
enum ObjType
{
	ENUM_MAP, ENUM_MONSTOR, ENUM_PLAYER, ENUM_WALL, ENUM_BULLET
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
};

class BaseObject
{
public:
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
	double get_radius(){return radius;}
	void set_radius(double t){radius = t;}
	int get_shapeType(){return shapeType;}
	void set_shapeType(int t){shapeType = t;}

	ObjType GetType();//���ض������ԣ�Player,Monster...��
	int GetTypeId();//������������ID
	int GetUniqueId();//���ض�һ�޶���ID

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

class Charater : public BaseObject
{
public:
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

class Player : public Charater
{
public:
	bool GetMouseStatus(MouseButton key);//��õ�ǰ��갴����Ϣ:��������,�򷵻�1;��û�а���,�򷵻�0;
	Position GetMousePosition();//��õ�ǰ����ڵ�ͼ�ϵ�λ�ã�ע�⣡���ǵ�ͼ�ϵ�λ�ö�������Ļ�ϵ�λ�ã�����
	bool GetKeyboardStatus(char key);//��õ�ǰ���̰�������Ϣ:��������,�򷵻�1;��û�а���,�򷵻�0;

	virtual std::vector<BaseObject*> Update(int now, int period) override;//�������
	virtual void OnCollision(BaseObject* obj) override;//������ײ
};

class Monster : public Charater
{
public:
	virtual std::vector<BaseObject*> Update(int now, int period) override;//�������
	virtual void OnCollision(BaseObject* obj) override;//������ײ
};

class Item : public BaseObject
{
public:
	virtual void Use(Charater* user);//ʹ����Ʒ

	virtual std::vector<BaseObject*> Update(int now, int period) override;//�������
	virtual void OnCollision(BaseObject* obj) override;//������ײ
};

class Bullet : public BaseObject
{
public:
	double GetAttack();
	virtual std::vector<BaseObject*> Update(int now, int period) override;//�������
	virtual void OnCollision(BaseObject* obj) override;//������ײ
	
	double get_attack(){return radius;}
	void set_attack(double t){radius = t;}

protected:
	double attack;//������
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