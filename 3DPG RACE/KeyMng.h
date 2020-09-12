#pragma once
#include"DxLib.h"

enum KEY_CODE {
	P1_UP = 0,
	P1_DOWN,
	P1_RIGHT ,
	P1_LEFT,
	P1_SPACE,
	P1_ENTER,
	P1_LCtrl,
	P1_Y,
	P1_X,
	P1_B,
	P1_Z,
	P1_RB,
	P1_LB,
	P1_PAUSE,
	P1_START,

	KEY_MAX
};

#define lpKeyMng KeyMng::GetInstance()

class KeyMng
{
private:
	KeyMng() { 
		Init();
	}
	KeyMng(const KeyMng&) {}
	KeyMng& operator=(const KeyMng& k) {}
	~KeyMng() {}

public:
	static KeyMng& GetInstance() {
		static KeyMng keyInst;
		return keyInst;
	}
	void Init(void);
	bool Update();	//キー情報更新

	int newKey[KEY_MAX] = { 0 };
	int trgKey[KEY_MAX] = { 0 };
	int oldKey[KEY_MAX] = { 0 };
	int upKey[KEY_MAX] = { 0 };

	XINPUT_STATE input;
};