#include "DxLib.h"
#include "GameTask.h"
#include "Camera.h"
#include "Player.h"

Camera::Camera(Player* p)
{
	player = p;
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
	pos = VGet(0.0f, 350.0f, -1500.0f);
	target = VGet(0.0f, 0.0f, 0.0f);


}

void Camera::Update()
{
	//player->Update();
	pos = VGet(player->boxCamPos.x, player->boxCamPos.y, player->boxCamPos.z);
	target = VGet(player->boxPos.x, player->boxPos.y + 200.0f, player->boxPos.z);

	// test
	//pos = VGet(0, player->boxCamPos.y, 0);
	//target = VGet(player->boxPos.x, player->boxPos.y, player->boxPos.z);

	// ----- SetCameraPositionAndTarget_UpVecY(pos, target)���g�p����ꍇ

	// ��׾��
	SetCameraPositionAndTarget_UpVecY(pos, target);
	//SetCameraPositionAndAngle(pos, 0.0f,player->front.x * player->front.z, 0.0f);

	//DrawFormatString(0, 16 * 3, 0xffffff,
		//	"camPos( %.1f, %.1f, %.1f )", pos.x, pos.y, pos.z, true);

	//DrawFormatString(0, 16 * 4, 0xffffff,
		//	"targetPos( %.1f, %.1f, %.1f )", target.x, target.y, target.z, true);
}