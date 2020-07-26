#pragma once

enum KEY_MODE {
	// ----- P1
	P1_UP,
	P1_RIGHT,
	P1_DOWN,
	P1_LEFT,
	P1_RA,
	P1_RB,

	P1_W,
	P1_D,
	P1_S,
	P1_A,

	P1_LA,
	P1_LB,
	P1_PAUSE,
	KEY_MAX
};

// ----- �ݸ���݂�KeyCheck�׽���`����
class KeyMng
{
private:
	// ----- �ݽ�׸��Q
	KeyMng();	// ��̫�ĺݽ�׸���private�ɂ��ĊO�����琶���ł��Ȃ��l�ɂ���
	KeyMng(const KeyMng&){}						// ----- ��߰�ݽ�׸���private��
	KeyMng& operator=(const KeyMng& g){}		// ----- ������Z�q�̵��ްײ�ނ�private��
	~KeyMng(){}	// �޽�׸�
	
public:
	// ----- KeyMng��޼ު�Ă̎��̂�Ԃ�(�ݸ����)
	static KeyMng& GetInstance() {
		static KeyMng keyInstance;	// KeyMng�̎��̂𐶐��BkeyInstance�ɕێ�
		return keyInstance;
	}

	void Init(void);
	bool Update();	// ����ԍX�V(��ٰ�ߍX�V)

	bool newKey[KEY_MAX];
	bool trgKey[KEY_MAX];
	bool upKey[KEY_MAX];
	bool oldKey[KEY_MAX];

	// ----- ����̨��
	// configKey[]�̂��ꂼ��̔ԍ������ꂼ��KEY_MODE�̔ԍ��ɑΉ����Ă���
	// ��)configKey[0] �� P1_UP

	// ���������s���ꍇ�́A����̷����͂��������ꍇnewKey[configkey[�`]]���������ꂽ���ɂ���
	// ��)if(CheckHitKey(KEY_INPUT_RIGHT)) newKey[configKey[2]] = 1;
	// ���E���������ꂽ��2:P1_DOWN�������ꂽ���ɂ���

	int configKey[KEY_MAX];	// ����̨�ޗp�̷��z��
	void SetKeyConfig(int, int);	// ����̨�޾��
};
