#include "DriveTrain.h"
#include <cmath>
#include "KeyMng.h"
#include "GameTask.h"

DriveTrain::DriveTrain()
{
	gearNum = -1;
	gRatio = { 3.626,2.188,1.541,1.213,1.000,0.767 };
}

DriveTrain::~DriveTrain()
{
}

float DriveTrain::ReverseTorque(float brakePower)
{
	float rT = (brakePower * TIRE_DIAMETER) + (ROLLING_FRICTION * (LOAD * G_ACCELERATION) * TIRE_DIAMETER);
	return rT;
}

float DriveTrain::AirResistance(float v)
{
	float D = 1.0f / 2.0f * AIR_DENSITY * pow(v, 2) * FRONTAL_PROJECTED_AREA * CD;
	return D;
}

float DriveTrain::TireAcceleration(float torque, float I)
{
	float tA = (torque / I) * DT;
	return tA;
}

float DriveTrain::PropellerShaftVel(float wheelVel)
{
	float ppVel = FINAL * wheelVel;
	return ppVel;
}

float DriveTrain::engineVel(float ppVel)
{
	float eVel = gRatio[gearNum] * ppVel;
	return 0.0f;
}



void DriveTrain::Update(float clutch, float engineTorque, float rpm, int gearNum)
{
	
}

void DriveTrain::Draw(float clutch, int gearNum)
{
	int rS = 420 - clutch * 255;
	DrawBox(250, 420, 300, rS, 0x00ff00, true);
	DrawBox(250, 420, 300, 420 - 255, 0xffffff, false);

	DrawString(210, 440, "DrivingForce", 0xff0000);

	DrawFormatString(210, 460, 0xffffff, "RightStick:%.2f", clutch);

	DrawFormatString(210, 480, 0xffffff, "torque:%.2f", mainTorque - reverseTorque);

	DrawFormatString(360, 460, 0xffffff, "wheelVel:%.2f", wheelReverseVel);

	DrawFormatString(360, 480, 0xffffff, "shaftVel:%.2f", shaftVel);

	DrawFormatString(510, 500, 0xffffff, "engineVel:%.2f", engineVel);

	DrawFormatString(510, 460, 0xffffff, "wheelRpm:%.2f", wheelRpm);

	DrawFormatString(510, 480, 0xffffff, "shaftRpm:%.2f", shaftRpm);

	DrawFormatString(660, 460, 0xffffff, "speed:%.2f", carSpeed);

	DrawFormatString(660, 500, 0xffffff, "missionVel:%.2f", missionVel);

	if (gearNum != -1)
	{
		DrawFormatString(210, 500, 0xffffff, "Gear:%d", gearNum + 1);
	}
	else
	{
		DrawString(210, 500, "Gear:N",0xffffff);
	}
}