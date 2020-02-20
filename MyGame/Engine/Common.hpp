#pragma once
#include "..\DXSupports.h"
//对象属性，用来辨别对象的性质属性
/*
ENUM_MAP, ENUM_WALL是BaseObject类
ENUM_PLAYER是Player类
ENUM_MONSTOR是Monstor类
ENUM_ITEM是Item类
ENUM_BULLET是Bullet类
*/

enum ObjType
{
	ENUM_MAP, ENUM_WALL, ENUM_PLAYER, ENUM_MONSTOR, ENUM_ITEM, ENUM_BULLET, ENUM_UNINIT
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
	Position() :x(0), y(0) {}
	Position(const Position& p) :x(p.x), y(p.y) {}
	Position(double _x, double _y) :x(_x), y(_y) {}

	operator POINT() {
		POINT p;
		p.x = LONG(x);
		p.y = LONG(y);
		return p;
	}

	inline
		Position operator - (const Position& a)
	{
		return Position(x - a.x, y - a.y);
	}

	inline
		Position operator + (const Position& a)
	{
		return Position(x + a.x, y + a.y);
	}

	inline
		Position operator / (const double& a)
	{
		return Position(x / a, y / a);
	}

	inline
		Position operator * (const double& a)
	{
		return Position(x * a, y * a);
	}

	inline
		double Norm()//计算向量的模（向量的L2范数）
	{
		return sqrt(x * x + y * y);
	}

	inline
		void Standardize()//向量标准化（化归成单位向量）
	{
		if (!(x == 0 && y == 0))
		{
			double length = Norm();
			x /= length;
			y /= length;
		}
	}

	friend inline std::ostream& operator << (std::ostream& f, Position& p)
	{
		f << p.x << " " << p.y;
		return f;
	}

	friend inline std::istream& operator >> (std::istream& f, Position& p)
	{
		f >> p.x >> p.y;
		return f;
	}

	Position operator - (const double& t)
	{
		Position p(*this);
		p.x -= t;
		p.y -= t;
		return p;
	}

	Position operator + (const double& t)
	{
		Position p(*this);
		p.x += t;
		p.y += t;
		return p;
	}

	bool operator < (const Position& t) const
	{
		return x < t.x && y < t.y;
	}

	bool operator > (const Position& t) const
	{
		return x > t.x && y > t.y;
	}
};

//速度矢量结构体
struct MainVelocity
{
	double x, y;
	MainVelocity() :x(0), y(0) {}
	MainVelocity(double _x, double _y) :x(_x), y(_y) {}
};