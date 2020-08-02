#include "Tire.h"
#include "DxLib.h"
#include "GameTask.h"
#include "ResourceMng.h"
#include <cmath>

Tire::Tire()
{
	front.left.pos = { posCenter.x - T_OFFSET_X,posCenter.y - T_OFFSET_Y };
	front.right.pos = { posCenter.x + T_OFFSET_X,posCenter.y - T_OFFSET_Y };
	rear.left.pos = { posCenter.x - T_OFFSET_X,posCenter.y + T_OFFSET_Y };
	rear.right.pos = { posCenter.x + T_OFFSET_X,posCenter.y + T_OFFSET_Y };
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

float Tire::SlipRate(VECTOR2 v, float rv, float wheelAngle)
{
	// 滑り比を出すにはタイヤの角速度と車速を使用

	// Fタイヤが曲がっているとき、v = Vzcosθ + Vxsinθ
	// v:タイヤ進行速度
	// rv:タイヤ角速度

	float moveSpeed = 0.0f;
	float s = 0.0f;

	if (wheelAngle != 0.0f)
	{
		moveSpeed = v.y * cos(wheelAngle) + v.x * sin(wheelAngle);
	}
	else
	{
		moveSpeed = v.y;
	}

	if (rv != 0.0f)
	{
		if (driveFlag)
		{
			// おかしい
			s = (rv * TIRE_DIAMETER - moveSpeed) / (rv * TIRE_DIAMETER);
		}
		else
		{
			s = (rv * TIRE_DIAMETER - moveSpeed) / moveSpeed;
		}
	}
	else
	{
		return -1.0f;
	}

	//saveSlipRate = s;
	return s;
}

float Tire::SlipAngle(VECTOR2 dirVec, VECTOR2 yawVec)
{
	float theta = atan(Cross(yawVec, dirVec) / Dot(yawVec, dirVec));
	return theta;
}

VECTOR2 Tire::TireForce(float slipRate, float slipAngle)
{
	float Ks = 83000;
	float Ktheta = 54000;
	VECTOR2 f = { 0,0 };

	if(driveFlag)
	{
		float lambda = sqrt(pow(slipRate, 2) + pow((Ktheta / Ks), 2) * pow((1 - slipRate), 2) * pow(tan(slipAngle), 2));
		float cTheta = -slipRate / lambda;
		float sTheta = (Ktheta * tan(slipAngle) * (1.0f - slipRate)) / (Ks * lambda);
		if (slipRate == 0.0f && lambda == 0.0f)
		{
			cTheta = 0.0f;
			sTheta = 0.0f;
		}
		float epsilon = 1 - (Ks / (3.0f * MU * ONE_TIRE_LOAD) * lambda);

		if (epsilon > 0)
		{
			f.y = Ks * slipRate * pow(epsilon, 2) - 6.0f * MU * ONE_TIRE_LOAD * cTheta * (1.0f / 6.0f - 1.0f / 2.0f * pow(epsilon, 2) + 1.0f / 3.0f * pow(epsilon, 3));
			f.x = Ktheta * (1 - slipRate) * tan(slipAngle) * pow(epsilon, 2) + 6.0f * MU * ONE_TIRE_LOAD * sTheta * (1.0f / 6.0f - 1.0f / 2.0f * pow(epsilon, 2) + 1.0f / 3.0f * pow(epsilon, 3));
		}
		else if (epsilon <= 0)
		{
			f.y = -MU * ONE_TIRE_LOAD * cTheta;
			f.x = MU * ONE_TIRE_LOAD * sTheta;
		}
	}
	else
	{
		float lambda = sqrt(pow(slipRate, 2) + pow((Ktheta / Ks), 2) * pow(tan(slipAngle), 2));
		float cTheta = -slipRate / lambda;
		float sTheta = (Ktheta * tan(slipAngle)) / (Ks * lambda);
		if (slipRate == 0.0f && lambda == 0.0f)
		{
			cTheta = 0.0f;
			sTheta = 0.0f;
		}
		float epsilon = 1 - (Ks / (3.0f * MU * ONE_TIRE_LOAD) * (lambda / (1 + slipRate)));

		if (epsilon > 0)
		{
			f.y = ((Ks * slipRate) / (1.0f + slipRate)) * pow(epsilon, 2) - 6.0f * MU * ONE_TIRE_LOAD * cTheta * (1.0f / 6.0f - 1.0f / 2.0f * pow(epsilon, 2) + 1.0f / 3.0f * pow(epsilon, 3));
			f.x = ((Ktheta * tan(slipAngle)) / (1 + slipRate)) * pow(epsilon, 2) + 6.0f * MU * ONE_TIRE_LOAD * sTheta * (1.0f / 6.0f - 1.0f / 2.0f * pow(epsilon, 2) + 1.0f / 3.0f * pow(epsilon, 3));
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

float Tire::WheelRot(float kph)
{
	float outer = TIRE_DIAMETER * PI;
	float mpm = (kph * 1000) / 60;
	float rpm = mpm / outer;
	float rps = rpm / 60 * 2 * PI;
	return rps;
}

void Tire::Draw()
{
	//DrawFormatString(SCREEN_SIZE_X - 200, 460, 0xffffff, "VFAW_F = %.2f", LOAD_FL);
	//DrawFormatString(SCREEN_SIZE_X / 2 + 300, 50, 0xffffff, "VFAW_FR = %.2f", LOAD_FR);
	//DrawFormatString(SCREEN_SIZE_X - 200, 480, 0xffffff, "VFAW_R = %.2f", LOAD_RL);
	//DrawFormatString(SCREEN_SIZE_X / 2 + 300, SCREEN_SIZE_Y - 150 - 100, 0xffffff, "VFAW_RR = %.2f", LOAD_RR);

	DrawRotaGraph(posCenter.x, posCenter.y, 0.10f, deg, IMAGE_ID("image/body.png"), true);
	//DrawCircle(front.left.pos.x, front.left.pos.y, 2, 0xffffff, true);
	//DrawCircle(front.right.pos.x, front.right.pos.y, 2, 0xffffff, true);
	//DrawCircle(rear.left.pos.x, rear.left.pos.y, 2, 0xffffff, true);
	//DrawCircle(rear.right.pos.x, rear.right.pos.y, 2, 0xffffff, true);

	DrawLine(posCenter.x, posCenter.y, posCenter.x - dirVec.x * 30, posCenter.y - dirVec.y * 30, 0xff0000, 1);
	DrawLine(posCenter.x, posCenter.y, posCenter.x - yawVec.x * 30, posCenter.y - yawVec.y * 30, 0x00ff00, 1);
	DrawLine(front.centerPos.x,front.centerPos.y,rear.centerPos.x,rear.centerPos.y, 0x0000ff, 1);

	//DrawFormatString(600, 520, 0xffffff, "tF.RR.x:%.2f,tF.RR.y:%.2f", rear.right.tireForce.x, rear.right.tireForce.y);
	DrawFormatString(450, 440, 0xffffff, "deg.x:%.2f", deg);

	DrawFormatString(600, 440, 0xffffff, "moveVec.x:%.2f,moveVec.y:%.2f", moveVec.x,moveVec.y);
	//DrawFormatString(600, 460, 0xffffff, "allVector.x:%.2f,allVector.y:%.2f", allVector.x, allVector.y);
	DrawFormatString(600, 480, 0xffffff, "tF.RL(tireForce.x):%.2f\ntF.RL(tireForce.y):%.2f", rear.left.tireForce.x, rear.left.tireForce.y);
	DrawFormatString(600, 520, 0xffffff, "tF.RL(slipRate):%.2f", front.left.slipRate);
}

tuple<VECTOR2> Tire::Update(float engineTorque, float steering, int gearNum, float accel, float driveTireVel, float speed, VECTOR vectorSpeed)
{
	this->vectorSpeed = VECTOR2(vectorSpeed.x, vectorSpeed.z);

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
	dirVec = dirVec.Normalize();
	rWheelVec = dirVec;

	wheelBaseDistance = dirVec.Magnitude();
	treadDistanceVec = VECTOR2((posCenter.x + T_OFFSET_X) - (posCenter.x - T_OFFSET_X), (posCenter.y - T_OFFSET_Y) - (posCenter.y - T_OFFSET_Y));
	treadDistanceVecNorm = treadDistanceVec.Normalize();

	treadDistance = treadDistanceVec.Magnitude();

	wheelAngle = steering * WHEEL_ANGLE_MAX;
	wheelAngle *= PI / 180;

	yawVec = FrontWheelAngle(dirVec, wheelAngle);
	yawVec = yawVec.Normalize();
	fWheelVec = yawVec;

	turnRad = TurningRadius(abs(wheelAngle)) * 10;

	if (engineTorque != 0.0f)
	{
		driveFlag = true;
	}
	else
	{
		driveFlag = false;
	}

	// 非駆動輪のタイヤ角速度は車速から求める

	if (signbit(steering))
	{
		lr = -1;
	}
	else
	{
		lr = 1;
	}

	////Front////
	// FL
	front.left.slipRate = SlipRate(this->vectorSpeed, driveTireVel, wheelAngle);
	front.left.slipAngle = abs(SlipAngle(dirVec,yawVec));

	front.left.tireForce = TireForce(front.left.slipRate, front.left.slipAngle);

	// FR
	front.right.slipRate = SlipRate(this->vectorSpeed, driveTireVel, wheelAngle);
	front.right.slipAngle = abs(SlipAngle(dirVec, yawVec));

	front.right.tireForce = TireForce(front.right.slipRate, front.right.slipAngle);


	////Rear////
	// RL
	rear.left.slipRate = SlipRate(this->vectorSpeed, driveTireVel, 0.0f);
	rear.left.slipAngle = abs(SlipAngle(dirVec, this->vectorSpeed));

	rear.left.tireForce = TireForce(rear.left.slipRate, rear.left.slipAngle);

	// RR
	rear.right.slipRate = SlipRate(this->vectorSpeed, driveTireVel, 0.0f);
	rear.right.slipAngle = abs(SlipAngle(dirVec, this->vectorSpeed));

	rear.right.tireForce = TireForce(rear.right.slipRate, rear.right.slipAngle);


	// 左右輪の真ん中の座標をとる
	front.centerPos = VECTOR2((front.left.pos.x + front.right.pos.x) / 2, (front.left.pos.y + front.right.pos.y) / 2);
	rear.centerPos = VECTOR2((rear.left.pos.x + rear.right.pos.x) / 2, (rear.left.pos.y + rear.right.pos.y) / 2);

	// ベクトルでつなげる
	VECTOR2 centerVec = front.centerPos - rear.centerPos;

	allTireForce = { front.left.tireForce.x + front.right.tireForce.x ,rear.left.tireForce.y + rear.right.tireForce.y };

	if (gearNum >= 0 && allTireForce.y < 0)
	{
		allTireForce.y = 0.0f;
	}

	if (allTireForce.y != 0.0f)
	{
		allTireForce = allTireForce.Normalize();
	}
	else
	{
		allTireForce.y = speed;
		//allTireForce = allTireForce.Normalize();
	}

	return forward_as_tuple(allTireForce);
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

