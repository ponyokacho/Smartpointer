#include "TitleScene.h"
#include "KeyMng.h"
#include "GameTask.h"
#include "ResourceMng.h"

TitleScene::TitleScene()
{
	_model = MV1LoadModel("model/ae86Cp.mv1");
	_wheelFRModel = MV1LoadModel("model/ae86RWheel.mv1");
	_wheelFLModel = MV1LoadModel("model/ae86LWheel.mv1");
	_wheelRRModel = MV1LoadModel("model/ae86RWheel.mv1");
	_wheelRLModel = MV1LoadModel("model/ae86LWheel.mv1");

	MV1SetPosition(_model, VGet(320.0f, 300.0f, -3000.0f));

	MV1SetupCollInfo(_model, 0, 8, 8, 8);

	Init();
}

TitleScene::~TitleScene()
{
}

void TitleScene::Init()
{
	_fadein = 150;
	_fadeinImage = 255;
}

void TitleScene::Draw()
{
}

void TitleScene::Update()
{
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

	SetDrawBlendMode(DX_BLENDMODE_ALPHA, _fadein);
	DrawBox(0, 0, SCREEN_SIZE_X, SCREEN_SIZE_Y, 0x000000, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	if (lpGameTask.GetTitleCnt() < 500)
	{
		replayFlag = false;
		DrawRotaGraph(SCREEN_SIZE_X / 2, SCREEN_SIZE_Y / 2 - 50, 0.25f, 0.0f, IMAGE_ID("image/letsGoKurumaModoki.png"), true);
	}
	else
	{
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, _fadeinImage);
		DrawRotaGraph(SCREEN_SIZE_X / 2, SCREEN_SIZE_Y / 2 - 50, 0.25f, 0.0f, IMAGE_ID("image/letsGoKurumaModoki.png"), true);
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
		replayFlag = true;
		if (_fadein > 0)
		{
			_fadein--;
		}
		if (_fadeinImage > 0)
		{
			_fadeinImage -= 5;
		}
	}

	lpGameTask.SetReplayFlag(replayFlag);

	if (_time % 60 >= 20)
	{
		DrawRotaGraph(SCREEN_SIZE_X / 2, SCREEN_SIZE_Y / 2 + 200, 0.25f, 0.0f, IMAGE_ID("image/pressBButton.png"), true);
	}

	if (lpKeyMng.newKey[P1_X])
	{
		cnt += 0.1f;
	}

}
