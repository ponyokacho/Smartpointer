#pragma once

#include <vector>
#include <memory>
#include "DxLib.h"
#include "VECTOR2.h"
#include "Drawpoint.h"

#define  lpGameTask GameTask::GetInstance()

constexpr int SCREEN_SIZE_X(960);
constexpr int SCREEN_SIZE_Y(540);

constexpr float G_ACCELERATION = 9.8; // m/s^2
constexpr float MU = 0.8f;
constexpr float LOAD = 1250;		// kg

constexpr float ONE_TIRE_LOAD = LOAD * G_ACCELERATION / 4;

using namespace std;



class GameTask
{
public:
	// ¼Ý¸ÞÙÄÝ
	static void Create(void);
	static GameTask& GetInstance(void)
	{
		Create();
		return *s_Instance;
	}

	int SystemInit();
	void GameInit();
	void Update();
	void Draw();

private:
	static GameTask* s_Instance;

	float Ks = 83000;
	float Ktheta = 54000;

	float slideRate = -1.0f;
	float slideAngle = 0.0f;

	bool isDrive = true;
	bool tmp = true;

	vector<shared_ptr<Drawpoint>>dp;

	VECTOR2 f = { 0,0 };
};

