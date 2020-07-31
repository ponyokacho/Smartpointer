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
		slideRate = -1.0f;
		slideAngle = 0.0f;
		dp.clear();
	}
	tmp = isDrive;

	if (slideAngle < 1.0f)
	{
		if (isDrive)
		{
			float lambda = sqrt(pow(slideRate, 2) + pow((Ktheta / Ks), 2) * pow((1 - slideRate), 2) * pow(tan(slideAngle), 2));
			float cTheta = -slideRate / lambda;
			float sTheta = (Ktheta * tan(slideAngle) * (1.0f - slideRate)) / (Ks * lambda);
			if (slideRate == 0.0f && lambda == 0.0f)
			{
				cTheta = 0.0f;
				sTheta = 0.0f;
			}
			float epsilon = 1 - (Ks / (3.0f * MU * ONE_TIRE_LOAD) * lambda);

			if (epsilon > 0)
			{
				f.y = Ks * slideRate * pow(epsilon, 2) - 6.0f * MU * ONE_TIRE_LOAD * cTheta * (1.0f / 6.0f - 1.0f / 2.0f * pow(epsilon, 2) + 1.0f / 3.0f * pow(epsilon, 3));
				f.x = Ktheta * (1 - slideRate) * tan(slideAngle) * pow(epsilon, 2) + 6.0f * MU * ONE_TIRE_LOAD * sTheta * (1.0f / 6.0f - 1.0f / 2.0f * pow(epsilon, 2) + 1.0f / 3.0f * pow(epsilon, 3));
			}
			else if (epsilon <= 0)
			{
				f.y = -MU * ONE_TIRE_LOAD * cTheta;
				f.x = MU * ONE_TIRE_LOAD * sTheta;
			}

			if (slideRate < 1.0f)
			{
				dp.push_back(make_shared<Drawpoint>(f, slideRate, slideAngle));
				slideRate += 0.01f;
			}
			else
			{
				slideRate = -1.0f;
				slideAngle += 0.2f;
			}
		}
		else
		{
			float lambda = sqrt(pow(slideRate, 2) + pow((Ktheta / Ks), 2) * pow(tan(slideAngle), 2));
			float cTheta = -slideRate / lambda;
			float sTheta = (Ktheta * tan(slideAngle)) / (Ks * lambda);
			if (slideRate == 0.0f && lambda == 0.0f)
			{
				cTheta = 0.0f;
				sTheta = 0.0f;
			}
			float epsilon = 1 - (Ks / (3.0f * MU * ONE_TIRE_LOAD) * (lambda / (1 + slideRate)));

			if (epsilon > 0)
			{
				f.y = ((Ks * slideRate) / (1.0f + slideRate)) * pow(epsilon, 2) - 6.0f * MU * ONE_TIRE_LOAD * cTheta * (1.0f / 6.0f - 1.0f / 2.0f * pow(epsilon, 2) + 1.0f / 3.0f * pow(epsilon, 3));
				f.x = ((Ktheta * tan(slideAngle)) / (1 + slideRate)) * pow(epsilon,2) + 6.0f * MU * ONE_TIRE_LOAD * sTheta * (1.0f / 6.0f - 1.0f / 2.0f * pow(epsilon, 2) + 1.0f / 3.0f * pow(epsilon, 3));
			}
			else if (epsilon <= 0)
			{
				f.y = -MU * ONE_TIRE_LOAD * cTheta;
				f.x = MU * ONE_TIRE_LOAD * sTheta/* - tmp*/;
			}

			if (slideRate < 1.0f)
			{
				dp.push_back(make_shared<Drawpoint>(f, slideRate, slideAngle));
				slideRate += 0.01f;
			}
			else
			{
				slideRate = -1.0f;
				slideAngle += 0.2f;
			}
		}
	}

	Draw();
}

void GameTask::Draw()
{	
	DrawLine(0, SCREEN_SIZE_Y / 2, SCREEN_SIZE_X, SCREEN_SIZE_Y / 2, 0xffffff, 1);
	DrawString(SCREEN_SIZE_X / 2 + 5, 10, "ãÏìÆóÕ(ê‘)ÅEâ°óÕ(ê¬)", 0xffffff);
	DrawString(SCREEN_SIZE_X - 50, SCREEN_SIZE_Y / 2 - 20, "ääÇËî‰", 0xffffff);
	DrawLine(SCREEN_SIZE_X / 2, 0, SCREEN_SIZE_X / 2, SCREEN_SIZE_Y, 0xffffff, 1);

	if (isDrive)
	{
		DrawString(5, 5, "ãÏìÆ", 0xffffff);
	}
	else
	{
		DrawString(5, 5, "êßìÆ", 0xffffff);
	}

	for (auto i : dp)
	{
		(*i).Draw();
	}
}
