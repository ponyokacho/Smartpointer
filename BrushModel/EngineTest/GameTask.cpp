#include "GameTask.h"
#include "DxLib.h"
#include <cmath>
#include "KeyMng.h"

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
}

void GameTask::GameInit()
{
	KeyMng::GetInstance().Init();

	dp_a.push_back(make_shared<Drawpoint>(tireForce_a, slipRate_a, slipAngle_a, true));
}

void GameTask::Update()
{
	KeyMng::GetInstance().Update();

	if (KeyMng::GetInstance().trgKey[P1_SPACE])
	{
		isDrive = !isDrive;
	}

	if (tmp != isDrive)
	{
		slipRate = -1.0f;
		slipAngle = 0.0f;
		dp.clear();
	}
	tmp = isDrive;


	if (slipAngle < 1.0f)
	{
		tireForce = TireForce(slipRate, slipAngle);

		if (slipRate < 1.0f)
		{
			dp.push_back(make_shared<Drawpoint>(tireForce, slipRate, slipAngle, false));
			slipRate += 0.01f;
		}
		else
		{
			slipRate = -1.0f;
			slipAngle += 0.2f;
		}
	}

	if (KeyMng::GetInstance().newKey[P1_UP])
	{
		count++;
		if (count > 100)
		{
			slipRate_a += 0.001f;
			if (slipRate_a > 1.0f)
			{
				slipRate_a = 1.0f;
			}
		}
	}
	else if (KeyMng::GetInstance().newKey[P1_DOWN])
	{
		count++;
		if (count > 100)
		{
			slipRate_a -= 0.001f;
			if (slipRate_a < -1.0f)
			{
				slipRate_a = -1.0f;
			}
		}
	}
	else if (KeyMng::GetInstance().newKey[P1_RIGHT])
	{
		count++;
		if (count > 100)
		{
			slipAngle_a += 0.001f;
			if (slipAngle_a > 1.0f)
			{
				slipAngle_a = 1.0f;
			}
		}
	}
	else if (KeyMng::GetInstance().newKey[P1_LEFT])
	{
		count++;
		if (count > 100)
		{
			slipAngle_a -= 0.001f;
			if (slipAngle_a < 0.0f)
			{
				slipAngle_a = 0.0f;
			}
		}
	}
	else
	{
		count = 0;
	}

	if (KeyMng::GetInstance().trgKey[P1_UP])
	{
		slipRate_a += 0.01f;
		if (slipRate_a > 1.0f)
		{
			slipRate_a = 1.0f;
		}
	}
	if (KeyMng::GetInstance().trgKey[P1_DOWN])
	{
		slipRate_a -= 0.01f;
		if (slipRate_a < -1.0f)
		{
			slipRate_a = -1.0f;
		}
	}
	if (KeyMng::GetInstance().trgKey[P1_RIGHT])
	{
		slipAngle_a += 0.01f;
		if (slipAngle_a > 1.0f)
		{
			slipAngle_a = 1.0f;
		}
	}
	if (KeyMng::GetInstance().trgKey[P1_LEFT])
	{
		slipAngle_a -= 0.01f;
		if (slipAngle_a < 0.0f)
		{
			slipAngle_a = 0.0f;
		}
	}

	tireForce_a = TireForce(slipRate_a, slipAngle_a);

	if (tireForce_a_old != tireForce_a)
	{
		dp_a.pop_back();
		dp_a.push_back(make_shared<Drawpoint>(tireForce_a, slipRate_a, slipAngle_a, true));
	}

	Draw();
	tireForce_a_old = tireForce_a;
}

void GameTask::Draw()
{	
	DrawLine(0, SCREEN_SIZE_Y / 2, SCREEN_SIZE_X, SCREEN_SIZE_Y / 2, 0xffffff, 1);
	DrawString(SCREEN_SIZE_X / 2 + 5, 10, "駆動力(赤,黄)・横力(青,水)", 0xffffff);
	DrawString(SCREEN_SIZE_X - 50, SCREEN_SIZE_Y / 2 - 20, "滑り比", 0xffffff);
	DrawLine(SCREEN_SIZE_X / 2, 0, SCREEN_SIZE_X / 2, SCREEN_SIZE_Y, 0xffffff, 1);

	for (auto i : dp)
	{
		(*i).Draw();
	}

	for (auto i : dp_a)
	{
		(*i).Draw();
	}

	if (isDrive)
	{
		DrawString(5, 5, "駆動", 0xffffff);
		DrawFormatString(5, 25, 0xffffff, "指定値:SlipRate(%.2f),slipAngle(%.2f)", slipRate_a, slipAngle_a);
		DrawFormatString(5, 45, 0xffffff, "指定値でのタイヤフォース:(%.2f,%.2f)", tireForce_a.x, tireForce_a.y);
		DrawFormatString(5, 65, 0xffffff, "指定値でのタイヤフォース(正規化):(%.2f,%.2f)", tireForce_a.Normalize().x, tireForce_a.Normalize().y);
	}
	else
	{
		DrawString(5, 5, "制動", 0xffffff);
		DrawFormatString(5, 25, 0xffffff, "指定値:SlipRate(%.2f),slipAngle(%.2f)", slipRate_a, slipAngle_a);
		DrawFormatString(5, 45, 0xffffff, "指定値でのタイヤフォース:(%.2f,%.2f)", tireForce_a.x, tireForce_a.y);
		DrawFormatString(5, 65, 0xffffff, "指定値でのタイヤフォース(正規化):(%.2f,%.2f)", tireForce_a.Normalize().x, tireForce_a.Normalize().y);
	}
}

VECTOR2 GameTask::TireForce(float slipRate,float slipAngle)
{
	VECTOR2 f = { 0,0 };

	if (isDrive)
	{
		float lambda = sqrt(pow(slipRate, 2) + pow((Ktheta / Ks), 2) * pow((1 - slipRate), 2) * pow(tan(slipAngle), 2));
		float cTheta = -slipRate / lambda;
		float sTheta = (Ktheta * tan(slipAngle) * (1.0f - slipRate)) / (Ks * lambda);
		if (slipRate == 0.0f && lambda == 0.0f)
		{
			cTheta = 0.0f;
			sTheta = 0.0f;
		}
		float epsilon = 1 - (Ks / (3.0f * MU * ONE_TIRE_LOAD) * lambda);

		if (epsilon > 0)
		{
			f.y = Ks * slipRate * pow(epsilon, 2) - 6.0f * MU * ONE_TIRE_LOAD * cTheta * (1.0f / 6.0f - 1.0f / 2.0f * pow(epsilon, 2) + 1.0f / 3.0f * pow(epsilon, 3));
			f.x = Ktheta * (1 - slipRate) * tan(slipAngle) * pow(epsilon, 2) + 6.0f * MU * ONE_TIRE_LOAD * sTheta * (1.0f / 6.0f - 1.0f / 2.0f * pow(epsilon, 2) + 1.0f / 3.0f * pow(epsilon, 3));
		}
		else if (epsilon <= 0)
		{
			f.y = -MU * ONE_TIRE_LOAD * cTheta;
			f.x = MU * ONE_TIRE_LOAD * sTheta;
		}
	}
	else
	{
		float lambda = sqrt(pow(slipRate, 2) + pow((Ktheta / Ks), 2) * pow(tan(slipAngle), 2));
		float cTheta = -slipRate / lambda;
		float sTheta = (Ktheta * tan(slipAngle)) / (Ks * lambda);
		if (slipRate == 0.0f && lambda == 0.0f)
		{
			cTheta = 0.0f;
			sTheta = 0.0f;
		}
		float epsilon = 1 - (Ks / (3.0f * MU * ONE_TIRE_LOAD) * (lambda / (1 + slipRate)));

		if (epsilon > 0)
		{
			f.y = ((Ks * slipRate) / (1.0f + slipRate)) * pow(epsilon, 2) - 6.0f * MU * ONE_TIRE_LOAD * cTheta * (1.0f / 6.0f - 1.0f / 2.0f * pow(epsilon, 2) + 1.0f / 3.0f * pow(epsilon, 3));
			f.x = ((Ktheta * tan(slipAngle)) / (1 + slipRate)) * pow(epsilon, 2) + 6.0f * MU * ONE_TIRE_LOAD * sTheta * (1.0f / 6.0f - 1.0f / 2.0f * pow(epsilon, 2) + 1.0f / 3.0f * pow(epsilon, 3));
		}
		else if (epsilon <= 0)
		{
			f.y = -MU * ONE_TIRE_LOAD * cTheta;
			f.x = MU * ONE_TIRE_LOAD * sTheta/* - tmp*/;
		}
	}
	return f;
}
