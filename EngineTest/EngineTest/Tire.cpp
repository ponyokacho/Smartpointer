#include "Tire.h"
#include "DxLib.h"
#include "GameTask.h"
#include "ResourceMng.h"
#include <cmath>

Tire::Tire()
{
	posFl = { posCenter.x - T_OFFSET_X,posCenter.y - T_OFFSET_Y };
	posFr = { posCenter.x + T_OFFSET_X,posCenter.y - T_OFFSET_Y };
	posRl = { posCenter.x - T_OFFSET_X,posCenter.y + T_OFFSET_Y };
	posRr = { posCenter.x + T_OFFSET_X,posCenter.y + T_OFFSET_Y };
}

Tire::~Tire()
{
}

float Tire::VerticalForceAtWheelPitch(float pitch)
{
	// ピッチ
	float Np = 2 / WHEEL_BASE * pitch * HIGHT_OF_G;

	return Np;
}

float Tire::VerticalForceAtWheelRoll(float roll)
{
	// ロール
	float Nr = 2 / TREAD * roll * HIGHT_OF_G;

	return Nr;
}

float Tire::InertialForce(float engineTorque, int gearNum)
{
	// 慣性力(N)
	float a = (engineTorque * (G_RATIO[gearNum] * FINAL) / (TIRE_DIAMETER / 2.0f)) / (LOAD * G_ACCELERATION);
	return a;
}

float Tire::SlideRate(VECTOR2 v, float rv, float wheelAngle)
{
	// 滑り比を出すにはタイヤの角速度と車速を使用

	// Fタイヤが曲がっているとき、v = Vzcosθ + Vxsinθ
	// v:タイヤ進行速度
	// rv:タイヤ角速度

	float tireSpeed = 0.0f;
	float s = 0.0f;

	if (wheelAngle != 0.0f)
	{
		tireSpeed = v.y * cos(wheelAngle) + v.x * sin(wheelAngle);
	}
	else
	{
		tireSpeed = v.y;
	}

	if (driveFlag)
	{
		s = (rv * TIRE_DIAMETER - tireSpeed) / (rv * TIRE_DIAMETER);
	}
	else
	{
		s = (rv * TIRE_DIAMETER - tireSpeed) / tireSpeed;
	}
	return s;
}

float Tire::SlideAngle(VECTOR2 dirVec, VECTOR2 yawVec)
{
	float theta = atan(Cross(yawVec, dirVec) / Dot(yawVec, dirVec));
	return theta;
}

VECTOR2 Tire::TireForce(float slideRate, float slideAngle)
{
	float Ks = 83000;
	float Ktheta = 54000;
	VECTOR2 f = { 0,0 };

	if(driveFlag)
	{
		float lambda = sqrt(pow(slideRate, 2) + pow((Ktheta / Ks), 2) * pow((1 - slideRate), 2) * pow(tan(slideAngle), 2));
		float cTheta = -slideRate / lambda;
		float sTheta = (Ktheta * tan(slideAngle) * (1.0f - slideRate)) / (Ks * lambda);
		if (slideRate == 0.0f && lambda == 0.0f)
		{
			cTheta = 0.0f;
			sTheta = 0.0f;
		}
		float epsilon = 1 - (Ks / (3.0f * MU * ONE_TIRE_LOAD) * lambda);

		if (epsilon > 0)
		{
			f.y = Ks * slideRate * pow(epsilon, 2) - 6.0f * MU * ONE_TIRE_LOAD * cTheta * (1.0f / 6.0f - 1.0f / 2.0f * pow(epsilon, 2) + 1.0f / 3.0f * pow(epsilon, 3));
			f.x = Ktheta * (1 - slideRate) * tan(slideAngle) * pow(epsilon, 2) + 6.0f * MU * ONE_TIRE_LOAD * sTheta * (1.0f / 6.0f - 1.0f / 2.0f * pow(epsilon, 2) + 1.0f / 3.0f * pow(epsilon, 3));
		}
		else if (epsilon <= 0)
		{
			f.y = -MU * ONE_TIRE_LOAD * cTheta;
			f.x = MU * ONE_TIRE_LOAD * sTheta;
		}
	}
	else
	{
		float lambda = sqrt(pow(slideRate, 2) + pow((Ktheta / Ks), 2) * pow(tan(slideAngle), 2));
		float cTheta = -slideRate / lambda;
		float sTheta = (Ktheta * tan(slideAngle)) / (Ks * lambda);
		if (slideRate == 0.0f && lambda == 0.0f)
		{
			cTheta = 0.0f;
			sTheta = 0.0f;
		}
		float epsilon = 1 - (Ks / (3.0f * MU * ONE_TIRE_LOAD) * (lambda / (1 + slideRate)));

		if (epsilon > 0)
		{
			f.y = ((Ks * slideRate) / (1.0f + slideRate)) * pow(epsilon, 2) - 6.0f * MU * ONE_TIRE_LOAD * cTheta * (1.0f / 6.0f - 1.0f / 2.0f * pow(epsilon, 2) + 1.0f / 3.0f * pow(epsilon, 3));
			f.x = ((Ktheta * tan(slideAngle)) / (1 + slideRate)) * pow(epsilon, 2) + 6.0f * MU * ONE_TIRE_LOAD * sTheta * (1.0f / 6.0f - 1.0f / 2.0f * pow(epsilon, 2) + 1.0f / 3.0f * pow(epsilon, 3));
		}
		else if (epsilon <= 0)
		{
			f.y = -MU * ONE_TIRE_LOAD * cTheta;
			f.x = MU * ONE_TIRE_LOAD * sTheta;
		}
	}

	return VECTOR2(f.x,f.y);
}

VECTOR2 Tire::FrontWheelAngle(VECTOR2 v, float steering)
{
	VECTOR2 s;
	s.x = v.x * cos(steering) - v.y * sin(steering);
	s.y = v.x * sin(steering) + v.y * cos(steering);

	return s;
}

void Tire::Draw()
{
	DrawFormatString(SCREEN_SIZE_X - 200, 460, 0xffffff, "VFAW_F = %.2f", LOAD_FL);
	//DrawFormatString(SCREEN_SIZE_X / 2 + 300, 50, 0xffffff, "VFAW_FR = %.2f", LOAD_FR);
	DrawFormatString(SCREEN_SIZE_X - 200, 480, 0xffffff, "VFAW_R = %.2f", LOAD_RL);
	//DrawFormatString(SCREEN_SIZE_X / 2 + 300, SCREEN_SIZE_Y - 150 - 100, 0xffffff, "VFAW_RR = %.2f", LOAD_RR);



	DrawRotaGraph(posCenter.x, posCenter.y, 0.10f, 0, IMAGE_ID("images/body.png"), true);
	//DrawRotaGraph(posFl.x, posFl.y , 0.10f, wheelAngle, IMAGE_ID("images/tire.png"), true);
	//DrawRotaGraph(posFr.x, posFr.y, 0.10f, wheelAngle, IMAGE_ID("images/tire.png"), true);
	//DrawRotaGraph(posRl.x, posRl.y, 0.10f, 0, IMAGE_ID("images/tire.png"), true);
	//DrawRotaGraph(posRr.x, posRr.y, 0.10f, 0, IMAGE_ID("images/tire.png"), true);
	DrawCircle(posFl.x, posFl.y, 2, 0xffffff, true);
	DrawCircle(posFr.x, posFr.y, 2, 0xffffff, true);
	DrawCircle(posRl.x, posRl.y, 2, 0xffffff, true);
	DrawCircle(posRr.x, posRr.y, 2, 0xffffff, true);

	DrawLine(posCenter.x, posCenter.y, posCenter.x - dirVec.x, posCenter.y - dirVec.y, 0xff0000, 1);
	DrawLine(posCenter.x, posCenter.y, posCenter.x - yawVec.x, posCenter.y - yawVec.y, 0x00ff00, 1);
	DrawLine(posCenter.x, posCenter.y, posCenter.x - treadDistanceVec.x, posCenter.y - treadDistanceVec.y, 0x0000ff, 1);
	//DrawLine(outRearWheelPos.x, outRearWheelPos.y, outRearWheelPos.x + (treadDistanceVecNorm.x * turnRad), outRearWheelPos.y + (treadDistanceVecNorm.y * turnRad), 0xffff00, 1);

	DrawFormatString(600, 520, 0xffffff, "tF.x:%.2f,tF.y:%.2f", tireForce.x, tireForce.y);
}

void Tire::Update(float engineTorque, float steering, int gearNum, float accel,float driveTireVel)
{
	VECTOR2 Vfaw = VECTOR2(0, InertialForce(engineTorque, gearNum));
	float Np = VerticalForceAtWheelPitch(Vfaw.y);		// 符号は後で変更 タイヤ浮いてたら0
	if (accel)
	{
		LOAD_FL = ONE_TIRE_LOAD + Np;		// 符号は後で変更 タイヤ浮いてたら0
		LOAD_FR = ONE_TIRE_LOAD + Np;
		LOAD_RL = ONE_TIRE_LOAD - Np;
		LOAD_RR = ONE_TIRE_LOAD - Np;
	}
	else
	{
		LOAD_FL = ONE_TIRE_LOAD - Np;		// 符号は後で変更 タイヤ浮いてたら0
		LOAD_FR = ONE_TIRE_LOAD - Np;
		LOAD_RL = ONE_TIRE_LOAD + Np;
		LOAD_RR = ONE_TIRE_LOAD + Np;
	}

	//if() 左旋回中なら
	//else if()　右旋回中なら
	//else　直進中なら

	dirVec = VECTOR2((posCenter.x - T_OFFSET_X) - (posCenter.x - T_OFFSET_X), (posCenter.y + T_OFFSET_Y) - (posCenter.y - T_OFFSET_Y));
	rWheelVec = dirVec;

	wheelBaseDistance = dirVec.Magnitude();
	treadDistanceVec = VECTOR2((posCenter.x + T_OFFSET_X) - (posCenter.x - T_OFFSET_X), (posCenter.y - T_OFFSET_Y) - (posCenter.y - T_OFFSET_Y));
	treadDistanceVecNorm = treadDistanceVec.Normalize();

	treadDistance = treadDistanceVec.Magnitude();

	wheelAngle = steering * WHEEL_ANGLE_MAX;
	wheelAngle *= PI / 180;

	yawVec = FrontWheelAngle(dirVec, wheelAngle);
	fWheelVec = yawVec;
	sAngle = SlideAngle(dirVec, yawVec);

	turnRad = TurningRadius(abs(wheelAngle)) * 10;

	if (engineTorque != 0.0f)
	{
		driveFlag = true;
	}
	else
	{
		driveFlag = false;
	}

	float sr = SlideRate(VECTOR2(0, 0), driveTireVel, wheelAngle);
	float sa = abs(SlideAngle(dirVec, yawVec));
	tireForce = TireForce(sr,sa);
}

// 外積
float Tire::Cross(VECTOR2 va, VECTOR2 vb)
{
	return ((va.y * vb.x) - (vb.y * va.x));
}

// 内積
float Tire::Dot(VECTOR2 va, VECTOR2 vb)
{
	return ((va.x * vb.x) + (va.y * vb.y));
}

float Tire::TurningRadius(float wheelAngle)
{
	if (wheelAngle != 0)
	{
		float r = (WHEEL_BASE / sin(wheelAngle) + sqrt(pow(WHEEL_BASE, 2) + pow(WHEEL_BASE / tan(wheelAngle) + TREAD, 2))) / 2;
		return r;
	}
	return 0;
}

