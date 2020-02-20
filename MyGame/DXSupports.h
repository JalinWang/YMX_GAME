#pragma once
#ifndef DXSUPPORT
#define DXSUPPORT

#include "ddraw.h"
#define DIRECTINPUT_VERSION 0x0800
#include "dinput.h"

#define BT0 0
#define PI 3.1415926f

#define TYPE_COUNT      7
#define TYPE_PLAYER		0
#define TYPE_OBJECT		1
#define TYPE_MONSTER	2
#define TYPE_BULLET		3
#define TYPE_MAP		4
#define TYPE_ALPHA		5
#define TYPE_WALL		6

#define SCROLL_SPEED	20
#define BULLET_SPEED	50



#define CHECK(x) if(FAILED(x))return false
#define DIKEYDOWN(n) (KeyState[n]&0x80)
#define LBDOWN (MouseState.rgbButtons[0]&0x80)
#define RBDOWN (MouseState.rgbButtons[1]&0x80)

#define INSIDE(point,rect) (point.x > rect.left && point.x < rect.right && point.y > rect.top && point.y < rect.bottom)

//#define TEST
#include <math.h>
#ifdef TEST

typedef struct MyBullet
{
	UCHAR type;
	POINT pos;
	POINT speed;
	DWORD angle;
	struct MyBullet* next;
}BULLET, * LPBULLET;

void TestInit();

#endif

typedef struct MySurface
{
	const char* name;
	LPDIRECTDRAWSURFACE7 sf;
	POINT size;
	DWORD count;
}SURFACE, * LPSURFACE;

typedef struct BITMAP_FILE_TAG
{
	BITMAPFILEHEADER header;
	BITMAPINFOHEADER info;
	PALETTEENTRY palette[256];
	UCHAR* buffer;
}BITMAP_FILE, * BITMAP_FILE_PTR;

extern inline bool DirectSurfaceInit(HWND hWnd);
extern inline void DirectSufaceRelease();
LPDIRECTDRAWSURFACE7 DDrawCreateSurface(int width, int height, int memFlages);
bool CreateSurfaceFromBitmap(LPSURFACE sur, int memFlags);
extern inline bool LoadResource();
POINT GetMapSize();
bool LoadMap(int);
extern inline void ReleaseResource();
extern inline void ChangeCursor(int index);
extern inline void DrawCursor();
bool DrawHPBar(int max, int HP);
bool DrawObject(int type_0, int type_1, POINT pos, UCHAR scale, int cnt);
bool DrawObject(int type_0, int type_1, POINT pos, double Height, double Width, int cnt);
bool MyDrawText(const char* text, POINT pos, DWORD scale);
bool MyDrawNum(DWORD i, POINT pos, DWORD scale);
bool DrawRect(RECT rect, DWORD color);
bool MyDrawButton(RECT rect, const char* text, DWORD textScale = 1);
int DrawStartMenu();
int DrawPauseMenu();
int DrawEndMenu(int score);
extern inline bool ScreenUpdate();
///////////////////////////////////////////////////////////////////////////////////////
//make sure to include dinput.lib,dinput8.lib,dxguid.lib before calling the init function
//make sure to call this function before calling FetchXXXState
//make sure to call DirectInputRelease at the end of the program after calling this function
extern inline bool DirectInputInit(HINSTANCE hInst, HWND hWnd);
//make sure to call this function at the end of the program if DirectInputInit is called before
extern inline void DirectInputRelease();
//make sure to DirectInputInit returns true(it does so most of the time) before calling the following 2 functions
//these functions should be called once every 17ms
//if they return false, there should be an hardware error
extern inline bool FetchKeyState();
extern inline bool FetchMouseState();
//make sure to call FetchMouseState first before calling the following functions, or the result might be wrong
bool LBClick();
bool RBClick();
bool GetMouseState(int key);
POINT GetMousePosition();
//make sure to call FetchKeyState first, or the result might be wrong
bool GetKeyState(DWORD key);
/////////////////////////////////////////////////////////////////////////////////////
#ifdef TEST
extern inline void SetCamera();
void SetCamera(POINT FakeMouse);
bool KeyProcess();
int GameMainSimCode();
#else
extern inline void SetCamera(POINT player);
#endif

int LoadBitmapFile(BITMAP_FILE_PTR bitmap, const char* filename);
int UnloadBitmapFile(BITMAP_FILE_PTR bitmap);

#endif