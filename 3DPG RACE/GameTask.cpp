#include "GameTask.h"
#include "KeyMng.h"
#include "DxLib.h"
#include "Player.h"
#include "Field.h"
#include "Camera.h"

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

	GameInit();
	return 0;
}

void GameTask::GameInit()
{
	e.push_back(make_shared<Engine>());
	d.push_back(make_shared<DriveTrain>());
	t.push_back(make_shared<Tire>());

	p.push_back(std::make_shared<Player>());
	c.push_back(std::make_shared<Camera>(p.back()));
	f.push_back(std::make_shared<Field>());
}

void GameTask::GameUpdate()
{
	Control();

	if (clutch > 1.0f)
	{
		clutch = 1.0f;
	}

	DrawBox(0, 0, SCREEN_SIZE_X, SCREEN_SIZE_Y, 0x9DCCff, true);

	for (auto i : p)
	{
		if (!t.empty())
		{
			tie(vectorSpeed,vectorSpeedRot, dirVecRot, carPos, fWheelVecRot, acceleration) = (*i).Update(tireForce,dirVec,fWheelVec,speed,lr,steering);
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
		tie(engineTorque, rpm, onlyEngineVel) = (*i).Update(accel);
		i->Draw(accel, input.RightTrigger);
	}
	for (auto i : d)
	{
		i->SetSoundVol(volume);
		tie(driveTireVel, wheelTorque, speed) = (*i).Update(clutch, engineTorque, rpm, gearNum, onlyEngineVel);
		i->Draw(clutch, gearNum);
	}
	for (auto i : t)
	{
		tie(tireForce, dirVec, fWheelVec, lr) = (*i).Update(engineTorque, steering, gearNum, accel, driveTireVel, speed, vectorSpeed, vectorSpeedRot, dirVecRot, fWheelVecRot,acceleration);
		i->Draw();

		np = i->GetPitchLoad();
		nr = i->GetRollLoad();
	}

}

void GameTask::Control()
{
	KeyMng::GetInstance().Update();

	GetJoypadXInputState(DX_INPUT_PAD1, &input);

	if (shiftUp)
	{
		accel = 0.0f;
		gearNum = -1;
		shiftUpCnt++;
		if (shiftUpCnt == 30)
		{
			shiftUpCnt = 0;
			gearNum = saveGearNum;
			shiftUp = false;
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

	if (!transmission)
	{
		if (KeyMng::GetInstance().trgKey[P1_LB])
		{
			gearNum--;
			if (gearNum < -1)
			{
				gearNum = -1;
			}
		}
		if (KeyMng::GetInstance().trgKey[P1_RB])
		{
			gearNum++;
			if (gearNum > MAX_GEAR - 1)
			{
				gearNum = MAX_GEAR - 1;
			}
			shiftUp = true;
			saveGearNum = gearNum;
		}
	}
	else
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

			if (gearNum == 1)
			{
				shiftDownTiming = 10.0f;
			}
			else
			{
				shiftDownTiming = 50.0f;
			}

			if (gearMinSpeed[gearNum] + shiftDownTiming > speed)
			{
				if (gearNum != 0)
				{
					gearNum--;
					if (gearNum < -1)
					{
						gearNum = -1;
					}
				}
			}
		}
	}
}
