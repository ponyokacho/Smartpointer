#pragma once
#include "DxLib.h"

class Player;

struct Point {
	Point() {};
	Point(VECTOR p) : point(p) {};
	virtual ~Point() {};
	VECTOR point = { 0.0f,0.0f,0.0f };
};

enum class FieldStatus
{
	Non,
	Turf,
	Wall,
};

struct Goal
{
	VECTOR _goalStart;
	VECTOR _goalEnd;
};

class Field {

private:
	int floorTestModel;
	int circuitModel;
	VECTOR floorTestPos;
	MV1_COLL_RESULT_POLY hitPos;
	VECTOR blueTarget;	// ê¬ÇÃµÃﬁºﬁ™∏ƒÇÃç¿ïW
	FieldStatus _field;
	Goal _goal;

	int count = 0;
	std::vector<shared_ptr<Point>>p;

public:
	Field(); 
	~Field();

	void Init();
	void Update();
	void Render(VECTOR);

	bool HitCollision(VECTOR a, VECTOR b, VECTOR wallA, VECTOR wallB);

	Player* player;

	const FieldStatus& GetField()
	{
		return _field;
	}

	const Goal& GetGoal()
	{
		return _goal;
	}
};