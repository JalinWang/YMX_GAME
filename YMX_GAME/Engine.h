#pragma once
#include "Modules.h"
#include "Setting.h"

//namespace nm_GameEngine {}

class Engine
{
public:
	~Engine();

	void Init();

	Player *get_player();

	ResModule *get_mRes();
	RenderModule *get_mRender();
	SceneModule *get_mScene();
private:
	Player player;
	vector<BaseObject> objList;

	ResModule mRes;
	RenderModule mRender;
	SceneModule mScene;

public:
	static Engine *get_Instance();
private:
	static Engine *instance = new Engine();

	Engine();
};
