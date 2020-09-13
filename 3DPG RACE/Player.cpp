#include "math.h"
#include "GameTask.h"
#include "Player.h"
#include "Tire.h"
#include "KeyMng.h"

Player::Player(int num)
{
	ModelInit();
	_playerNum = num;
}

Player::~Player()
{
}

void Player::ModelInit()
{
	wheel.front.right.model = MV1LoadModel("model/ae86RWheel.mv1");
	wheel.front.left.model = MV1LoadModel("model/ae86LWheel.mv1");
	wheel.rear.right.model = MV1LoadModel("model/ae86RWheel.mv1");
	wheel.rear.left.model = MV1LoadModel("model/ae86LWheel.mv1");
	boxModel = MV1LoadModel("model/box.mv1");
	carModel = MV1LoadModel("model/ae86Cp.mv1");
	camModel = MV1LoadModel("model/box.mv1");
	Init();

	ChangeLightTypeDir(VGet(0.1f, 0.9f, 0.0f));

}

void Player::Init()
{
	carPos = PLAYER_POS_OFFSET;
	carScl = VGet(1.0f, 1.0f, 1.0f);
	carMat = MGetScale(carScl);
	boxPos = carPos;
	wheelFRPos = VGet(0.0f, 0.0f, 0.0f);
	wheelFLPos = VGet(0.0f, 0.0f, 0.0f);
	wheelRRPos = VGet(0.0f, 0.0f, 0.0f);
	wheelRLPos = VGet(0.0f, 0.0f, 0.0f);

	cam.scl = VGet(1.0f, 1.0f, 1.0f);
	cam.mat = MGetScale(cam.scl);
	cam.pos = VGet(0.0f, 0.0f, 0.0f);

	// 車の座標を上にあげるための平行移動
	offsetMat = MGetTranslate(PLAYER_POS_OFFSET);
	carOffsetPos = VTransform(carPos, offsetMat);
	carPos = VAdd(VGet(-8000.0f, 0.0f, 40300.0f), carOffsetPos);
	deg.yaw = 1.85f;
	carMat = MGetScale(carScl);

	//当たり判定の初期設定
	_hitBox._startPos.y = 110.0f;
	_hitBox._endPos.y = 110.0f;
	_hitBox._wallStart.y = 150.0f;
	_hitBox._wallEnd.y = 150.0f;

	beforeCarPos = carPos;
}

//---ﾎﾞﾃﾞｨにﾎｲｰﾙを追従させる
void Player::Update()
{
	steering = lpGameTask.GetSteering();
	speed = lpGameTask.GetSpeed();
	tireForce = VGet(lpGameTask.GetTireForce().x * steering, 0.0f, lpGameTask.GetTireForce().y);
	dirVec = VGet(lpGameTask.GetDirVec().x, 0.0f, lpGameTask.GetDirVec().y);
	fWheelVec = VGet(lpGameTask.GetFrontWheelVec().x, 0.0f, lpGameTask.GetFrontWheelVec().y);
	lr = lpGameTask.GetLRFlag();

	//float tmp = (10.0f * (tanhf((1.0f / 100.0f) * pow(speed, 2.0f))));

	float tmp = 0.0f;
	if (speed < 50.0f)
	{
		// 指数関数
		tmp = pow(1.058, speed) - 1.0f;
	}
	else
	{
		// ｼｸﾞﾓｲﾄﾞ関数
		tmp = ((tanhf(speed - 100.0f / 2.0f) + 1.0f) / 2.0f) * 15.0f;
	}

	deg.yaw += ((tireForce.x) * (DT * (tmp + 2.0f)));
	deg.pitch = -lpGameTask.GetPitchLoad();
	if (deg.pitch < -0.5f)
	{
		deg.pitch = -0.5f;
	}
	if (deg.pitch > 0.5f)
	{
		deg.pitch = 0.5f;
	}

	if (deg.oldPitch - deg.pitch > 0.3f || deg.oldPitch - deg.pitch < -0.3f)
	{
		deg.pitch = 0.0f;
	}
	deg.oldPitch = deg.pitch;
	deg.roll = abs(lpGameTask.GetRollLoad());

	if (deg.roll > 0.10f)
	{
		deg.roll = 0.10f;
	}

	MATRIX RotYMat = MMult(carMat, MGetRotY(deg.yaw));

	dirVecRot = VTransform(dirVec, RotYMat);
	// vectorSpeedは回転させる必要ない
	vectorSpeedRot = VTransform(vectorSpeed, MGetRotY(-deg.yaw));

	//addMoveVec = VAdd(VGet((tireForce.x * lr) * DT, 0.0f, (tireForce.z * speed) * DT),addMoveVec);

	// 車の移動・回転
	tireForceRot = VTransform(tireForce, MMult(carMat, MGetRotY(deg.yaw)));
	moveMat = MGetTranslate(VGet((tireForceRot.x * speed), 0.0f, tireForceRot.z * speed));
	carPos = VTransform(carPos, moveMat);
	carPosMat = MGetTranslate(carPos);
	MV1SetMatrix(carModel, MMult(MMult(MMult(MGetRotX(deg.pitch), MGetRotZ(deg.roll * lr)), MGetRotY(deg.yaw)), carPosMat));

	VECTOR playerFrontPosOffset = VTransform(VGet(0.0f, 80.0f, 500.0f), MMult(carMat, MGetRotY(deg.yaw)));
	carFrontPos = VAdd(carPos, playerFrontPosOffset);
	tireViewFrontPos = VGet(carFrontPos.x + 200.0f, 0.0f, carFrontPos.z);

	// タイヤの移動・回転
	wheel.front.rotMatY = MGetRotY(deg.yaw - lpGameTask.GetWheelAngle());
	wheel.rear.rotMatY = MGetRotY(deg.yaw);
	wheel.front.tireRotX += lpGameTask.GetDriveTireVel() * DT;
	wheel.rear.tireRotX += lpGameTask.GetDriveTireVel() * DT;

	// FL
	wheel.front.left.moveOffset = VAdd(VTransform(VGet(-WHEEL_OFFSET.x, WHEEL_OFFSET.y, WHEEL_OFFSET.z), MMult(carMat, MGetRotY(deg.yaw))), VGet(carPos.x, 0.0f, carPos.z));
	MV1SetMatrix(wheel.front.left.model, MMult(MMult(MMult(MGetRotX(wheel.front.tireRotX), carMat), wheel.front.rotMatY), MGetTranslate(wheel.front.left.moveOffset)));
	// FR
	wheel.front.right.moveOffset = VAdd(VTransform(VGet(WHEEL_OFFSET.x, WHEEL_OFFSET.y, WHEEL_OFFSET.z), MMult(carMat, MGetRotY(deg.yaw))), VGet(carPos.x, 0.0f, carPos.z));
	MV1SetMatrix(wheel.front.right.model, MMult(MMult(MMult(MGetRotX(wheel.front.tireRotX), carMat), wheel.front.rotMatY), MGetTranslate(wheel.front.right.moveOffset)));
	// RL
	wheel.rear.left.moveOffset = VAdd(VTransform(VGet(-WHEEL_OFFSET.x, WHEEL_OFFSET.y, -WHEEL_OFFSET.z), MMult(carMat, MGetRotY(deg.yaw))), VGet(carPos.x, 0.0f, carPos.z));
	MV1SetMatrix(wheel.rear.left.model, MMult(MMult(MMult(MGetRotX(wheel.rear.tireRotX), carMat), wheel.rear.rotMatY), MGetTranslate(wheel.rear.left.moveOffset)));
	// RR
	wheel.rear.right.moveOffset = VAdd(VTransform(VGet(WHEEL_OFFSET.x, WHEEL_OFFSET.y, -WHEEL_OFFSET.z), MMult(carMat, MGetRotY(deg.yaw))), VGet(carPos.x, 0.0f, carPos.z));
	MV1SetMatrix(wheel.rear.right.model, MMult(MMult(MMult(MGetRotX(wheel.rear.tireRotX), carMat), wheel.rear.rotMatY), MGetTranslate(wheel.rear.right.moveOffset)));

	// カメラ移動・回転
	if (KeyMng::GetInstance().trgKey[P1_Y])
	{
		cam.view++;
	}

	if (!lpGameTask.GetTitleFlag())
	{
		if (cam.view > 2)
		{
			cam.view = 0;
		}
	}

	if (lpGameTask.GetTitleFlag())
	{
		cam.view = 3;
	}
	lpGameTask.SetView(cam.view);

	float camAccelOffset = speed / lpGameTask.GetMaxSpeed();

	if (cam.view == 0)
	{
		cam.offset = { 0.0f, 250.0f, -900.0f + (200.0f * camAccelOffset) };
		camPosOffset = VTransform(cam.offset, MMult(cam.mat, MGetRotY(deg.yaw)));
	}
	else if (cam.view == 1)
	{
		cam.offset = { 0.0f, 110.0f, 150.0f + (lpGameTask.GetMaxSpeed() * camAccelOffset) };
		MATRIX tmp;
		tmp = MMult(cam.mat, MGetRotY(deg.yaw));
		camPosOffset = VTransform(cam.offset, tmp);
	}
	else if (cam.view == 2)
	{
		cam.offset = { 250.0f,0.0f, -300.0f + (lpGameTask.GetMaxSpeed() * camAccelOffset) };
		camPosOffset = VTransform(cam.offset, MMult(cam.mat, MGetRotY(deg.yaw)));
	}
	else
	{
		VECTOR2 vp = { viewPoint[viewNum].x ,viewPoint[viewNum].z };
		VECTOR2 nVp = { 0.0f,0.0f };
		if (viewNum != 10)
		{
			nVp = VECTOR2(viewPoint[viewNum + 1].x, viewPoint[viewNum + 1].z);
		}
		else
		{
			nVp = VECTOR2(viewPoint[0].x, viewPoint[0].z);
		}
		VECTOR2 cp = { carPos.x ,carPos.z };
		VECTOR2 camToCarVec = cp - vp;
		VECTOR2 nextVec = cp - nVp;

		float mgn = camToCarVec.Magnitude();
		float nMgn = nextVec.Magnitude();

		if (viewNum == 6)
		{
			minTime = 500;
		}
		else
		{
			minTime = 120;
		}

		if (changeFlag)
		{
			changeViewNumCounter++;
			if (changeViewNumCounter > minTime)
			{
				changeFlag = false;
				changeViewNumCounter = 0;
			}
		}

		if (!changeFlag)
		{
			if (mgn > nMgn)
			{
				changeFlag = true;
				viewNum++;
				if (viewNum > 10)
				{
					viewNum = 0;
				}
			}
		}
	}

	if (cam.view != 3)
	{
		cam.pos = VAdd(VGet(carPos.x, carPos.y, carPos.z), camPosOffset);
	}
	else
	{
		if (viewNum != 8 && viewNum != 9)
		{
			cam.pos = viewPoint[viewNum];
		}
		else
		{
			cam.pos = VGet(carFrontPos.x - 500.0f, carFrontPos.y, carFrontPos.z - 300.0f);
		}
	}

	// 前フレからの移動量
	vectorSpeed = VGet(dirVecRot.x * speed, dirVecRot.y, dirVecRot.z * speed);
	vectorSpeed = VSub(beforeCarPos, carPos);
	vectorSpeed.x *= -1;
	vectorSpeed.z *= -1;
	beforeCarPos = carPos;

	acceleration = -(oldSpeed - speed) * DT;
	oldSpeed = speed;

	//DrawFormatString(0, 40, 0xffffff, "tireForce.x,y,z(%.2f,%.2f,%.2f)", tireForce.x, tireForce.y, tireForce.z);
	//DrawFormatString(0, 80, 0xffffff, "speed:%.2f", speed);
	//DrawFormatString(0, 100, 0xffffff, "deg:p(%.3f),y(%.3f),r(%.3f)", deg.pitch,deg.yaw,deg.roll);
	//DrawFormatString(0, 120, 0xffffff, "acceleration:(%.5f)", acceleration);
	//DrawFormatString(0, 140, 0xffffff, "vectorSpeedRot:x(%.2f),y(%.2f),z(%.2f)", vectorSpeedRot.x,vectorSpeedRot.y,vectorSpeedRot.z);

	lpGameTask.SetVectorSpeed(vectorSpeed);
	lpGameTask.SetVectorSpeedRot(vectorSpeedRot);
	lpGameTask.SetDirVecRot(dirVecRot);
	lpGameTask.SetCarPos(carPos);
	lpGameTask.SetFrontWheelVecRot(fWheelVec);
	lpGameTask.SetAcceleration(acceleration);
}

void Player::Render()
{
	//MV1DrawModel(boxModel);
	if (_playerNum == 0 && !lpGameTask.GetTitleFlag())
	{
		MV1SetOpacityRate(carModel, 0.2f);

		MV1SetOpacityRate(wheel.front.right.model, 0.2f);
		MV1SetOpacityRate(wheel.front.left.model, 0.2f);
		MV1SetOpacityRate(wheel.rear.right.model, 0.2f);
		MV1SetOpacityRate(wheel.rear.left.model, 0.2f);
	}


	MV1DrawModel(carModel);
	MV1DrawModel(wheel.front.right.model);
	MV1DrawModel(wheel.front.left.model);
	MV1DrawModel(wheel.rear.right.model);
	MV1DrawModel(wheel.rear.left.model);

	MV1RefreshCollInfo(carModel, -1);

	VECTOR tmp = { vectorSpeed.x * 500.0f ,vectorSpeed.y * 500.0f ,vectorSpeed.z * 500.0f };
	VECTOR tmp1 = { dirVecRot.x * 500.0f ,0.0f ,dirVecRot.z * 500.0f };
	VECTOR tmp2 = { fWheelVec.x * 500.0f ,0.0f ,fWheelVec.z * 500.0f };

	if (lpGameTask.GetLT())
	{
		MV1SetMaterialDifColor(carModel, 10, GetColorF(1.0f, 0.1f, 0.1f, 1.0f));
		MV1SetMaterialEmiColor(carModel, 10, GetColorF(0.8f, 0.1f, 0.1f, 1.0f));
	}
	else
	{
		MV1SetMaterialDifColor(carModel, 10, GetColorF(0.5f, 0.0f, 0.0f, 1.0f));
		MV1SetMaterialEmiColor(carModel, 10, GetColorF(0.3f, 0.0f, 0.0f, 1.0f));
	}

	MV1SetMaterialDifColor(carModel, 8, GetColorF(1.0f, 1.0f, 0.1f, 1.0f));
	MV1SetMaterialEmiColor(carModel, 8, GetColorF(0.8f, 0.8f, 0.1f, 1.0f));

	//DrawLine3D(carPos, VAdd(carPos, tmp), 0xffffff);
	//DrawLine3D(carPos, VAdd(carPos, tmp1), 0xff0000);
	//DrawLine3D(carPos, VAdd(carPos, tmp2), 0xffff00);

	//DrawFormatString(0, 0, 0xffffff, "carPos.x,y,z(%.2f,%.2f,%.2f)", carPos.x * DT, carPos.y * DT, carPos.z * DT);
	//DrawFormatString(0, 20, 0xffffff, "vectorSpeed.x,y,z(%.2f,%.2f,%.2f)", vectorSpeed.x, vectorSpeed.y, vectorSpeed.z);

	_hitBox._startPos = VGet(carPos.x + tireForceRot.x, _hitBox._startPos.y, carPos.z + tireForceRot.z);
	_hitBox._endPos = VGet(carPos.x - 10, _hitBox._endPos.y, carPos.z - 10);
	_hitBox._wallStart = VGet(carPos.x + tireForceRot.x, _hitBox._wallStart.y, carPos.z + tireForceRot.z);
	_hitBox._wallEnd = VGet(carPos.x - 10, _hitBox._wallEnd.y, carPos.z - 10);
}


bool Player::HitGoalCollision(VECTOR a, VECTOR b)
{
	bool rtnFlag = false;

	auto hitCheck = MV1CollCheck_Capsule(carModel, -1, a, b, 50.0f);

	if (hitCheck.HitNum > 0)
	{
		rtnFlag = true;
	}
	return rtnFlag;
}

// 外積
float Player::Cross(VECTOR va, VECTOR vb)
{
	return ((va.z * vb.x) - (vb.z * va.x));
}

// 内積
float Player::Dot(VECTOR va, VECTOR vb)
{
	return ((va.x * vb.x) + (va.z * vb.z));
}
