#include "define_classes.h"
#include "Engine.h"

Position BaseObject::GetPos()
{
	return pos;
}

void BaseObject::SetPos(double x,double y)
{
	pos.x = x;
	pos.y = y;
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

void Character::IncreaseHP(double dHP)
{
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

	direction.Standardize();//将方向向量化归为单位向量

	double tempx = 1.1 * direction.x * radius;//这里的1.1是为了防止射出去的子弹打到自己
	double tempy = 1.1 * direction.y * radius;
	newobj->SetPos(pos.x+tempx, pos.y+tempy);

	tempx = direction.x * newobj->GetBaseSpeed();//设置子弹速度
	tempy = direction.y * newobj->GetBaseSpeed();
	newobj->SetVelocity(tempx, tempy);

	std::vector<BaseObject*> tmp;
	tmp.push_back((BaseObject*)newobj);

	return tmp;
}

std::vector<BaseObject*> Player::Update(int now, int period)
{
	//获得用户按键信息，并更新速度
	Position direction = Position(0, 0);
	if(GetKeyboardStatus('W'))
	{
		direction.y += 1;
	}
	if(GetKeyboardStatus('A'))
	{
		direction.x -= 1;
	}
	if(GetKeyboardStatus('S'))
	{
		direction.y -= 1;
	}
	if(GetKeyboardStatus('D'))
	{
		direction.x += 1;
	}
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
		IncreaseHP(HPIncrement * period);
		lastTime -= period;
	}
	//至此，该对象的血量已经最终完全确定下来

	std::vector<BaseObject*> tmp;
	//当前时间>=上次开火时间+子弹间隔*子弹发射倍率，而且玩家按下鼠标左键，开火！
	if ((now >= lastFireTime + bulletPeriod * bulletPeriodRate) && GetMouseStatus(ENUM_LBUTTON))
	{
		lastFireTime += bulletPeriod * bulletPeriodRate;
		tmp = Fire(GetMousePosition()-pos);
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
		IncreaseHP(HPIncrement * period);
		lastTime -= period;
	}
	//至此，该对象的血量已经最终完全确定下来

	std::vector<BaseObject*> tmp;
	//当前时间>=上次开火时间+子弹间隔*子弹发射倍率，开火！
	if (now >= lastFireTime + bulletPeriod * bulletPeriodRate)
	{
		lastFireTime += bulletPeriod * bulletPeriodRate;

		Engine* eng = Engine::get_Instance();
		Player* player = eng->get_player();
		tmp = Fire(player->GetPos()-pos);
	}

	return tmp;
}

void Item::Use(Character* user)
{
	switch (typeId)//硬编码，根据每个道具具体的效果对user的属性进行修改
	{
	case 0:
		break;
	case 1:
		break;
	default:
		break;
	}
}

double Bullet::GetAttack()
{
	return attack;
}

std::vector<BaseObject*> Bullet::Update(int now, int period)
{
	int dx = v.x * period, dy = v.y * period;
	IncreasePos(dx, dy);
}