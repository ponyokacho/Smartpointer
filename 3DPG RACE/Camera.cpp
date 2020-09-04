#include "DxLib.h"
#include "GameTask.h"
#include "Camera.h"
#include "Player.h"

Camera::Camera(std::shared_ptr<Player> p) : player(*p)
{
	Init();
}

void Camera::Init()
{
	// ----- ��ׂ̕`��͈͂�ݒ肷��
	// (���s10.0�`1000�܂ł��ׂ̕`��͈͂Ƃ���)
	c_near = 10.0f;	// ��O
	c_far = 40000.0f;	// ��
	SetCameraNearFar(c_near, c_far);	// �������

	// ----- ��ׂ̎����ύX����
	fov = (DX_PI_F/180) * 60;	// ����(60�x)
	SetupCamera_Perspective(fov);	// �������

	// ----- ���ߖ@���(���s���̂Ȃ����)
	//ySize = 500.0f;	// ���ߖ@��ׂ̏ꍇ��Y���W�ر�ݒ�
	//SetupCamera_Ortho(ySize);

	// ----- ��l�̶�ׂ̏����l
	//pos = VGet(0.0f, 0.0f, -700.0f);
	pos = VGet(0.0f, 0.0f, 0.0f);
	target = VGet(0.0f, 0.0f, 0.0f);
}

void Camera::Update()
{
	//player->Update();
	pos = VGet(player.cam.pos.x, player.cam.pos.y, player.cam.pos.z);
	if (!player.cam.view)
	{
		target = VAdd(player.carPos, TARGET_OFFSET);
	}
	else
	{
		target = VAdd(player.carFrontPos,VGet(0.0f,player.deg.pitch * -200.0f,player.deg.roll * 200.0f));
	}


	// ��׾��
	SetCameraPositionAndTarget_UpVecY(pos, target);
}