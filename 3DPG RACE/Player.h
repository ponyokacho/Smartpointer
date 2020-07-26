#pragma once
#include "DxLib.h"

class Player {
private:
	//--- ����
	int bodyModel;
	int wheelFRModel;
	int wheelFLModel;
	int wheelRRModel;
	int wheelRLModel;
	int boxModel;		// �{��
	int boxRModel;
	int boxCarModel;
	int boxCamModel;
	//--- ��]
	float carRolY;		// �Ԗ{�̉�]
	//--- �O��
	float wheelFRRolX;	// FR���X����]
	float wheelFLRolX;	// FL���X����]
	float wheelFRolY;	// F���Y����]

	//--- ���
	float wheelRRRolX;	// R���X����]
	float wheelRLRolX;	// R���X����]
	//--- �{��
	float boxRolY;

	float boxRRolY;

	//---body
	float bodyRolY;


	//--- �v�Z�p
	MATRIX carMat;					// �S�̂̉�]�ړ����W
	VECTOR carPos;					// �S�̂̍��W(ܰ��ލ��W)
	VECTOR carScl;					// �S�̂̊g�嗦
	VECTOR xAxis, yAxis, zAxis;		// ��]��
	
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
	VECTOR front;					// ���فE�ޯ�

	// ��חpbox
	MATRIX boxCamMat;
	VECTOR boxCamPos;
	VECTOR boxCamScl;

	// wheelPos
	VECTOR wheelFRPos;
	VECTOR wheelFLPos;
	VECTOR wheelRRPos;
	VECTOR wheelRLPos;
};