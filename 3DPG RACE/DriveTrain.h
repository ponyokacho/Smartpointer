#pragma once

#include <vector>
#include "Engine.h"

using namespace std;

class DriveTrain
{
public:
	DriveTrain();
	~DriveTrain();

	int GetSoundVol() { return volume; }
	void SetSoundVol(int vol) { volume = vol; }

	float ReverseTorque(float brakePower);
	float AirResistance(float v);
	float DriveTireAcceleration(float torque, float I);
	float EngineToMinssionVel(float wheelVel);
	float EngineVel(int gearNum, float ppVel);
	float MainTorque(float engineTorque, int gearNum, float clutch);
	tuple<float, float> EngineAndMission(float engineVel, float missionVel, float clutch);

	float CarSpeedTire(float radPerSecT);
	float CarSpeedEngine(float rpmE,int gearNum);
	float MaxTireVel(float speed);

	tuple<float, float> Update(float clutch, float engineTorque, float rpm, int gearNum,float onlyEngineVel);
	void Draw(float clutch, int gearNum);
	void Sound();

private:
	int gearNum = -1;
	
	float wheelTorque = 0.0f;
	float wheelTorqueDelta = 0.0f;
	float reverseTorque = 0.0f;
	float airResistance = 0.0f;
	float brakePower = 0.0f;
	float driveTireVel = 0.0f;
	float engineToMinssionVel = 0.0f;
	float mainTorque = 0.0f;
	float engineVel = 0.0f;
	float missionVel = 0.0f;
	float reverseMissionVel = 0.0f;
	float actualEngineRpm = 0.0f;
	float speed = 0.0f;
	array<float, 5>gearMinSpeed = { 0.0f };
	array<float, 5>gearMaxSpeed = { 0.0f };
	array<float, 5>gearMinTireRpm = { 0.0f };
	array<float, 5>gearMinTireVel = { 0.0f };

	float freq = 50000;
	int volume = 100;
	int count = 0;
};

