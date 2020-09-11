 #include "GameTask.h"
#include "KeyMng.h"
#include "DxLib.h"
#include "Player.h"
#include "Field.h"
#include "Camera.h"
#include "ResourceMng.h"
#include "Engine.h"
#include "DriveTrain.h"
#include "Tire.h"
#include "UI.h"

#include <vector>
#include <memory>

GameTask *GameTask::s_Instance = nullptr;

void GameTask::Create(void)
{
	if (!s_Instance)
	{
		s_Instance = new GameTask();
	}
}

int GameTask::SystemInit()
{
	SetWindowText ("Template");
	SetGraphMode(SCREEN_SIZE_X, SCREEN_SIZE_Y, 16);
	ChangeWindowMode(true);
	if (DxLib_Init() == -1) {
		return -1;
	}
	SetDrawScreen(DX_SCREEN_BACK);
	
	updateMode = MENU;

	GameInit();

	return 0;
}

void GameTask::GameInit()
{
	e.push_back(make_shared<Engine>());
	d.push_back(make_shared<DriveTrain>());
	t.push_back(make_shared<Tire>());

	p.push_back(make_shared<Player>());
	c.push_back(make_shared<Camera>(p.back()));
	f.push_back(make_shared<Field>());
	
	u.push_back(make_shared<UI>());

	ChangeVolumeSoundMem(255 * 80 / 100, SOUND_ID("sounds/shift_change.wav"));

	DIV_IMAGE_ID("image/brake2types.png", 2, 2, 1, 0, 0, 2778, 2505, absImg);
	DIV_IMAGE_ID("image/gear2types.png", 2, 2, 1, 0, 0, 1826, 2753,gearImg);
	DIV_IMAGE_ID("image/mt_m_2types.png", 2, 2, 1, 0, 0, 166, 97, mtImg);
	DIV_IMAGE_ID("image/at_m_2types.png", 2, 2, 1, 0, 0, 156, 97, atImg);
	DIV_IMAGE_ID("image/line_m_all.png", 4, 1, 4, 0, 0, 807, 49, lineImg);
	DIV_IMAGE_ID("image/strong_m2types.png", 2, 2, 1, 0, 0, 484, 97, strongImg);
	DIV_IMAGE_ID("image/weak_m2types.png", 2, 2, 1, 0, 0, 363, 97, weakImg);
	count = 0;
}

void GameTask::GameUpdate()
{
	KeyMng::GetInstance().Update();

	FPS();

	(this->*mode[updateMode])();
}

void GameTask::GameMenu()
{
	DrawFormatString(0, 0, 0xff0000, "MENU");

	for (auto i : f)
	{
		i->Update();
		i->Render();
	}
	for (auto i : c)
	{
		i->Update();
	}

	if (_abs.flag)
	{
		bs = IMAGE_ID("image/on_m.png");
	}
	else
	{
		bs = IMAGE_ID("image/off_m.png");
	}

	if (transmission)
	{
		tm = mtImg[0];
	}
	else
	{
		tm = atImg[0];
	}
	DrawRotaGraph(SCREEN_SIZE_X / 2, SCREEN_SIZE_Y / 2, 1.0f, 0.0f, IMAGE_ID("image/toka.png"), true);
	DrawRotaGraph(SCREEN_SIZE_X / 2, 80, 0.6f, 0.0f, IMAGE_ID("image/setting.png"), true);
	DrawRotaGraph(SCREEN_SIZE_X / 2 - 200, SCREEN_SIZE_Y / 2, 0.07f + sizeB, 0.0f, absImg[!_abs.flag], true);
	DrawRotaGraph(SCREEN_SIZE_X / 2 + 200, SCREEN_SIZE_Y / 2, 0.07f + sizeG, 0.0f, gearImg[!transmission], true);

	DrawRotaGraph(SCREEN_SIZE_X / 2 - 250, SCREEN_SIZE_Y / 2 + 150, 0.3f, 0.0f, IMAGE_ID("image/abs_m.png"), true);
	DrawRotaGraph(SCREEN_SIZE_X / 2 - 200, SCREEN_SIZE_Y / 2 + 150, 0.3f, 0.0f, IMAGE_ID("image/colon.png"), true);
	DrawRotaGraph(SCREEN_SIZE_X / 2 - 150, SCREEN_SIZE_Y / 2 + 150, 0.3f, 0.0f, bs, true);


	DrawRotaGraph(SCREEN_SIZE_X / 2 + 150, SCREEN_SIZE_Y / 2 + 150, 0.3f, 0.0f, IMAGE_ID("image/transmission_m.png"), true);
	DrawRotaGraph(SCREEN_SIZE_X / 2 + 200, SCREEN_SIZE_Y / 2 + 150, 0.3f, 0.0f, IMAGE_ID("image/colon.png"), true);
	DrawRotaGraph(SCREEN_SIZE_X / 2 + 250, SCREEN_SIZE_Y / 2 + 150, 0.3f, 0.0f, tm, true);
	DrawRotaGraph(SCREEN_SIZE_X - 100, SCREEN_SIZE_Y - 70 + (5.0f * sin(count / 10.0f)), 0.2f, 0.0f, IMAGE_ID("image/XboxCon.png"), true);
	DrawRotaGraph(SCREEN_SIZE_X - 275, SCREEN_SIZE_Y - 65 + (5.0f * sin(count / 10.0f)), 0.2f, 0.0f, IMAGE_ID("image/pressStart.png"), true);
	DrawRotaGraph(SCREEN_SIZE_X - 232, SCREEN_SIZE_Y - 40 + (5.0f * sin(count / 10.0f)), 0.19f, 0.0f, IMAGE_ID("image/select.png"), true);

	if (_abs.flag)
	{
		if (_abs.power != 3)
		{
			DrawRotaGraph(SCREEN_SIZE_X / 2 - 200, SCREEN_SIZE_Y / 2 + 200, 0.3f, 0.0f, lineImg[_abs.power + 1], true);
		}
		DrawRotaGraph(SCREEN_SIZE_X / 2 - 310, SCREEN_SIZE_Y / 2 + 230, 0.2f, 0.0f, weakImg[0], true);
		DrawRotaGraph(SCREEN_SIZE_X / 2 - 90, SCREEN_SIZE_Y / 2 + 230, 0.2f, 0.0f, strongImg[0], true);

		if (_abs.power == 0)
		{
			pointPos = VECTOR2(SCREEN_SIZE_X / 2 - 315, SCREEN_SIZE_Y / 2 + 200);
		}
		else if (_abs.power == 1)
		{
			pointPos = VECTOR2(SCREEN_SIZE_X / 2 - 200, SCREEN_SIZE_Y / 2 + 200);
		}
		else
		{
			pointPos = VECTOR2(SCREEN_SIZE_X / 2 - 85, SCREEN_SIZE_Y / 2 + 200);
		}
		DrawRotaGraph(pointPos.x,pointPos.y, 0.2f + sizeP, 0.0f, IMAGE_ID("image/redPoint.png"), true);
	}
	else
	{
		DrawRotaGraph(SCREEN_SIZE_X / 2 - 200, SCREEN_SIZE_Y / 2 + 200, 0.3f, 0.0f, lineImg[0], true);
		DrawRotaGraph(SCREEN_SIZE_X / 2 - 310, SCREEN_SIZE_Y / 2 + 230, 0.2f, 0.0f, weakImg[1], true);
		DrawRotaGraph(SCREEN_SIZE_X / 2 - 90, SCREEN_SIZE_Y / 2 + 230, 0.2f, 0.0f, strongImg[1], true);
	}


	pov = GetJoypadPOVState(DX_INPUT_PAD1, 0);

	if (chooseNumY == 0)
	{
		if (pov == 9000 && pov != oldPov)
		{
			chooseNumX++;
			if (chooseNumX > 1)
			{
				chooseNumX = 1;
			}
		}
		else if (pov == 27000 && pov != oldPov)
		{
			chooseNumX--;
			if (chooseNumX < 0)
			{
				chooseNumX = 0;
			}
		}
	}
	else
	{
		count++;
		sizeG = 0.0f;
		sizeB = 0.0f;
		sizeP = (0.07f * sin(count / 10.0f)) + 0.07f;
		if (pov == 9000 && pov != oldPov)
		{
			_abs.power++;
			if (_abs.power > 2)
			{
				_abs.power = 2;
			}
		}
		else if (pov == 27000 && pov != oldPov)
		{
			_abs.power--;
			if (_abs.power < 0)
			{
				_abs.power = 0;
			}
		}
	}
	
	if (chooseNumX == 0 && _abs.flag)
	{
		if (pov == 0 && pov != oldPov)
		{
			chooseNumY--;
			if (chooseNumY < 0)
			{
				chooseNumY = 0;
			}
		}
		else if (pov == 18000 && pov != oldPov)
		{
			chooseNumY++;
			if (chooseNumY > 1)
			{
				chooseNumY = 1;
			}
		}
	}

	oldPov = pov;

	if (chooseNumX == 0 && chooseNumY == 0)
	{
		count++;
		sizeG = 0.0f;
		sizeB = 0.01f * sin(count / 10.0f);
		sizeP = 0.0f;
		if (lpKeyMng.trgKey[P1_B])
		{
			_abs.flag = !_abs.flag;
		}
	}
	else if (chooseNumX == 1 && chooseNumY == 0)
	{
		count++;
		sizeB = 0.0f;
		sizeG = 0.01f * sin(count / 10.0f);
		sizeP = 0.0f;
		if (lpKeyMng.trgKey[P1_B])
		{
			transmission = !transmission;
		}
	}

	if (lpKeyMng.trgKey[P1_START])
	{
		updateMode = MAIN;
	}
}

void GameTask::GameMain()
{
	Control();

	if (clutch > 1.0f)
	{
		clutch = 1.0f;
	}

	DrawBox(0, 0, SCREEN_SIZE_X, SCREEN_SIZE_Y, 0x9DCCff, true);
	//DrawFormatString(0, 0, 0xffffff, "%d", fps);

	for (auto i : p)
	{
		if (!t.empty())
		{
			(*i).Update();
			i->Render();
		}
	}
	for (auto i : c)
	{
		i->Update();
	}
	for (auto i : f)
	{
		i->Update();
		i->Render(carPos);
	}

	for (auto i : e)
	{
		(*i).Update();
		i->Draw();
	}
	for (auto i : d)
	{
		i->SetSoundVol(volume);
		(*i).Update();
		i->Draw();
	}
	for (auto i : t)
	{
		(*i).Update();
		i->Draw();

		np = i->GetPitchLoad();
		nr = i->GetRollLoad();
	}

	for (auto i : u)
	{
		i->Update();
		i->Draw();
	}

	DrawFormatString(0, 0, 0xff0000, "carPos.x,y,z(%.2f,%.2f,%.2f)", carPos.x, carPos.y, carPos.z);
}

void GameTask::Control()
{
	GetJoypadXInputState(DX_INPUT_PAD1, &input);

	shift = (shiftUp || shiftDown);

	if (shift)
	{
		accel = 0.0f;
		gearNum = -1;

		int tmp = 0.0f;
		if (shiftUp)
		{
			tmp = 30;
		}
		else
		{
			tmp = 10;
		}

		shiftCnt++;
		if (shiftCnt == tmp)
		{
			shiftCnt = 0;
			gearNum = saveGearNum;
			shiftUp = false;
			shiftDown = false;
		}

		if (shiftCnt == 1)
		{
			if (!CheckSoundMem(SOUND_ID("sounds/shift_change.wav"))) 
			{
				PlaySoundMem(SOUND_ID("sounds/shift_change.wav"), DX_PLAYTYPE_BACK);
			}
		}
	}
	else
	{
		accel = input.RightTrigger;
		throttlePercent = 1.0f / 255.0f;
		accel *= throttlePercent;
	}

	volume = pow(1.025f, (accel * 100)) + 90;

	brake = input.LeftTrigger;
	brake *= throttlePercent;

	if (gearNum == -1)
	{
		clutch = 0.0f;
	}
	else
	{
		clutch = 1.0f;
	}

	steering = input.ThumbLX;
	steeringPercent = 1.0f / 32767;
	steering *= steeringPercent;
	if (steering < 0.15f && steering > -0.15f)
	{
		steering = 0.0f;
	}

	if (transmission)
	{
		if (!shift)
		{
			if (KeyMng::GetInstance().trgKey[P1_LB])
			{	
				if (gearNum < -1)
				{
					gearNum = -1;
				}
				if (gearNum > 0)
				{
					gearNum--;
					shiftDown = true;
					saveGearNum = gearNum;
				}
			}
			if (KeyMng::GetInstance().trgKey[P1_RB])
			{
				gearNum++;
				if (gearNum > MAX_GEAR - 1)
				{
					gearNum = MAX_GEAR - 1;
				}
				else
				{
					shiftUp = true;
					saveGearNum = gearNum;
				}
			}
		}
	}
	else
	{
		if (!shift)
		{
			if (gearNum != -1)
			{
				if (gearMaxSpeed[gearNum] - 10.0f < speed)
				{
					if (gearNum != MAX_GEAR - 1)
					{
						gearNum++;
						shiftUp = true;
						saveGearNum = gearNum;
					}
				}

				shiftDownTiming = 20.0f * gearNum;

				if (gearMinSpeed[gearNum] + shiftDownTiming > speed)
				{
					if (gearNum != 0)
					{
						gearNum--;
						shiftDown = true;
						saveGearNum = gearNum;
						if (gearNum < -1)
						{
							gearNum = -1;
						}
					}
				}
			}
		}
	}
}

void GameTask::FPS()
{
	// 現在のシステム時間を取得
	NowTime = GetNowHiPerformanceCount();

	// 前回取得した時間からの経過時間を秒に変換してセット
	// ( GetNowHiPerformanceCount で取得できる値はマイクロ秒単位なので 1000000 で割ることで秒単位になる )
	deltaTime = (NowTime - Time) / 1000000.0f;

	// 今回取得した時間を保存
	Time = NowTime;

	// FPS関係の処理( 1秒経過する間に実行されたメインループの回数を FPS とする )
	FPSCounter++;
	if (NowTime - FPSCheckTime > 1000000)
	{
		fps = FPSCounter;
		FPSCounter = 0;
		FPSCheckTime = NowTime;
	}
}
