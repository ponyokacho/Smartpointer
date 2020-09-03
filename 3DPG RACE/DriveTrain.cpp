#include "DriveTrain.h"
#include <cmath>
#include "KeyMng.h"
#include "GameTask.h"
#include "Engine.h"
#include "ResourceMng.h"

DriveTrain::DriveTrain()
{
	gearNum = -1;
	for (int i = 0; i < MAX_GEAR; i++)
	{
		gearMaxSpeed[i] = (int)CarSpeedEngine(ACTUAL_MAX_RPM, i);
		gearMinSpeed[i] = CarSpeedEngine(IDOL_RPM, i);
		lpGameTask.SetGearMaxSpeed(gearMaxSpeed[i], i);
		lpGameTask.SetGearMinSpeed(gearMinSpeed[i], i);
		float tmp = (gearMinSpeed[i] / 60) * 1000;
		gearMinTireRpm[i] = tmp / TIRE_PERIMETER;
		gearMinTireVel[i] = (gearMinTireRpm[i] / 60) * (2 * PI);
	}
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

float DriveTrain::CarSpeedTire(float radPerSecT)
{
	float rpmT = radPerSecT * (60.0f / 2.0f * PI);
	speed = ((PI * TIRE_DIAMETER * rpmT) / 60.0f);
	speed = (speed / 3600) * 1000;
	return speed;
}

float DriveTrain::CarSpeedEngine(float rpmE, int gearNum)
{
	if (gearNum != -1)
	{
		float speed = TIRE_PERIMETER * (60 * rpmE) / (1000 * (G_RATIO[gearNum] * FINAL));
		return speed;
	}
	return 0.0f;
}

float DriveTrain::MaxTireVel(float speed)
{
	float oneMinDist = speed / 60.0f;		// km
	oneMinDist *= 1000;						// m
	float rpm = oneMinDist / TIRE_PERIMETER;
	float radPerSec = rpm / 60 * 2 * PI;
	return radPerSec;
}

tuple<float, float, float> DriveTrain::Update(float clutch, float engineTorque, float rpm, int gearNum, float onlyEngineVel)
{
	Sound();

	brakePower = lpGameTask.GetBrake();

	wheelTorque = mainTorque - reverseTorque;
	if (wheelTorque < 0)
	{
		wheelTorque = 0;
	}
	airResistance = AirResistance(((speed * 1000.0f) / (60.0f * 60.0f)));
	reverseTorque = ReverseTorque(brakePower)/* + airResistance*/;
	float maxTireVel = MaxTireVel(CarSpeedEngine(ACTUAL_MAX_RPM, gearNum));
	if (maxTireVel > driveTireVel)
	{
		driveTireVel += DriveTireAcceleration(mainTorque, 25.0f);
	}
	if (driveTireVel > 0)
	{
		driveTireVel -= DriveTireAcceleration(reverseTorque, 10.0f);
	}
	else
	{
		driveTireVel = 0.0f;
	}

	if (gearNum != -1)
	{
		if (driveTireVel < gearMinTireVel[gearNum])
		{
			driveTireVel = gearMinTireVel[gearNum];
		}

		if (driveTireVel > maxTireVel)
		{
			driveTireVel = maxTireVel;
		}
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

	actualEngineRpm = (engineVel / (2 * PI) * 60);
	lpGameTask.SetActualRpm(actualEngineRpm);

	// アイドリング
	if (actualEngineRpm < IDOL_RPM)
	{
		actualEngineRpm = IDOL_RPM;
	}

	if (gearNum != -1)
	{
		speed = CarSpeedEngine(actualEngineRpm,gearNum);
	}
	else
	{
		speed = CarSpeedTire(driveTireVel);
	}
	
	return forward_as_tuple(driveTireVel,wheelTorque,speed);
}

void DriveTrain::Draw(float clutch, int gearNum)
{
	int rS = 420 - clutch * 255;
	//DrawBox(250, 420, 300, rS, 0x00ff00, true);
	//DrawBox(250, 420, 300, 420 - 255, 0xffffff, false);

	//// エンジン回転
	////DrawBox(145, 420, 175, 420 - 255, 0xffffff, false);
	////DrawBox(145, 420, 175, engineVel * 0.0005, 0xff0000, true);
	//// ミッション回転
	////DrawBox(175, 420, 205, 420 - 255, 0xffffff, false);

	DrawString(210, 440, "DrivingForce", 0xff0000);

	DrawFormatString(210, 460, 0xffffff, "brake:%.2f", brakePower);

	//DrawFormatString(210, 460, 0xffffff, "aEngineRpm:%.2f", actualEngineRpm);

	//DrawFormatString(360, 460, 0xffffff, "driveTireVel:%.2f", driveTireVel);

	//DrawFormatString(360, 520, 0xffffff, "airResistance:%.2f", airResistance);

	//DrawFormatString(600, 460, 0xffffff, "engineVel:%.2f", engineVel);
	//DrawFormatString(600, 480, 0xffffff, "missionVel:%.2f", missionVel);

	//DrawFormatString(600, 500, 0xffffff, "wheelTorque:%.2f", wheelTorque);

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
	ChangeVolumeSoundMem(255 * volume / 100, SOUND_ID("sounds/car_idoling.mp3"));
	SetFrequencySoundMem(freq + (engineVel / (2 * PI) * 60) * 20, SOUND_ID("sounds/car_idoling.mp3"));
}