#include "DXSupports.h"
#include <vector>
#include <string>
#include <fstream>
#include <iostream>
#include <cstring>
using namespace std;

vector<vector<SURFACE>*> sfList(TYPE_COUNT);
SURFACE HPBar;

#define RES_DIR ".\\Res\\Image\\"
vector<SURFACE> wall, cur, obj, map, mon, bul, player;
DWORD curIndex = 0;
DWORD mapIndex = 0;

#ifdef TEST
BULLET bulletList;
LPBULLET bListLast;
#endif
SURFACE alpha;
SURFACE num;

RECT DefaultButton[5];

DWORD ButtonColorU;
DWORD ButtonColorS;

LPDIRECTDRAW7 lpdd;

LPDIRECTDRAWSURFACE7 lpddsmain;
LPDIRECTDRAWSURFACE7 lpddsback;
LPDIRECTDRAWCLIPPER lpddclipper;

LPDIRECTINPUT8 lpdi;
LPDIRECTINPUTDEVICE8 lpdiKey;
LPDIRECTINPUTDEVICE8 lpdiMouse;
UCHAR KeyState[256];
DIMOUSESTATE MouseState;

bool KeyAcquired;
bool MouseAcquired;

extern int ScreenWidth;
extern int ScreenWidthS;
extern int ScreenHeight;
extern int ScreenHeightS;

int monster_state;

POINT curPos;
RECT camera;

#ifdef TEST
POINT FakePlayer;

void TestInit()
{
	if (map[0].size.x > ScreenWidth)
	{
		camera.left = (map[0].size.x - ScreenWidth) >> 1;
		camera.right = camera.left + ScreenWidth;
	}
	else
	{
		camera.left = 0;
		camera.right = map[0].size.x;
	}

	if (map[0].size.y > ScreenHeight)
	{
		camera.top = (map[0].size.y - ScreenHeight) >> 1;
		camera.bottom = camera.top + ScreenHeight;
	}

	FakePlayer.x = (map[0].size.x >> 1);
	FakePlayer.y = (map[0].size.y >> 1);

	bListLast = &bulletList;

}
#endif


/////////////////////////////////////////////////////////////////////////////////////
#define DISPLAY
inline bool DirectSurfaceInit(HWND hWnd)
{
	DDSURFACEDESC2 ddsd;
	LPRGNDATA region_data;

	CHECK(DirectDrawCreateEx(NULL, (LPVOID*)& lpdd, IID_IDirectDraw7, NULL));

#ifdef DISPLAY
	CHECK(lpdd->SetCooperativeLevel(hWnd, DDSCL_ALLOWMODEX | DDSCL_ALLOWREBOOT | DDSCL_EXCLUSIVE | DDSCL_FULLSCREEN));
		if (SUCCEEDED(lpdd->SetDisplayMode(1920, 1200, 32, 0, 0)))
		{
			ScreenWidth = 1920;
			ScreenHeight = 1200;
		}
		else if (SUCCEEDED(lpdd->SetDisplayMode(1920, 1080, 32, 0, 0)))
		{
			ScreenWidth = 1920;
			ScreenHeight = 1080;
		}
		else if (SUCCEEDED(lpdd->SetDisplayMode(1680, 1050, 32, 0, 0)))
		{
			ScreenWidth = 1680;
			ScreenHeight = 1050;
		}
		else if (SUCCEEDED(lpdd->SetDisplayMode(1600, 1200, 32, 0, 0)))
		{
			ScreenWidth = 1600;
			ScreenHeight = 1200;
		}
		else if (SUCCEEDED(lpdd->SetDisplayMode(1600, 900, 32, 0, 0)))
		{
			ScreenWidth = 1600;
			ScreenHeight = 900;
		}
		else if (SUCCEEDED(lpdd->SetDisplayMode(1440, 900, 32, 0, 0)))
		{
			ScreenWidth = 1440;
			ScreenHeight = 900;
		}
		else if (SUCCEEDED(lpdd->SetDisplayMode(1366, 768, 32, 0, 0)))
		{
			ScreenWidth = 1366;
			ScreenHeight = 768;
		}
		else if (SUCCEEDED(lpdd->SetDisplayMode(1360, 768, 32, 0, 0)))
		{
			ScreenWidth = 1360;
			ScreenHeight = 768;
		}
		else 
			CHECK(lpdd->SetDisplayMode(ScreenWidth, ScreenHeight, 32, 0, 0));
	memset(&ddsd, 0, sizeof(ddsd));
	ddsd.dwSize = sizeof(ddsd);
	ddsd.dwFlags = DDSD_CAPS | DDSD_BACKBUFFERCOUNT;
	ddsd.ddsCaps.dwCaps = DDSCAPS_PRIMARYSURFACE | DDSCAPS_COMPLEX | DDSCAPS_FLIP;
	ddsd.dwBackBufferCount = 1;

	CHECK(lpdd->CreateSurface(&ddsd, &lpddsmain, NULL));

	ddsd.ddsCaps.dwCaps = DDSCAPS_BACKBUFFER;

	CHECK(lpddsmain->GetAttachedSurface(&ddsd.ddsCaps, &lpddsback));

	CHECK(lpdd->CreateClipper(0, &lpddclipper, NULL));

	region_data = (LPRGNDATA)malloc(sizeof(RGNDATAHEADER) + 1 * sizeof(RECT));

	RECT screen = { 0,0,ScreenWidth,ScreenHeight };

	memcpy(region_data->Buffer, &screen, sizeof(RECT));

	region_data->rdh.dwSize = sizeof(RGNDATAHEADER);
	region_data->rdh.iType = RDH_RECTANGLES;
	region_data->rdh.nCount = 1;
	region_data->rdh.nRgnSize = sizeof(RECT);
	region_data->rdh.rcBound.left = 0;
	region_data->rdh.rcBound.right = ScreenWidth;
	region_data->rdh.rcBound.top = 0;
	region_data->rdh.rcBound.bottom = ScreenHeight;

	if (FAILED(lpddclipper->SetClipList(region_data, 0)))
	{
		free(region_data);
		return false;
	}

	lpddsback->SetClipper(lpddclipper);

	free(region_data);
#endif // DISPLAY
	return true;
}

inline void DirectSufaceRelease()
{
	if (lpddclipper)
	{
		lpddclipper->Release();
		lpddclipper = NULL;
	}

	if (lpddsback)
	{
		lpddsback->Release();
		lpddsback = NULL;
	}

	if (lpddsmain)
	{
		lpddsmain->Release();
		lpddsmain = NULL;
	}

	if (lpdd)
	{
		lpdd->Release();
		lpdd = NULL;
	}
}

inline void DrawCursor()
{
	RECT rect;
	rect.top = curPos.y - (cur[curIndex].size.y >> 2);
	rect.left = curPos.x - (cur[curIndex].size.x >> 2);
	rect.bottom = rect.top + (cur[curIndex].size.y >> 1);
	rect.right = rect.left + (cur[curIndex].size.x >> 1);
	lpddsback->Blt(&rect, cur[curIndex].sf, NULL, DDBLT_WAIT | DDBLT_KEYSRC, NULL);
}

inline void ChangeCursor(int index)
{
	if (index >= cur.size())
		curIndex = 0;
	else
		curIndex = index;
}

POINT GetMapSize()
{
	return map[mapIndex].size;
}

bool LoadMap(int index)
{
	if (map[mapIndex].sf)
	{
		map[mapIndex].sf->Release();
	}
	mapIndex = index;
	if (!CreateSurfaceFromBitmap(&map[mapIndex], DDSCAPS_VIDEOMEMORY))
		return false;
	return true;
}

void ParseFile(const string& fileName, vector<SURFACE>& v)
{
	ifstream f(RES_DIR + fileName);
	while (!f.eof())
	{
		SURFACE s;
		s.sf = NULL;
		string* imageFile = new string;
		f >> s.count >> *imageFile;
		if (*imageFile == "")
			break;
		(*imageFile) = RES_DIR + *imageFile + ".bmp";
		s.name = imageFile->c_str();
		v.push_back(s);
	}
	f.close();
	return;
}

inline bool LoadResource()
{
	sfList[TYPE_MAP] = &map;
	sfList[TYPE_PLAYER] = &player;
	sfList[TYPE_MONSTER] = &mon;
	sfList[TYPE_BULLET] = &bul;
	sfList[TYPE_OBJECT] = &obj;
	sfList[TYPE_WALL] = &wall;
	ParseFile("Walls.txt", wall);
	ParseFile("Cursors.txt", cur);
	ParseFile("Items.txt", obj);
	ParseFile("Maps.txt", map);
	ParseFile("Monsters.txt", mon);
	ParseFile("Bullets.txt", bul);
	//SURFACE s;
	//s.name = RES_DIR"bul0.bmp";
	//s.count = 1;
	//bul.push_back(s);
	ParseFile("Players.txt", player);

	for (unsigned int i = 0; i < wall.size(); i++)
	{
		if (!CreateSurfaceFromBitmap(&wall[i], DDSCAPS_VIDEOMEMORY))
			return false;
	}

	for (unsigned int i = 0; i < obj.size(); i++)
	{
		if (!CreateSurfaceFromBitmap(&obj[i], DDSCAPS_VIDEOMEMORY))
			return false;
	}

	for (unsigned int i = 0; i < player.size(); i++)
	{
		if (!CreateSurfaceFromBitmap(&player[i], DDSCAPS_VIDEOMEMORY))
			return false;
	}

	for (unsigned int i = 0; i < cur.size(); i++)
	{
		if (!CreateSurfaceFromBitmap(&cur[i], DDSCAPS_VIDEOMEMORY))
			return false;
	}
	curIndex = 0;

	for (unsigned int i = 0; i < mon.size(); i++)
	{
		if (!CreateSurfaceFromBitmap(&mon[i], DDSCAPS_VIDEOMEMORY))
			return false;
	}

	for (unsigned int i = 0; i < bul.size(); i++)
	{
		if (!CreateSurfaceFromBitmap(&bul[i], DDSCAPS_VIDEOMEMORY))
			return false;
	}

	HPBar.name = RES_DIR"HPBar.bmp";
	HPBar.count = 1;

	if (!CreateSurfaceFromBitmap(&HPBar, DDSCAPS_VIDEOMEMORY))
		return false;

	alpha.name = RES_DIR"alphabet.bmp";
	alpha.count = 26;
	if (!CreateSurfaceFromBitmap(&alpha, DDSCAPS_VIDEOMEMORY))
		return false;

	num.name = RES_DIR"num.bmp";
	num.count = 10;
	if (!CreateSurfaceFromBitmap(&num, DDSCAPS_VIDEOMEMORY))
		return false;

	DefaultButton[0] = { (ScreenWidth >> 2),(ScreenHeight >> 3) * 3,(ScreenWidth - (ScreenWidth >> 2)),(ScreenHeight >> 5) * 15 };
	DefaultButton[4] = { (ScreenWidth >> 2),(ScreenHeight >> 3) * 7,(ScreenWidth - (ScreenWidth >> 2)),(ScreenHeight >> 5) * 31 };
	DefaultButton[2].left = (ScreenWidth >> 2);
	DefaultButton[2].top = (ScreenHeight >> 3) * 5;
	DefaultButton[2].right = (ScreenWidth - (ScreenWidth >> 2));
	DefaultButton[2].bottom = (ScreenHeight >> 5) * 23;
	DefaultButton[3].left = (ScreenWidth >> 2);
	DefaultButton[3].top = (ScreenHeight >> 3) * 6;
	DefaultButton[3].right = (ScreenWidth - (ScreenWidth >> 2));
	DefaultButton[3].bottom = (ScreenHeight >> 5) * 27;

	ButtonColorU = 0xffffff;
	ButtonColorS = 0xffff00;
	return true;
}

inline void ReleaseResource()
{
	if (HPBar.sf)
	{
		HPBar.sf->Release();
		HPBar.sf = NULL;
	}
	if (alpha.sf)
	{
		alpha.sf->Release();
		alpha.sf = NULL;
	}
	if (num.sf)
	{
		num.sf->Release();
		num.sf = NULL;
	}
	for (unsigned int i = 0; i < wall.size(); i++)
	{
		if (wall[i].sf)
		{
			wall[i].sf->Release();
			wall[i].sf = NULL;
		}
	}
	for (unsigned int i = 0; i < obj.size(); i++)
	{
		if (obj[i].sf)
		{
			obj[i].sf->Release();
			obj[i].sf = NULL;
		}
	}
	for (unsigned int i = 0; i < player.size(); i++)
	{
		if (player[i].sf)
		{
			player[i].sf->Release();
			player[i].sf = NULL;
		}
	}
	for (unsigned int i = 0; i < bul.size(); i++)
	{
		if (bul[i].sf)
		{
			bul[i].sf->Release();
			bul[i].sf = NULL;
		}
	}
	for (unsigned int i = 0; i < mon.size(); i++)
	{
		if (mon[i].sf)
		{
			mon[i].sf->Release();
			mon[i].sf = NULL;
		}
	}
	for (unsigned int i = 0; i < cur.size(); i++)
	{
		if (cur[i].sf)
		{
			cur[i].sf->Release();
			cur[i].sf = NULL;
		}
	}
	for (unsigned int i = 0; i < map.size(); i++)
	{
		if (map[i].sf)
		{
			map[i].sf->Release();
			map[i].sf = NULL;
		}
	}
}

bool DrawObject(int type_0, int type_1, POINT pos, double Height, double Width, int cnt)
{
	RECT rect;
	RECT src;
	DWORD state;
	LPSURFACE tmp;
	tmp = &((*sfList[type_0])[type_1]);
	if (type_0 == TYPE_MAP)
	{
		CHECK(lpddsback->Blt(NULL, map[type_1].sf, &camera, DDBLT_WAIT, NULL));
		return true;
	}
	state = cnt % tmp->count;
	rect.right = (int)Width;
	rect.bottom = (int)Height;
	rect.left = (pos.x - (rect.right >> 1)) - camera.left;
	rect.top = (pos.y - (rect.bottom >> 1)) - camera.top;
	rect.right += rect.left;
	rect.bottom += rect.top;
	if (rect.left > ScreenWidth || rect.bottom < 0 || rect.right < 0 || rect.top > ScreenHeight)
		return true;
	src.left = state * tmp->size.x;
	src.right = src.left + tmp->size.x;
	src.top = 0;
	src.bottom = tmp->size.y;
	CHECK(lpddsback->Blt(&rect, tmp->sf, &src, DDBLT_WAIT | DDBLT_KEYSRC, NULL));
	
	return true;
}

bool DrawHPBar(int max, int HP)
{
	RECT rect;
	rect.left = 0;
	rect.top = 0;
	rect.right = HPBar.size.x;
	rect.bottom = HPBar.size.y;
	CHECK(lpddsback->Blt(&rect, HPBar.sf, NULL, DDBLT_WAIT | DDBLT_KEYSRC, NULL));
	rect.right = 600 * HP / max;
	rect.bottom = 64;
	DrawRect(rect, 0xff0000);
}

bool DrawObject(int type_0, int type_1, POINT pos, UCHAR scale, int cnt)
{
	RECT rect;
	RECT src;
	DWORD state;
	LPSURFACE tmp = & ((*sfList[type_0])[type_1]);

	if (type_0 == TYPE_MAP)
	{
		CHECK(lpddsback->Blt(NULL, map[type_1].sf, &camera, DDBLT_WAIT, NULL));
		return true;
	}
	
	state = cnt % tmp->count;
	rect.left = (pos.x - ((tmp->size.x >> 1) >> scale)) - camera.left;
	rect.top = (pos.y - ((tmp->size.y >> 1) >> scale)) - camera.top;
	rect.right = rect.left + (tmp->size.x >> scale);
	rect.bottom = rect.top + (tmp->size.y >> scale);
	if (rect.left > ScreenWidth || rect.bottom < 0 || rect.right < 0 || rect.top > ScreenHeight)
		return true;
	src.left = state * tmp->size.x;
	src.right = src.left + tmp->size.x;
	src.top = 0;
	src.bottom = tmp->size.y;
	CHECK(lpddsback->Blt(&rect, tmp->sf, &src, DDBLT_WAIT | DDBLT_KEYSRC, NULL));
	return true;
}

bool MyDrawButton(RECT rect, const char* text, DWORD textScale)
{
	bool isSelect = true;
	POINT tp;
	int len;
	for (len = 0; text[len]; len++);
	if (curPos.x > rect.right || curPos.x < rect.left)
		isSelect = false;
	if (curPos.y > rect.bottom || curPos.y < rect.top)
		isSelect = false;
	if (!DrawRect(rect, (isSelect) ? ButtonColorS : ButtonColorU))
		return false;
	tp.x = rect.right - rect.left;
	tp.x -= ((len * alpha.size.x) >> textScale);
	tp.x >>= 1;
	tp.x += rect.left;
	tp.y = rect.bottom - rect.top;
	tp.y -= (alpha.size.y >> textScale);
	tp.y >>= 1;
	tp.y += rect.top;
	//return MyDrawText(text, { rect.left + tmp,rect.top + 20 }, 1);
	return MyDrawText(text, tp, textScale);
}

bool MyDrawText(const char* text, POINT pos, DWORD scale)
{
	RECT dst, src;
	DWORD width, height;
	char c;
	width = (alpha.size.x >> scale);
	height = (alpha.size.y >> scale);
	dst.left = pos.x;
	dst.top = pos.y;
	dst.bottom = dst.top + height;
	src.top = 0;
	src.bottom = alpha.size.y;
	for (int i = 0; c = text[i]; i++)
	{
		dst.right = dst.left + width;
		if (c == ' ')
		{
			dst.left = dst.right;
			continue;
		}
		c &= ~0x20;
		c -= 'A';
		src.left = c * alpha.size.x;
		if (src.left < 0) return true;
		src.right = src.left + alpha.size.x;
		CHECK(lpddsback->Blt(&dst, alpha.sf, &src, DDBLT_WAIT | DDBLT_KEYSRC, NULL));
		dst.left = dst.right;
	}
	return true;
}

bool DrawRect(RECT rect, DWORD color)
{
	DDBLTFX ddbltfx;
	memset(&ddbltfx, 0, sizeof(ddbltfx));
	ddbltfx.dwSize = sizeof(ddbltfx);
	ddbltfx.dwFillColor = color;
	if (SUCCEEDED(lpddsback->Blt(&rect, NULL, NULL, DDBLT_WAIT | DDBLT_COLORFILL, &ddbltfx)))
		return true;
	else
		return false;
}

int DrawStartMenu()
{
	RECT rect;
	POINT tmp;
	rect.left = 0;
	rect.top = 0;
	rect.right = ScreenWidth;
	rect.bottom = ScreenHeight;
	//rect = { 0,0,ScreenWidth,ScreenHeight };
	DrawRect(rect, 0x88aaff);
	rect.left = 40;
	rect.top = 40;
	rect.right = ScreenWidth - 40;
	rect.bottom = ScreenHeight - 40;
	//rect = { 40,40, ScreenWidth - 40,ScreenHeight - 40 };
	DrawRect(rect, 0x6688ff);
	tmp.x = (ScreenWidth - (alpha.size.x << 2)) >> 1;
	tmp.y = 150;
	MyDrawText("Menu", tmp, 0);
	MyDrawButton(DefaultButton[0], "Start");
	MyDrawButton(DefaultButton[4], "Quit");
	if (LBDOWN)
	{
		if INSIDE(curPos, DefaultButton[0])
			return 1;
		else if INSIDE(curPos, DefaultButton[4])
		{
			return 2;
		}
	}
	return 0;
}

int DrawPauseMenu()
{
	RECT rect;
	POINT tmp;
	rect.top = (ScreenHeight - 480) >> 1;
	rect.left = (ScreenWidth - 640) >> 1;
	rect.right = rect.left + 640;
	rect.bottom = rect.top + 480;
	DrawRect(rect, 0x88aaff);
	//rect = { 700,280,1300,720 };
	rect.top = (ScreenHeight - 460) >> 1;
	rect.left = (ScreenWidth - 620) >> 1;
	rect.right = rect.left + 620;
	rect.bottom = rect.top + 460;
	DrawRect(rect, 0x6688ff);
	tmp.x = rect.left + 50;
	tmp.y = rect.top + 50;
	MyDrawText("PAUSE", tmp, 1);
	//rect = { 900,450,1250,500 };
	rect.left += 200;
	rect.top += 200;
	rect.right -= 50;
	rect.bottom = (rect.top + 50);
	MyDrawButton(rect, "Resume", 2);
	if ((LBDOWN) && (INSIDE(curPos, rect)))
	{
		return 1;
	}
	//rect = { 900, 600,1250,650 };
	rect.top += 150;
	rect.bottom += 150;
	MyDrawButton(rect, "Quit", 2);
	tmp.x = camera.left + rect.left - 100;
	tmp.y = camera.top + rect.top - 100;
	DrawObject(TYPE_PLAYER, 0, tmp, 1, 0);
	if ((LBDOWN) && (INSIDE(curPos, rect)))
	{
		return 2;
	}
	tmp.x = 500;
	tmp.y = 500;
	return 0;
}

int DrawEndMenu(int score)
{
	RECT rect;
	POINT tmp;
	rect.left = 0;
	rect.top = 0;
	rect.right = ScreenWidth;
	rect.bottom = ScreenHeight;
	//rect = { 0,0,ScreenWidth,ScreenHeight };
	DrawRect(rect, (score)?0x88aaff:0xff6644);
	rect.left = 40;
	rect.top = 40;
	rect.right = ScreenWidth - 40;
	rect.bottom = ScreenHeight - 40;
	//rect = { 40,40, ScreenWidth - 40,ScreenHeight - 40 };
	DrawRect(rect, (score)?0x6688ff:0xff2200);
	tmp.x = (ScreenWidth - (alpha.size.x << 3) - alpha.size.x) >> 1;
	tmp.y = 150;
	if (score == 0)
	{
		MyDrawText("GAME OVER", tmp, 0);
	}
	else
	{
		MyDrawText("YOU WIN!", tmp, 0);
		tmp.y = 350;
		tmp.x = 150;
		MyDrawText("score", tmp, 1);
		tmp.x = 400;
		tmp.y = 400;
		MyDrawNum(score, tmp, 0);
	}

	MyDrawButton(DefaultButton[3], "ReStart");
	MyDrawButton(DefaultButton[4], "Quit");
	if (LBDOWN)
	{
		if INSIDE(curPos, DefaultButton[3])
			return 1;
		else if INSIDE(curPos, DefaultButton[4])
		{
			return 2;
		}
	}
	return 0;
}

bool MyDrawNum(DWORD i, POINT pos, DWORD scale)
{
	char text[16];
	DWORD tmp;
	DWORD len;
	RECT dst, src;
	DWORD width, height;
	char c;
	len = 0;
	for (tmp = i; tmp; tmp /= 10)
	{
		len++;
	}
	text[len] = 0;
	for (; len; len--)
	{
		text[len - 1] = (i % 10) + '0';
		i /= 10;
	}
	width = (num.size.x >> scale);
	height = (num.size.y >> scale);
	dst.left = pos.x;
	dst.top = pos.y;
	dst.bottom = dst.top + height;
	src.top = 0;
	src.bottom = num.size.y;
	for (int i = 0; c = text[i]; i++)
	{
		dst.right = dst.left + width;
		c -= '0';
		src.left = c * num.size.x;
		if (src.left < 0) return true;
		src.right = src.left + num.size.x;
		CHECK(lpddsback->Blt(&dst, num.sf, &src, DDBLT_WAIT | DDBLT_KEYSRC, NULL));
		dst.left = dst.right;
	}
	return true;
}

extern inline bool ScreenUpdate()
{
	CHECK(lpddsmain->Flip(NULL, DDFLIP_WAIT));
	return true;
}

/////////////////////////////////////////////////////////////////////////////////////////////

inline bool DirectInputInit(HINSTANCE hInst, HWND hWnd)
{
	CHECK(DirectInput8Create(hInst, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)& lpdi, NULL));

	CHECK(lpdi->CreateDevice(GUID_SysKeyboard, &lpdiKey, NULL));

	CHECK(lpdiKey->SetCooperativeLevel(hWnd, DISCL_BACKGROUND | DISCL_NONEXCLUSIVE));

	CHECK(lpdiKey->SetDataFormat(&c_dfDIKeyboard));

	CHECK(lpdiKey->Acquire());

	KeyAcquired = true;

	CHECK(lpdi->CreateDevice(GUID_SysMouse, &lpdiMouse, NULL));

	CHECK(lpdiMouse->SetCooperativeLevel(hWnd, DISCL_BACKGROUND | DISCL_NONEXCLUSIVE));

	CHECK(lpdiMouse->SetDataFormat(&c_dfDIMouse));

	CHECK(lpdiMouse->Acquire());

	MouseAcquired = true;

	GetCursorPos(&curPos);

	ShowCursor(false);

	return true;
}

inline void DirectInputRelease()
{
	if (MouseAcquired)
	{
		lpdiMouse->Unacquire();
		MouseAcquired = false;
	}

	if (lpdiMouse)
	{
		lpdiMouse->Release();
		lpdiMouse = NULL;
	}
	if (KeyAcquired)
	{
		lpdiKey->Unacquire();
		KeyAcquired = false;
	}

	if (lpdiKey)
	{
		lpdiKey->Release();
		lpdiKey = NULL;
	}

	if (lpdi)
	{
		lpdi->Release();
		lpdi = NULL;
	}
}

bool LBClick()
{
	static bool isLDown = false;
	if (MouseState.rgbButtons[0] & 0x80)
	{
		if (!isLDown)
		{
			isLDown = true;
			return true;
		}
	}
	else
		isLDown = false;
	return false;
}

bool RBClick()
{
	static bool isRDown = false;
	if (MouseState.rgbButtons[0] & 0x80)
	{
		if (!isRDown)
		{
			isRDown = true;
			return true;
		}
	}
	else
		isRDown = false;
	return false;
}

bool GetMouseState(int key)
{
	return (MouseState.rgbButtons[key] & 0x80) ? true : false;
}

POINT GetMousePosition()
{
	POINT p;
	p.x = curPos.x + camera.left;
	p.y = curPos.y + camera.top;
	return p;
}

bool GetKeyState(DWORD key)
{
	return (KeyState[key] & 0x80) ? true : false;
}

inline bool FetchKeyState()
{
	HRESULT hr;
	while (FAILED(hr = lpdiKey->GetDeviceState(256, KeyState)))
	{
		if (hr == DIERR_INPUTLOST || hr == DIERR_NOTACQUIRED)
		{
			MessageBox(NULL, L"Keyboard Lost. Game Ended.", L"Error", MB_OK);
			return false;
		}
	}
	return true;
}

inline bool FetchMouseState()
{
	HRESULT hr;
	while (FAILED(hr = lpdiMouse->GetDeviceState(sizeof(DIMOUSESTATE), &MouseState)))
	{
		if (hr == DIERR_INPUTLOST || hr == DIERR_NOTACQUIRED)
		{
			MessageBox(NULL, L"Mouse Lost. Game Ended.", L"Error", MB_OK);
			return false;
		}
	}
	curPos.x += MouseState.lX;
	curPos.x = max(0, curPos.x);
	curPos.x = min(ScreenWidthS, curPos.x);
	curPos.y += MouseState.lY;
	curPos.y = max(0, curPos.y);
	curPos.y = min(ScreenHeightS, curPos.y);
	return true;
}

///////////////////////////////////////////////////////////////////////////////
#ifdef TEST

inline void SetCamera()
{
	RECT Pcamera;
	POINT cameraP;
	cameraP.x = camera.left;
	cameraP.y = camera.top;
	if (ScreenWidth < map[mapIndex].size.x)
	{
		Pcamera.left = ((curPos.x + camera.left + FakePlayer.x) >> 1) - (ScreenWidth >> 1);
		if (Pcamera.left < 0)
			Pcamera.left = 0;
		Pcamera.right = Pcamera.left + ScreenWidth;
		if (Pcamera.right > map[mapIndex].size.x)
		{
			Pcamera.right = map[mapIndex].size.x;
			Pcamera.left = Pcamera.right - ScreenWidth;
		}
	}
	else
	{
		Pcamera.left = 0;
		Pcamera.right = map[mapIndex].size.x;
	}
	if (ScreenHeight < map[mapIndex].size.y)
	{
		Pcamera.top = ((curPos.y + camera.top + FakePlayer.y) >> 1) - (ScreenHeight >> 1);

		if (Pcamera.top < 0)
			Pcamera.top = 0;

		Pcamera.bottom = Pcamera.top + ScreenHeight;

		if (Pcamera.bottom > map[mapIndex].size.y)
		{
			Pcamera.bottom = map[mapIndex].size.y;
			Pcamera.top = Pcamera.bottom - ScreenHeight;
		}
	}
	else
	{
		Pcamera.top = 0;
		Pcamera.bottom = map[mapIndex].size.y;
	}

	int tmp;
	tmp = Pcamera.left - camera.left;
	if (tmp < 0)
	{
		camera.left += max(-SCROLL_SPEED, tmp);
		camera.right = ScreenWidth + camera.left;
	}
	else if (tmp > 0)
	{
		camera.left += min(SCROLL_SPEED, tmp);
		camera.right = ScreenWidth + camera.left;
	}

	tmp = Pcamera.top - camera.top;
	if (tmp < 0)
	{
		camera.top += max(-SCROLL_SPEED, tmp);
		camera.bottom = ScreenHeight + camera.top;
	}
	else if (tmp > 0)
	{
		camera.top += min(SCROLL_SPEED, tmp);
		camera.bottom = ScreenHeight + camera.top;
	}
	curPos.x -= (camera.left - cameraP.x);
	curPos.y -= (camera.top - cameraP.y);
}

void SetCamera(POINT FakeMouse)
{
	RECT Pcamera;
	POINT cameraP;
	cameraP.x = camera.left;
	cameraP.y = camera.top;
	if (ScreenWidth < map[mapIndex].size.x)
	{
		Pcamera.left = ((FakeMouse.x + camera.left + FakePlayer.x) >> 1) - (ScreenWidth >> 1);
		if (Pcamera.left < 0)
			Pcamera.left = 0;
		Pcamera.right = Pcamera.left + ScreenWidth;
		if (Pcamera.right > map[mapIndex].size.x)
		{
			Pcamera.right = map[mapIndex].size.x;
			Pcamera.left = Pcamera.right - ScreenWidth;
		}
	}
	else
	{
		Pcamera.left = 0;
		Pcamera.right = map[mapIndex].size.x;
	}
	if (ScreenHeight < map[mapIndex].size.y)
	{
		Pcamera.top = ((FakeMouse.y + camera.top + FakePlayer.y) >> 1) - (ScreenHeight >> 1);

		if (Pcamera.top < 0)
			Pcamera.top = 0;

		Pcamera.bottom = Pcamera.top + ScreenHeight;

		if (Pcamera.bottom > map[mapIndex].size.y)
		{
			Pcamera.bottom = map[mapIndex].size.y;
			Pcamera.top = Pcamera.bottom - ScreenHeight;
		}
	}
	else
	{
		Pcamera.top = 0;
		Pcamera.bottom = map[mapIndex].size.y;
	}

	int tmp;
	tmp = Pcamera.left - camera.left;
	if (tmp < 0)
	{
		camera.left += max(-SCROLL_SPEED, tmp);
		camera.right = ScreenWidth + camera.left;
	}
	else if (tmp > 0)
	{
		camera.left += min(SCROLL_SPEED, tmp);
		camera.right = ScreenWidth + camera.left;
	}

	tmp = Pcamera.top - camera.top;
	if (tmp < 0)
	{
		camera.top += max(-SCROLL_SPEED, tmp);
		camera.bottom = ScreenHeight + camera.top;
	}
	else if (tmp > 0)
	{
		camera.top += min(SCROLL_SPEED, tmp);
		camera.bottom = ScreenHeight + camera.top;
	}
	FakeMouse.x -= (camera.left - cameraP.x);
	FakeMouse.y -= (camera.top - cameraP.y);
}

bool KeyProcess()
{
	if DIKEYDOWN(DIK_ESCAPE)
	{
		return true;
	}
	if DIKEYDOWN(DIK_DOWN)
	{
		FakePlayer.y += 5;
		FakePlayer.y = min(map[mapIndex].size.y, FakePlayer.y);
	}
	if DIKEYDOWN(DIK_UP)
	{
		FakePlayer.y -= 5;
		FakePlayer.y = max(0, FakePlayer.y);
	}
	if DIKEYDOWN(DIK_LEFT)
	{
		FakePlayer.x -= 5;
		FakePlayer.x = max(0, FakePlayer.x);
	}
	if DIKEYDOWN(DIK_RIGHT)
	{
		FakePlayer.x += 5;
		FakePlayer.x = min(map[mapIndex].size.x, FakePlayer.x);
	}
	return false;
}

int GameMainSimCode()
{
	if (LBDOWN)
	{
		int tmpx;
		int tmpy;
		int tmpl;
		bListLast->next = (LPBULLET)malloc(sizeof(BULLET));
		bListLast = bListLast->next;
		bListLast->pos = FakePlayer;
		bListLast->type = BT0;
		bListLast->next = NULL;
		tmpx = curPos.x + camera.left - FakePlayer.x;
		tmpy = curPos.y + camera.top - FakePlayer.y;
		tmpl = (int)sqrt(tmpx * tmpx + tmpy * tmpy);
		bListLast->speed.x = tmpx * BULLET_SPEED / tmpl;
		bListLast->speed.y = tmpy * BULLET_SPEED / tmpl;
		float angle = acos(((float)tmpy) / ((float)tmpl));
		angle *= 360;
		angle /= PI;
		bListLast->angle = (DWORD)angle;
		if (tmpx < 0)
			bListLast->angle = 360 - bListLast->angle;
	}
	if RBDOWN
	{
		return 1;
	}
	SetCamera();

	monster_state += 1;
	monster_state %= 16;

	POINT monPos = { 500,1000 };
	DrawObject(TYPE_MAP, 0, monPos, 0, 0);
	DrawObject(TYPE_MONSTER, 0, monPos, 0, monster_state);
	monPos = { 1000,500 };
	DrawObject(TYPE_OBJECT, 1, monPos, 0, 0);
	LPBULLET btmp = &bulletList;
	for (LPBULLET bi = bulletList.next; bi; bi = btmp->next)
	{
		if (bi->pos.x < 0 || bi->pos.x > map[mapIndex].size.x || bi->pos.y < 0 || bi->pos.y > map[mapIndex].size.y)
		{
			btmp->next = bi->next;
			free(bi);
			continue;
		}
		bi->pos.x += bi->speed.x;
		bi->pos.y += bi->speed.y;

		DrawObject(TYPE_BULLET, 0, bi->pos, 3, 0);
		btmp = bi;
	}
	bListLast = btmp;
	DrawObject(TYPE_PLAYER, 0, FakePlayer, 1, monster_state >> 2);
	monPos.x = 0;
	monPos.y = 0;
	return 0;
}

#else

inline void SetCamera(POINT player)
{
	RECT Pcamera;
	POINT cameraP;
	cameraP.x = camera.left;
	cameraP.y = camera.top;
	if (ScreenWidth < map[mapIndex].size.x)
	{
		Pcamera.left = ((curPos.x + camera.left + player.x) >> 1) - (ScreenWidth >> 1);
		if (Pcamera.left < 0)
			Pcamera.left = 0;
		Pcamera.right = Pcamera.left + ScreenWidth;
		if (Pcamera.right > map[mapIndex].size.x)
		{
			Pcamera.right = map[mapIndex].size.x;
			Pcamera.left = Pcamera.right - ScreenWidth;
		}
	}
	else
	{
		Pcamera.left = 0;
		Pcamera.right = map[mapIndex].size.x;
	}
	if (ScreenHeight < map[mapIndex].size.y)
	{
		Pcamera.top = ((curPos.y + camera.top + player.y) >> 1) - (ScreenHeight >> 1);

		if (Pcamera.top < 0)
			Pcamera.top = 0;

		Pcamera.bottom = Pcamera.top + ScreenHeight;

		if (Pcamera.bottom > map[mapIndex].size.y)
		{
			Pcamera.bottom = map[mapIndex].size.y;
			Pcamera.top = Pcamera.bottom - ScreenHeight;
		}
	}
	else
	{
		Pcamera.top = 0;
		Pcamera.bottom = map[mapIndex].size.y;
	}

	int tmp;
	tmp = Pcamera.left - camera.left;
	if (tmp < 0)
	{
		camera.left += max(-SCROLL_SPEED, tmp);
		camera.right = ScreenWidth + camera.left;
	}
	else if (tmp > 0)
	{
		camera.left += min(SCROLL_SPEED, tmp);
		camera.right = ScreenWidth + camera.left;
	}

	tmp = Pcamera.top - camera.top;
	if (tmp < 0)
	{
		camera.top += max(-SCROLL_SPEED, tmp);
		camera.bottom = ScreenHeight + camera.top;
	}
	else if (tmp > 0)
	{
		camera.top += min(SCROLL_SPEED, tmp);
		camera.bottom = ScreenHeight + camera.top;
	}
	curPos.x -= (camera.left - cameraP.x);
	curPos.y -= (camera.top - cameraP.y);
}

#endif

int LoadBitmapFile(BITMAP_FILE_PTR bitmap, const char* filename)
{
	int file_handle, index;
	UCHAR* temp_buffer = NULL;
	OFSTRUCT file_data;

	if ((file_handle = OpenFile(filename, &file_data, OF_READ)) == -1)
		return -1;

	_lread(file_handle, &bitmap->header, sizeof(BITMAPFILEHEADER));
	if (bitmap->header.bfType != 0x4D42)
	{
		_lclose(file_handle);
		return -1;
	}

	_lread(file_handle, &bitmap->info, sizeof(BITMAPINFOHEADER));

	if (bitmap->info.biBitCount == 8)
	{
		_lread(file_handle, &bitmap->palette, 256 * sizeof(PALETTEENTRY));
		for (index = 0; index < 256; index++)
		{
			int temp_color = bitmap->palette[index].peRed;
			bitmap->palette[index].peRed = bitmap->palette[index].peBlue;
			bitmap->palette[index].peBlue = temp_color;
			bitmap->palette[index].peFlags = PC_NOCOLLAPSE;
		}
	}
	_llseek(file_handle, -(int)(bitmap->info.biSizeImage), SEEK_END);

	int RowLength = (bitmap->info.biBitCount >> 3) * (bitmap->info.biWidth);
	while (RowLength % 4)
		RowLength++;

	if (bitmap->info.biBitCount == 8 ||
		bitmap->info.biBitCount == 16 ||
		bitmap->info.biBitCount == 24 ||
		bitmap->info.biBitCount == 32)
	{
		if (bitmap->buffer)
		{
			free(bitmap->buffer);
			bitmap->buffer = NULL;
		}


		if (!(bitmap->buffer = (UCHAR*)malloc(bitmap->info.biSizeImage)))
		{
			_lclose(file_handle);
			return -1;
		}
		for (int i = bitmap->info.biHeight; i; i--)
			_lread(file_handle, ((bitmap->buffer) + (i - 1) * RowLength), RowLength);
	}
	else
	{
		_lclose(file_handle);
		return -1;
	}
	_lclose(file_handle);
	return 0;
}

int UnloadBitmapFile(BITMAP_FILE_PTR bitmap)
{
	if (bitmap->buffer)
		free(bitmap->buffer);
	bitmap->buffer = NULL;
	return 0;
}

LPDIRECTDRAWSURFACE7 DDrawCreateSurface(int width, int height, int memFlages)
{
	DDSURFACEDESC2 ddsdt;
	LPDIRECTDRAWSURFACE7 lpdds;
	memset(&ddsdt, 0, sizeof(ddsdt));
	ddsdt.dwSize = sizeof(ddsdt);
	ddsdt.dwFlags = DDSD_CAPS | DDSD_WIDTH | DDSD_HEIGHT | DDSD_CKSRCBLT;
	ddsdt.dwWidth = width;
	ddsdt.dwHeight = height;
	ddsdt.ddsCaps.dwCaps = DDSCAPS_OFFSCREENPLAIN | memFlages;
	ddsdt.ddckCKSrcBlt.dwColorSpaceHighValue = 0;
	ddsdt.ddckCKSrcBlt.dwColorSpaceLowValue = 0;
	if (FAILED(lpdd->CreateSurface(&ddsdt, &lpdds, NULL)))
		return NULL;

	DDCOLORKEY color_key;
	color_key.dwColorSpaceHighValue = 0;
	color_key.dwColorSpaceLowValue = 0;
	lpdds->SetColorKey(DDCKEY_SRCBLT, &color_key);

	return lpdds;
}

bool CreateSurfaceFromBitmap(LPSURFACE sur, int memFlags)
{
	BITMAP_FILE bmTmp;
	DDSURFACEDESC2 ddsdt;
	UCHAR blue, green, red;
	int color, i, j;
	int sfPos;
	int sfBase = 0;
	int bmPos = 0;
	bmTmp.buffer = NULL;

	if (LoadBitmapFile(&bmTmp, sur->name))
		return false;

	sur->size.x = bmTmp.info.biWidth / (sur->count);
	sur->size.y = bmTmp.info.biHeight;


	sur->sf = DDrawCreateSurface(bmTmp.info.biWidth, bmTmp.info.biHeight, memFlags);
	if (sur->sf == NULL)
		return false;
	memset(&ddsdt, 0, sizeof(ddsdt));
	ddsdt.dwSize = sizeof(ddsdt);
	CHECK(sur->sf->Lock(NULL, &ddsdt, DDLOCK_SURFACEMEMORYPTR | DDLOCK_WAIT, NULL));


	for (i = 0; i < bmTmp.info.biHeight; i++)
	{
		sfPos = (sfBase >> 2);
		for (j = 0; j < bmTmp.info.biWidth; j++)
		{
			blue = bmTmp.buffer[bmPos];
			bmPos++;
			green = bmTmp.buffer[bmPos];
			bmPos++;
			red = bmTmp.buffer[bmPos];
			bmPos++;
			color = blue + (green << 8) + (red << 16);
			((int*)ddsdt.lpSurface)[sfPos] = color;
			sfPos++;
			if (bmTmp.info.biBitCount == 32)
				bmPos++;
		}
		while (bmPos % 4)
			bmPos++;
		sfBase += ddsdt.lPitch;
	}
	CHECK(sur->sf->Unlock(NULL));

	UnloadBitmapFile(&bmTmp);

	return true;
}