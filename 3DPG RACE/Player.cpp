#include "math.h"
#include "GameTask.h"
#include "Player.h"
#include "Tire.h"
#include "KeyMng.h"

Player::Player()
{
	Init();
}

Player::~Player()
{
}


void Player::Init()
{
	wheelFRModel = MV1LoadModel("model/ae86RWheel.mv1");
	wheelFLModel = MV1LoadModel("model/ae86LWheel.mv1");
	wheelRRModel = MV1LoadModel("model/ae86RWheel.mv1");
	wheelRLModel = MV1LoadModel("model/ae86LWheel.mv1");
	boxModel = MV1LoadModel("model/box.mv1");
	carModel = MV1LoadModel("model/ae86Cp.mv1");
	camModel = MV1LoadModel("model/box.mv1");

	carPos = PLAYER_POS_OFFSET;
	carScl = VGet(1.0f, 1.0f, 1.0f);
	carMat = MGetScale(carScl);
	boxPos = carPos;
	wheelFRPos = VGet(0.0f, 0.0f, 0.0f);
	wheelFLPos = VGet(0.0f, 0.0f, 0.0f);
	wheelRRPos = VGet(0.0f, 0.0f, 0.0f);
	wheelRLPos = VGet(0.0f, 0.0f, 0.0f);

	cam.scl = VGet(1.0f,1.0f,1.0f);
	cam.mat = MGetScale(cam.scl);
	cam.pos = VGet(0.0f,0.0f,0.0f); 

	// 車の座標を上にあげるための平行移動
	offsetMat = MGetTranslate(PLAYER_POS_OFFSET);
	carOffsetPos = VTransform(carPos, offsetMat);
	carPos = carOffsetPos;
	carMat = MGetScale(carScl);

	beforeCarPos = carPos;

	ChangeLightTypeDir(VGet(0.1f, 0.9f, 0.0f));
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

	// これで回転・移動
	MV1SetMatrix(carModel, MMult(MMult(MMult(MGetRotX(deg.pitch), MGetRotZ(deg.roll * lr)), MGetRotY(deg.yaw)), carPosMat));

	//addMoveVec = VAdd(VGet((tireForce.x * lr) * DT, 0.0f, (tireForce.z * speed) * DT),addMoveVec);

	// 車の移動
	tireForceRot = VTransform(tireForce, MMult(carMat, MGetRotY(deg.yaw)));
	moveMat = MGetTranslate(VGet((tireForceRot.x * speed), tireForce.y, tireForceRot.z * speed));
	carPos = VTransform(carPos, moveMat);
	carPosMat = MGetTranslate(carPos);

	VECTOR playerFrontPosOffset = VTransform(VGet(0.0f, 80.0f, 500.0f),MMult(carMat,MGetRotY(deg.yaw)));
	carFrontPos = VAdd(carPos, playerFrontPosOffset);

	// カメラ移動・回転
	if (KeyMng::GetInstance().trgKey[P1_Y])
	{
		cam.view = !cam.view;
		lpGameTask.SetView(cam.view);
	}

	VECTOR camPosOffset = { 0.0f,0.0f,0.0f };
	if (cam.view)
	{
		cam.offset = { 0.0f, 110.0f, 150.0f };
		MATRIX tmp;
		tmp = MMult(cam.mat, MGetRotY(deg.yaw));
		camPosOffset = VTransform(cam.offset, tmp);
	}
	else
	{
		cam.offset = { 0.0f, 250.0f, -900.0f };
		camPosOffset = VTransform(cam.offset, MMult(cam.mat, MGetRotY(deg.yaw)));
	}

	cam.pos = VAdd(carPos, camPosOffset);

	// 前フレからの移動量
	vectorSpeed = VGet(dirVecRot.x * speed,dirVecRot.y,dirVecRot.z * speed);
	vectorSpeed = VSub(beforeCarPos,carPos);
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
	MV1DrawModel(carModel);
	//MV1DrawModel(wheelFRModel);
	//MV1DrawModel(wheelFLModel);
	//MV1DrawModel(wheelRRModel);
	//MV1DrawModel(wheelRLModel);
	//MV1DrawModel(camModel);

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

	//DrawLine3D(carPos, VAdd(carPos, tmp), 0xffffff);
	//DrawLine3D(carPos, VAdd(carPos, tmp1), 0xff0000);
	//DrawLine3D(carPos, VAdd(carPos, tmp2), 0xffff00);

	//DrawFormatString(0, 0, 0xffffff, "carPos.x,y,z(%.2f,%.2f,%.2f)", carPos.x * DT, carPos.y * DT, carPos.z * DT);
	//DrawFormatString(0, 20, 0xffffff, "vectorSpeed.x,y,z(%.2f,%.2f,%.2f)", vectorSpeed.x, vectorSpeed.y, vectorSpeed.z);
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
