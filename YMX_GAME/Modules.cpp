#include "stdafx.h"
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

using namespace std;
using namespace inifile;

//using nm_GameEngine::ResModule;
//using nm_GameEngine::RenderModule;
//using nm_GameEngine::SceneModule;

vector<string> listFiles(string dir)
{
	vector<string> ret;
	intptr_t handle;
	_finddata_t findData;

	handle = _findfirst(dir.c_str, &findData);    // 查找目录中的第一个文件
	if (handle == -1)
	{
		cout << "Failed to find first file!\n";
		return;
	}

	do
	{
		if (findData.attrib & _A_SUBDIR
			&& strcmp(findData.name, ".") == 0
			&& strcmp(findData.name, "..") == 0
			)    // 是否是子目录并且不为"."或".."
			cout << findData.name << "\t<dir>\n";
		else
		{
			cout << findData.name << "\t" << findData.size << endl;
			ret.push_back(string(findData.name));
		}
	} while (_findnext(handle, &findData) == 0);    // 查找目录中的下一个文件

	cout << "Done!\n";
	_findclose(handle);    // 关闭搜索句柄
}

//TODO: 改改改！！！！！！！！！！！！！一点都不通用
vector<Event> ResModule::LoadMap(ObjType type, int typeId)
{
	// 256 * 256
	fstream file;
	Position pos;
	Event event;
	vector<Event> ret;

	file.open("Res\\Map\\Round0\\Walls.txt", ios::in);
	while(file >> pos)
		ret.push_back(Event(pos, 0, ENUM_WALL, 0));
	file.close();

	file.open("Res\\Map\\Round0\\Monsters.txt", ios::in);
	while(file >> event)
	{
		event.set_type(ENUM_MONSTOR);
		//event.set_typeId(0);
		ret.push_back(event);
	}
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

	ini.GetDoubleValue(typeIdStr, "radius", &dTemp);
	p->set_radius(dTemp);
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

	LoadBaseObject(p, typeIdStr);
}

bool ResModule::LoadMonster(Monster *p, const string &typeIdStr)
{
	LoadCharacter(p, typeIdStr);
}

bool ResModule::LoadPlayer(Player *p, const string &typeIdStr)
{
	LoadCharacter(p, typeIdStr);
}

bool ResModule::LoadBullet(Bullet *p, const string &typeIdStr)
{
	double dTemp;
	ini.GetDoubleValue(typeIdStr, "HPmaximum", &dTemp);
	p->set_attack(dTemp);
	LoadBaseObject(p, typeIdStr);
}

bool ResModule::LoadWall(Wall *p, const string &typeIdStr)
{
	LoadBaseObject(p, typeIdStr);
}

BaseObject *ResModule::Load(ObjType type, int typeId)
{
	BaseObject *obj;
	switch (type)
	{
	case ENUM_MONSTOR:
		obj = &monsters[typeId];
		break;
	case ENUM_BULLET:
		obj = &bullets[typeId];
		break;
	case ENUM_PLAYER:
		obj = &players[typeId];
		break;
	case ENUM_WALL:
		obj = &walls[typeId];
		break;
	case ENUM_MAP:
	case ENUM_ITEM:
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

	ini.Load(dir + "Monsters.ini");
	ini.GetSections(&sections);
	for(auto section:sections)
	{
		Monster t;
		LoadMonster(&t, section);
		monsters.push_back(t);
	}
	sections.clear();

	ini.Load(dir + "Players.ini");
	ini.GetSections(&sections);
	for(auto section:sections)
	{
		Player t;
		LoadPlayer(&t, section);
		players.push_back(t);
	}
	sections.clear();

	ini.Load(dir + "Bullets.ini");
	ini.GetSections(&sections);
	for(auto section:sections)
	{
		Bullet t;
		LoadBullet(&t, section);
		bullets.push_back(t);
	}
	sections.clear();

	ini.Load(dir + "Walls.ini");
	ini.GetSections(&sections);
	for(auto section:sections)
	{
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

inline void SceneModule::Init()
{
	Engine *eng = Engine::get_Instance();
	vector<Event> eventsList = (eng->get_mRes)->LoadMap(ENUM_MAP, Setting::ROUND_0);
	for (auto i : eventsList)
		heap.push(i);
}

void SceneModule::Run()
{
	while (true)
	{
		//延时
		std::this_thread::sleep_for(std::chrono::milliseconds(Setting::period));
		while (heap.top().occurTime <= now)
		{
			objList.push_back(heap.top().Occur());
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

			if((*cur)->IsDeleted())
			{
				objList.erase(cur);
				continue;
			}
			auto t = (*cur)->Update(now, Setting::period);
			toAdd.insert(toAdd.end(), t.begin(), t.end());
		}

		objList.insert(objList.end(), toAdd.begin(), toAdd.end());

		for(auto cur:objList)
			(Engine::get_Instance())->get_mRender()->ShowIamge(cur);
	}
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
	if(i->GetType() == 0 && j->GetType() == 0)
	{
		Position distance = i->GetPos() - j->GetPos();
		return distance.Norm < i->GetRadius() + j->GetRadius();
	}

	if(i->GetType() == 1 && j->GetType() == 1)
	{
		Position distance = i->GetPos() - j->GetPos();
		if(abs(distance.x) > (i->GetWidth() + j->GetWidth())/2.0)
			return 0;
		if(abs(distance.y) > (i->GetHeight() + j->GetHeight())/2.0)
			return 0;
		return 1;
	}

	if(i->GetType() == 1)
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

void SceneModule::CollisionDetect()
{
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
}
