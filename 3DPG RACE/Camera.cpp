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
	// ----- ｶﾒﾗの描画範囲を設定する
	// (奥行10.0～1000までをｶﾒﾗの描画範囲とする)
	c_near = 10.0f;	// 手前
	c_far = 40000.0f;	// 奥
	SetCameraNearFar(c_near, c_far);	// 初期ｾｯﾄ

	// ----- ｶﾒﾗの視野を変更する
	fov = (DX_PI_F/180) * 60;	// 視野(60度)
	SetupCamera_Perspective(fov);	// 初期ｾｯﾄ

	// ----- 遠近法ｶﾒﾗ(奥行情報のないｶﾒﾗ)
	//ySize = 500.0f;	// 遠近法ｶﾒﾗの場合のY座標ｴﾘｱ設定
	//SetupCamera_Ortho(ySize);

	// ----- 一人称ｶﾒﾗの初期値
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

	// ----- SetCameraPositionAndTarget_UpVecY(pos, target)を使用する場合

	// ｶﾒﾗｾｯﾄ
	SetCameraPositionAndTarget_UpVecY(pos, target);
	//SetCameraPositionAndAngle(pos, 0.0f,player->front.x * player->front.z, 0.0f);

	//DrawFormatString(0, 16 * 3, 0xffffff,
		//	"camPos( %.1f, %.1f, %.1f )", pos.x, pos.y, pos.z, true);

	//DrawFormatString(0, 16 * 4, 0xffffff,
		//	"targetPos( %.1f, %.1f, %.1f )", target.x, target.y, target.z, true);
}