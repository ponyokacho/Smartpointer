
#include "GameTask.h"
#include "Drawpoint.h"
#include "DxLib.h"

Drawpoint::Drawpoint(VECTOR2 pos, float slideRate, float slideAngle)
{
	this->pos = pos;
	this->slideRate = slideRate;
	this->slideAngle = slideAngle;
}

Drawpoint::~Drawpoint()
{
}

void Drawpoint::Draw()
{
	DrawCircle(SCREEN_SIZE_X / 2 + (slideRate * SCREEN_SIZE_X / 2), SCREEN_SIZE_Y / 2 - pos.y * 0.02f, 1, GetColor(255 * (1 - slideAngle),0,0),true);	// ‹ì“®—Í
	DrawCircle(SCREEN_SIZE_X / 2 + (slideRate * SCREEN_SIZE_X / 2), SCREEN_SIZE_Y / 2 - pos.x * 0.02f, 1, GetColor(0,0,255 * (1 - slideAngle)), true); // ‰¡—Í
}
