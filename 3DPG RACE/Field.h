#pragma once
#include "DxLib.h"

class Player;

struct Point {
	Point() {};
	Point(VECTOR p) : point(p) {};
	virtual ~Point() {};
	VECTOR point = { 0.0f,0.0f,0.0f };
};

class Field {

private:
	int floorTestModel;
	int circuitModel;
	VECTOR floorTestPos;
	MV1_COLL_RESULT_POLY hitPos;
	VECTOR blueTarget;	// 青のｵﾌﾞｼﾞｪｸﾄの座標

	int count = 0;
	std::vector<shared_ptr<Point>>p;

public:
	Field(); 
	~Field();

	void Init();
	void Update();
	void Render(VECTOR);

	Player* player;
};