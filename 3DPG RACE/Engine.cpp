#include "Engine.h"
#include <string>
#include "DxLib.h"
#include "GameTask.h"

Engine::Engine()
{
	Init();
}

Engine::~Engine()
{
}

void Engine::Init()
{
	rpm = IDOL_RPM;
}

float Engine::CalcForwardTorque(float topTorque, float bottomTorque, float topRpm, float bottomRpm, float T_rpm)
{
	float fT = bottomTorque + ((topTorque - bottomTorque) * (T_rpm - bottomRpm) / (topRpm - bottomRpm));
	return fT;
}

float Engine::AccelTorque(float forwardTorque, float accel)
{
	float T = forwardTorque * accel;
	return T;
}

float Engine::CalcAngularAccel(float I, float Torque)
{
	float angularAccel = Torque / I * DT;
	return angularAccel;
}

float Engine::CalcRpm(float AA)
{	
	float rpm = AA * 60 / PI;
	return rpm;
}

float Engine::CalcReverseTorque(float R, int rpm)
{
	float rT = R * rpm;
	return rT;
}

float Engine::CalcPower(float Torque, float rpm)
{
	float power = 2 * PI * Torque * rpm / 60 / 1000; 
	return power;
}

float Engine::ChangeUnit(float kgf)
{

	return 0.0f;
}

void Engine::Update()
{
	accel = lpGameTask.GetRT();
	//if (thousand < MAX_THOUSAND)
	{
		forwardTorque = CalcForwardTorque(t[thousand + 1], t[thousand], (thousand + 1) * 1000, thousand * 1000, rpm);
	}

	torque = AccelTorque(forwardTorque, accel);

	angularAccel += CalcAngularAccel(1, engineTorque);		// Iは動きを見ながら調整
	rpm = CalcRpm(angularAccel);

	if (rpm > MAX_RPM) rpm = MAX_RPM;
	if (rpm < IDOL_RPM) rpm = IDOL_RPM;

	if (rpm != IDOL_RPM)
	{
		reverseTorque = CalcReverseTorque(0.007, rpm);	// Rは動きを見ながら調整
	}
	else
	{
		reverseTorque = 0.0f;
	}
	engineTorque = torque - reverseTorque;

	thousand = rpm / 1000;

	lpGameTask.SetEngineTorque(torque);
	lpGameTask.SetRpm(rpm);
	lpGameTask.SetOnlyEngineVel(angularAccel);
}

void Engine::Draw()
{
	int rt = 420 - lpGameTask.GetRT();
	//DrawBox(50, 420, 100, rt, 0x00ff00, true);
	//DrawBox(50, 420, 100, 420 - 255, 0xffffff, false);

	//DrawString(10, 440, "Engine", 0xff0000);

	//DrawFormatString(10, 460, 0xffffff, "RightTrigger:%.2f", accel);

	//DrawFormatString(10, 480, 0xffffff, "Torque:%.2f", torque);

	//DrawFormatString(10, 500, 0xffffff, "rpm:%.2f", rpm);

	//DrawFormatString(10, 520, 0xffffff, "engineVel:%.2f", angularAccel);
}
