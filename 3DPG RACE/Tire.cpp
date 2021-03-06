#include "Tire.h"
#include "DxLib.h"
#include "GameTask.h"
#include "ResourceMng.h"
#include <cmath>

Tire::Tire()
{
	Init();
}

Tire::~Tire()
{
}

void Tire::Init()
{
	front.left.pos = { posCenter.x - T_OFFSET_X,posCenter.y - T_OFFSET_Y };
	front.right.pos = { posCenter.x + T_OFFSET_X,posCenter.y - T_OFFSET_Y };
	rear.left.pos = { posCenter.x - T_OFFSET_X,posCenter.y + T_OFFSET_Y };
	rear.right.pos = { posCenter.x + T_OFFSET_X,posCenter.y + T_OFFSET_Y };
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
	float f = (engineTorque * (G_RATIO[gearNum] * FINAL) / (TIRE_DIAMETER / 2.0f)) / (LOAD * G_ACCELERATION);
	return f;
}

float Tire::CentrifugalForce(float v, float r, float m)
{
	float f = m * (pow(v, 2) / r);
	return f;
}

float Tire::SlipRatio(VECTOR2 v, float rv, float wheelAngle)
{
	// 滑り比を出すにはタイヤの角速度と車速を使用

	// Fタイヤが曲がっているとき、v = Vzcosθ + Vxsinθ
	// v:タイヤ進行速度
	// rv:タイヤ角速度

	float moveSpeed = 0.0f;
	float s = 0.0f;

	moveSpeed = v.y * cos(wheelAngle) + v.x * sin(wheelAngle);

	// km/h→m/sに変換
	moveSpeed = (moveSpeed / 3600) * 1000;

	// 単位を合わせる
	if (rv != 0.0f)
	{
		// 周速度(m/s) = rv(rad/s) * (TIRE_DIAMETER(m) / 2)
		float peripheralVel = rv * (TIRE_DIAMETER / 2);

		//if (acceleration > 0.0f/*driveFlag*/) // 駆動
		//{
		//	s = (peripheralVel - moveSpeed) / peripheralVel;
		//}
		//else // 制動
		{
			s = (moveSpeed - peripheralVel) / moveSpeed;
		}
	}

	if (lpGameTask.GetBrake())
	{
		if (!lpGameTask.GetABSFlag())
		{
			if (lpGameTask.GetLT() > 0.8f)
			{
				s = -1.0f;
			}
			else
			{
				s = -1.0f * lpGameTask.GetLT();
			}
			
		}
		else
		{
			if (lpGameTask.GetABSPower() == 0)
			{
				s = -0.4f;
			}
			else if(lpGameTask.GetABSPower() == 1)
			{
				s = -0.3f;
			}
			else
			{
				s = -0.2f;
			}
		}
	}

	return s;
}

float Tire::SlipAngle(VECTOR2 dirVec, VECTOR2 yawVec)
{
	float theta = atan(Cross(yawVec, dirVec) / Dot(yawVec, dirVec));
	return theta;
}

VECTOR2 Tire::TireForce(float slipRatio, float slipAngle)
{
	float Ks = 83000;
	float Ktheta = 54000;
	VECTOR2 f = { 0,0 };

	if(acceleration > 0.0f)
	{
		float lambda = sqrt(pow(slipRatio, 2) + pow((Ktheta / Ks), 2) * pow((1 - slipRatio), 2) * pow(tan(slipAngle), 2));
		float cTheta = -slipRatio / lambda;
		float sTheta = (Ktheta * tan(slipAngle) * (1.0f - slipRatio)) / (Ks * lambda);
		if (slipRatio == 0.0f && lambda == 0.0f)
		{
			cTheta = 0.0f;
			sTheta = 0.0f;
		}
		float epsilon = 1 - (Ks / (3.0f * MU * ONE_TIRE_LOAD) * lambda);

		if (epsilon > 0)
		{
			f.y = Ks * slipRatio * pow(epsilon, 2) - 6.0f * MU * ONE_TIRE_LOAD * cTheta * (1.0f / 6.0f - 1.0f / 2.0f * pow(epsilon, 2) + 1.0f / 3.0f * pow(epsilon, 3));
			f.x = Ktheta * (1 - slipRatio) * tan(slipAngle) * pow(epsilon, 2) + 6.0f * MU * ONE_TIRE_LOAD * sTheta * (1.0f / 6.0f - 1.0f / 2.0f * pow(epsilon, 2) + 1.0f / 3.0f * pow(epsilon, 3));
		}
		else if (epsilon <= 0)
		{
			f.y = -MU * ONE_TIRE_LOAD * cTheta;
			f.x = MU * ONE_TIRE_LOAD * sTheta;
		}
	}
	else
	{
		float lambda = sqrt(pow(slipRatio, 2) + pow((Ktheta / Ks), 2) * pow(tan(slipAngle), 2));
		float cTheta = -slipRatio / lambda;
		float sTheta = (Ktheta * tan(slipAngle)) / (Ks * lambda);
		if (slipRatio == 0.0f && lambda == 0.0f)
		{
			cTheta = 0.0f;
			sTheta = 0.0f;
		}
		float epsilon = 1 - (Ks / (3.0f * MU * ONE_TIRE_LOAD) * (lambda / (1 + slipRatio)));

		if (epsilon > 0)
		{
			f.y = ((Ks * slipRatio) / (1.0f + slipRatio)) * pow(epsilon, 2) - 6.0f * MU * ONE_TIRE_LOAD * cTheta * (1.0f / 6.0f - 1.0f / 2.0f * pow(epsilon, 2) + 1.0f / 3.0f * pow(epsilon, 3));
			f.x = ((Ktheta * tan(slipAngle)) / (1 + slipRatio)) * pow(epsilon, 2) + 6.0f * MU * ONE_TIRE_LOAD * sTheta * (1.0f / 6.0f - 1.0f / 2.0f * pow(epsilon, 2) + 1.0f / 3.0f * pow(epsilon, 3));
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
	//DrawCircle(front.left.pos.x, front.left.pos.y, 2, 0xffffff, true);
	//DrawCircle(front.right.pos.x, front.right.pos.y, 2, 0xffffff, true);
	//DrawCircle(rear.left.pos.x, rear.left.pos.y, 2, 0xffffff, true);
	//DrawCircle(rear.right.pos.x, rear.right.pos.y, 2, 0xffffff, true);

	//DrawLine(posCenter.x, posCenter.y, posCenter.x - dirVec.x * 30, posCenter.y - dirVec.y * 30, 0xff0000, 1);
	//DrawLine(posCenter.x, posCenter.y, posCenter.x - fWheelVecRot.x * 30, posCenter.y - fWheelVecRot.y * 30, 0x00ff00, 1);
	//DrawLine(front.centerPos.x,front.centerPos.y,rear.centerPos.x,rear.centerPos.y, 0x0000ff, 1);

	//DrawFormatString(600, 380, 0xffffff, "speed:%.2f", speed);
	//DrawFormatString(600, 400, 0xffffff, "nonDriveTireVel:%.2f", nonDriveTireVel);
	//DrawFormatString(600, 420, 0xffffff, "driveTireVel:%.2f",driveTireVel);
	//DrawFormatString(600, 440, 0xffffff, "loadFR:%.2f,loadRR:%.2f", front.right.load,rear.right.load);
	//DrawFormatString(600, 460, 0xffffff, "SlipRatioRR:(%.2f),slipAngleRR:(%.2f)", rear.right.slipRatio, rear.right.slipAngle);
	//DrawFormatString(600, 480, 0xffffff, "SlipRatioFR:(%.2f),slipAngleFR:(%.2f)", front.right.slipRatio, front.right.slipAngle);
	//DrawFormatString(600, 500, 0xffffff, "allTireForce:(%.2f,%.2f)", allTireForce.x, allTireForce.y);
	//DrawFormatString(600, 520, 0xffffff, "nonDriveTireVel:(%.2f)", nonDriveTireVel);
}

void Tire::Update()
{
	//vectorSpeed = VECTOR2(vectorSpeed.x, vectorSpeed.z);;
	vectorSpeedRot = VECTOR2(lpGameTask.GetVectorSpeedRot().x, lpGameTask.GetVectorSpeedRot().z);
	dirVecRotVec2 = VECTOR2(lpGameTask.GetDirVecRot().x, lpGameTask.GetDirVecRot().z);
	driveTireVel = lpGameTask.GetDriveTireVel();
	speed = lpGameTask.GetSpeed();
	acceleration = lpGameTask.GetAcceleration();
	steering = lpGameTask.GetSteering();
	gearNum = lpGameTask.GetGearNum();
	accel = lpGameTask.GetRT();
	engineTorque = lpGameTask.GetEngineTorque();

	wheelTorque = lpGameTask.GetWheelTorque();

	// vectorSpeedは速度なのでNormalize()しないこと
	if (dirVecRotVec2 != VECTOR2(0.0f, 0.0f)) 
	{
		dirVecRotVec2 = dirVecRotVec2.Normalize();
	}

	// 旋回半径求まらないので仮
	VECTOR2 Vfaw = VECTOR2(steering, InertialForce(acceleration * 3000, gearNum));
	nr = VerticalForceAtWheelRoll((Vfaw.x * speed)* 0.0015f);
	np = VerticalForceAtWheelPitch(Vfaw.y);		// 符号は後で変更 タイヤ浮いてたら0

	if (accel)
	{
		front.left.load = ONE_TIRE_LOAD + np;		// 符号は後で変更 タイヤ浮いてたら0
		front.right.load = ONE_TIRE_LOAD + np;
		rear.left.load = ONE_TIRE_LOAD - np;
		rear.right.load = ONE_TIRE_LOAD - np;
	}
	else
	{
		front.left.load = ONE_TIRE_LOAD - np;		// 符号は後で変更 タイヤ浮いてたら0
		front.right.load = ONE_TIRE_LOAD - np;
		rear.left.load = ONE_TIRE_LOAD + np;
		rear.right.load = ONE_TIRE_LOAD + np;
	}

	//if() 左旋回中なら
	//else if()　右旋回中なら
	//else　直進中なら

	dirVec = VECTOR2((posCenter.x - T_OFFSET_X) - (posCenter.x - T_OFFSET_X), (posCenter.y + T_OFFSET_Y) - (posCenter.y - T_OFFSET_Y));
	dirVec = dirVec.Normalize();
	rWheelVec = dirVecRotVec2;

	wheelAngle = -steering * (WHEEL_ANGLE_MAX);
	wheelAngle *= PI / 180;

	fWheelVecRot = dirVecRotVec2;
	fWheelVecRot = FrontWheelAngle(fWheelVecRot, wheelAngle);
	if (fWheelVecRot != VECTOR2(0.0f, 0.0f))
	{
		fWheelVecRot = fWheelVecRot.Normalize();
	}

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
	nonDriveTireVel = (speed / 60) * 1000 / TIRE_PERIMETER;
	nonDriveTireVel = (nonDriveTireVel / 60) * 2 * PI;

	if (signbit(steering))
	{
		lr = -1;
	}
	else
	{
		lr = 1;
	}

	//// 前フレから何度回転したか
	//float cosTheta = beforeVec.VectorDot(beforeVec,vectorSpeed) / (beforeVec.Magnitude() * vectorSpeed.Magnitude());
	//deltaD = acos(cosTheta);
	//beforeVec = vectorSpeed;
	//// 回転させたvectorSpeedを作る？
	//vectorSpeedRot.x = vectorSpeed.x * cos(deltaD) - vectorSpeed.y * sin(deltaD);
	//vectorSpeedRot.y = vectorSpeed.x * sin(deltaD) + vectorSpeed.y * cos(deltaD);

	// "vectorSpeed.y.Normalize() * speed"にする?

	////Front////
	if (steering == 0.0f || beforeLr != lr)
	{
		count = 0;
	}
	else
	{
		count++;
	}
	beforeLr = lr;

	// FL
	front.left.slipRatio = SlipRatio(VECTOR2(vectorSpeedRot.x,vectorSpeedRot.y), nonDriveTireVel, wheelAngle);
	//if (count == 1)
	{
		front.left.slipAngle = abs(SlipAngle(dirVecRotVec2, fWheelVecRot));
	}
	//else
	//{
	//	front.left.slipAngle = abs(SlipAngle(dirVecRotVec2, vectorSpeed));
	//}

	front.left.tireForce = TireForce(front.left.slipRatio, front.left.slipAngle);
	if (!(front.left.tireForce == VECTOR2(0.0f, 0.0f)))
	{
		front.left.tireForce = front.left.tireForce.Normalize();
	}

	// FR
	front.right.slipRatio = SlipRatio(VECTOR2(vectorSpeedRot.x, vectorSpeedRot.y), nonDriveTireVel, wheelAngle);
	//if (count == 1)
	{
		front.right.slipAngle = abs(SlipAngle(dirVecRotVec2, fWheelVecRot));
	}
	//else
	//{
	//	front.right.slipAngle = abs(SlipAngle(dirVecRotVec2, vectorSpeed));
	//}

	front.right.tireForce = TireForce(front.right.slipRatio, front.right.slipAngle);
	if (!(front.right.tireForce == VECTOR2(0.0f, 0.0f)))
	{
		front.right.tireForce = front.right.tireForce.Normalize();
	}

	////Rear////
	// RL
	rear.left.slipRatio = SlipRatio(VECTOR2(vectorSpeedRot.x, vectorSpeedRot.y), driveTireVel, 0.0f);
	rear.left.slipAngle = abs(SlipAngle(dirVecRotVec2, fWheelVecRot));

	rear.left.tireForce = TireForce(rear.left.slipRatio, rear.left.slipAngle);
	if (!(rear.left.tireForce == VECTOR2(0.0f, 0.0f)))
	{
		rear.left.tireForce = rear.left.tireForce.Normalize();
	}

	// RR
	rear.right.slipRatio = SlipRatio(VECTOR2(vectorSpeedRot.x, vectorSpeedRot.y), driveTireVel, 0.0f);
	rear.right.slipAngle = abs(SlipAngle(dirVecRotVec2 , fWheelVecRot));

	rear.right.tireForce = TireForce(rear.right.slipRatio, rear.right.slipAngle);
	if (!(rear.right.tireForce == VECTOR2(0.0f, 0.0f)))
	{
		rear.right.tireForce = rear.right.tireForce.Normalize();
	}

	volume = 255 * (65 * (abs(front.left.slipRatio) * 5)) / 100;
	if (volume > 255 * 65 / 100)
	{
		volume = 255 * 65 / 100;
	}
	ChangeVolumeSoundMem(volume, SOUND_ID("sounds/slip2.wav"));

	if (speed > 50.0f)
	{
		if (!CheckSoundMem(SOUND_ID("sounds/slip2.wav")))
		{
			PlaySoundMem(SOUND_ID("sounds/slip2.wav"), DX_PLAYTYPE_LOOP);
		}
	}
	else
	{
		StopSoundMem(SOUND_ID("sounds/slip2.wav"));
	}

	// 左右輪の真ん中の座標をとる
	front.centerPos = VECTOR2((front.left.pos.x + front.right.pos.x) / 2, (front.left.pos.y + front.right.pos.y) / 2);
	rear.centerPos = VECTOR2((rear.left.pos.x + rear.right.pos.x) / 2, (rear.left.pos.y + rear.right.pos.y) / 2);

	// ベクトルでつなげる
	VECTOR2 centerVec = front.centerPos - rear.centerPos;

	allTireForce = { (front.left.tireForce.x + front.right.tireForce.x + rear.left.tireForce.x + rear.right.tireForce.x) 
		,(front.left.tireForce.y + front.right.tireForce.y + rear.left.tireForce.y + rear.right.tireForce.y)};

	if (steering == 0.0f)
	{
		allTireForce = VECTOR2(0.0f, 1.0f);
	}

	if (gearNum >= 0 && allTireForce.y <= 0.0f)
	{
		allTireForce.y = speed;
	}

	if (gearNum == -1)
	{
		if (speed != 0.0f)
		{
			allTireForce = VECTOR2(allTireForce.x, speed);
			lpGameTask.SetTireForce(allTireForce.Normalize());
			lpGameTask.SetDirVec(dirVec);
			lpGameTask.SetFrontWheelVecRot(VGet(fWheelVecRot.x,0.0f,fWheelVecRot.y));
			lpGameTask.SetLRFlag(lr);
		}
		else
		{
			lpGameTask.SetTireForce(VECTOR2(0.0f, 0.0f));
			lpGameTask.SetDirVec(dirVec);
			lpGameTask.SetFrontWheelVecRot(VGet(fWheelVecRot.x, 0.0f, fWheelVecRot.y));
			lpGameTask.SetLRFlag(lr);
		}
	}

	lpGameTask.SetTireForce(allTireForce.Normalize());
	lpGameTask.SetDirVec(dirVec);
	lpGameTask.SetFrontWheelVecRot(VGet(fWheelVecRot.x, 0.0f, fWheelVecRot.y));
	lpGameTask.SetLRFlag(lr);
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

