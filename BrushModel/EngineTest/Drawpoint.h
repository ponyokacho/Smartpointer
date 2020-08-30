#pragma once
#include "VECTOR2.h"
class Drawpoint
{
public:
	Drawpoint() {};
	Drawpoint(VECTOR2 pos,float slipRate,float slipAngle,bool flag);
	~Drawpoint();

	void Draw();
private:
	VECTOR2 pos;
	float slipRate;
	float slipAngle;
	bool flag = false;
};

