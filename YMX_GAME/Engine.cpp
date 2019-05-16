#include "stdafx.h"
#include "Engine.h"
#include <thread>
#include <functional>


//using nm_GameEngine::Engine;

Engine* Engine::instance = new Engine();

Engine::Engine()
{
}


Engine::~Engine()
{
}

void Engine::Init()
{
	mRes.Init();
	mScene.Init();
	player = *(dynamic_cast<Player*>(mRes.Load(ENUM_PLAYER, 0)));
}

void Engine::Run()
{
	std::thread backgoundThread(mScene.Run, &mScene);
	backgoundThread.detach();
}

Engine* Engine::get_Instance()
{
	return instance;
}