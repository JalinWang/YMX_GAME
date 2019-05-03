#include <queue>
#include <string>
#include <vector>
#include <thread>
#include <chrono>
#include "define_classes.h"

using namespace std;

#define ROUND_0 0
int now;	//当前时间

//60帧/s -> 17ms/帧
#define PERIOD 17

//TODO: 改成单例模式
class ResModule{
public:
	vector<Event> LoadMap(ObjType type, int typeId);//加载地图事件
	void Load(BaseObject &obj, ObjType type, int typeId);//加载属性、贴图等
	void Init();//初始化，检查文件存在与否
}mRes;

class RenderModule{
public:
	void ShowIamge(BaseObject *obj);
}mRender;

class SceneModule{
public:
	vector<BaseObject *> objList;
	priority_queue<Event> heap;
	SceneModule();
	void Init()
	{
		vector<Event> eventsList = mRes.LoadMap(ENUM_MAP, ROUND_0);
		for(auto i : eventsList)
			heap.push(i);
	}
	void Run()
	{
		while(true)
		{
			//延时
			std::this_thread::sleep_for(std::chrono::milliseconds(PERIOD));
			while(heap.top().occurTime == now)
			{
				Event &t = heap.top();
				objList.push_back(t.Occur());
				heap.pop();
			}

			CollisionDetect();

			//显示地图背景
			//mRender.show_img(wall, background)
			
			//遍历更新物体并显示
			for(auto obj : objList)
			{
				obj->Update();
				mRender.ShowIamge(obj);
			}
		}
	}
	void CollisionDetect()
	{
		for(auto i : objList)
			for(auto j : objList)
				if(Detect(i, j))
				{
					i->OnCollision(j);
					j->OnCollision(i);
				}
	}
	bool Detect(BaseObject *i, BaseObject *j);//  判断
}mScene;

void Welcome();// 界面、欢迎信息等

int main()
{
	Welcome();
	mRes.Init();
	mScene.Init();
	//开个新线程后台运行
	thread backgoundThread(mScene.Run, &mScene);
	backgoundThread.detach();
	
}