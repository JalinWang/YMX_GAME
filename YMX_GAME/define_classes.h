#pragma once 
#include <iostream>

/*
TODO:
人类和怪物可以分成好多阵营，以免误伤
*/

enum ObjType
{
	ENUM_MAP, ENUM_MONSTOR, ENUM_PLAYER, ENUM_WALL, ENUM_BULLET
};

//ENUM_LBUTTON表示鼠标左键, ENUM_MBUTTON表示鼠标中键, ENUM_RBUTTON表示鼠标右键
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
	bool GetMouseStatus(MouseButton key);//获得当前鼠标按键信息:若被按下,则返回1;若没有按下,则返回0;
	Position GetMousePosition();//获得当前鼠标在地图上的位置（注意！！是地图上的位置而不是屏幕上的位置！！）

};
//TODO: give to shiyang
class Keyboard
{
public:
	bool GetKeyboardStatus(char key);//获得当前键盘按键的信息:若被按下,则返回1;若没有按下,则返回0;
};

class BaseObject
{
public:
	Position GetPos();//返回Position结构体
	MainVelocity GetVelocity();//返回MainVelocity结构体
	void IncreaseVelocity(int dx, int dy);//两个方向速度增量为dx, dy
	ObjType GetType();//返回对象属性（Player,Monster...）
	int GetTypeId();//返回物体种类ID
	int GetUniqueId();//返回独一无二的ID

	virtual void Update(int now, int period);//常规更新
	virtual void OnCollision(BaseObject* obj);//处理碰撞

protected:
	Position pos;//位置横、纵坐标
	MainVelocity v;//横向、纵向速度
	int baseSpeed;//该物体能够主动移动的基础速度
	ObjType type;//对象属性（Player,Monster...）
	int typeId;//物体种类ID
	int uniqueId;//每个物体独一无二的ID
	int shapeType;//几何形状的种类：0为圆形，1为矩形
	int radius;//圆形半径
	int width;//矩形宽度
	int height;//矩形高度
};

class Charater : public BaseObject
{
public:
	//virtual void Update(int now, int period);//常规更新
	//virtual void OnCollision(BaseObject* obj);//处理碰撞

	//virtual void Fire();//开火

protected://先假设弹匣子弹无限
	int bulletType, bulletPeriod, lastFireTime;//子弹类型、子弹发射周期、上次开火时间
	float bulletPeriodRate;//子弹发射周期速度倍率(控制子弹发射速度)
	int HP, HPIncrement, lastTime;//血量、血量增速(可正可负)、buff持续时间
	float velocityRate;//速度倍率(控制人物的移动速度大小)
};

class Player : public Charater
{
public:
	virtual void Update(int now, int period) override;//常规更新
	virtual void OnCollision(BaseObject* obj) override;//处理碰撞

	void Fire();//开火
};

class Monster : public Charater
{
public:
	virtual void Update(int now, int period) override;//常规更新
	virtual void OnCollision(BaseObject* obj) override;//处理碰撞

	void Fire();//开火
};

class Item : public BaseObject
{
public:
	virtual void Use(Player* user) const;//使用物品

	virtual void Update(int now, int period) override;//常规更新
	virtual void OnCollision(BaseObject* obj) override;//处理碰撞
};

class Bullet : BaseObject
{
public:
	virtual void Update(int now, int period) override;//常规更新
	virtual void OnCollision(BaseObject* obj) override;//处理碰撞
	
protected:
	int attack;//攻击力
};

//map Event
class Event{
public:
	int occurTime; //出现时间
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