#pragma once
#include "DxLib.h"
#include "VECTOR2.h"
#include <vector>
#include <memory>

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
	std::tuple<VECTOR,VECTOR,VECTOR,VECTOR,float> Update(VECTOR2 tireForce, VECTOR2 dirVec, const VECTOR2 fWheelVec, float speed, const int lr);
	void Render();

	float Cross(VECTOR va, VECTOR vb);
	float Dot(VECTOR va, VECTOR vb);

	float speed = 0.0f;
	float oldSpeed = 0.0f;
	float deg = 0.0f;

	// ¶Ò×—pbox
	MATRIX camMat;
	VECTOR camPos;
	VECTOR camScl;
	VECTOR camVec;

	// wheelPos
	VECTOR wheelFRPos;
	VECTOR wheelFLPos;
	VECTOR wheelRRPos;
	VECTOR wheelRLPos;

	VECTOR boxPos;

	VECTOR carPos;
	VECTOR carVec = { 0.0f,0.0f,0.0f };
	VECTOR carFrontPos = { 0.0f,0.0f,0.0f };
	VECTOR carOffsetPos;
	MATRIX carMat;
	VECTOR carScl;
	MATRIX offsetMat;
	MATRIX moveMat;

	VECTOR vectorSpeed = { 0.0f,0.0f ,0.0f };
	VECTOR beforeCarPos = { 0.0f,0.0f ,0.0f };

	VECTOR tireForce = { 0.0f, 0.0f ,0.0f };
	VECTOR tireForceRot = { 0.0f,0.0f,0.0f };
	VECTOR addMoveVec = { 0.0f,0.0f,0.0f };
	VECTOR dirVec = { 0.0f,0.0f,0.0f };
	VECTOR dirVecRot = { 0.0f,0.0f,0.0f };
	float acceleration = 0.0f;

	VECTOR fWheelVec = { 0.0f,0.0f,0.0f };
	VECTOR fWheelVecRot = { 0.0f,0.0f,0.0f };

	int count = 0;
};