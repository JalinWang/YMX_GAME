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
#include "Setting.h"
#include "..\Libs\inifile.h"

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
	vector<Event> LoadRound(const string & round);//���ص�ͼ�¼�
	BaseObject * Load(ObjType type, int typeId);//���ɶ���
	void Init();//��ʼ����Ԥ�����������͵Ķ���
private:
	vector<Monster> monsters;
	vector<Bullet> bullets;
	vector<Player> players;
	vector<Wall> walls;
	//vector<Item> items;

	bool LoadBaseObject(BaseObject *p, const string &typeIdStr);
	bool LoadCharacter(Character *p, const string &typeIdStr);
	bool LoadMonster(Monster *p, const string &typeIdStr);
	bool LoadPlayer(Player *p, const string &typeIdStr);
	bool LoadBullet(Bullet *p, const string &typeIdStr);
	bool LoadWall(Wall *p, const string &typeIdStr);
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
	void Release();
	void Init();
	int Run();

	int GetMonCnt();
	//void AddObj(BaseObject *);
	//void DeleteObj(BaseObject *);
	//BaseObject *NewObj(ObjType type, int typeId);
private:
	list<BaseObject*> objList;
	priority_queue<Event> heap;
	int monCnt;
	AABBTree aabbTree;

	void AddObj(BaseObject*);
	bool MayCollide(BaseObject* i, BaseObject* j);//�ж���û�б�Ҫ����j��i����ײ���
	bool Detect(BaseObject* i, BaseObject* j);//�ж�j�Ƿ�ײ����i
	void CollisionDetect();//��ײ��⺯��
	//	//Singleton
	//public:
	//	SceneModule* getInstance();
	//private:
	//	SceneModule* instance;
};