
#include "GameTask.h"
#include "Drawpoint.h"
#include "DxLib.h"

Drawpoint::Drawpoint(VECTOR2 pos, float slipRate, float slipAngle,bool flag)
{
	this->pos = pos;
	this->slipRate = slipRate;
	this->slipAngle = slipAngle;
	this->flag = flag;
}

Drawpoint::~Drawpoint()
{
}

void Drawpoint::Draw()
{
	if (!this->flag)
	{
		DrawCircle(SCREEN_SIZE_X / 2 + (slipRate * SCREEN_SIZE_X / 2), SCREEN_SIZE_Y / 2 - pos.y * 0.09f, 1, GetColor(255 * (1 - slipAngle), 0, 0), true);	// ‹ì“®—Í
		DrawCircle(SCREEN_SIZE_X / 2 + (slipRate * SCREEN_SIZE_X / 2), SCREEN_SIZE_Y / 2 - pos.x * 0.09f, 1, GetColor(0, 0, 255 * (1 - slipAngle)), true); // ‰¡—Í
	}
	else
	{
		DrawCircle(SCREEN_SIZE_X / 2 + (slipRate * SCREEN_SIZE_X / 2), SCREEN_SIZE_Y / 2 - pos.y * 0.09f, 1, GetColor(255, 255, 0), true);	// ‹ì“®—Í
		DrawCircle(SCREEN_SIZE_X / 2 + (slipRate * SCREEN_SIZE_X / 2), SCREEN_SIZE_Y / 2 - pos.x * 0.09f, 1, GetColor(0, 255, 255), true); // ‰¡—Í
	}
}
