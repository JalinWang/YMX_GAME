#include "define_classes.h"
#include "Engine.h"
#include <iostream>
#include <fstream>
#include <stdlib.h>
using namespace std;
#include "..\DXSupports.h"

extern UCHAR KeyState[256];
extern DIMOUSESTATE MouseState;
extern POINT curPos;
extern RECT camera;

Position BaseObject::GetPos()
{
	return pos;
}

void BaseObject::SetPos(double x,double y)
{
	pos.x = x;
	pos.y = y;
}
void BaseObject::SetPos(const Position &p)
{
	pos = p;
}

void BaseObject::IncreasePos(double dx, double dy)
{
	pos.x += dx;
	pos.y += dy;
}

MainVelocity BaseObject::GetVelocity()
{
	return v;
}

void BaseObject::SetVelocity(double x,double y)
{
	v.x = x;
	v.y = y;
}

void BaseObject::IncreaseVelocity(double dx, double dy)
{
	v.x += dx;
	v.y += dy;
}

double BaseObject::GetBaseSpeed()
{
	return baseSpeed;
}

ObjType BaseObject::GetType()
{
	return type;
}

int BaseObject::GetTypeId()
{
	return typeId;
}
int BaseObject::GetUniqueId()
{
	return uniqueId;
}

int BaseObject::GetShapeType()
{
	return shapeType;
}

double BaseObject::GetRadius()
{
	return radius;
}

double BaseObject::GetWidth()
{
	return width;
}

double BaseObject::GetHeight()
{
	return height;
}

void BaseObject::Delete()
{
	isDeleted = 1;
}

bool BaseObject::IsDeleted()
{
	return isDeleted;
}

void BaseObject::UpdateAABB()
{
	if (shapeType == 0)
	{
		aabb.minPoint = Position(pos - radius);
		aabb.maxPoint = Position(pos + radius);
	}
	else
	{
		aabb.minPoint = Position(pos.x - width / 2, pos.y - height / 2);
		aabb.maxPoint = Position(pos.x + width / 2, pos.y + height / 2);
	}
	aabb.pObj = static_cast<void*>(this);
}

void Character::IncreaseHP(double dHP)
{
	if (dHP < 0)//根据角色防御力计算角色受到的伤害
	{
		dHP *= DEF;
	}
	HP += dHP;
	HP = HP>HPmaximum?HPmaximum:HP;//这句话是防止血量超限
	if(HP < 0)//如果角色死亡，则打上删除标记
	{
		Delete();
	}
}

std::vector<BaseObject*> Character::Fire(Position direction)
{
	Engine* eng = Engine::get_Instance();
	ResModule* res = eng->get_mRes();
	Bullet* newobj = dynamic_cast<Bullet*>(res->Load(ENUM_BULLET, bulletType));//dynamic_cast是为了防止强制类型转换出错
	newobj->set_attack(newobj->get_attack() * attackRate);//把人物buff加上
	direction.Standardize();//将方向向量化归为单位向量

	double tempx = 1.1 * direction.x * (radius + newobj->get_radius());//这里的1.1是为了防止射出去的子弹打到自己
	double tempy = 1.1 * direction.y * (radius + newobj->get_radius());
	newobj->SetPos(pos.x+tempx, pos.y+tempy);

	tempx = direction.x * newobj->GetBaseSpeed();//设置子弹速度
	tempy = direction.y * newobj->GetBaseSpeed();
	newobj->SetVelocity(tempx, tempy);

	std::vector<BaseObject*> tmp;
	tmp.push_back((BaseObject*)newobj);
	//*
	int bulletnum = 0;
	if (bulletType == 12)
	{
		bulletnum = 12;
	}
	if (bulletType == 13)
	{
		bulletnum = 6;
	}
	if (bulletType == 14)
	{
		bulletnum = 12;
	}

	for (int i = 1; i <= bulletnum; ++i) {
		//对方向向量进行随机扰动，得到霰弹
		Position delta = { (double)rand() / 32767 / 2 , (double)rand() / 32767 / 2 };
		Position newdirection = direction + delta;
		newdirection.Standardize();
		newobj = dynamic_cast<Bullet*>(res->Load(ENUM_BULLET, bulletType));//dynamic_cast是为了防止强制类型转换出错
		newobj->set_attack(newobj->get_attack() * attackRate);//把人物buff加上

		double tempx = 1.1 * newdirection.x * (radius + newobj->get_radius());//这里的1.1是为了防止射出去的子弹打到自己
		double tempy = 1.1 * newdirection.y * (radius + newobj->get_radius());
		newobj->SetPos(pos.x + tempx, pos.y + tempy);

		tempx = newdirection.x * newobj->GetBaseSpeed();//设置子弹速度
		tempy = newdirection.y * newobj->GetBaseSpeed();
		newobj->SetVelocity(tempx, tempy);

		tmp.push_back((BaseObject*)newobj);
	}
	//*/
	return tmp;
}

bool Player::GetMouseStatus(MouseButton key)
{
	return (MouseState.rgbButtons[key] & 0x80) ? true : false;
}

Position Player::GetMousePosition()
{
	Position p;
	p.x = curPos.x + camera.left;
	p.y = curPos.y + camera.top;
	return p;
}

bool Player::GetKeyboardStatus(char key)
{
	return (KeyState[key] & 0x80) ? true : false;
}

std::vector<BaseObject*> Player::Update(int now, int period)
{
	//获得用户按键信息，并更新速度
	Position direction = Position(0, 0);
	if(GetKeyboardStatus(DIK_S))//CHANGED
	{
		direction.y += 1;
	}
	if(GetKeyboardStatus(DIK_A))
	{
		direction.x -= 1;
	}
	if(GetKeyboardStatus(DIK_W))
	{
		direction.y -= 1;
	}
	if(GetKeyboardStatus(DIK_D))
	{
		direction.x += 1;
	}
	direction.Standardize();
	IncreaseVelocity(baseSpeed*velocityRate*direction.x, baseSpeed*velocityRate*direction.y);
	//至此，该对象的速度已经最终完全确定下来

	POINT p = GetMapSize();
	//更新位置坐标
	IncreasePos(v.x * period, v.y * period);
	if (pos.x < 0 || pos.x > p.x)
		IncreasePos(-v.x * period, 0);
	if (pos.y < 0 || pos.y > p.y)
		IncreasePos(0, -v.y * period);
	//将速度重新置零
	SetVelocity(0, 0);


	//更新血量
	if(lastTime > 0)
	{
		IncreaseHP(HPIncreasement * period);
		lastTime -= period;
	}
	//至此，该对象的血量已经最终完全确定下来

	std::vector<BaseObject*> tmp;
	//当前时间>=上次开火时间+子弹间隔*子弹发射倍率，而且玩家按下鼠标左键，开火！
	if ((now >= lastFireTime + bulletPeriod * bulletPeriodRate) && GetMouseStatus(ENUM_LBUTTON))
	{
		lastFireTime = now;
		tmp = Fire(GetMousePosition() - pos);
	}
	return tmp;
}

std::vector<BaseObject*> Monster::Update(int now, int period)
{
	Engine* eng = Engine::get_Instance();
	Player* player = eng->get_player();

	Position direction = player->GetPos() - GetPos();
	direction.Standardize();
	IncreaseVelocity(baseSpeed*velocityRate*direction.x, baseSpeed*velocityRate*direction.y);
	//至此，该对象的速度已经最终完全确定下来

	//更新位置坐标
	IncreasePos(v.x * period, v.y * period);

	//将速度重新置零
	SetVelocity(0, 0);

	//更新血量
	if(lastTime > 0)
	{
		IncreaseHP(HPIncreasement * period);
		lastTime -= period;
	}
	//至此，该对象的血量已经最终完全确定下来

	std::vector<BaseObject*> tmp;
	//当前时间>=上次开火时间+子弹间隔*子弹发射倍率，开火！
	if (now >= lastFireTime + bulletPeriod * bulletPeriodRate)
	{
		/*ofstream of("log.txt",ios::app);
		of << now << endl;
		of << lastFireTime << endl;
		of << bulletPeriod << endl;
		of << bulletPeriodRate << endl;
		of << lastFireTime + bulletPeriod * bulletPeriodRate << endl;
		of << endl;
		of.close();*/
		lastFireTime = now;

		Engine* eng = Engine::get_Instance();
		Player* player = eng->get_player();
		tmp = Fire(player->GetPos() - pos);
	}

	return tmp;
}

void Item::Use(Character* user)
{
	//硬编码，如果typeId <= 25，那么说明这个道具是一把枪，那么将子弹类型设置为相应的类型
	if (typeId <= 25)
	{
		user->set_bulletType(typeId);
	}
	switch (typeId)//硬编码，根据每个道具具体的效果对user的属性进行修改
	{
	case 0://HK_USP手枪，射速1Hz
		user->set_bulletPeriod(1000/1);
		break;
	case 1://沙漠之鹰,射速1Hz
		user->set_bulletPeriod(1000 / 1);
		break;
	case 2://未来高科技手枪，射速3Hz
		user->set_bulletPeriod(1000 / 3);
		break;
	case 3://UZI，射速18Hz
		user->set_bulletPeriod(1000 / 18);
		break;
	case 4://MP5，射速12Hz
		user->set_bulletPeriod(1000 / 12);
		break;
	case 5://56式冲锋枪，射速6Hz
		user->set_bulletPeriod(1000 / 6);
		break;
	case 6://M16A4，射速8Hz
		user->set_bulletPeriod(1000 / 8);
		break;
	case 7://MG3，射速12Hz
		user->set_bulletPeriod(1000 / 12);
		break;
	case 8://袖珍加特林，射速20Hz
		user->set_bulletPeriod(1000 / 20);
		break;
	case 9://手持M134，射速30Hz
		user->set_bulletPeriod(1000 / 30);
		break;
	case 10://手持1130近防炮，射速100Hz
		user->set_bulletPeriod(1000 / 100);
		break;
	case 11://斯太尔AUG，射速8Hz
		user->set_bulletPeriod(1000 / 8);
		break;
	case 12://S686，射速1Hz
		user->set_bulletPeriod(1000 / 1);
		break;
	case 13://气锤，射速2Hz
		user->set_bulletPeriod(1000 / 2);
		break;
	case 14://霰弹风暴，射速5Hz
		user->set_bulletPeriod(1000 / 5);
		break;
	case 15://高斯狙击枪，射速0.5Hz
		user->set_bulletPeriod(1000 / 0.5);
		break;
	case 16://RPG-7，射速0.5Hz
		user->set_bulletPeriod(1000 / 0.5);
		break;
	case 17://AT4加强版，射速0.5Hz
		user->set_bulletPeriod(1000 / 0.5);
		break;
	case 18://榴弹发射器MGL-140，射速2Hz
		user->set_bulletPeriod(1000 / 2);
		break;
	case 19://三联装战舰主炮，射速2Hz
		user->set_bulletPeriod(1000 / 2);
		break;
	case 20://手持核子发射器，射速0.3Hz
		user->set_bulletPeriod(1000 / 0.3);
		break;
	case 21://喷火器，射速10Hz
		user->set_bulletPeriod(1000 / 10);
		break;
	case 22://冰冻枪，射速10Hz
		user->set_bulletPeriod(1000 / 10);
		break;
	case 23://激光加特林，射速20Hz
		user->set_bulletPeriod(1000 / 20);
		break;
	case 24://四联装电浆火箭炮，射速2.5H
		user->set_bulletPeriod(1000 / 2.5);
		break;
	case 25://手持天基炮，射速20Hz
		user->set_bulletPeriod(1000 / 20);
		break; 
	case 26://垃圾桶里的皮马甲（零级甲）
		user->set_DEF(0.8);
		break;
	case 27://橄榄球防护服（一级甲）
		user->set_DEF(0.6);
		break;
	case 28://专业三防防弹衣（二级甲）
		user->set_DEF(0.4);
		break;
	case 29://动力装甲（三级甲）
		user->set_DEF(0.2);
		break;
	case 30://急救包
		user->IncreaseHP(user->get_HPmaximum()*0.2);
		break;
	case 31://医疗箱
		user->IncreaseHP(user->get_HPmaximum());
		break;
	case 32://子弹倍速
		user->set_bulletPeriodRate(0.5);
		break;
	case 33://子弹伤害
		user->set_attackRate(2);
		break;
	case 34://肾上腺素
		user->set_bulletPeriodRate(0.666);
		user->set_attackRate(1.5);
		user->add_HPIncreasement(0.1);
		user->set_lastTime(10000);
		break;
	case 35://高级红牛
		user->set_velocityRate(2);
		break;
	default:
		break;
	}
}

std::vector<BaseObject*> Bullet::Update(int now, int period)
{
	double dx = v.x * period, dy = v.y * period;
	IncreasePos(dx, dy);
	POINT p = GetMapSize();
	RECT rect = { 0,0, p.x, p.y};
	if (!INSIDE(POINT(pos), rect))
		Delete();
	return vector<BaseObject*>();
}

BaseObject* Event::Occur()
{
	auto mRes = Engine::get_Instance()->get_mRes();
	auto ret = mRes->Load(type, typeId);
	ret->SetPos(pos);
	return ret;
}

void Monster::OnCollision(BaseObject* obj)
{
	struct Position MainPosition;  //主物体坐标
	MainPosition = GetPos();
	double MyVelocity;    //主物体速度
	MyVelocity = GetBaseSpeed();
	double Radius;  //主物体半径
	Radius = GetRadius();

	struct Position SlavePosition;  //从物体坐标
	SlavePosition = obj->GetPos();

	struct Position MyPosition;    //处理向量
	struct Position AimOne;    //相交的坐标1
	struct Position AimTwo;	//相交的坐标2
	struct Position Aim;	//处理坐标
	double Useless;  //处理复杂的变量计算

	double oldHP = HP;

	Position fuckwall;//专门用来记录墙对于人物的速度增量

	switch (obj->get_type())
	{
	case ENUM_MONSTOR:
		MyPosition = MainPosition - SlavePosition;   //主物体坐标为getpos，从物体坐标为pos
		MyPosition.Standardize();
		IncreaseVelocity(MyVelocity * MyPosition.x * velocityRate, MyVelocity * MyPosition.y * velocityRate);  //速度增加函数
		break;
	case ENUM_PLAYER:
		MyPosition = MainPosition - SlavePosition;   //主物体坐标为getpos，从物体坐标为pos
		MyPosition.Standardize();
		IncreaseVelocity(MyVelocity * MyPosition.x * velocityRate, MyVelocity * MyPosition.y * velocityRate);  //速度增加函数
		break;
	case ENUM_WALL:
		double Width, Height;  //从物体宽度、高度
		Width = obj->GetWidth();
		Height = obj->GetHeight();
		if (MainPosition.y<(SlavePosition.y + 0.5 * Height - Radius) && MainPosition.y>(SlavePosition.y - 0.5 * Height + Radius) && MainPosition.x < (SlavePosition.x - 0.5 * Width))
		{
			fuckwall = { -velocityRate * MyVelocity, 0 };
		}
		else if (MainPosition.y<(SlavePosition.y + 0.5 * Height - Radius) && MainPosition.y>(SlavePosition.y - 0.5 * Height + Radius) && MainPosition.x > (SlavePosition.x + 0.5 * Width))
		{
			fuckwall = { velocityRate * MyVelocity, 0 };
		}
		else if (MainPosition.x<(SlavePosition.x + 0.5 * Width - Radius) && MainPosition.x>(SlavePosition.x - 0.5 * Width + Radius) && MainPosition.y < (SlavePosition.y - 0.5 * Height))
		{
			fuckwall = { 0, -velocityRate * MyVelocity };
		}
		else if (MainPosition.x<(SlavePosition.x + 0.5 * Width - Radius) && MainPosition.x>(SlavePosition.x - 0.5 * Width + Radius) && MainPosition.y > (SlavePosition.y + 0.5 * Height))
		{
			fuckwall = { 0, velocityRate * MyVelocity };
		}
		else if (MainPosition.y < (SlavePosition.y - 0.5 * Height + Radius) && MainPosition.x < (SlavePosition.x - 0.5 * Width + Radius))
		{
			AimOne.x = SlavePosition.x - 0.5 * Width;
			Useless = sqrt(Radius * Radius - (AimOne.x - MainPosition.x) * (AimOne.x - MainPosition.x));
			AimOne.y = MainPosition.y + Useless;
			AimTwo.y = SlavePosition.y - 0.5 * Height;
			Useless = sqrt(Radius * Radius - (AimTwo.y - MainPosition.y) * (AimTwo.y - MainPosition.y));
			AimTwo.x = MainPosition.x + Useless;
			Aim = (AimOne + AimTwo) / 2;      //设置为两个碰撞点的中点
			MyPosition = MainPosition - Aim;
			MyPosition.Standardize();
			fuckwall = { velocityRate * MyVelocity * MyPosition.x, velocityRate * MyVelocity * MyPosition.y };
		}
		else if (MainPosition.y<(SlavePosition.y - 0.5 * Height + Radius) && MainPosition.x>(SlavePosition.x + 0.5 * Width - Radius))
		{
			AimOne.x = SlavePosition.x + 0.5 * Width;
			Useless = sqrt(Radius * Radius - (AimOne.x - MainPosition.x) * (AimOne.x - MainPosition.x));
			AimOne.y = MainPosition.y + Useless;
			AimTwo.y = SlavePosition.y - 0.5 * Height;
			Useless = sqrt(Radius * Radius - (AimTwo.y - MainPosition.y) * (AimTwo.y - MainPosition.y));
			AimTwo.x = MainPosition.x - Useless;
			Aim = (AimOne + AimTwo) / 2;      //设置为两个碰撞点的中点
			MyPosition = MainPosition - Aim;
			MyPosition.Standardize();
			fuckwall = { velocityRate * MyVelocity * MyPosition.x, velocityRate * MyVelocity * MyPosition.y };
		}
		else if (MainPosition.y > (SlavePosition.y + 0.5 * Height - Radius) && MainPosition.x < (SlavePosition.x - 0.5 * Width + Radius))
		{
			AimOne.x = SlavePosition.x - 0.5 * Width;
			Useless = sqrt(Radius * Radius - (AimOne.x - MainPosition.x) * (AimOne.x - MainPosition.x));
			AimOne.y = MainPosition.y - Useless;
			AimTwo.y = SlavePosition.y + 0.5 * Height;
			Useless = sqrt(Radius * Radius - (AimTwo.y - MainPosition.y) * (AimTwo.y - MainPosition.y));
			AimTwo.x = MainPosition.x + Useless;
			Aim = (AimOne + AimTwo) / 2;      //设置为两个碰撞点的中点
			MyPosition = MainPosition - Aim;
			MyPosition.Standardize();
			fuckwall = { velocityRate * MyVelocity * MyPosition.x, velocityRate * MyVelocity * MyPosition.y };
		}
		else if (MainPosition.y > (SlavePosition.y + 0.5 * Height - Radius) && MainPosition.x > (SlavePosition.x + 0.5 * Width - Radius))
		{
			AimOne.x = SlavePosition.x + 0.5 * Width;
			Useless = sqrt(Radius * Radius - (AimOne.x - MainPosition.x) * (AimOne.x - MainPosition.x));
			AimOne.y = MainPosition.y - Useless;
			AimTwo.y = SlavePosition.y + 0.5 * Height;
			Useless = sqrt(Radius * Radius - (AimTwo.y - MainPosition.y) * (AimTwo.y - MainPosition.y));
			AimTwo.x = MainPosition.x - Useless;
			Aim = (AimOne + AimTwo) / 2;      //设置为两个碰撞点的中点
			MyPosition = MainPosition - Aim;
			MyPosition.Standardize();
			fuckwall = { velocityRate * MyVelocity * MyPosition.x, velocityRate * MyVelocity * MyPosition.y };
		}
		fuckwall = fuckwall * 4;
		IncreaseVelocity(fuckwall.x, fuckwall.y);
		break;
	case ENUM_BULLET:
		IncreaseHP(-dynamic_cast<Bullet*>(obj)->get_attack() * DEF);
		dynamic_cast<Bullet*>(obj)->set_attack(dynamic_cast<Bullet*>(obj)->get_attack() - oldHP);
		break;
	default:
		break;
	}
}

//TODO:以更加优美的方式实现OnCollision，monster对player的影响要考虑
void Player::OnCollision(BaseObject* obj)
{
	struct Position MainPosition;  //主物体坐标
	MainPosition = GetPos();
	double MyVelocity;    //主物体速度
	MyVelocity = GetBaseSpeed();
	double Radius;  //主物体半径
	Radius = GetRadius();

	struct Position SlavePosition;  //从物体坐标
	SlavePosition = obj->GetPos();

	struct Position MyPosition;    //处理向量
	struct Position AimOne;    //相交的坐标1
	struct Position AimTwo;	//相交的坐标2
	struct Position Aim;	//处理坐标
	double Useless;  //处理复杂的变量计算


	double oldHP = HP;

	Position fuckwall;//专门用来记录墙对于人物的速度增量

	switch (obj->get_type())
	{
	case ENUM_MONSTOR:
		MyPosition = MainPosition - SlavePosition;   //主物体坐标为getpos，从物体坐标为pos
		MyPosition.Standardize();
		IncreaseVelocity(MyVelocity * MyPosition.x * velocityRate, MyVelocity * MyPosition.y * velocityRate);  //速度增加函数
		break;
	case ENUM_PLAYER:
		MyPosition = MainPosition - SlavePosition;   //主物体坐标为getpos，从物体坐标为pos
		MyPosition.Standardize();
		IncreaseVelocity(MyVelocity * MyPosition.x * velocityRate, MyVelocity * MyPosition.y * velocityRate);  //速度增加函数
		break;
	case ENUM_WALL:
		double Width, Height;  //从物体宽度、高度
		Width = obj->GetWidth();
		Height = obj->GetHeight();
		if (MainPosition.y<(SlavePosition.y + 0.5 * Height - Radius) && MainPosition.y>(SlavePosition.y - 0.5 * Height + Radius) && MainPosition.x < (SlavePosition.x - 0.5 * Width))
		{
			fuckwall = { -velocityRate * MyVelocity, 0 };
		}
		else if (MainPosition.y<(SlavePosition.y + 0.5 * Height - Radius) && MainPosition.y>(SlavePosition.y - 0.5 * Height + Radius) && MainPosition.x > (SlavePosition.x + 0.5 * Width))
		{
			fuckwall = { velocityRate * MyVelocity, 0 };
		}
		else if (MainPosition.x<(SlavePosition.x + 0.5 * Width - Radius) && MainPosition.x>(SlavePosition.x - 0.5 * Width + Radius) && MainPosition.y < (SlavePosition.y - 0.5 * Height))
		{
			fuckwall = { 0, -velocityRate * MyVelocity };
		}
		else if (MainPosition.x<(SlavePosition.x + 0.5 * Width - Radius) && MainPosition.x>(SlavePosition.x - 0.5 * Width + Radius) && MainPosition.y > (SlavePosition.y + 0.5 * Height))
		{
			fuckwall = { 0, velocityRate * MyVelocity };
		}
		else if (MainPosition.y < (SlavePosition.y - 0.5 * Height + Radius) && MainPosition.x < (SlavePosition.x - 0.5 * Width + Radius))
		{
			AimOne.x = SlavePosition.x - 0.5 * Width;
			Useless = sqrt(Radius * Radius - (AimOne.x - MainPosition.x) * (AimOne.x - MainPosition.x));
			AimOne.y = MainPosition.y + Useless;
			AimTwo.y = SlavePosition.y - 0.5 * Height;
			Useless = sqrt(Radius * Radius - (AimTwo.y - MainPosition.y) * (AimTwo.y - MainPosition.y));
			AimTwo.x = MainPosition.x + Useless;
			Aim = (AimOne + AimTwo) / 2;      //设置为两个碰撞点的中点
			MyPosition = MainPosition - Aim;
			MyPosition.Standardize();
			fuckwall = { velocityRate * MyVelocity * MyPosition.x, velocityRate * MyVelocity * MyPosition.y };
		}
		else if (MainPosition.y<(SlavePosition.y - 0.5 * Height + Radius) && MainPosition.x>(SlavePosition.x + 0.5 * Width - Radius))
		{
			AimOne.x = SlavePosition.x + 0.5 * Width;
			Useless = sqrt(Radius * Radius - (AimOne.x - MainPosition.x) * (AimOne.x - MainPosition.x));
			AimOne.y = MainPosition.y + Useless;
			AimTwo.y = SlavePosition.y - 0.5 * Height;
			Useless = sqrt(Radius * Radius - (AimTwo.y - MainPosition.y) * (AimTwo.y - MainPosition.y));
			AimTwo.x = MainPosition.x - Useless;
			Aim = (AimOne + AimTwo) / 2;      //设置为两个碰撞点的中点
			MyPosition = MainPosition - Aim;
			MyPosition.Standardize();
			fuckwall = { velocityRate * MyVelocity * MyPosition.x, velocityRate * MyVelocity * MyPosition.y };
		}
		else if (MainPosition.y > (SlavePosition.y + 0.5 * Height - Radius) && MainPosition.x < (SlavePosition.x - 0.5 * Width + Radius))
		{
			AimOne.x = SlavePosition.x - 0.5 * Width;
			Useless = sqrt(Radius * Radius - (AimOne.x - MainPosition.x) * (AimOne.x - MainPosition.x));
			AimOne.y = MainPosition.y - Useless;
			AimTwo.y = SlavePosition.y + 0.5 * Height;
			Useless = sqrt(Radius * Radius - (AimTwo.y - MainPosition.y) * (AimTwo.y - MainPosition.y));
			AimTwo.x = MainPosition.x + Useless;
			Aim = (AimOne + AimTwo) / 2;      //设置为两个碰撞点的中点
			MyPosition = MainPosition - Aim;
			MyPosition.Standardize();
			fuckwall = { velocityRate * MyVelocity * MyPosition.x, velocityRate * MyVelocity * MyPosition.y };
		}
		else if (MainPosition.y > (SlavePosition.y + 0.5 * Height - Radius) && MainPosition.x > (SlavePosition.x + 0.5 * Width - Radius))
		{
			AimOne.x = SlavePosition.x + 0.5 * Width;
			Useless = sqrt(Radius * Radius - (AimOne.x - MainPosition.x) * (AimOne.x - MainPosition.x));
			AimOne.y = MainPosition.y - Useless;
			AimTwo.y = SlavePosition.y + 0.5 * Height;
			Useless = sqrt(Radius * Radius - (AimTwo.y - MainPosition.y) * (AimTwo.y - MainPosition.y));
			AimTwo.x = MainPosition.x - Useless;
			Aim = (AimOne + AimTwo) / 2;      //设置为两个碰撞点的中点
			MyPosition = MainPosition - Aim;
			MyPosition.Standardize();
			fuckwall = { velocityRate * MyVelocity * MyPosition.x, velocityRate * MyVelocity * MyPosition.y };
		}
		fuckwall = fuckwall * 4;
		IncreaseVelocity(fuckwall.x, fuckwall.y);
		break;
	case ENUM_BULLET:
		IncreaseHP(-dynamic_cast<Bullet*>(obj)->get_attack() * DEF);
		dynamic_cast<Bullet*>(obj)->set_attack(dynamic_cast<Bullet*>(obj)->get_attack() - oldHP);
		break;
	default:
		break;
	}
}

std::vector<BaseObject*> Item::Update(int now, int period)
{
	return std::vector<BaseObject*>();
}

void Item::OnCollision(BaseObject* obj)
{
	Character* user = NULL;
	switch (obj->get_type())
	{
	case ENUM_PLAYER:
		user = (Character*)obj;
		Use(user);
		Delete();
		break;
	default:
		break;
	}
}

void Bullet::OnCollision(BaseObject* obj)
{
	switch (obj->get_type())
	{
	case ENUM_PLAYER:
		Delete();
		break;
	case ENUM_WALL:
		Delete();
		break;
	case ENUM_MONSTOR:
		if (typeId >= 15 && attack > 0) {
			
		}
		else {
			Delete();
		}
		break;
	default:
		break;
	}
}
