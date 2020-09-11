#pragma once
#include "DxLib.h"
#include "VECTOR2.h"
#include <vector>
#include <memory>
;
constexpr VECTOR PLAYER_POS_OFFSET = { 0.0f,70.0f,0.0f };

constexpr VECTOR WHEEL_OFFSET = { 135.0f, 80.0f, 250.0f };

class Player {
private:
	//--- ﾓﾃﾞﾙ
	int boxModel;// 本体
	int carModel;
	int camModel;

public:
	Player();
	~Player();

	void Init();
	void Update();
	void Render();

	float Cross(VECTOR va, VECTOR vb);
	float Dot(VECTOR va, VECTOR vb);

	float speed = 0.0f;
	float oldSpeed = 0.0f;

	// ｶﾒﾗ用box
	struct Camera {
		int view = 0;	// 0:三人称,1:一人称 2:タイヤ,3:リプレイ
		VECTOR pos = VGet(0.0f, 0.0f, 1.0f);
		MATRIX mat;
		VECTOR scl;
		VECTOR vec;
		VECTOR offset = { 0.0f, 0.0f, 1.0f };
	};
	Camera cam;

	VECTOR boxPos;

	VECTOR carPos;
	VECTOR carVec = { 0.0f,0.0f,1.0f };
	VECTOR carFrontPos = { 0.0f,0.0f,1.0f };
	VECTOR tireViewFrontPos = { 0.0f,0.0f,0.0f };
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

	VECTOR camPosOffset = { 0.0f,0.0f,0.0f };

	int count = 0;

	float steering = 0.0f;

	int lr = 0;

	struct Deg{
		float yaw = 0.0f;
		float pitch = 0.0f;
		float oldPitch = 0.0f;
		float roll = 0.0f;
	};
	Deg deg;

	struct Wheel
	{
		struct Front
		{
			struct Left
			{
				VECTOR pos = { 0.0f,0.0f,0.0f };
				float deg = 0.0f;
				int model = 0;
				VECTOR moveOffset = { 0.0f,0.0f,0.0f };
				
			};
			struct Right
			{
				VECTOR pos = { 0.0f,0.0f,0.0f };
				float deg = 0.0f;
				int model = 0;
				VECTOR moveOffset = { 0.0f,0.0f,0.0f };
			};
			
			float tireRotX = 0.0f;
			MATRIX rotMatY;

			Left left;
			Right right;
		};
		struct Rear
		{
			struct Left
			{
				VECTOR pos = { 0.0f,0.0f,0.0f };
				float deg = 0.0f;
				int model = 0;
				VECTOR moveOffset = { 0.0f,0.0f,0.0f };
			};
			struct Right
			{
				VECTOR pos = { 0.0f,0.0f,0.0f };
				float deg = 0.0f;
				int model = 0;
				VECTOR moveOffset = { 0.0f,0.0f,0.0f };
			};

			float tireRotX = 0.0f;
			MATRIX rotMatY;

			Left left;
			Right right;
		};
		Front front;
		Rear rear;
	};
	Wheel wheel;

	array<VECTOR, 11>viewPoint = {VGet(242,600,37714),VGet(35207,500,20096),VGet(8420,500,23231),VGet(-9118,500,27067),VGet(-30049,100,28264),
		VGet(-14561,500,16833),VGet(1408,1000,-4890),VGet(40322,100,-42065),VGet(19664,140,-35776)/*<-ストレート->*/,VGet(-31218,140,7036),VGet(-41584,1000,42912)
	};
	int viewNum = 0;
	int changeViewNumCounter = 0;
	int minTime = 0;
	bool changeFlag = false;
};