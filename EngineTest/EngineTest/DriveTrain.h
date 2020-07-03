#pragma once

#include <vector>
#include "Engine.h"

using namespace std;

class DriveTrain
{
public:
	DriveTrain();
	~DriveTrain();

	float ReverseTorque(float brakePower);
	float AirResistance(float v);
	float DriveTireAcceleration(float torque, float I);
	float PropellerShaftVel(float wheelVel);
	float EngineVel(int gearNum, float ppVel);
	float MainTorque(float engineTorque, int gearNum, float clutch);
	tuple<float, float> EngineAndMission(float engineVel, float missionVel, float clutch);

	float CarSpeed(float tirePerimeter, float engineRpm, int gearNum);

	void Update(float clutch, float engineTorque, float rpm, int gearNum,float onlyEngineVel);
	void Draw(float clutch, int gearNum);
private:
	int gearNum = -1;
	
	float wheelTorque = 0.0f;
	float reverseTorque = 0.0f;
	float airResistance = 0.0f;
	float driveTireAcceleration = 0.0f;
	float driveTireVel = 0.0f;
	float propellerVel = 0.0f;
	float mainTorque = 0.0f;
	float engineVel = 0.0f;
	float missionVel = 0.0f;

	float speed = 0.0f;
};

