#include "OptionScene.h"
#include "GameTask.h"
#include "KeyMng.h"
#include "ResourceMng.h"

OptionScene::OptionScene()
{
	_option = SelectOption::ABS;
	_optionSize = { SCREEN_SIZE_X / 5 + SCREEN_SIZE_X / 2 ,100 };

	DIV_IMAGE_ID("image/brake2types.png", 2, 2, 1, 0, 0, 2778, 2505, absImg);
	DIV_IMAGE_ID("image/gear2types.png", 2, 2, 1, 0, 0, 1826, 2753, gearImg);
	DIV_IMAGE_ID("image/mt_m_2types.png", 2, 2, 1, 0, 0, 166, 97, mtImg);
	DIV_IMAGE_ID("image/at_m_2types.png", 2, 2, 1, 0, 0, 156, 97, atImg);
	DIV_IMAGE_ID("image/line_m_all.png", 4, 1, 4, 0, 0, 807, 49, lineImg);
	DIV_IMAGE_ID("image/strong_m2types.png", 2, 2, 1, 0, 0, 484, 97, strongImg);
	DIV_IMAGE_ID("image/weak_m2types.png", 2, 2, 1, 0, 0, 363, 97, weakImg);
	count = 0;

}

OptionScene::~OptionScene()
{
}

void OptionScene::Draw()
{

	if (lpGameTask.GetABSFlag())
	{
		bs = IMAGE_ID("image/on_m.png");
	}
	else
	{
		bs = IMAGE_ID("image/off_m.png");
	}

	if (lpGameTask.GetTransmission())
	{
		tm = mtImg[0];
	}
	else
	{
		tm = atImg[0];
	}
	DrawRotaGraph(SCREEN_SIZE_X / 2, SCREEN_SIZE_Y / 2, 1.0f, 0.0f, IMAGE_ID("image/toka.png"), true);
	DrawRotaGraph(SCREEN_SIZE_X / 2, 80, 0.6f, 0.0f, IMAGE_ID("image/setting.png"), true);
	DrawRotaGraph(SCREEN_SIZE_X / 2 - 200, SCREEN_SIZE_Y / 2, 0.07f + sizeB, 0.0f, absImg[!lpGameTask.GetABSFlag()], true);
	DrawRotaGraph(SCREEN_SIZE_X / 2 + 200, SCREEN_SIZE_Y / 2, 0.07f + sizeG, 0.0f, gearImg[!lpGameTask.GetTransmission()], true);

	DrawRotaGraph(SCREEN_SIZE_X / 2 - 250, SCREEN_SIZE_Y / 2 + 150, 0.3f, 0.0f, IMAGE_ID("image/abs_m.png"), true);
	DrawRotaGraph(SCREEN_SIZE_X / 2 - 200, SCREEN_SIZE_Y / 2 + 150, 0.3f, 0.0f, IMAGE_ID("image/colon.png"), true);
	DrawRotaGraph(SCREEN_SIZE_X / 2 - 150, SCREEN_SIZE_Y / 2 + 150, 0.3f, 0.0f, bs, true);


	DrawRotaGraph(SCREEN_SIZE_X / 2 + 150, SCREEN_SIZE_Y / 2 + 150, 0.3f, 0.0f, IMAGE_ID("image/transmission_m.png"), true);
	DrawRotaGraph(SCREEN_SIZE_X / 2 + 200, SCREEN_SIZE_Y / 2 + 150, 0.3f, 0.0f, IMAGE_ID("image/colon.png"), true);
	DrawRotaGraph(SCREEN_SIZE_X / 2 + 250, SCREEN_SIZE_Y / 2 + 150, 0.3f, 0.0f, tm, true);
	DrawRotaGraph(SCREEN_SIZE_X - 100, SCREEN_SIZE_Y - 70 + (5.0f * sin(count / 10.0f)), 0.2f, 0.0f, IMAGE_ID("image/XboxCon.png"), true);
	DrawRotaGraph(SCREEN_SIZE_X - 275, SCREEN_SIZE_Y - 65 + (5.0f * sin(count / 10.0f)), 0.2f, 0.0f, IMAGE_ID("image/pressStart.png"), true);
	DrawRotaGraph(SCREEN_SIZE_X - 232, SCREEN_SIZE_Y - 40 + (5.0f * sin(count / 10.0f)), 0.19f, 0.0f, IMAGE_ID("image/select.png"), true);

	if (lpGameTask.GetABSFlag())
	{
		if (lpGameTask.GetABSPower() != 3)
		{
			DrawRotaGraph(SCREEN_SIZE_X / 2 - 200, SCREEN_SIZE_Y / 2 + 200, 0.3f, 0.0f, lineImg[lpGameTask.GetABSPower() + 1], true);
		}
		DrawRotaGraph(SCREEN_SIZE_X / 2 - 310, SCREEN_SIZE_Y / 2 + 230, 0.2f, 0.0f, weakImg[0], true);
		DrawRotaGraph(SCREEN_SIZE_X / 2 - 90, SCREEN_SIZE_Y / 2 + 230, 0.2f, 0.0f, strongImg[0], true);

		if (lpGameTask.GetABSPower() == 0)
		{
			pointPos = VECTOR2(SCREEN_SIZE_X / 2 - 315, SCREEN_SIZE_Y / 2 + 200);
		}
		else if (lpGameTask.GetABSPower() == 1)
		{
			pointPos = VECTOR2(SCREEN_SIZE_X / 2 - 200, SCREEN_SIZE_Y / 2 + 200);
		}
		else
		{
			pointPos = VECTOR2(SCREEN_SIZE_X / 2 - 85, SCREEN_SIZE_Y / 2 + 200);
		}
		DrawRotaGraph(pointPos.x, pointPos.y, 0.2f + sizeP, 0.0f, IMAGE_ID("image/redPoint.png"), true);
	}
	else
	{
		DrawRotaGraph(SCREEN_SIZE_X / 2 - 200, SCREEN_SIZE_Y / 2 + 200, 0.3f, 0.0f, lineImg[0], true);
		DrawRotaGraph(SCREEN_SIZE_X / 2 - 310, SCREEN_SIZE_Y / 2 + 230, 0.2f, 0.0f, weakImg[1], true);
		DrawRotaGraph(SCREEN_SIZE_X / 2 - 90, SCREEN_SIZE_Y / 2 + 230, 0.2f, 0.0f, strongImg[1], true);
	}

}

void OptionScene::Update()
{
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
			lpGameTask.AddABSPower();
			if (lpGameTask.GetABSPower() > 2)
			{
				lpGameTask.SetABSPower(2);
			}
		}
		else if (pov == 27000 && pov != oldPov)
		{
			lpGameTask.SubABSPower();
			if (lpGameTask.GetABSPower() < 0)
			{
				lpGameTask.SetABSPower(0);
			}
		}
	}

	if (chooseNumX == 0 && lpGameTask.GetABSFlag())
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
			lpGameTask.SetABSFlag(!lpGameTask.GetABSFlag());
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
			lpGameTask.SetTransmission(!lpGameTask.GetTransmission());
		}
	}
}

const SelectOption& OptionScene::GetOption()
{
	return _option;
}

void OptionScene::SetOption(SelectOption select)
{
	_option = select;
}
