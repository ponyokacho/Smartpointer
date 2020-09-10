#include "TitleScene.h"
#include "KeyMng.h"

TitleScene::TitleScene()
{
	_model = MV1LoadModel("model/ae86Cp.mv1");
	_wheelFRModel = MV1LoadModel("model/ae86RWheel.mv1");
	_wheelFLModel = MV1LoadModel("model/ae86LWheel.mv1");
	_wheelRRModel = MV1LoadModel("model/ae86RWheel.mv1");
	_wheelRLModel = MV1LoadModel("model/ae86LWheel.mv1");

	MV1SetPosition(_model, VGet(320.0f, 300.0f, -3000.0f));

	MV1SetupCollInfo(_model, 0, 8, 8, 8);

}

TitleScene::~TitleScene()
{
}

void TitleScene::Draw()
{
	MV1DrawModel(_model);
	MV1DrawModel(_wheelFRModel);
	MV1DrawModel(_wheelFLModel);
	MV1DrawModel(_wheelRRModel);
	MV1DrawModel(_wheelRLModel);

	float random = -0.01f + (0.01f * GetRand(3));

	MV1SetRotationXYZ(_model, VGet(random, 8, 0));

	MV1SetRotationXYZ(_wheelFRModel, VGet(0, 8, 0));
	MV1SetRotationXYZ(_wheelFLModel, VGet(0, 8, 0));
	MV1SetRotationXYZ(_wheelRRModel, VGet(0, 8, 0));
	MV1SetRotationXYZ(_wheelRLModel, VGet(0, 8, 0));

}

void TitleScene::Update()
{
	KeyMng::GetInstance().Update();

	++_time;
	auto tmp = MMult(boxMat, MGetTranslate(_pos));

	MV1SetPosition(_model, _pos);

	auto wheelFRtmp = VTransform(VGet(-_wheelFOffset.x, -_wheelFOffset.y, _wheelFOffset.z), tmp);
	auto wheelFLtmp = VTransform(VGet(_wheelFOffset.x, -_wheelFOffset.y, _wheelFOffset.z), tmp);

	MV1SetPosition(_wheelFRModel, wheelFRtmp);
	MV1SetPosition(_wheelFLModel, wheelFLtmp);

	auto wheelRRtmp = VTransform(VGet(-_wheelROffset.x, -_wheelROffset.y, _wheelROffset.z), tmp);
	auto wheelRLtmp = VTransform(VGet(_wheelROffset.x, -_wheelROffset.y, _wheelROffset.z), tmp);

	MV1SetPosition(_wheelRRModel, wheelRRtmp);
	MV1SetPosition(_wheelRLModel, wheelRLtmp);


	if (_pos.x < -6000.0f)
	{
		_pos.x += 128.0f;
		_pos.z -= 32.0f;
	}
	else
	{
		SetFontSize(100);

		DrawString(301, 101, "RACE LOAD", 0x000000);
		DrawString(300, 100, "RACE LOAD", 0xffffff);
		SetFontSize(16);
		if (_time % 60 >= 20)
		{
			DrawString(431, 401, "Press Button", 0x000000);
			DrawString(430, 400, "Press Button", 0xffffff);
		}
	}

	if (lpKeyMng.newKey[P1_X])
	{
		cnt += 0.1f;
	}

}
