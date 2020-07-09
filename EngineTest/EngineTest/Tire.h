#pragma once

#include "VECTOR2.h"


class Tire
{
public:
	Tire();
	~Tire();

	float VerticalForceAtWheelPitch(float pitch);
	float VerticalForceAtWheelRoll(float roll);
	float InertialForce(float engineTorque, int gearNum);
	float SlideRate(VECTOR2 v,float rv,float wheelAngle);
	float SlideAngle(VECTOR2 dirVec, VECTOR2 yawVec);
	VECTOR2 TireForce(float slideRate, float slideAngle);
	VECTOR2 FrontWheelAngle(VECTOR2 v,float steering);

	void Draw();
	void Update(float engineTorque,float steering,int gearNum,float accel,float driveTireVel);

	float Cross(VECTOR2 va, VECTOR2 vb);
	float Dot(VECTOR2 va, VECTOR2 vb);
	float TurningRadius(float wheelAngle);

private:
	float LOAD_FL;
	float LOAD_FR;
	float LOAD_RL;
	float LOAD_RR;

	float wheelAngle = 0.0f;
	float turnRad = 0.0f;

	float sAngle = 0.0f;

	bool driveFlag = false;

	VECTOR2 posCenter = {645,230};
	VECTOR2 posFl;
	VECTOR2 posFr;
	VECTOR2 posRl;
	VECTOR2 posRr;

	VECTOR2 treadDistanceVec = { 0,0 };
	VECTOR2 treadDistanceVecNorm = { 0,0 };
	float treadDistance = 0.0f;
	float wheelBaseDistance = 0.0f;

	VECTOR2 dirVec = { 0,0 };
	VECTOR2 yawVec = { 0,0 };
	VECTOR2 fWheelVec = { 0,0 };
	VECTOR2 rWheelVec = { 0,0 };
	VECTOR2 tireForce = { 0,0 };
};
 
