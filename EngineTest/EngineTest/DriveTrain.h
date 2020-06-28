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
	float TireAcceleration(float torque, float I);
	float PropellerShaftVel(float wheelVel);
	float engineVel(float ppVel);

	void Update(float clutch, float engineTorque, float rpm, int gearNum);
	void Draw(float clutch, int gearNum);
private:

	vector<float>gRatio;
	float gearNum = -1;
};

