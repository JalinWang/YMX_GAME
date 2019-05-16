#pragma once
#include <queue>
#include <string>
#include <vector>
#include <list>
#include <thread>
#include <chrono>
#include <functional>
#include <iostream>
#include "define_classes.h"
#include "Engine.h"
#include "Setting.h"

#include "inifile.h"

using namespace std;
using namespace inifile;

/*namespace nm_GameEngine {
class ResModule mRes;
class RenderModule mRender;
class SceneModule mScene;
}*/

class ResModule
{
public:
	vector<Event> LoadMap(ObjType type, int typeId);//加载地图事件
	BaseObject * Load(ObjType type, int typeId);//生成对象
	void Init();//初始化，预加载所有类型的对象
private:
	vector<Monster> monsters;
	vector<Bullet> bullets;
	vector<Player> players;
	vector<Wall> walls;
	//vector<Item> items;

	bool ResModule::LoadBaseObject(BaseObject *p, const string &typeIdStr);
	bool ResModule::LoadCharacter(Character *p, const string &typeIdStr);
	bool ResModule::LoadMonster(Monster *p, const string &typeIdStr);
	bool ResModule::LoadPlayer(Player *p, const string &typeIdStr);
	bool ResModule::LoadBullet(Bullet *p, const string &typeIdStr);
	bool ResModule::LoadWall(Wall *p, const string &typeIdStr);
	IniFile ini;

//	//Singleton
//public:
//	ResModule* getInstance();
//private:
//	ResModule* instance;
};

class RenderModule
{
public:
	void ShowIamge(BaseObject* obj);

//	//Singleton
//public:
//	RenderModule* getInstance();
//private:
//	RenderModule* instance;
};

class SceneModule {
public:
	//SceneModule():cnt(0){}
	//SceneModule(ResModule*, RenderModule*);
	void Init();
	void Run();
	bool MayCollide(BaseObject* i, BaseObject* j);//判断有没有必要进行j对i的碰撞检测
	bool Detect(BaseObject* i, BaseObject* j);//判断j是否撞上了i
	void CollisionDetect();//碰撞检测函数

	//void AddObj(BaseObject *);
	//void DeleteObj(BaseObject *);
	//BaseObject *NewObj(ObjType type, int typeId);
private:
	list<BaseObject*> objList;
	list<BaseObject*> objListToAdd;
	priority_queue<Event> heap;
	//	//Singleton
	//public:
	//	SceneModule* getInstance();
	//private:
	//	SceneModule* instance;
};
