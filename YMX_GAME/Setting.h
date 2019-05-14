#pragma once
/*namespace nm_GameEngine {
	//class Setting;
	Setting setting;
}*/

int now;	//当前时间
/*
class Setting
{
public:
	~Setting();

	//60 frame/s -> 17ms/frame
	int period = 17;
	Setting();

public:
	Setting* getInstance();
private:
	Setting* instance = nullptr;
};*/

class Setting
{
public:
	//60 frame/s -> 17ms/frame
	static const int period = 17;
	static const int ROUND_0 = 0;
};

