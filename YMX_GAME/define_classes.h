#pragma once 
#include <iostream>
#include <math.h>
#include <vector>

/*
TODO:
1.人类和怪物可以分成好多阵营，以免误伤
2.检查各种函数接口和返回值，避免直接传变量，最好全部改成传指针，提高效率（反面典型：Update函数）
*/

//对象属性，用来辨别对象的数据类型
enum ObjType
{
	ENUM_MAP, ENUM_MONSTOR, ENUM_PLAYER, ENUM_WALL, ENUM_BULLET
};

//ENUM_LBUTTON表示鼠标左键, ENUM_MBUTTON表示鼠标中键, ENUM_RBUTTON表示鼠标右键
enum MouseButton
{
	ENUM_LBUTTON, ENUM_MBUTTON, ENUM_RBUTTON
};

//位置矢量结构体
struct Position
{
	double x, y;
	Position(){}
	Position(double _x, double _y):x(_x), y(_y){}
	Position operator - (Position& a)
	{
		return Position(x-a.x, y-a.y);
	}

	double Norm()//计算向量的模（向量的L2范数）
	{
		return sqrt(x*x + y*y);
	}

	void Standardize()//向量标准化（化归成单位向量）
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

//速度矢量结构体
struct MainVelocity
{
	double x, y;
};

class BaseObject
{
public:
	Position GetPos();//返回Position结构体
	void SetPos(double x,double y);//设置物体位置
	void IncreasePos(double dx, double dy);//两个方向位移为dx, dy

	MainVelocity GetVelocity();//返回MainVelocity结构体
	void SetVelocity(double x,double y);//设置物体速度
	void IncreaseVelocity(double dx, double dy);//两个方向速度增量为dx, dy

	double GetBaseSpeed();//返回该对象能够主动移动的基础速度
	void set_baseSpeed(double t){baseSpeed = t;}

	double get_radius(){return radius;}
	void set_radius(double t){radius = t;}
	double get_radius(){return radius;}
	void set_radius(double t){radius = t;}
	int get_shapeType(){return shapeType;}
	void set_shapeType(int t){shapeType = t;}

	ObjType GetType();//返回对象属性（Player,Monster...）
	int GetTypeId();//返回物体种类ID
	int GetUniqueId();//返回独一无二的ID

	void Delete();//删除该物体
	bool IsDeleted();//该物体是否被删除，如果是返回1；

	virtual std::vector<BaseObject*> Update(int now, int period);//常规更新
	virtual void OnCollision(BaseObject* obj);//处理碰撞

protected:
	Position pos;//位置横、纵坐标
	MainVelocity v;//横向、纵向速度
	double baseSpeed;//该物体能够主动移动的基础速度
	ObjType type;//对象属性（Player,Monster...）
	int typeId;//物体种类ID
	int uniqueId;//每个物体独一无二的ID
	int shapeType;//几何形状的种类：0为圆形，1为矩形
	double radius;//圆形半径
	double width;//矩形宽度
	double height;//矩形高度
	bool isDeleted;//删除标记
};

class Charater : public BaseObject
{
public:
	//virtual void Update(int now, int period);//常规更新
	//virtual void OnCollision(BaseObject* obj);//处理碰撞
	void IncreaseHP(double dHP);//改变HP
	std::vector<BaseObject*> Fire(Position direction);

	double get_HPmaximum(){return radius;}
	void set_HPmaximum(double t){radius = t;}
	int get_bulletType(){return shapeType;}
	void set_bulletType(int t){shapeType = t;}
	int get_bulletPeriod(){return shapeType;}
	void set_bulletPeriod(int t){shapeType = t;}

protected://先假设弹匣子弹无限
	int bulletType, bulletPeriod, lastFireTime;//子弹类型、子弹发射周期、上次开火时间
	double bulletPeriodRate;//子弹发射周期速度倍率(控制子弹发射速度)
	double HPmaximum, HP, HPIncrement;//血量上限、血量、血量增速(可正可负)
	int lastTime;//buff持续时间
	double velocityRate;//速度倍率(实现人物的加速、减速buff)
};

class Player : public Charater
{
public:
	bool GetMouseStatus(MouseButton key);//获得当前鼠标按键信息:若被按下,则返回1;若没有按下,则返回0;
	Position GetMousePosition();//获得当前鼠标在地图上的位置（注意！！是地图上的位置而不是屏幕上的位置！！）
	bool GetKeyboardStatus(char key);//获得当前键盘按键的信息:若被按下,则返回1;若没有按下,则返回0;

	virtual std::vector<BaseObject*> Update(int now, int period) override;//常规更新
	virtual void OnCollision(BaseObject* obj) override;//处理碰撞
};

class Monster : public Charater
{
public:
	virtual std::vector<BaseObject*> Update(int now, int period) override;//常规更新
	virtual void OnCollision(BaseObject* obj) override;//处理碰撞
};

class Item : public BaseObject
{
public:
	virtual void Use(Charater* user);//使用物品

	virtual std::vector<BaseObject*> Update(int now, int period) override;//常规更新
	virtual void OnCollision(BaseObject* obj) override;//处理碰撞
};

class Bullet : public BaseObject
{
public:
	double GetAttack();
	virtual std::vector<BaseObject*> Update(int now, int period) override;//常规更新
	virtual void OnCollision(BaseObject* obj) override;//处理碰撞
	
	double get_attack(){return radius;}
	void set_attack(double t){radius = t;}

protected:
	double attack;//攻击力
};

//map Event
class Event
{
public:
	int occurTime; //出现时间
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