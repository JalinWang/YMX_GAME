#include "Engine.h"
#include <thread>
#include <functional>


//using nm_GameEngine::Engine;

Engine* Engine::instance = new Engine();

int Engine::NowTime()
{
	using namespace chrono;
	//auto t = steady_clock::now();
	milliseconds time_span = duration_cast<milliseconds>(steady_clock::now() - _startTime);
	return static_cast<int>(time_span.count());
}

Engine::~Engine()
{
}

void Engine::Init()
{
	_startTime = chrono::steady_clock::now();
	mRes.Init();
	//terminate();

	//mRes.Load(ENUM_PLAYER, 0);

	//terminate();
	player = *(dynamic_cast<Player*>(mRes.Load(ENUM_PLAYER, 0)));
//TODO:调试用！！！！！！！For Debugging
	//terminate();
	player.SetPos(1600, 1280);

	//顺序！！必须放这里！！！
	mScene.Init();
}

Engine* Engine::get_Instance()
{
	return instance;
}