#include "DriveTrain.h"
#include <cmath>
#include "KeyMng.h"
#include "GameTask.h"
#include "Engine.h"
#include "ResourceMng.h"

DriveTrain::DriveTrain()
{
	gearNum = -1;
	ChangeVolumeSoundMem(255 * 80 / 100, SOUND_ID("sounds/car_idoling.mp3"));
	PlaySoundMem(SOUND_ID("sounds/car_idoling.mp3"), DX_PLAYTYPE_LOOP);
}

DriveTrain::~DriveTrain()
{
}

float DriveTrain::ReverseTorque(float brakePower)
{
	float rT = (brakePower * TIRE_DIAMETER) + (ROLLING_FRICTION * ONE_TIRE_LOAD * TIRE_DIAMETER);
	return rT;
}

float DriveTrain::AirResistance(float v)
{
	float D = (1.0f / 2.0f) * AIR_DENSITY * pow(v, 2) * FRONTAL_PROJECTED_AREA * CD;
	return D;
}

float DriveTrain::DriveTireAcceleration(float torque, float I)
{
	float tA = torque / I * DT;
	return tA;
}

float DriveTrain::EngineToMinssionVel(float wheelVel)
{
	float ppVel = FINAL * (wheelVel * 2) / 2;
	return ppVel;
}

float DriveTrain::EngineVel(int gearNum,float ppVel)
{
	if (gearNum != -1)
	{
		float eVel = G_RATIO[gearNum] * ppVel;
		return eVel;
	}
	return 0;
}


float DriveTrain::MainTorque(float engineTorque, int gearNum, float clutch)
{
	if (gearNum != -1)
	{
		float mainTorque = (engineTorque / 2) * (G_RATIO[gearNum] * FINAL) * clutch;	// 片輪のみのトルク
		return mainTorque;
	}
	return 0;
}

tuple<float, float> DriveTrain::EngineAndMission(float engineVel, float missionVel, float clutch)
{
	if (engineVel > missionVel)
	{
		missionVel += engineVel * 0.005f * clutch;
		engineVel -= engineVel * 0.005f * clutch;
	}
	else if(engineVel < missionVel)
	{
		missionVel -= engineVel * 0.005f * clutch;
		engineVel += engineVel * 0.005f * clutch;
	}
	return forward_as_tuple(engineVel, missionVel);
}

float DriveTrain::MaxCarSpeed(int gearNum)
{
	if (gearNum != -1)
	{
		float speed = TIRE_PERIMETER * (60 * ACTUAL_MAX_RPM) / (1000 * (G_RATIO[gearNum] * FINAL));
		return speed;
	}
	return 0;
}

float DriveTrain::MaxTireVel(float speed)
{
	float oneMinDist = speed / 60.0f;		// km
	oneMinDist *= 1000;						// m
	float rpm = oneMinDist / TIRE_PERIMETER;
	float radPerSec = rpm / 60 * 2 * PI;
	return radPerSec;
}

tuple<float, float> DriveTrain::Update(float clutch, float engineTorque, float rpm, int gearNum, float onlyEngineVel)
{
	Sound();
	wheelTorque = mainTorque - reverseTorque;
	if (wheelTorque < 0)
	{
		wheelTorque = 0;
	}
	airResistance = AirResistance(((speed * 1000.0f) / (60.0f * 60.0f)));
	reverseTorque = ReverseTorque(0)/* + airResistance*/;
	if (MaxTireVel(MaxCarSpeed(gearNum)) > driveTireVel)
	{
		driveTireVel += DriveTireAcceleration(mainTorque, 20.0f);
	}
	if (driveTireVel > 0)
	{
		driveTireVel -= DriveTireAcceleration(reverseTorque, 10.0f);
	}
	else
	{
		driveTireVel = 0.0f;
	}
	engineToMinssionVel = EngineToMinssionVel(driveTireVel);
	mainTorque = MainTorque(engineTorque, gearNum, clutch);		// 片輪のみの場合のトルク
	
	engineVel = EngineVel(gearNum, engineToMinssionVel);
	if (clutch == 0.0f)
	{
		engineVel = onlyEngineVel;
		reverseMissionVel = missionVel * 0.001f;
		missionVel -= reverseMissionVel;
	}
	else if (clutch != 1.0f)
	{
		tie(engineVel, missionVel) = EngineAndMission(engineVel, missionVel, clutch);
	}
	else 
	{
		missionVel = engineVel;
	}
	
	return forward_as_tuple(driveTireVel,wheelTorque);
}

void DriveTrain::Draw(float clutch, int gearNum)
{
	int rS = 420 - clutch * 255;
	DrawBox(250, 420, 300, rS, 0x00ff00, true);
	DrawBox(250, 420, 300, 420 - 255, 0xffffff, false);

	//// エンジン回転
	//DrawBox(145, 420, 175, 420 - 255, 0xffffff, false);
	//DrawBox(145, 420, 175, engineVel * 0.0005, 0xff0000, true);
	//// ミッション回転
	//DrawBox(175, 420, 205, 420 - 255, 0xffffff, false);

	DrawString(210, 440, "DrivingForce", 0xff0000);

	DrawFormatString(210, 460, 0xffffff, "RightStick:%.2f", clutch);

	DrawFormatString(360, 460, 0xffffff, "driveTireVel:%.2f", driveTireVel);

	DrawFormatString(360, 520, 0xffffff, "airResistance:%.2f", airResistance);

	DrawFormatString(600, 460, 0xffffff, "engineVel:%.2f", engineVel);
	DrawFormatString(600, 480, 0xffffff, "missionVel:%.2f", missionVel);

	DrawFormatString(600, 500, 0xffffff, "wheelTorque:%.2f", wheelTorque);

	

	if (gearNum != -1)
	{
		DrawFormatString(210, 500, 0xffffff, "Gear:%d", gearNum + 1);
	}
	else
	{
		DrawString(210, 500, "Gear:N",0xffffff);
	}
}

void DriveTrain::Sound()
{
	SetFrequencySoundMem(freq + (engineVel / (2 * PI) * 60) * 20, SOUND_ID("sounds/car_idoling.mp3"));
}