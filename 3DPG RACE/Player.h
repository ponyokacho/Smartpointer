#pragma once
#include "DxLib.h"

class Player {
private:
	//--- ﾓﾃﾞﾙ
	int bodyModel;
	int wheelFRModel;
	int wheelFLModel;
	int wheelRRModel;
	int wheelRLModel;
	int boxModel;		// 本体
	int boxRModel;
	int boxCarModel;
	int boxCamModel;
	//--- 回転
	float carRolY;		// 車本体回転
	//--- 前輪
	float wheelFRRolX;	// FRﾀｲﾔX軸回転
	float wheelFLRolX;	// FLﾀｲﾔX軸回転
	float wheelFRolY;	// FﾀｲﾔY軸回転

	//--- 後輪
	float wheelRRRolX;	// RﾀｲﾔX軸回転
	float wheelRLRolX;	// RﾀｲﾔX軸回転
	//--- 本体
	float boxRolY;

	float boxRRolY;

	//---body
	float bodyRolY;


	//--- 計算用
	MATRIX carMat;					// 全体の回転移動座標
	VECTOR carPos;					// 全体の座標(ﾜｰﾙﾄﾞ座標)
	VECTOR carScl;					// 全体の拡大率
	VECTOR xAxis, yAxis, zAxis;		// 回転軸
	
	VECTOR frontWheel;

	MATRIX wheelFRMat;
	MATRIX wheelFLMat;
	MATRIX wheelRRMat;
	MATRIX wheelRLMat;
	//VECTOR wheelFRPos;
	//VECTOR wheelFLPos;
	//VECTOR wheelRRPos;
	//VECTOR wheelRLPos;
	VECTOR wheelScl;

	// MATRIX boxMat;
	// VECTOR boxPos;
	VECTOR boxScl;

	MATRIX boxFRMat;
	VECTOR boxFRPos;
	VECTOR boxFRScl;

	MATRIX boxFLMat;
	VECTOR boxFLPos;
	VECTOR boxFLScl;

	MATRIX boxRMat;
	VECTOR boxRPos;
	VECTOR boxRScl;

	MATRIX boxCarMat;
	VECTOR boxCarPos;
	VECTOR boxCarScl;


	VECTOR offsetFR;
	VECTOR offsetFL;
	VECTOR offsetRR;
	VECTOR offsetRL;
	VECTOR wheelFOffset;
	VECTOR wheelROffset;

	VECTOR bodyOffset;

	VECTOR boxCamOffset;
	int LRFlag;
	float sum;

	float speed;
	int udFlag;
	int count;
	int rolFlag;
	float dirCross;
	int dirCrossFlag;
	float dirLim;
	float rolAngle;

	VECTOR dirVec;
	VECTOR wheelPos;
	VECTOR fVec;
	VECTOR wheelAngle;
	VECTOR dirLimVec;
	VECTOR frontDeg;
	VECTOR frontWheelDeg;

public:
	Player();
	~Player();

	void Init();
	void Update();
	void Render();

	float Cross(VECTOR va, VECTOR vb);
	float Dot(VECTOR va, VECTOR vb);

	MATRIX boxMat;
	VECTOR boxPos;
	VECTOR front;					// ｱｸｾﾙ・ﾊﾞｯｸ

	// ｶﾒﾗ用box
	MATRIX boxCamMat;
	VECTOR boxCamPos;
	VECTOR boxCamScl;

	// wheelPos
	VECTOR wheelFRPos;
	VECTOR wheelFLPos;
	VECTOR wheelRRPos;
	VECTOR wheelRLPos;
};