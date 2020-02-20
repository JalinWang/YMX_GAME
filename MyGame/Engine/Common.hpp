#pragma once
#include "..\DXSupports.h"
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
	ENUM_MAP, ENUM_WALL, ENUM_PLAYER, ENUM_MONSTOR, ENUM_ITEM, ENUM_BULLET, ENUM_UNINIT
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
		double Norm()//����������ģ��������L2������
	{
		return sqrt(x * x + y * y);
	}

	inline
		void Standardize()//������׼��������ɵ�λ������
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

//�ٶ�ʸ���ṹ��
struct MainVelocity
{
	double x, y;
	MainVelocity() :x(0), y(0) {}
	MainVelocity(double _x, double _y) :x(_x), y(_y) {}
};