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
	vector<Event> LoadMap(ObjType type, int typeId);//���ص�ͼ�¼�
	void Load(BaseObject& obj, ObjType type, int typeId);//�������ԡ���ͼ��
	void Init();//��ʼ��������ļ��������

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
	bool Detect(BaseObject * i, BaseObject * j);//  �ж�
private:
	ResModule *mRes;
	RenderModule *mRender;

	//Singleton
public:
	SceneModule* getInstance();
private:
	SceneModule* instance;
};
