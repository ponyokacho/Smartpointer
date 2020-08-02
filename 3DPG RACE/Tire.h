#pragma once

#include "VECTOR2.h"
#include "DxLib.h"

class Tire
{
public:
	Tire();
	~Tire();

	float VerticalForceAtWheelPitch(float pitch);
	float VerticalForceAtWheelRoll(float roll);
	float InertialForce(float engineTorque, int gearNum);
	float SlipRate(VECTOR2 v,float rv,float wheelAngle);
	float SlipAngle(VECTOR2 dirVec, VECTOR2 yawVec);
	VECTOR2 TireForce(float slipRate, float slipAngle);
	VECTOR2 FrontWheelAngle(VECTOR2 v,float steering);
	float WheelRot(float kph);

	void Draw();
	std::tuple<VECTOR2> Update(float engineTorque,float steering,int gearNum,float accel,float driveTireVel,float speed,VECTOR vectorSpeed);

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

	VECTOR2 treadDistanceVec = { 0,0 };
	VECTOR2 treadDistanceVecNorm = { 0,0 };
	float treadDistance = 0.0f;
	float wheelBaseDistance = 0.0f;

	float saveSlipRate = 0.0f;

	VECTOR2 dirVec = { 0,0 };
	VECTOR2 yawVec = { 0,0 };
	VECTOR2 fWheelVec = { 0,0 };
	VECTOR2 rWheelVec = { 0,0 };

	struct Front {
		struct L {
			VECTOR2 tireForce = { 0,0 };
			VECTOR2 pos = { 0,0 };
			VECTOR2 preYaw = { 0.0f,0.0f };
			float slipAngle = 0.0f;
			float slipRate = 0.0f;
		};
		struct R {
			VECTOR2 tireForce = { 0,0 };
			VECTOR2 pos = { 0,0 };
			VECTOR2 preYaw = { 0.0f,0.0f };
			float slipAngle = 0.0f;
			float slipRate = 0.0f;
		};
		VECTOR2 centerPos = { 0.0f,0.0f };

		L left;
		R right;
	};
	Front front;

	struct Rear {
		struct L {
			VECTOR2 tireForce = { 0,0 };
			VECTOR2 pos = { 0,0 };
			float slipAngle = 0.0f;
			float preSlipAngle = 0.0f;
			float slipRate = 0.0f;
		};
		struct R {
			VECTOR2 tireForce = { 0,0 };
			VECTOR2 pos = { 0,0 };
			float slipAngle = 0.0f;
			float preSlipAngle = 0.0f;
			float slipRate = 0.0f;
		};
		VECTOR2 centerPos = {0.0f,0.0f};

		L left;
		R right;
	};
	Rear rear;

	int lr = 0;
	float deg = 0.0f;

	VECTOR2 moveVec = { 0.0f,0.0f };
	VECTOR2 allTireForce = { 0.0f,0.0f };
	VECTOR2 vectorSpeed = { 0.0f,0.0f };
};
 
