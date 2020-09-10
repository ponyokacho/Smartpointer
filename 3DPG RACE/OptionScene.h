#pragma once
#include <DxLib.h>
#include "VECTOR2.h"


enum SelectOption
{
	NON,
	ABS,
	POWER,
	TRANSMISSION,
	MAX,
};

class OptionScene
{
public:
	OptionScene();
	~OptionScene();

	void Draw();
	void Update();

	const VECTOR& GetPos()
	{
		return _pos;
	}

	void SetPos(VECTOR p)
	{
		_pos = p;
	}

private:
	VECTOR _pos;
	VECTOR2 _optionSize;
	unsigned int _intervalSize = 50;

	SelectOption _option;

};

