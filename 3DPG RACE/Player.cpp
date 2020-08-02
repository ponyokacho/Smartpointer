#include "math.h"
#include "GameTask.h"
#include "Player.h"

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
	boxPos = carPos;
	wheelFRPos = VGet(0.0f, 0.0f, 0.0f);
	wheelFLPos = VGet(0.0f, 0.0f, 0.0f);
	wheelRRPos = VGet(0.0f, 0.0f, 0.0f);
	wheelRLPos = VGet(0.0f, 0.0f, 0.0f);

	camScl = VGet(1.0f,1.0f,1.0f);
	camMat = MGetScale(camScl);
	camPos = VGet(0.0f,0.0f,0.0f); 

	// 車の座標を上にあげるための平行移動
	offsetMat = MGetTranslate(PLAYER_POS_OFFSET);
	carOffsetPos = VTransform(carPos, offsetMat);
	carPos = carOffsetPos;
}

//---ﾎﾞﾃﾞｨにﾎｲｰﾙを追従させる
tuple<VECTOR> Player::Update(VECTOR2 tireForce,float speed)
{
	this->speed = speed;
	// これでtireForce.x分回転
	//MV1SetMatrix(carModel, MGetRotY(tireForce.x));

	if (speed == tireForce.y && tireForce.y != 0.0f)
	{
		this->speed = 1.0f;
	}
	// 車の移動
	moveMat = MGetTranslate(VGet(tireForce.x * this->speed,0.0f,tireForce.y * this->speed));
	carPos = VTransform(carPos, moveMat);

	MV1SetMatrix(carModel, moveMat);

	camPos = VAdd(carPos, CAMERA_OFFSET);

	// MV1SetMatrix()を使用中はMV1SetPosition()が使えないため、
	// 単位行列を適用して、MV1SetPosition()を使用できるようにする。
	MV1SetMatrix(carModel, MGetIdent());

	MV1SetPosition(carModel, carPos);
	MV1SetRotationXYZ(carModel, VGet(0.0f, tireForce.x, 0.0f));

	// 前フレからの移動量
	vectorSpeed = VSub(beforeCarPos,carPos);
	vectorSpeed.x *= -1;
	vectorSpeed.z *= -1;
	beforeCarPos = carPos;

	DrawFormatString(0, 40, 0xffffff, "tireForce.x,y(%.2f,%.2f)", tireForce.x, tireForce.y);
	DrawFormatString(0, 60, 0xffffff, "speed:%.2f", this->speed);

	return forward_as_tuple(vectorSpeed);
}

void Player::Render()
{
	MV1DrawModel(boxModel);
	MV1DrawModel(carModel);
	MV1DrawModel(wheelFRModel);
	MV1DrawModel(wheelFLModel);
	MV1DrawModel(wheelRRModel);
	MV1DrawModel(wheelRLModel);
	MV1DrawModel(camModel);

	DrawFormatString(0, 0, 0xffffff, "carPos.x,y,z(%.2f,%.2f,%.2f)", carPos.x, carPos.y, carPos.z);
	DrawFormatString(0, 20, 0xffffff, "vectorSpeed.x,y,z(%.2f,%.2f,%.2f)", vectorSpeed.x, vectorSpeed.y, vectorSpeed.z);
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
