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

	// �Ԃ̍��W����ɂ����邽�߂̕��s�ړ�
	moveMat = MGetTranslate(PLAYER_POS_OFFSET);
	carPos = VTransform(carPos, moveMat);
}

//---���ި��β�ق�Ǐ]������
void Player::Update(VECTOR2 pos, VECTOR2 dirVec,VECTOR2 yawVec,float wheelAngle,float deg)
{

	// �����deg����]
	MV1SetMatrix(carModel, MGetRotY(deg));

	camPos = VAdd(carPos, CAMERA_OFFSET);

	// MV1SetMatrix()���g�p����MV1SetPosition()���g���Ȃ����߁A
	// �P�ʍs���K�p���āAMV1SetPosition()���g�p�ł���悤�ɂ���B
	MV1SetMatrix(carModel, MGetIdent());

	MV1SetPosition(carModel, carPos);
	MV1SetRotationXYZ(carModel, VGet(0.0f, deg, 0.0f));
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
