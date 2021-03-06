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
	// ----- ｶﾒﾗの描画範囲を設定する
	// (奥行10.0〜1000までをｶﾒﾗの描画範囲とする)
	c_near = 10.0f;	// 手前
	c_far = 600000.0f;	// 奥
	SetCameraNearFar(c_near, c_far);	// 初期ｾｯﾄ

	// ----- ｶﾒﾗの視野を変更する
	fov = (DX_PI_F/180) * 65;	// 視野(60度)
	SetupCamera_Perspective(fov);	// 初期ｾｯﾄ

	// ----- 遠近法ｶﾒﾗ(奥行情報のないｶﾒﾗ)
	//ySize = 500.0f;	// 遠近法ｶﾒﾗの場合のY座標ｴﾘｱ設定
	//SetupCamera_Ortho(ySize);

	// ----- 一人称ｶﾒﾗの初期値
	//pos = VGet(0.0f, 0.0f, -700.0f);
	pos = VGet(0.0f, 0.0f, 0.0f);
	target = VGet(0.0f, 0.0f, 0.0f);
}

void Camera::Update()
{
	if (!lpGameTask.GetOptionFlag())
	{
		pos = VGet(player.cam.pos.x, player.cam.pos.y, player.cam.pos.z);
		if (player.cam.view == 0)
		{
			target = VAdd(player.carPos, TARGET_OFFSET);
		}
		else if (player.cam.view == 1)
		{
			target = VAdd(player.carFrontPos, VGet(0.0f, player.deg.pitch * -200.0f, player.deg.roll * 200.0f));
		}
		else if (player.cam.view == 2)
		{
			target = player.tireViewFrontPos;
		}
		else
		{
			target = player.GetCarPos();
		}
	}
	else
	{
		pos = VGet(-12000, 400, 38928);
		target = VGet(-12858, 140, 38024);
	}


	// ｶﾒﾗｾｯﾄ
	SetCameraPositionAndTarget_UpVecY(pos, target);
}

void Camera::SetCamera(std::shared_ptr<Player> p)
{
	player = *p;
}
