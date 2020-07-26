#pragma once
#include "DxLib.h"

class Player;

class Field {

private:
	int floorTestModel;
	int circuitModel;
	VECTOR floorTestPos;
	MV1_COLL_RESULT_POLY hitPos;
	VECTOR blueTarget;	// 青のｵﾌﾞｼﾞｪｸﾄの座標

public:
	Field(); 
	~Field();

	void Init();
	void Update();
	void Render();

	Player* player;
};