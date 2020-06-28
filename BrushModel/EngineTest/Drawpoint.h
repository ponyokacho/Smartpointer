#pragma once
#include "VECTOR2.h"
class Drawpoint
{
public:
	Drawpoint() {};
	Drawpoint(VECTOR2 pos,float slideRate,float slideAngle);
	~Drawpoint();

	void Draw();
private:
	VECTOR2 pos;
	float slideRate;
	float slideAngle;
};

