#pragma once

class Camera;
class Player;
class Enemy;
class Primitive;
class Polygon_;

class GameTask
{
private:
	// ----- �ݽ�׸��Q
	GameTask(){}	// ��̫�ĺݽ�׸���private�ɂ��ĊO�����琶���ł��Ȃ��l�ɂ���
	GameTask(const GameTask&){}					// ----- ��߰�ݽ�׸���private��
	GameTask& operator=(const GameTask&){}		// ----- ������Z�q�̵��ްײ�ނ�private��
	~GameTask(){}	// �޽�׸�

	// ��ʑJ�ڗp
	enum GAME_MODE{
		GAME_INIT,
		GAME_MAIN,
		GAME_OVER,
		GAME_CLEAR
	};
	GAME_MODE gameMode;

	int GameInit(void);
	int GameMain(void);
	int GameOver(void);
	int GameClear(void);

	// �ް�ٰ�ߗp�֐��߲���
	//int (GameTask::*gLoopPtr)(void);

public:
	static int SCREEN_SIZE_X;
	static int SCREEN_SIZE_Y;

	int SystemInit(void);
	static GameTask &GetInstance(void){	
		static GameTask gInstance;	// GameTask�̎��̂𐶐��BgInstance�ɕۑ�����
		return gInstance;
	}
	int Update(void);
	int animCnt;

	Camera* cam;
	Player* player;
	Enemy* enemy[20];
	Primitive* prim;

	Polygon_* poly;
};