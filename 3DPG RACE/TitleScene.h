#pragma once
#include <DxLib.h>
class TitleScene
{

public:
	TitleScene();
	~TitleScene();

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


private:
	VECTOR _pos;
	int _time = 0;
	int _model;
	int _wheelFRModel, _wheelRRModel;
	int _wheelFLModel, _wheelRLModel;

	float cnt = 0.0f;

	VECTOR boxScl = VGet(1.0f, 1.0f, 1.0f);		// Î²°Ù‚Æ“¯‚¶Šg‘å—¦‚É‚·‚é
	MATRIX boxMat = MGetScale(boxScl);

	VECTOR _wheelFOffset = VGet(250.0f, 100.0f, 135.0f);
	VECTOR _wheelROffset = VGet(-250.0f, 100.0f, -135.0f);

	bool replayFlag = false;
};

