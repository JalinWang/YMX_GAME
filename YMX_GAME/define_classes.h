#pragma once 

enum ObjType {
	ENUM_MAP, ENUM_MONSTOR, ENUM_PLAYER, ENUM_WALL, ENUM_BULLET
};

class BaseObject
{
public:
	virtual void Delete() const;//ɾ������
	virtual void Update() const;//�������
	virtual void OnCollision(BaseObject* obj) const;//������ײ

protected:
	int positionX, positionY;//�ᡢ������
	int velocityX, velocityY;//���������ٶ�
	ObjType type;
	int typeId;//��������ID
	int uniqueId;//ÿ�������һ�޶���ID
	int shapeType;//������״�����ࣺ0ΪԲ�Σ�1Ϊ����
	int radius;//Բ�ΰ뾶
	int width;//���ο��
	int height;//���θ߶�
};

class Charater : public BaseObject
{
public:
	virtual void Update() const;//�������
	virtual void OnCollision(BaseObject* obj) const;//������ײ

	virtual void Fire() const;//����
	virtual void Move() const;//�ƶ�

protected://�ȼ��赯ϻ�ӵ�����
	int bulletType, bulletPeriod, lastFireTime;//�ӵ����͡��ӵ��������ڡ��ϴο���ʱ��
	float bulletPeriodRate;//�ӵ����������ٶȱ���(�����ӵ������ٶ�)
	int HP, HPIncrement, lastTime;//Ѫ����Ѫ������(�����ɸ�)��Ѫ�����ٱ��ʡ�buff����ʱ��
	float velocityRate;//�ٶȱ���(����������ƶ��ٶȴ�С)
};

class Player : public Charater
{
public:
	void Update() const override;//�������
	void OnCollision(BaseObject* obj) const override;//������ײ

	void Fire() const override;//����
	void Move() const override;//�ƶ�
};

class Monster : public Charater
{
public:
	void Update() const override;//�������
	void OnCollision(BaseObject* obj) const override;//������ײ

	void Fire() const override;//����
	void Move() const override;//�ƶ�
};

class Item : public BaseObject
{
public:
	void Use(Player* user) const;//ʹ����Ʒ

	void Update() const override;//�������
	void OnCollision(BaseObject* obj) const override;//������ײ
};

class Bullet : BaseObject
{
public:
	void Update() const override;//�������
	void OnCollision(BaseObject* obj) const override;//������ײ
	
protected:
	int attack;//������
};


//map Event
class Event{
public:
	int occurTime; //����ʱ��
	int typeId;
	ObjType type;
	int posX, posY;
	bool operator < (const Event &t)
	{
		return occurTime < t.occurTime;
	}
	BaseObject *Occur() const
	{
		BaseObject *ret;
		switch (type)
		{
		case ENUM_MAP:
			break;
		case ENUM_MONSTOR:
			break;
		case ENUM_PLAYER:
			ret = new Player();
			break;
		case ENUM_WALL:
			break;
		case ENUM_BULLET:
			break;
		default:
			break;
		}
		return ret;
	}
};