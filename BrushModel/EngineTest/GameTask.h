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
	VECTOR2 TireForce(float slipRate, float slipAngle);

private:
	static GameTask* s_Instance;

	float Ks = 83000;
	float Ktheta = 54000;

	float slipRate = -1.0f;
	float slipAngle = 0.0f;

	int count = 0;

	// Žw’è’l
	float slipRate_a = 0.0f;
	float slipAngle_a = 0.0f;

	bool isDrive = true;
	bool tmp = true;

	vector<shared_ptr<Drawpoint>>dp;
	vector<shared_ptr<Drawpoint>>dp_a;

	VECTOR2 tireForce = { 0.0f,0.0f };
	VECTOR2 tireForce_a = { 0.0f,0.0f };
	VECTOR2 tireForce_a_old = { 0.0f,0.0f };
};

