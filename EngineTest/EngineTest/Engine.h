#pragma once

#include <array>
#include "DxLib.h"
#include "KeyMng.h"
#include <tuple>

constexpr float CHANGE_TORQUE = 9.806652;

constexpr int IDOL_RPM = 1000;
constexpr int MAX_RPM = 7999;

constexpr int MAX_THOUSAND = 7;

using namespace std;

class Engine
{
public:
	Engine();
	~Engine();

	float CalcForwardTorque(float topTorque,float bottomTorque,float topRpm,float bottomRpm,float T_rpm);		// エンジンが生む正のトルク
	float AccelTorque(float forwardTorque, float accel);		//アクセル開度によりトルクが増減
	float CalcAngularAccel(float I, float Torque);		// 角加速度計算
	float CalcRpm(float AA);							// rpm変換
	float CalcReverseTorque(float R,int rpm);			// 慣性モーメントによる負のトルク
	float CalcPower(float Torque,float rpm);			// パワー計算

	float ChangeUnit(float kgf);	

	tuple<float,float, float> Update(float accel);
	void Draw(float accel, float rightTrigger);

private:
	array<float, 10>t = {0,140,170,200,180,200,210,180,0}; // N・m

	int thousand = 1;
	float forwardTorque = 0.0f;
	float torque = 0.0f;
	float reverseTorque = 0.0f;
	float engineTorque = 0.0f;
	float angularAccel = 0.0f;
	float rpm = IDOL_RPM;
};

