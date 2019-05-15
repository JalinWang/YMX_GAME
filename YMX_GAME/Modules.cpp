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

using namespace std;

//using nm_GameEngine::ResModule;
//using nm_GameEngine::RenderModule;
//using nm_GameEngine::SceneModule;

class Pos{
public:
	int x, y;
	friend inline istream & operator << (istream & f, Pos &p){
	    f << p.x << " " << p.y;
	    return p;
	}

	friend inline ostream & operator >> (ostream & f, Pos &p){
	    f >> p.x >> p.y;
	    return f;
	}
};

//map Event
class Event{
public:
	int occurTime; //����ʱ��
	int typeId;
	ObjType type;
	Pos pos;
	Event(){}
	Event(Pos p, int t):pos(p), occurTime(t){}
	bool operator < (const Event &t)
	{
		return occurTime < t.occurTime;
	}
	fstream& operator >> (fstream& f) {
		f >> occurTime >> pos.x >> pos.y >> typeId;
		type = ENUM_MONSTOR;
		return f;
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


vector<Event> ResModule::LoadMap(ObjType type, int typeId)
{
	// 256 * 256
	fstream file("Res\\Map\\Round0\\Walls.txt", ios::in);
	Pos pos;
	Event eve;
	vector<Event> ret;
	while(file >> pos)
		ret.push_back(Event(pos, 0));
	file.close();
	file.open("Res\\Map\\Round0\\Monsters.txt", ios::in);

	while(file >> eve)

	return vector<Event>();
}

void ResModule::Load(BaseObject& obj, ObjType type, int typeId)
{
}

void ResModule::Init()
{
}

ResModule* ResModule::getInstance()
{
	if (instance == nullptr)
		instance = new ResModule();
	return instance;
}

void RenderModule::ShowIamge(BaseObject* obj)
{
}

RenderModule* RenderModule::getInstance()
{
	if (instance == nullptr)
		instance = new RenderModule();
	return instance;
}

SceneModule::SceneModule(ResModule* res, RenderModule* render):
	mRes(res), mRender(render)
{
}

inline void SceneModule::Init()
{
	vector<Event> eventsList = mRes->LoadMap(ENUM_MAP, Setting::ROUND_0);
	for (auto i : eventsList)
		heap.push(i);
}

void SceneModule::Run()
{
	while (true)
	{
		//��ʱ
		std::this_thread::sleep_for(std::chrono::milliseconds(Setting::period));
		while (heap.top().occurTime == now)
		{
			objList.push_back(heap.top().Occur());
			heap.pop();
		}

		CollisionDetect();

		//��ʾ��ͼ����
		//mRender.show_img(wall, background)

		//�����������岢��ʾ
		for (auto obj : objList)
		{
			obj->Update();
			mRender->ShowIamge(obj);
		}
	}
}
void SceneModule::CollisionDetect()
{
	for (auto i : objList)
		for (auto j : objList)
			if (Detect(i, j))
			{
				i->OnCollision(j);
				j->OnCollision(i);
			}
}

bool SceneModule::Detect(BaseObject* i, BaseObject* j)
{
	return false;
}
/*
SceneModule* SceneModule::getInstance()
{
if (instance == nullptr)
instance = new SceneModule();
return instance;
}*/


