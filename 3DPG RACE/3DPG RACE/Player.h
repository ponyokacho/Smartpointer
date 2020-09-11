#pragma once
#include "DxLib.h"
#include "VECTOR2.h"
#include <vector>
#include <memory>
;
constexpr VECTOR PLAYER_POS_OFFSET = { 0.0f,100.0f,0.0f };

class Player {
private:
	//当たり判定
	struct Capsule
	{
		VECTOR _startPos;
		VECTOR _endPos;
		VECTOR _wallStart;
		VECTOR _wallEnd;
	};
	Capsule _hitBox;

	//--- ﾓﾃﾞﾙ
	int wheelFRModel;
	int wheelFLModel;
	int wheelRRModel;
	int wheelRLModel;
	int boxModel;// 本体
	int carModel;
	int camModel;

	int _playerNum = 0;

public:
	Player();
	Player(int num);
	~Player();

	void ModelInit();

	void Init();
	void Update();
	void Render();

	bool HitGoalCollision(VECTOR a, VECTOR b);

	float Cross(VECTOR va, VECTOR vb);
	float Dot(VECTOR va, VECTOR vb);

	float speed = 0.0f;
	float oldSpeed = 0.0f;

	// ｶﾒﾗ用box
	struct Camera {
		bool view = false;	// false:三人称,ture:一人称
		VECTOR pos = VGet(0.0f, 0.0f, 1.0f);
		MATRIX mat;
		VECTOR scl;
		VECTOR vec;
		VECTOR offset = { 0.0f, 0.0f, 1.0f };
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

	int lr = 0;

	struct Deg{
		float yaw = 0.0f;
		float pitch = 0.0f;
		float oldPitch = 0.0f;
		float roll = 0.0f;
	};
	Deg deg;

	const Capsule& GetHitBox()
	{
		return _hitBox;
	}

	const VECTOR& GetCarPos()
	{
		return carPos;
	}

	void SetCarPos(VECTOR p)
	{
		carPos = p;
	}

	const Deg& GetDeg()
	{
		return deg;
	}

	void SetDeg(float y, float p, float o, float r)
	{
		deg.yaw = y;
		deg.pitch = p;
		deg.oldPitch = o;
		deg.roll = r;
	}

	const VECTOR& GetVec()
	{
		return vectorSpeed;
	}

	void SetVec(VECTOR v)
	{
		vectorSpeed = v;
	}

	const float& GetSpeed()
	{
		return speed;
	}

	void SetSpeed(float s)
	{
		speed = s;
	}

};