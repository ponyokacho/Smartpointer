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
	carScl = VGet(1.0f, 1.0f, 1.0f);
	carMat = MGetScale(carScl);
	boxPos = carPos;
	wheelFRPos = VGet(0.0f, 0.0f, 0.0f);
	wheelFLPos = VGet(0.0f, 0.0f, 0.0f);
	wheelRRPos = VGet(0.0f, 0.0f, 0.0f);
	wheelRLPos = VGet(0.0f, 0.0f, 0.0f);

	camScl = VGet(1.0f,1.0f,1.0f);
	camMat = MGetScale(camScl);
	camPos = VGet(0.0f,0.0f,0.0f); 

	// �Ԃ̍��W����ɂ����邽�߂̕��s�ړ�
	offsetMat = MGetTranslate(PLAYER_POS_OFFSET);
	carOffsetPos = VTransform(carPos, offsetMat);
	carPos = carOffsetPos;
	carMat = MGetScale(carScl);

	beforeCarPos = carPos;
}

//---���ި��β�ق�Ǐ]������
tuple<VECTOR,VECTOR,VECTOR,VECTOR,float> Player::Update(const VECTOR2 tireForce,const VECTOR2 dirVec,const VECTOR2 fWheelVec, const float speed, const int lr)
{
	this->speed = speed;
	this->tireForce = VGet(tireForce.x * lr, 0.0f, tireForce.y);
	this->dirVec = VGet(dirVec.x, 0.0f, dirVec.y);
	this->fWheelVec = VGet(fWheelVec.x, 0.0f, fWheelVec.y);

	//float tmp = (10.0f * (tanhf((1.0f / 100.0f) * pow(this->speed, 2.0f))));

	// ���Ӳ�ފ֐�
	float tmp = (((tanhf(this->speed - 100.0f / 2.0f) + 1.0f) / 2.0f) * 20.0f);
	//deg += (this->tireForce.x) * (DT * width * tanhf(this->speed / 2) + width / 2);
	deg += (this->tireForce.x) * (DT * (tmp + 2.5f)) * lr;
	dirVecRot = VTransform(this->dirVec, MMult(carMat, MGetRotY(deg)));
	// vectorSpeed�͉�]������K�v�Ȃ�

	// �����tireForce.x����]
	MV1SetMatrix(carModel, MGetRotY(deg));

	//addMoveVec = VAdd(VGet((this->tireForce.x * lr) * DT, 0.0f, (this->tireForce.z * this->speed) * DT),addMoveVec);

	// �Ԃ̈ړ�
	tireForceRot = VTransform(this->tireForce, MMult(carMat, MGetRotY(deg)));
	moveMat = MGetTranslate(VGet(tireForceRot.x * this->speed, this->tireForce.y, tireForceRot.z * this->speed));
	carPos = VTransform(carPos, moveMat);

	MV1SetMatrix(carModel, moveMat);

	VECTOR playerFrontPosOffset = VTransform(VGet(0.0f, 0.0f, 500.0f),MMult(carMat,MGetRotY(deg)));
	carFrontPos = VAdd(carPos, playerFrontPosOffset);

	// �J�����ړ��E��]
	VECTOR camPosOffset = VTransform(CAMERA_OFFSET, MMult(camMat, MGetRotY(deg)));
	camPos = VAdd(carPos, camPosOffset);

	// MV1SetMatrix()���g�p����MV1SetPosition()���g���Ȃ����߁A
	// �P�ʍs���K�p���āAMV1SetPosition()���g�p�ł���悤�ɂ���B
	MV1SetMatrix(carModel, MGetIdent());

	MV1SetPosition(carModel, carPos);
	MV1SetRotationXYZ(carModel, VGet(0.0f, deg, 0.0f));

	MV1SetPosition(boxModel, carFrontPos);

	// �O�t������̈ړ���
	vectorSpeed = VGet(dirVecRot.x * this->speed,dirVecRot.y,dirVecRot.z * this->speed);
	vectorSpeed = VSub(beforeCarPos,carPos);
	vectorSpeed.x *= -1;
	vectorSpeed.z *= -1;
	beforeCarPos = carPos;
	
	acceleration = -(oldSpeed - this->speed) * DT;
	oldSpeed = this->speed;

	DrawFormatString(0, 40, 0xffffff, "tireForce.x,y,z(%.2f,%.2f,%.2f)", this->tireForce.x, this->tireForce.y, this->tireForce.z);
	DrawFormatString(0, 80, 0xffffff, "speed:%.2f", this->speed);
	DrawFormatString(0, 100, 0xffffff, "deg:%.3f", deg);
	DrawFormatString(0, 120, 0xffffff, "acceleration:(%.5f)", acceleration);

	return forward_as_tuple(vectorSpeed,dirVecRot,beforeCarPos,this->fWheelVec,acceleration);
}

void Player::Render()
{
	//MV1DrawModel(boxModel);
	MV1DrawModel(carModel);
	MV1DrawModel(wheelFRModel);
	MV1DrawModel(wheelFLModel);
	MV1DrawModel(wheelRRModel);
	MV1DrawModel(wheelRLModel);
	MV1DrawModel(camModel);

	VECTOR tmp = { vectorSpeed.x * 500.0f ,vectorSpeed.y * 500.0f ,vectorSpeed.z * 500.0f };
	VECTOR tmp1 = { dirVecRot.x * 500.0f ,0.0f ,dirVecRot.z * 500.0f };
	VECTOR tmp2 = { fWheelVec.x * 500.0f ,0.0f ,fWheelVec.z * 500.0f };

	DrawLine3D(carPos, VAdd(carPos, tmp), 0xffffff);
	DrawLine3D(carPos, VAdd(carPos, tmp1), 0xff0000);
	DrawLine3D(carPos, VAdd(carPos, tmp2), 0xffff00);

	DrawFormatString(0, 0, 0xffffff, "carPos.x,y,z(%.2f,%.2f,%.2f)", carPos.x * DT, carPos.y * DT, carPos.z * DT);
	DrawFormatString(0, 20, 0xffffff, "vectorSpeed.x,y,z(%.2f,%.2f,%.2f)", vectorSpeed.x, vectorSpeed.y, vectorSpeed.z);
}

// �O��
float Player::Cross(VECTOR va, VECTOR vb)
{
	return ((va.z * vb.x) - (vb.z * va.x));
}

// ����
float Player::Dot(VECTOR va, VECTOR vb)
{
	return ((va.x * vb.x) + (va.z * vb.z));
}