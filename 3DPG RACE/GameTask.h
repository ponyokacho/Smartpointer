#pragma once

class Camera;
class Field;
class Player;

#include <list>

class GameTask
{
private:
	// ----- �ݽ�׸��Q
	GameTask(){}	// ��̫�ĺݽ�׸���private�ɂ��ĊO�����琶���ł��Ȃ��l�ɂ���
	GameTask(const GameTask&){}					// ----- ��߰�ݽ�׸���private��
	GameTask& operator=(const GameTask&){}		// ----- ������Z�q�̵��ްײ�ނ�private��
	~GameTask() {}	// �޽�׸�

	// ��ʑJ�ڗp
	enum GAME_MODE{
		GAME_INIT,
		GAME_TITLE,
		GAME_MAIN,
		GAME_OVER,
		GAME_CLEAR
	};
	GAME_MODE gameMode;

	int GameInit(void);
	int GameTitle(void);
	int GameMain(void);
	int GameOver(void);
	int GameClear(void);

	int newKey;
	int oldKey;
	int trgKey;

	// �ް�ٰ�ߗp�֐��߲���
	//int (GameTask::*gLoopPtr)(void);

	
	Camera* camera;	// ��׵�޼ު��
	Field* field;	// ̨���ފǗ�


public:
	static int SCREEN_SIZE_X;
	static int SCREEN_SIZE_Y;

	int ObjCnt;

	void addObjCnt();
	void removeObjCnt();

	int SystemInit(void);
	static GameTask &GetInstance(void){	
		static GameTask gInstance;	// GameTask�̎��̂𐶐��BgInstance�ɕۑ�����
		return gInstance;
	}
	int Update(void);

	Player* player;	// �ԊǗ�
};