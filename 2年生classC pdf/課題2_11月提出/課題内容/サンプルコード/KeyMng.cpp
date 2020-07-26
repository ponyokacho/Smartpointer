#include <DxLib.h>
#include "KeyMng.h"

KeyMng::KeyMng()
{
	Init();
}

void KeyMng::Init()
{
	// ----- ﾌﾗｸﾞ全ｸﾘｱ
	// ※configもｸﾘｱされる
	for(int i = 0; i < KEY_MAX; i++){
		newKey[i] = false;
		trgKey[i] = false;
		upKey[i] = false;
		oldKey[i] = false;
		configKey[i] = i;	
	}
}

bool KeyMng::Update()
{
	for(int i = 0; i < KEY_MAX; i++){
		newKey[i] = false;	// 全部押していない事にする
		trgKey[i] = false;	// 全部押していない事にする
		upKey[i] = false;	// 全部押していない事にする
	}

	// ----- newKey
	if(CheckHitKey(KEY_INPUT_UP)) newKey[P1_UP] = true;
	if(CheckHitKey(KEY_INPUT_RIGHT)) newKey[P1_RIGHT] = true;
	if(CheckHitKey(KEY_INPUT_DOWN)) newKey[P1_DOWN] = true;
	if(CheckHitKey(KEY_INPUT_LEFT)) newKey[P1_LEFT] = true;
	if(CheckHitKey(KEY_INPUT_RSHIFT)) newKey[P1_RA] = true;
	if(CheckHitKey(KEY_INPUT_RCONTROL)) newKey[P1_RB] = true;

	if(CheckHitKey(KEY_INPUT_W)) newKey[P1_W] = true;
	if(CheckHitKey(KEY_INPUT_D)) newKey[P1_D] = true;
	if(CheckHitKey(KEY_INPUT_S)) newKey[P1_S] = true;
	if(CheckHitKey(KEY_INPUT_A)) newKey[P1_A] = true;
	if(CheckHitKey(KEY_INPUT_LSHIFT)) newKey[P1_LA] = true;
	if(CheckHitKey(KEY_INPUT_LCONTROL)) newKey[P1_LB] = true;
	if(CheckHitKey(KEY_INPUT_P)) newKey[P1_PAUSE] = true;

	// ----- trgKey & upKey & oldKey
	for(int i = 0; i < KEY_MAX; i++){
		trgKey[i] = newKey[i] & ~oldKey[i];	// trgKey
		upKey[i] = ~newKey[i] & oldKey[i];	// upKey
		oldKey[i] = newKey[i];	// oldKey
	}
	return true;
}

void KeyMng::SetKeyConfig(int n, int after)
{
	configKey[n] = after;
}