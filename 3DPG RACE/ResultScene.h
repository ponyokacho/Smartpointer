#pragma once
#include <DxLib.h>

class ResultScene
{

public:
	ResultScene();
	~ResultScene();

	void Draw();
	void Update();

	const VECTOR& GetPos()
	{
		return _pos;
	}

	void SetPos(VECTOR p)
	{
		_pos = p;
	}

	const bool& GetFlag();
	void SetFlag(bool flag);

private:
	VECTOR _pos;
	int _time = 0;

	int _model;
	int _wheelFRModel, _wheelRRModel;
	int _wheelFLModel, _wheelRLModel;

	VECTOR boxScl = VGet(1.0f, 1.0f, 1.0f);		// Î²°Ù‚Æ“¯‚¶Šg‘å—¦‚É‚·‚é
	MATRIX boxMat = MGetScale(boxScl);

	VECTOR _wheelFOffset = VGet(250.0f, 65.0f, 135.0f);
	VECTOR _wheelROffset = VGet(-250.0f, 65.0f, 135.0f);

	bool _resultFlag = false;


};

