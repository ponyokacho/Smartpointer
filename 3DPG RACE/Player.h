#pragma once
#include "DxLib.h"
#include "VECTOR2.h"
#include <vector>
#include <memory>
;
constexpr VECTOR PLAYER_POS_OFFSET = { 0.0f,100.0f,0.0f };

class Player {
private:
	//--- ﾓﾃﾞﾙ
	int wheelFRModel;
	int wheelFLModel;
	int wheelRRModel;
	int wheelRLModel;
	int boxModel;// 本体
	int carModel;
	int camModel;

public:
	Player();
	~Player();

	void Init();
	std::tuple<VECTOR,VECTOR,VECTOR,VECTOR,VECTOR,float> Update(VECTOR2 tireForce, VECTOR2 dirVec, const VECTOR2 fWheelVec,const int lr,const float steering);
	void Render();

	float Cross(VECTOR va, VECTOR vb);
	float Dot(VECTOR va, VECTOR vb);

	float speed = 0.0f;
	float oldSpeed = 0.0f;

	// ｶﾒﾗ用box
	struct Camera {
		bool view = false;	// false:三人称,ture:一人称
		VECTOR pos = VGet(0.0f, 0.0f, 0.0f);
		MATRIX mat;
		VECTOR scl;
		VECTOR vec;
		VECTOR offset = { 0.0f, 0.0f, 0.0f };
	};
	Camera cam;

	// wheelPos
	VECTOR wheelFRPos;
	VECTOR wheelFLPos;
	VECTOR wheelRRPos;
	VECTOR wheelRLPos;

	VECTOR boxPos;

	VECTOR carPos;
	VECTOR carVec = { 0.0f,0.0f,1.0f };
	VECTOR carFrontPos = { 0.0f,0.0f,1.0f };
	VECTOR carOffsetPos;
	MATRIX carMat;
	VECTOR carScl;
	MATRIX offsetMat;
	MATRIX moveMat;
	MATRIX carPosMat;

	VECTOR vectorSpeed = { 0.0f,0.0f ,1.0f };
	VECTOR vectorSpeedRot = { 0.0f,0.0f,1.0f };
	VECTOR oldVectorSpeedRot = { 0.0f,0.0f,1.0f };
	VECTOR beforeCarPos = { 0.0f,0.0f ,1.0f };

	VECTOR tireForce = { 0.0f, 0.0f ,1.0f };
	VECTOR tireForceRot = { 0.0f,0.0f,1.0f };
	VECTOR addMoveVec = { 0.0f,0.0f,1.0f };
	VECTOR dirVec = { 0.0f,0.0f,1.0f };
	VECTOR dirVecRot = { 0.0f,0.0f,1.0f };
	float acceleration = 0.0f;

	VECTOR fWheelVec = { 0.0f,0.0f,1.0f };
	VECTOR fWheelVecRot = { 0.0f,0.0f,1.0f };

	int count = 0;

	float steering = 0.0f;

	struct Deg{
		float yaw = 0.0f;
		float pitch = 0.0f;
		float oldPitch = 0.0f;
		float roll = 0.0f;
	};
	Deg deg;
};