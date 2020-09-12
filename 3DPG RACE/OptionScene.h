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

	const SelectOption& GetOption();
	void SetOption(SelectOption select);

	const VECTOR& GetPos()
	{
		return _pos;
	}

	void SetPos(VECTOR p)
	{
		_pos = p;
	}

private:

	// ÉÅÉjÉÖÅ[óp
	int absImg[2];
	int gearImg[2];
	int mtImg[2];
	int atImg[2];
	int lineImg[4];
	int strongImg[2];
	int weakImg[2];
	int pov = 0;
	int oldPov = 0;
	float count = 0;
	float sizeB = 0.0f;
	float sizeG = 0.0f;
	float sizeP = 0.0f;
	VECTOR2 pointPos = { 0.0f,0.0f };
	int chooseNumX = 0;
	int chooseNumY = 0;
	int bs = 0;
	int tm = 0;

	VECTOR _pos;
	VECTOR2 _optionSize;
	unsigned int _intervalSize = 50;

	SelectOption _option;

};

