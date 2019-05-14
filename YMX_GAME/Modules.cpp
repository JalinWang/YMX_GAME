#include "stdafx.h"
#include "Modules.h"

#include <queue>
#include <string>
#include <vector>
#include <thread>
#include <chrono>
#include <functional>
#include <iostream>
#include "define_classes.h"
#include "Modules.h"

using namespace std;

//using nm_GameEngine::ResModule;
//using nm_GameEngine::RenderModule;
//using nm_GameEngine::SceneModule;


vector<Event> ResModule::LoadMap(ObjType type, int typeId)
{
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


