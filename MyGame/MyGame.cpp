// MyGame.cpp : 定义应用程序的入口点。
//


#include "framework.h"
#include "MyGame.h"
#include "DXSupports.h"

#include "Engine/Engine.h"

#include <cstdio>

#define MAX_LOADSTRING 100

// 全局变量:
#define GAME_START		0
#define GAME_RUNNING	1
#define GAME_PAUSE		2
#define GAME_ENDING		3
#define GAME_INIT		4

extern const char* mapname[4];

int  GameState;

HINSTANCE hInst;                                // 当前实例
HWND hWnd;
WCHAR szTitle[MAX_LOADSTRING];                  // 标题栏文本
WCHAR szWindowClass[MAX_LOADSTRING];            // 主窗口类名
HWND hwndButton;
HIMAGELIST hImage;

HBITMAP hLogo;

bool GamePending;

int ScreenWidth;
int ScreenWidthS;
int ScreenHeight;
int ScreenHeightS;
extern UCHAR KeyState[256];

extern POINT curPos;

// 此代码模块中包含的函数的前向声明:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);
bool GameInit();
void GameMain();
void GameQuit();
void MemRelease();
DWORD GameRunningCode();

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR    lpCmdLine,
	_In_ int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	// TODO: 在此处放置代码。

	// 初始化全局字符串
	LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadStringW(hInstance, IDC_MYGAME, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	ScreenWidth = GetSystemMetrics(SM_CXSCREEN);
	//ScreenWidth = 800;
	ScreenWidthS = ScreenWidth - 1;
	ScreenHeight = GetSystemMetrics(SM_CYSCREEN);
	//ScreenHeight = 600;
	ScreenHeightS = ScreenHeight - 1;

	// 执行应用程序初始化:
	if (!InitInstance(hInstance, nCmdShow))
	{
		return FALSE;
	}

	HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_MYGAME));

	MSG msg;
	DWORD timer;
	GamePending = false;

	// 主消息循环:
	while (1)
	{
		timer = GetTickCount();
		timer += 17;
		if (GamePending)
		{
			GameMain();
		}
		if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
		{
			if (LOWORD(msg.message) == WM_QUIT)
				break;
			if (!GamePending)
				if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
				{
					TranslateMessage(&msg);
					DispatchMessage(&msg);
				}
		}
		while (GetTickCount() < timer);
	}
	MemRelease();

	return (int)msg.wParam;
}



//
//  函数: MyRegisterClass()
//
//  目标: 注册窗口类。
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEXW wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_MYGAME));
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	//wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_MYGAME);
	wcex.lpszMenuName = NULL;
	wcex.lpszClassName = szWindowClass;
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassExW(&wcex);
}

//
//   函数: InitInstance(HINSTANCE, int)
//
//   目标: 保存实例句柄并创建主窗口
//
//   注释:
//
//        在此函数中，我们在全局变量中保存实例句柄并
//        创建和显示主程序窗口。
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
	hInst = hInstance; // 将实例句柄存储在全局变量中

	hWnd = CreateWindowW(
		szWindowClass,
		szTitle,
		//WS_OVERLAPPEDWINDOW,
		WS_POPUP | WS_VISIBLE,
		(ScreenWidth - 640) >> 1,
		(ScreenHeight - 480) >> 1,
		640,
		480,
		nullptr,
		nullptr,
		hInstance,
		nullptr);

	if (!hWnd)
	{
		return FALSE;
	}

	hImage = ImageList_Create(640, 480, ILC_COLOR24, 1, 0);
	hLogo = (HBITMAP)LoadImageA(hInst, MAKEINTRESOURCEA(IDB_BITMAP1), IMAGE_BITMAP, 640, 480, LR_DEFAULTCOLOR);
	if (ImageList_Add(
		hImage,
		hLogo,
		NULL) == -1)
	{
		MessageBox(hWnd, L"Failed", L"Error", MB_OK);
	}


	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	return TRUE;
}

//
//  函数: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  目标: 处理主窗口的消息。
//
//  WM_COMMAND  - 处理应用程序菜单
//  WM_PAINT    - 绘制主窗口
//  WM_DESTROY  - 发送退出消息并返回
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	HDC hdc;
	RECT rect = { 0,0,640,480 };
	switch (message)
	{

	case WM_LBUTTONDOWN:

		if (!GamePending)
		{
			GamePending = GameInit();
			if (!GamePending)
				MessageBox(hWnd, L"Failed", L"Report", MB_OK);
			return 0;
		}

		break;
	case WM_RBUTTONDOWN:
		DefWindowProc(hWnd, WM_CLOSE, 0, 0);
		return 0;
	case WM_PAINT:
	{
		hdc = GetDC(hWnd);
		ImageList_Draw(
			hImage,
			0,
			hdc,
			0, 0,
			ILD_NORMAL);
	}
	break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		break;
	}
	if (GamePending)
		return 0;
	return DefWindowProc(hWnd, message, wParam, lParam);
}

bool GameInit()
{
	if (!DirectSurfaceInit(hWnd))
		return false;

	if (!DirectInputInit(hInst, hWnd))
		return false;

	if (!LoadResource())
		return false;
	if (!LoadMap(0))
		return false;
	GameState = GAME_START;
	//terminate();

	return true;
}

void GameQuit()
{
	GamePending = false;
	PostMessage(hWnd, WM_CLOSE, 0, 0);
	ShowCursor(true);
}

void MemRelease()
{
	ReleaseResource();

	DirectInputRelease();

	DirectSufaceRelease();

	if (hImage)
	{
		ImageList_Destroy(hImage);
		hImage = NULL;
	}

	if (hLogo)
	{
		DeleteObject(hLogo);
		hLogo = NULL;
	}
}

void GameMain()
{
	double hp = Engine::get_Instance()->get_player()->get_HP();
	//fetch keyboard state
	FetchKeyState();
	//fetch mouse state
	FetchMouseState();

#ifdef TEST
	if (KeyProcess())
		GameQuit();

#endif // TEST
	switch (GameState)
	{
	case GAME_START:
		switch (DrawStartMenu())
		{
		case 0:
			break;
		case 1:
			GameState = GAME_INIT;
			break;
		case 2:
			GameQuit();
			break;
		}
		break;
	case GAME_INIT:
		//Game Init code here
		//
		//TO-DO
		//
		Engine::get_Instance()->Init();
#ifdef TEST
		TestInit();
#endif // TEST
		GameState = GAME_RUNNING;
	case GAME_RUNNING:
	{
#ifdef TEST
		if (GameMainSimCode())
			GameState = GAME_PAUSE;
		break;
#else 
		FetchKeyState();
		if (DIKEYDOWN(DIK_ESCAPE))
		{
			GameQuit();
			break;
		}
		GameState = GameRunningCode();
		break;
#endif
	}
	case GAME_PAUSE:
	{
		switch (DrawPauseMenu())
		{
		case 0:
			break;
		case 1:
			GameState = GAME_RUNNING;
			break;
		case 2:
			GameState = GAME_ENDING;
			break;
		}
		break;
	}
	case GAME_ENDING:
		switch (DrawEndMenu(hp <= 0 ? 0 : int(hp)))
		{
		case 0:
			break;
		case 1:
			GameState = GAME_INIT;
			break;
		case 2:
			GameQuit();
			break;
		}
		break;
	default:
		break;
	}
	DrawCursor();
	ScreenUpdate();
}

DWORD GameRunningCode()
{
	auto engine = Engine::get_Instance();

	if (engine->get_mScene()->Run())
		return GAME_RUNNING;
	else
		return GAME_ENDING;
}