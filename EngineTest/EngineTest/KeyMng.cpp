#include "KeyMng.h"

void KeyMng::Init()
{
	//�t���O�S�N���A
	for(int i=0;i<KEY_MAX;i++){
		newKey[i] = 0;
		trgKey[i] = 0;
		oldKey[i] = 0;
		upKey[i] = 0;
	}
}

bool KeyMng::Update()
{
	//�L�[���X�V
	for (int i = 0; i < KEY_MAX; i++) {
		newKey[i] = false;	// �S�������Ă��Ȃ����ɂ���
		trgKey[i] = false;	// �S�������Ă��Ȃ����ɂ���
		upKey[i] = false;	// �S�������Ă��Ȃ����ɂ���
	}

	if (CheckHitKey(KEY_INPUT_UP)) newKey[P1_UP] = true;
	if (CheckHitKey(KEY_INPUT_DOWN)) newKey[P1_DOWN] = true;
	if (CheckHitKey(KEY_INPUT_RIGHT)) newKey[P1_RIGHT] = true;
	if (CheckHitKey(KEY_INPUT_LEFT)) newKey[P1_LEFT] = true;
	if (CheckHitKey(KEY_INPUT_SPACE)) newKey[P1_SPACE] = true;
	if (CheckHitKey(KEY_INPUT_RETURN)) newKey[P1_ENTER] = true;
	if (CheckHitKey(KEY_INPUT_LCONTROL))newKey[P1_LCtrl] = true;
	if (CheckHitKey(KEY_INPUT_P))newKey[P1_PAUSE] = true;
	if (CheckHitKey(KEY_INPUT_X))newKey[P1_X] = true;
	if (CheckHitKey(KEY_INPUT_Z))newKey[P1_Z] = true;

	if (CheckHitKey(PAD_INPUT_UP) || CheckHitKey(PAD_INPUT_B)) newKey[P1_UP] = true;
	if (CheckHitKey(PAD_INPUT_DOWN)) newKey[P1_DOWN] = true;
	if (CheckHitKey(PAD_INPUT_RIGHT)) newKey[P1_RIGHT] = true;
	if (CheckHitKey(PAD_INPUT_LEFT)) newKey[P1_LEFT] = true;
	if (CheckHitKey(PAD_INPUT_X)) newKey[P1_SPACE] = true;
	if (CheckHitKey(PAD_INPUT_A)) newKey[P1_ENTER] = true;
	if (CheckHitKey(PAD_INPUT_START))newKey[P1_PAUSE] = true;

	// Xbox�R��
	GetJoypadXInputState(DX_INPUT_PAD1, &input);
	if (input.RightTrigger) newKey[P1_UP] = true;
	if (input.LeftTrigger) newKey[P1_DOWN] = true;
	if (input.Buttons[XINPUT_BUTTON_START]) newKey[P1_ENTER] = true;
	if (input.Buttons[XINPUT_BUTTON_DPAD_RIGHT]) newKey[P1_RIGHT] = true;
	if (input.Buttons[XINPUT_BUTTON_DPAD_LEFT]) newKey[P1_LEFT] = true;
	if ((GetJoypadInputState(DX_INPUT_PAD1) & PAD_INPUT_UP) != 0) newKey[P1_UP] = true;
	if ((GetJoypadInputState(DX_INPUT_PAD1) & PAD_INPUT_DOWN) != 0) newKey[P1_DOWN] = true;
	if ((GetJoypadInputState(DX_INPUT_PAD1) & PAD_INPUT_RIGHT) != 0) newKey[P1_RIGHT] = true;
	if ((GetJoypadInputState(DX_INPUT_PAD1) & PAD_INPUT_LEFT) != 0) newKey[P1_LEFT] = true;
	if ((GetJoypadInputState(DX_INPUT_PAD1) & PAD_INPUT_4) != 0) newKey[P1_Y] = true;
	if ((GetJoypadInputState(DX_INPUT_PAD1) & PAD_INPUT_3) != 0) newKey[P1_X] = true;
	if ((GetJoypadInputState(DX_INPUT_PAD1) & PAD_INPUT_2) != 0) newKey[P1_B] = true;
	if ((GetJoypadInputState(DX_INPUT_PAD1) & PAD_INPUT_6) != 0) newKey[P1_RB] = true;
	if ((GetJoypadInputState(DX_INPUT_PAD1) & PAD_INPUT_5) != 0) newKey[P1_LB] = true;

	if ((GetJoypadInputState(DX_INPUT_PAD1) & PAD_INPUT_1) != 0 || (GetJoypadInputState(DX_INPUT_PAD1) & PAD_INPUT_2) != 0) newKey[P1_SPACE] = true;
	if ((GetJoypadInputState(DX_INPUT_PAD1) & PAD_INPUT_8) != 0) newKey[P1_PAUSE] = true;

	//�g���Kand �A�b�v
	for (int i = 0; i < KEY_MAX; i++) {
		trgKey[i] = newKey[i] & ~oldKey[i];//�g���K
		upKey[i] = ~newKey[i] & oldKey[i];//�A�b�v

										  //�����[�v�̏���
		oldKey[i] = newKey[i];
	}
	return true;
}
