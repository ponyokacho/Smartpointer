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

	for (auto i : p)
	{
		tie(vectorSpeed) = (*i).Update(tireForce,speed);
		(*i).Render();
	}
	for (auto i : c)
	{
		(*i).Update();
	}
	for (auto i : f)
	{
		(*i).Update();
		(*i).Render();
	}

	for (auto i : e)
	{
		tie(engineTorque, rpm, onlyEngineVel) = (*i).Update(accel);
		(*i).Draw(accel, input.RightTrigger);
	}
	for (auto i : d)
	{
		tie(driveTireVel, wheelTorque, speed) = (*i).Update(clutch, engineTorque, rpm, gearNum, onlyEngineVel);
		(*i).Draw(clutch, gearNum);
	}
	for (auto i : t)
	{
		tie(tireForce) = (*i).Update(engineTorque, steering, gearNum, accel, driveTireVel, speed, vectorSpeed);
		(*i).Draw();
	}

}

void GameTask::Control()
{
	KeyMng::GetInstance().Update();

	GetJoypadXInputState(DX_INPUT_PAD1, &input);
	accel = input.RightTrigger;
	throttlePercent = 1.0f / 255.0f;
	accel *= throttlePercent;

	clutch = input.ThumbRY;
	clutchPercent = 1.0f / 32767;
	clutch *= clutchPercent;
	clutch = 1 - clutch;
	if (clutch < 0.05f)
	{
		clutch = 0.0f;
	}

	steering = input.ThumbLX;
	steeringPercent = 1.0f / 32767;
	steering *= clutchPercent;
	if (steering < 0.15f && steering > -0.15f)
	{
		steering = 0.0f;
	}

	//if (clutch < 0.5f)
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
		}
	}
}
