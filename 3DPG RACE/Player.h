#pragma once
#include "DxLib.h"
#include "VECTOR2.h"

constexpr VECTOR CAMERA_OFFSET = { 0.0f, 300.0f, -1000.0f };
constexpr VECTOR PLAYER_POS_OFFSET = { 0.0f,100.0f,0.0f };

class Player {
private:
	//--- ÓÃÞÙ
	int wheelFRModel;
	int wheelFLModel;
	int wheelRRModel;
	int wheelRLModel;
	int boxModel;// –{‘Ì
	int carModel;
	int camModel;

public:
	Player();
	~Player();

	void Init();
	std::tuple<VECTOR> Update(VECTOR2 tireForce,float speed);
	void Render();

	float Cross(VECTOR va, VECTOR vb);
	float Dot(VECTOR va, VECTOR vb);

	float speed = 0.0f;
	float deg = 0.0f;

	// ¶Ò×—pbox
	MATRIX camMat;
	VECTOR camPos;
	VECTOR camScl;

	// wheelPos
	VECTOR wheelFRPos;
	VECTOR wheelFLPos;
	VECTOR wheelRRPos;
	VECTOR wheelRLPos;

	VECTOR boxPos;

	VECTOR carPos;
	VECTOR carOffsetPos;
	MATRIX carMat;
	MATRIX offsetMat;
	MATRIX moveMat;

	VECTOR vectorSpeed = { 0.0f,0.0f ,0.0f };
	VECTOR beforeCarPos = { 0.0f,0.0f ,0.0f };

	VECTOR tireForce = { 0.0f, 0.0f ,0.0f };
};