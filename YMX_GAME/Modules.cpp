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


vector<Event> ResModule::LoadMap(ObjType type, int typeId)
{
	// 256 * 256
	fstream file;
	Pos pos;
	Event event;
	vector<Event> ret;

	file.open("Res\\Map\\Round0\\Walls.txt", ios::in);
	while(file >> pos)
		ret.push_back(Event(pos, 0));
	file.close();

	file.open("Res\\Map\\Round0\\Monsters.txt", ios::in);
	while(file >> event)
		ret.push_back(event);
	return ret;
}

void ResModule::Load(BaseObject *obj, ObjType type, int typeId)
{
	ifstream f;
	string dir("Res\\");
	vector<string> ret;
	switch (type)
	{
	case ENUM_MONSTOR:
		ret = listFiles(dir += "Monster\\");
		for(auto fileName: ret)
		{
			f.open(dir + fileName);
			f >> 
		}
		break;
	case ENUM_PLAYER:
		break;
	case ENUM_BULLET:
		break;

	case ENUM_WALL:
	case ENUM_MAP:
	default:
		break;
	}
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
		//延时
		std::this_thread::sleep_for(std::chrono::milliseconds(Setting::period));
		while (heap.top().occurTime == now)
		{
			objList.push_back(heap.top().Occur());
			heap.pop();
		}

		CollisionDetect();

		//显示地图背景
		//mRender.show_img(wall, background)

		//遍历更新物体并显示
		for (auto iter = objList.begin(); iter != objList.end();)
		{
			auto cur = iter;
			++iter;
			(*cur)->Update(now, Setting::period);
		}
		for(auto cur:objList)
			mRender->ShowIamge(cur);
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


