#pragma once
#include "DxLib.h"

constexpr VECTOR CAMERA_OFFSET = { 0.0f, 300.0f, -1000.0f };
constexpr VECTOR PLAYER_POS_OFFSET = { 0.0f,100.0f,0.0f };

class Player {
private:
	//--- ����
	int wheelFRModel;
	int wheelFLModel;
	int wheelRRModel;
	int wheelRLModel;
	int boxModel;// �{��
	int carModel;
	int camModel;

public:
	Player();
	~Player();

	void Init();
	void Update(VECTOR2 pos,VECTOR2 dirVec, VECTOR2 yawVec, float wheelAngle,float deg);
	void Render();

	float Cross(VECTOR va, VECTOR vb);
	float Dot(VECTOR va, VECTOR vb);

	// ��חpbox
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
	MATRIX carMat;
	MATRIX moveMat;
};