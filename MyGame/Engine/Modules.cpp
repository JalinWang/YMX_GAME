#include "Modules.h"

#include <queue>
#include <string>
#include <vector>
#include <thread>
#include <chrono>
#include <functional>
#include <iostream>
#include <fstream>
#include "define_classes.h"
#include "Modules.h"
#include <io.h>

#include "Engine.h"

#include "../DXSupports.h"

using namespace std;
using namespace inifile;

//using nm_GameEngine::ResModule;
//using nm_GameEngine::RenderModule;
//using nm_GameEngine::SceneModule;

//TODO: 改改改！！！！！！！！！！！！！一点都不通用
vector<Event> ResModule::LoadRound(const string & round)
{
	// 256 * 256
	fstream file;
	Event event;
	vector<Event> ret;

	file.open("Res\\Map\\" + round + "\\Walls.txt", ios::in);
	Position pos;
	int type;
	while(file >> pos >> type)
		ret.push_back(Event(pos, 0, ENUM_WALL, type));
	file.close();

	file.open("Res\\Map\\" + round + "\\Monsters.txt", ios::in);
	while(file >> event)
	{
		event.set_type(ENUM_MONSTOR);
		//event.set_typeId(0);
		ret.push_back(event);
	}
	file.close();

	file.open("Res\\Map\\" + round + "\\Items.txt", ios::in);
	while (file >> event)
	{
		event.set_type(ENUM_ITEM);
		//event.set_typeId(0);
		ret.push_back(event);
	}
	file.close();
	return ret;
}

bool ResModule::LoadBaseObject(BaseObject *p, const string &typeIdStr)
{
	double dTemp;
	int iTemp;

	ini.GetDoubleValue(typeIdStr, "baseSpeed", &dTemp);
	p->set_baseSpeed(dTemp);

	ini.GetIntValue(typeIdStr, "shapeType", &iTemp);
	p->set_shapeType(iTemp);

	if (iTemp == 0)
	{
		ini.GetDoubleValue(typeIdStr, "radius", &dTemp);
		p->set_radius(dTemp);
	}
	else if (iTemp == 1)
	{
		ini.GetDoubleValue(typeIdStr, "width", &dTemp);
		p->set_width(dTemp);

		ini.GetDoubleValue(typeIdStr, "height", &dTemp);
		p->set_height(dTemp);
	}
	return true;
}

bool ResModule::LoadCharacter(Character *p, const string &typeIdStr)
{
	double dTemp;
	int iTemp;

	ini.GetIntValue(typeIdStr, "bulletType", &iTemp);
	p->set_bulletType(iTemp);

	ini.GetIntValue(typeIdStr, "bulletPeriod", &iTemp);
	p->set_bulletPeriod(iTemp);

	ini.GetDoubleValue(typeIdStr, "HPmaximum", &dTemp);
	p->set_HPmaximum(dTemp);
	p->set_HP(dTemp);

	LoadBaseObject(p, typeIdStr);
	return true;
}

bool ResModule::LoadMonster(Monster *p, const string &typeIdStr)
{
	LoadCharacter(p, typeIdStr);
	return true;
}

bool ResModule::LoadPlayer(Player *p, const string &typeIdStr)
{
	LoadCharacter(p, typeIdStr);
	return true;
}

bool ResModule::LoadBullet(Bullet *p, const string &typeIdStr)
{
	double dTemp;
	ini.GetDoubleValue(typeIdStr, "attack", &dTemp);
	p->set_attack(dTemp);
	LoadBaseObject(p, typeIdStr);
	return true;
}

bool ResModule::LoadWall(Wall *p, const string &typeIdStr)
{
	LoadBaseObject(p, typeIdStr);

	return true;
}

BaseObject *ResModule::Load(ObjType type, int typeId)
{
	BaseObject* obj = nullptr;
	switch (type)
	{
	case ENUM_MONSTOR:
		obj = new Monster;
		*dynamic_cast<Monster *>(obj) = monsters[typeId];
		break;
	case ENUM_BULLET:
		obj = new Bullet;
		*dynamic_cast<Bullet*>(obj) = bullets[typeId];
		break;
	case ENUM_PLAYER:
		obj = new Player;
		*dynamic_cast<Player*>(obj) = players[typeId];
		break;
	case ENUM_WALL:
		obj = new Wall;
		*dynamic_cast<Wall*>(obj) = walls[typeId];
		break;
	case ENUM_ITEM:
		obj = new Item;
		break;
	case ENUM_MAP:
	default:
		break;
	}
	obj->set_type(type);
	obj->set_typeId(typeId);
	obj->set_uniqueId(Engine::get_Instance()->cnt++);
	return obj;
}

void ResModule::Init()
{
	const string dir("Res\\");
	vector<string> sections;

	//有默认段，即 无名段"" ！！！！！！！

	ini.Load(dir + "Monsters.ini");
	ini.GetSections(&sections);
	for(auto section:sections)
	{
		if (section == "")
			continue;
		Monster t;
		LoadMonster(&t, section);
		monsters.push_back(t);
	}
	sections.clear();

	ini.Load(dir + "Players.ini");
	ini.GetSections(&sections);
	for(auto section:sections)
	{
		if (section == "")
			continue;
		Player t;
		LoadPlayer(&t, section);
		players.push_back(t);
	}
	sections.clear();

	ini.Load(dir + "Bullets.ini");
	ini.GetSections(&sections);
	for(auto section:sections)
	{
		if (section == "")
			continue;
		Bullet t;
		LoadBullet(&t, section);
		bullets.push_back(t);
	}
	sections.clear();

	ini.Load(dir + "Walls.ini");
	ini.GetSections(&sections);
	for(auto section:sections)
	{
		if (section == "")
			continue;
		Wall t;
		LoadWall(&t, section);
		walls.push_back(t);
	}
	sections.clear();
}

//ResModule* ResModule::getInstance()
//{
//	if (instance == nullptr)
//		instance = new ResModule();
//	return instance;
//}

void RenderModule::ShowIamge(BaseObject* obj)
{
	int type_0 = 0;
	switch (obj->get_type())
	{
	case ENUM_MONSTOR:
		type_0 = TYPE_MONSTER;
		break;
	case ENUM_BULLET:
		type_0 = TYPE_BULLET;
		break;
	case ENUM_PLAYER:
		type_0 = TYPE_PLAYER;
		break;
	case ENUM_WALL:
		type_0 = TYPE_WALL;
		break;
	case ENUM_MAP:
		type_0 = TYPE_MAP;
		break;
	case ENUM_ITEM:
		type_0 = TYPE_OBJECT;
		break;
	default:
		break;
	}

	//int type_1 = obj->get_type() == ENUM_PLAYER ? dynamic_cast<Player*>(obj)->get_HP() / dynamic_cast<Player*>(obj)->get_HPmaximum() * 600 : obj->get_typeId();
	int type_1 = obj->get_typeId();
	POINT pos = POINT(obj->GetPos());
	double Height = obj->GetShapeType() == 0 ? 2 * obj->GetRadius() : obj->GetHeight();
	double Width = obj->GetShapeType() == 0 ? 2 * obj->GetRadius() : obj->GetWidth();
	int cnt = Engine::get_Instance()->frameCnt;

	if (type_0 == TYPE_OBJECT)
	{
		if (type_1 <= 25)
		{
			Height = 60;
			Width = 120;
		}
	}

	if (type_0 == TYPE_BULLET)
	{
		switch (type_1)
		{
		case 0:
			Height = 50;
			Width = 6;
			break;
		case 1:
			Height = 60;
			Width = 8;
			break;
		case 2:
			Height = 50;
			Width = 6;
			break;
		case 3:
			Height = 50;
			Width = 6;
			break;
		case 4:
			Height = 50;
			Width = 6;
			break;
		case 5:
			Height = 50;
			Width = 6;
			break;
		case 6:
			Height = 50;
			Width = 6;
			break;
		case 7:
			Height = 60;
			Width = 8;
			break;
		case 8:
			Height = 60;
			Width = 8;
			break;
		case 9:
			Height = 60;
			Width = 8;
			break;
		case 10:
			Height = 60;
			Width = 8;
			break;
		case 11:
			Height = 50;
			Width = 6;
			break;
		case 12:
			Height = 40;
			Width = 4;
			break;
		case 13:
			Height = 40;
			Width = 4;
			break;
		case 14:
			Height = 40;
			Width = 4;
			break;
		case 15:
			Height = 1000;
			Width = 8;
			break;
		case 16:
			Height = 200;
			Width = 30;
			break;
		case 17:
			Height = 200;
			Width = 30;
			break;
		case 18:
			Height = 150;
			Width = 20;
			break;
		case 19:
			Height = 300;
			Width = 40;
			break;
		case 20:
			Height = 500;
			Width = 100;
			break;
		case 21:
			Height = 150;
			Width = 40;
			break;
		case 22:
			Height = 150;
			Width = 40;
			break;
		case 23:
			Height = 50;
			Width = 6;
			break;
		case 24:
			Height = 80;
			Width = 300;
			break;
		case 25:
			Height = 1000;
			Width = 100;
			break;
		default:
			break;
		}
	}

	DrawObject(type_0, type_1, pos, Height, Width, cnt);
}

//RenderModule* RenderModule::getInstance()
//{
//	if (instance == nullptr)
//		instance = new RenderModule();
//	return instance;
//}
//
//SceneModule::SceneModule(ResModule* res, RenderModule* render):
//	mRes(res), mRender(render)
//{
//}
void SceneModule::Release()
{
	for (auto i : objList)
	{
		if(i->get_type() != ENUM_PLAYER)
			delete i;
	}
	objList.clear();
}

void SceneModule::Init()
{
	Release();
	Map *m = new Map();
	m->set_type(ENUM_MAP);
	m->set_typeId(0);
	m->set_uniqueId(Engine::get_Instance()->cnt++);
	objList.push_back(m);

	Engine* eng = Engine::get_Instance();

	objList.push_back(eng->get_player());
	fstream file;
	file.open("Res\\map\\current_round.txt", ios::in);
	string cur;
	file >> cur;
	file.close();

	vector<Event> eventsList = (eng->get_mRes())->LoadRound(cur);
	for (auto i : eventsList)
		heap.push(i);
}

int SceneModule::Run()
{
	auto engine = Engine::get_Instance();
	int now = engine->NowTime();

	++engine->frameCnt;
	while (!heap.empty() && heap.top().occurTime <= now)
	{
		Event t = heap.top();
		AddObj(t.Occur());
		heap.pop();
	}

	CollisionDetect();

	//显示地图背景
	//mRender.show_img(wall, background)

	//遍历更新物体并显示
	vector<BaseObject*> toAdd;
	for (auto iter = objList.begin(); iter != objList.end();)
	{
		auto cur = iter;
		++iter;

		if ((*cur)->IsDeleted())
		{
			if ((*cur)->get_type() == ENUM_PLAYER)
			{
				//terminate();
				return 0;
			}
			aabbTree.Remove((*cur)->get_AABB());
			delete (*cur);
			objList.erase(cur);
			continue;
		}
		auto t = (*cur)->Update(now, Setting::period);
		(*cur)->UpdateAABB();
		toAdd.insert(toAdd.end(), t.begin(), t.end());
	}
	aabbTree.AABBTreeUpdate();

	for (auto obj : toAdd)
		AddObj(obj);

	SetCamera(POINT(engine->get_player()->GetPos()));
	monCnt = 0;
	for (auto cur : objList)
	{
		(Engine::get_Instance())->get_mRender()->ShowIamge(cur);
		monCnt += (cur->get_type() == ENUM_MONSTOR);
	}
	//show on the top
	DrawHPBar((int)engine->get_player()->get_HPmaximum(), (int)engine->get_player()->get_HP());
	if (!monCnt)
		return 0;
	else
		return 1;
}

void SceneModule::AddObj(BaseObject* obj)
{
	objList.push_back(obj);

	obj->UpdateAABB();
	switch (obj->GetType())
	{
	case ENUM_MAP:
		break;
	case ENUM_WALL:
	case ENUM_ITEM:
	case ENUM_PLAYER:
	case ENUM_MONSTOR:
	case ENUM_BULLET:
		aabbTree.Add(obj->get_AABB());
		break;
	}
}

void SceneModule::CollisionDetect()
{
#define AABB
#ifdef AABB
	for (auto i = objList.begin(); i != objList.end();++i)
	{
		for(auto j = objList.begin(); j != objList.end();++j)
		{
			if(MayCollide(*i, *j) && Detect(*i, *j))//两个函数的先后顺序不可调换，利用C++语言的短路逻辑可以减小时间复杂度
			{
				(*i)->OnCollision(*j);
			}
		}
	}
#endif
#ifndef AABB
	auto t = aabbTree.ComputePairs();
	for (auto p : t)
	{
		auto a = static_cast<BaseObject*>(p.first->pObj);
		auto b = static_cast<BaseObject*>(p.second->pObj);

		if (MayCollide(a, b))
			a->OnCollision(b);
		if (MayCollide(b, a))
			b->OnCollision(a);
	}
#endif
}


bool SceneModule::MayCollide(BaseObject* i, BaseObject* j)//判断j对i是否可能有作用
{
	if(i == j)
	{
		return 0;
	}

	switch (i->GetType())
	{
	case ENUM_MAP:
		return 0;
	case ENUM_WALL:
		return 0;
	case ENUM_PLAYER:
		return j->GetType()!=ENUM_MAP;
	case ENUM_MONSTOR:
		return j->GetType()!=ENUM_MAP;
	case ENUM_ITEM:
		return j->GetType()!=ENUM_MAP && j->GetType()!=ENUM_WALL && j->GetType()!=ENUM_ITEM && j->GetType()!=ENUM_BULLET;
	case ENUM_BULLET:
		return j->GetType()!=ENUM_MAP && j->GetType()!=ENUM_ITEM;
	default://return 0是因为日后便于出bug，从而便于查错
		return 0;
	}

	return 0;
}

bool SceneModule::Detect(BaseObject* i, BaseObject* j)//碰撞检测
{
	if(i->GetShapeType() == 0 && j->GetShapeType() == 0)
	{
		Position distance = i->GetPos() - j->GetPos();
		return distance.Norm() < i->GetRadius() + j->GetRadius();
	}

	if(i->GetShapeType() == 1 && j->GetShapeType() == 1)
	{
		Position distance = i->GetPos() - j->GetPos();
		if(abs(distance.x) > i->GetWidth() + j->GetWidth()/2.0)
			return 0;
		if(abs(distance.y) > i->GetHeight() + j->GetHeight()/2.0)
			return 0;
		return 1;
	}

	if(i->GetShapeType() == 1)
	{
		std::swap(i, j);
	}

	Position distance = i->GetPos() - j->GetPos();
	if(abs(distance.x) > (i->GetRadius() + j->GetWidth())/2.0)
		return 0;
	if(abs(distance.y) > (i->GetRadius() + j->GetHeight())/2.0)
		return 0;
	return 1;
}

int SceneModule::GetMonCnt()
{
	return monCnt;
}
