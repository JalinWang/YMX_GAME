#pragma once
#include <queue>
#include <string>
#include <vector>
#include <thread>
#include <chrono>
#include <functional>
#include <iostream>
#include "define_classes.h"
//#include "Engine.h"
#include "Setting.h"

using namespace std;

/*namespace nm_GameEngine {
	class ResModule mRes;
	class RenderModule mRender;
	class SceneModule mScene;
}*/

class ResModule {
public:
	vector<Event> LoadMap(ObjType type, int typeId);//加载地图事件
	void Load(BaseObject& obj, ObjType type, int typeId);//加载属性、贴图等
	void Init();//初始化，检查文件存在与否

	//Singleton
public:
	ResModule* getInstance();
private:
	ResModule* instance;
};

class RenderModule {
public:
	void ShowIamge(BaseObject* obj);

	//Singleton
public:
	RenderModule* getInstance();
private:
	RenderModule* instance;
};

class SceneModule {
public:
	vector<BaseObject*> objList;
	priority_queue<Event> heap;
	SceneModule();
	void Init();
	void Run();
	void CollisionDetect();
	bool Detect(BaseObject * i, BaseObject * j);//  判断
private:
	ResModule *mRes;
	RenderModule *mRender;

	//Singleton
public:
	SceneModule* getInstance();
private:
	SceneModule* instance;
};
