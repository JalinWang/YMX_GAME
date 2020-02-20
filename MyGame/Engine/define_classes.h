#pragma once 
#include <iostream>
#include <math.h>
#include <vector>
#include "Common.hpp"
#include "AABBTree.hpp"

/*
基本说明：
1.程序中所有关于时间的量（now, period）都是int型整数，单位是毫秒
2.程序中的速度矢量的单位为：像素每毫秒
3.Item的效果是硬编码
4.
*/

/*
TODO:
1.人类和怪物可以分成好多阵营，以免误伤
2.检查各种函数接口和返回值，避免直接传变量，最好全部改成传指针，提高效率（反面典型：Update函数）
*/


class BaseObject
{
public:
	//BaseObject(){}
	BaseObject():pos(-1,-1), v(0, 0), baseSpeed(-1), type(ENUM_UNINIT), typeId(-1), uniqueId(-1), shapeType(-1), radius(-1), width(-1), height(-1), isDeleted(0){}

	Position GetPos();//返回Position结构体
	void SetPos(double x,double y);
	void SetPos(const Position& p);
	//设置物体位置
	void IncreasePos(double dx, double dy);//两个方向位移为dx, dy

	MainVelocity GetVelocity();//返回MainVelocity结构体
	void SetVelocity(double x,double y);//设置物体速度
	void IncreaseVelocity(double dx, double dy);//两个方向速度增量为dx, dy

	double GetBaseSpeed();//返回该对象能够主动移动的基础速度
	
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

	ObjType GetType();//返回对象属性（Player,Monster...）
	int GetTypeId();//返回物体种类ID
	int GetUniqueId();//返回独一无二的ID

	int GetShapeType();
	double GetRadius();
	double GetWidth();
	double GetHeight();

	void Delete();//删除该物体
	bool IsDeleted();//该物体是否被删除，如果是返回1；

	virtual std::vector<BaseObject*> Update(int now, int period) = 0;//常规更新
	virtual void OnCollision(BaseObject* obj) = 0;//处理碰撞
	void UpdateAABB();
	AABB* get_AABB() { return &aabb; }

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

	AABB aabb;

};

class Character : public BaseObject
{
public:
	//Character(){}
	Character():BaseObject(), bulletType(-1), bulletPeriod(-1), lastFireTime(0), bulletPeriodRate(1.0), attackRate(1.0), DEF(1.0), HPmaximum(-1), HP(-1), HPIncreasement(0), lastTime(0), velocityRate(1.0){}

	//virtual void Update(int now, int period);//常规更新
	//virtual void OnCollision(BaseObject* obj);//处理碰撞
	void IncreaseHP(double dHP);//改变HP
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

	//TODU:由于我引入了几个新的属性，所以相应的构造函数要改掉
protected://先假设弹匣子弹无限
	int bulletType, bulletPeriod, lastFireTime;//子弹类型、子弹发射周期、上次开火时间
	double bulletPeriodRate, attackRate;//子弹发射周期速度倍率(控制子弹发射速度)，伤害倍率
	double DEF;//防御力是一个介于0和1之间的实数，表示受到伤害的比率
	double HPmaximum, HP, HPIncreasement;//血量上限、血量、血量增速(可正可负)
	int lastTime;//buff持续时间
	double velocityRate;//速度倍率(实现人物的加速、减速buff)
};

class Player : public Character
{
public:
	//Player(){}
	Player():Character(){}

	/*
	bool GetMouseStatus(MouseButton key);//获得当前鼠标按键信息:若被按下,则返回1;若没有按下,则返回0;
	Position GetMousePosition();//获得当前鼠标在地图上的位置（注意！！是地图上的位置而不是屏幕上的位置！！）
	bool GetKeyboardStatus(char key);//获得当前键盘按键的信息:若被按下,则返回1;若没有按下,则返回0;
	*/

//TODO:调试用！！！！！！！For Debugging
	//bool GetMouseStatus(MouseButton key) { return false; }
	//Position GetMousePosition();//return Engine::get_Instance()->get_player()->GetPos();
	//bool GetKeyboardStatus(char key) { return 0; }
	
	bool GetMouseStatus(MouseButton key);
	Position GetMousePosition();
	bool GetKeyboardStatus(char key);

	virtual std::vector<BaseObject*> Update(int now, int period) override;//常规更新
	virtual void OnCollision(BaseObject* obj) override;//处理碰撞
};

class Monster : public Character
{
public:
	//Monster(){}
	Monster():Character(){}

	virtual std::vector<BaseObject*> Update(int now, int period) override;//常规更新
	virtual void OnCollision(BaseObject* obj) override;//处理碰撞
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

	void Use(Character* user);//使用物品

	virtual std::vector<BaseObject*> Update(int now, int period) override;//常规更新
	virtual void OnCollision(BaseObject* obj) override;//处理碰撞
};

class Bullet : public BaseObject
{
public:
	//Bullet(){}
	Bullet():BaseObject(), attack(-1){}

	double GetAttack();
	virtual std::vector<BaseObject*> Update(int now, int period) override;//常规更新
	virtual void OnCollision(BaseObject* obj) override;//处理碰撞
	
		
	double get_attack(){return attack;}
	void set_attack(double t){ attack = t;}
	
protected:
	double attack;//攻击力
};

class Wall : public BaseObject
{
public:
	//Wall(){}
	Wall():BaseObject(){}

	virtual std::vector<BaseObject*> Update(int now, int period) override//常规更新
	{
		return std::vector<BaseObject*>();
	}
	virtual void OnCollision(BaseObject* obj) override//处理碰撞
	{
		return ;
	}
};

class Map : public BaseObject
{
public:
	//Wall(){}
	Map():BaseObject() {}

	virtual std::vector<BaseObject*> Update(int now, int period) override//常规更新
	{
		return std::vector<BaseObject*>();
	}
	virtual void OnCollision(BaseObject* obj) override//处理碰撞
	{
		return;
	}
};

//map Event
class Event
{
public:
	int occurTime; //出现时间
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