#pragma once

class Camera;
class Player;
class Enemy;
class Primitive;
class Polygon_;

class GameTask
{
private:
	// ----- ｺﾝｽﾄﾗｸﾀ群
	GameTask(){}	// ﾃﾞﾌｫﾙﾄｺﾝｽﾄﾗｸﾀをprivateにして外部から生成できない様にする
	GameTask(const GameTask&){}					// ----- ｺﾋﾟｰｺﾝｽﾄﾗｸﾀをprivate化
	GameTask& operator=(const GameTask&){}		// ----- 代入演算子のｵｰﾊﾞｰﾗｲﾄﾞをprivate化
	~GameTask(){}	// ﾃﾞｽﾄﾗｸﾀ

	// 画面遷移用
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

	// ｹﾞｰﾑﾙｰﾌﾟ用関数ﾎﾟｲﾝﾀｰ
	//int (GameTask::*gLoopPtr)(void);

public:
	static int SCREEN_SIZE_X;
	static int SCREEN_SIZE_Y;

	int SystemInit(void);
	static GameTask &GetInstance(void){	
		static GameTask gInstance;	// GameTaskの実体を生成。gInstanceに保存する
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