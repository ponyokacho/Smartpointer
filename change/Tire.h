#pragma once

#include "VECTOR2.h"
#include "DxLib.h"
#include <tuple>

using namespace std;

class Tire
{
public:
	Tire();
	~Tire();

	void Init();

	float GetPitchLoad() 
	{
		return np;
	}
	float GetRollLoad()
	{
		return nr;
	}

	const float& GetWheelAngle()
	{
		return wheelAngle;
	}

	float VerticalForceAtWheelPitch(float pitch);
	float VerticalForceAtWheelRoll(float roll);
	float InertialForce(float engineTorque, int gearNum);
	float CentrifugalForce(float v, float r, float m);
	float SlipRatio(VECTOR2 v,float rv,float wheelAngle);
	float SlipAngle(VECTOR2 dirVec, VECTOR2 yawVec);
	VECTOR2 TireForce(float slipRatio, float slipAngle);
	VECTOR2 FrontWheelAngle(VECTOR2 v,float steering);
	float WheelRot(float kph);

	void Draw();
	void Update();

	float Cross(VECTOR2 va, VECTOR2 vb);
	float Dot(VECTOR2 va, VECTOR2 vb);
	float TurningRadius(float wheelAngle);

private:
	float wheelAngle = 0.0f;
	float turnRad = 0.0f;

	float sAngle = 0.0f;

	bool driveFlag = false;

	VECTOR2 posCenter = {645,230};

	VECTOR2 dirVec = { 0.0f,1.0f };
	VECTOR2 fWheelVec = { 0.0f,1.0f };
	VECTOR2 fWheelVecRot = { 0.0f,1.0f };
	VECTOR2 rWheelVec = { 0.0f,1.0f };

	struct Front {
		struct L {
			VECTOR2 tireForce = { 0.0f,1.0f };
			VECTOR2 pos = { 0.0f,1.0f };
			VECTOR2 preYaw = { 0.0f,1.0f };
			float slipAngle = 0.0f;
			float slipRatio = 0.0f;
			float load = 0.0f;
		};
		struct R {
			VECTOR2 tireForce = { 0.0f,1.0f };
			VECTOR2 pos = { 0.0f,1.0f };
			VECTOR2 preYaw = { 0.0f,1.0f };
			float slipAngle = 0.0f;
			float slipRatio = 0.0f;
			float load = 0.0f;
		};
		VECTOR2 centerPos = { 0.0f,1.0f };

		L left;
		R right;
	};
	Front front;

	struct Rear {
		struct L {
			VECTOR2 tireForce = { 0.0f,1.0f };
			VECTOR2 pos = { 0.0f,1.0f };
			float slipAngle = 0.0f;
			float preSlipAngle = 0.0f;
			float slipRatio = 0.0f;
			float load = 0.0f;
		};
		struct R {
			VECTOR2 tireForce = { 0.0f,1.0f };
			VECTOR2 pos = { 0.0f,1.0f };
			float slipAngle = 0.0f;
			float preSlipAngle = 0.0f;
			float slipRatio = 0.0f;
			float load = 0.0f;
		};
		VECTOR2 centerPos = {0.0f,1.0f};

		L left;
		R right;
	};
	Rear rear;

	int lr = 0;
	int beforeLr = 0;
	float deg = 0.0f;

	VECTOR2 allTireForce = { 0.0f,1.0f };
	VECTOR2 vectorSpeed = { 0.0f,1.0f };

	VECTOR2 dirVecRotVec2 = { 0.0f,1.0f };

	VECTOR2 beforeVec = { 0.0f,1.0f };
	VECTOR2 vectorSpeedRot = { 0.0f,1.0f };

	float driveTireVel = 0.0f;
	float nonDriveTireVel = 0.0f;
	float speed = 0.0f;
	float acceleration = 0.0f;

	float np = 0.0f;
	float nr = 0.0f;

	float deltaD = 0.0f;
	
	int count = 0;

	float wheelTorque = 0.0f;
	int sign = 1;

	int volCount = 0;
	int volume = 0;

	float steering = 0.0f;
	int gearNum = -1;
	float accel = 0.0f;
	float engineTorque = 0.0f;
};
 
