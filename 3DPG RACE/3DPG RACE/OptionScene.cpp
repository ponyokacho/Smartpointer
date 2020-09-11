#include "OptionScene.h"
#include "GameTask.h"
#include "KeyMng.h"
#include "ResourceMng.h"

OptionScene::OptionScene()
{
	_option = SelectOption::ABS;
	_optionSize = { SCREEN_SIZE_X / 5 + SCREEN_SIZE_X / 2 ,100 };
}

OptionScene::~OptionScene()
{
}

void OptionScene::Draw()
{
	SetFontSize(40);

	DrawString(_optionSize.x, _optionSize.y, "ABSflag", GetColor(100, 100, 100));
	DrawString(_optionSize.x, _optionSize.y * 2, "power", GetColor(100, 100, 100));
	DrawString(_optionSize.x, _optionSize.y * 3, "transMission", GetColor(100, 100, 100));
	DrawString(_optionSize.x + 50, _optionSize.y * 4, "Start", GetColor(100, 100, 100));

	//‘I‘ð
	if (_option != SelectOption::NON)
	{
		if (_option == SelectOption::ABS)
		{
			DrawString(_optionSize.x, _optionSize.y, "ABSflag", GetColor(255, 255, 255));
		}
		if (_option == SelectOption::POWER)
		{
			DrawString(_optionSize.x, _optionSize.y * 2, "power", GetColor(255, 255, 255));
		}
		if (_option == SelectOption::TRANSMISSION)
		{
			DrawString(_optionSize.x, _optionSize.y * 3, "transMission", GetColor(255, 255, 255));
		}
		if (_option == SelectOption::MAX)
		{
			DrawString(_optionSize.x + 50, _optionSize.y * 4, "Start", GetColor(255, 255, 255));
		}
	}

	SetFontSize(30);

	//ABS On or Off
	DrawString(_optionSize.x + _intervalSize, _intervalSize + _optionSize.y, "off", GetColor(100, 100, 100));
	DrawString(_optionSize.x + _intervalSize * 3, _intervalSize + _optionSize.y, "on", GetColor(100, 100, 100));

	//Power 0 or 1 or 2
	DrawString(_optionSize.x + _intervalSize, _intervalSize + _optionSize.y * 2, "0", GetColor(100, 100, 100));
	DrawString(_optionSize.x + _intervalSize * 2, _intervalSize + _optionSize.y * 2, "1", GetColor(100, 100, 100));
	DrawString(_optionSize.x + _intervalSize * 3, _intervalSize + _optionSize.y * 2, "2", GetColor(100, 100, 100));

	//transmission MT or AT
	DrawString(_optionSize.x + _intervalSize, _intervalSize + _optionSize.y * 3, "MT", GetColor(100, 100, 100));
	DrawString(_optionSize.x + _intervalSize * 3, _intervalSize + _optionSize.y * 3, "AT", GetColor(100, 100, 100));

	if (_option != SelectOption::NON)
	{
		if (_option == SelectOption::ABS)
		{
			if (!lpGameTask.GetABSFlag())
			{
				DrawString(_optionSize.x + _intervalSize, _intervalSize + _optionSize.y, "off", GetColor(255, 255, 255));
			}
			else
			{
				DrawString(_optionSize.x + _intervalSize * 3, _intervalSize + _optionSize.y, "on", GetColor(255, 255, 255));
			}
		}
		if (_option == SelectOption::POWER)
		{
			if (lpGameTask.GetABSPower() == 0)
			{
				DrawString(_optionSize.x + _intervalSize, _intervalSize + _optionSize.y * 2, "0", GetColor(255, 255, 255));
			}
			else if (lpGameTask.GetABSPower() == 1)
			{
				DrawString(_optionSize.x + _intervalSize * 2, _intervalSize + _optionSize.y * 2, "1", GetColor(255, 255, 255));
			}
			else if (lpGameTask.GetABSPower() == 2)
			{
				DrawString(_optionSize.x + _intervalSize * 3, _intervalSize + _optionSize.y * 2, "2", GetColor(255, 255, 255));
			}
		}
		if (_option == SelectOption::TRANSMISSION)
		{
			if (!lpGameTask.GetTransmission())
			{
				DrawString(_optionSize.x + _intervalSize, _intervalSize + _optionSize.y * 3, "MT", GetColor(255, 255, 255));
			}
			else
			{
				DrawString(_optionSize.x + _intervalSize * 3, _intervalSize + _optionSize.y * 3, "AT", GetColor(255, 255, 255));
			}

		}
	}

	SetFontSize(16);
}

void OptionScene::Update()
{

	if (lpKeyMng.trgKey[P1_DOWN])
	{
		PlaySoundMem(SOUND_ID("sounds/cursor3.mp3"), DX_PLAYTYPE_BACK);

		if (_option == SelectOption::ABS)
		{
			if (lpGameTask.GetABSFlag())
			{
				_option = SelectOption::POWER;
			}
			else
			{
				_option = SelectOption::TRANSMISSION;
			}
		}
		else if (_option == SelectOption::POWER)
		{
			_option = SelectOption::TRANSMISSION;
		}
		else if (_option == SelectOption::TRANSMISSION)
		{
			_option = SelectOption::MAX;
		}
	}
	if (lpKeyMng.trgKey[P1_UP])
	{
		PlaySoundMem(SOUND_ID("sounds/cursor3.mp3"), DX_PLAYTYPE_BACK);

		if (_option == SelectOption::POWER)
		{
			_option = SelectOption::ABS;
		}
		else if (_option == SelectOption::TRANSMISSION)
		{
			if (lpGameTask.GetABSFlag())
			{
				_option = SelectOption::POWER;
			}
			else
			{
				_option = SelectOption::ABS;
			}
		}
		else if (_option == SelectOption::MAX)
		{
			_option = SelectOption::TRANSMISSION;
		}
	}

	if (lpKeyMng.trgKey[P1_RIGHT])
	{
		PlaySoundMem(SOUND_ID("sounds/cursor7.mp3"), DX_PLAYTYPE_BACK);

		if (_option == SelectOption::ABS)
		{
			if (!lpGameTask.GetABSFlag())
			{
				lpGameTask.SetABSFlag(true);
			}
		}
		else if (_option == SelectOption::POWER)
		{
			unsigned int powerMax = 2;
			if (lpGameTask.GetABSPower() < 2)
			{
				lpGameTask.AddABSPower();
			}
		}
		else if (_option == SelectOption::TRANSMISSION)
		{
			if (!lpGameTask.GetTransmission())
			{
				lpGameTask.SetTransmission(true);
			}
		}
	}
	if (lpKeyMng.trgKey[P1_LEFT])
	{
		PlaySoundMem(SOUND_ID("sounds/cursor7.mp3"), DX_PLAYTYPE_BACK);

		if (_option == SelectOption::ABS)
		{
			if (lpGameTask.GetABSFlag())
			{
				lpGameTask.SetABSFlag(false);
			}
		}
		else if (_option == SelectOption::POWER)
		{
			if (lpGameTask.GetABSPower() > 0)
			{
				lpGameTask.SubABSPower();
			}
		}
		else if (_option == SelectOption::TRANSMISSION)
		{
			if (lpGameTask.GetTransmission())
			{
				lpGameTask.SetTransmission(false);
			}
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
