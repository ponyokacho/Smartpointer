#include "DxLib.h"
#include "GameTask.h"
#include "KeyMng.h"
#include "Camera.h"
#include "Field.h"
#include "Player.h"

int GameTask::SCREEN_SIZE_X = 1024;
int GameTask::SCREEN_SIZE_Y = 600;


void GameTask::addObjCnt()
{
	ObjCnt++;
}

void GameTask::removeObjCnt()
{
	ObjCnt--;
}

int GameTask::SystemInit()
{
	SetWindowText("RACE");
	SetGraphMode(SCREEN_SIZE_X, SCREEN_SIZE_Y, 32);	// 1024�~600�ޯ�32bit�ɐݒ�
	ChangeWindowMode(true);				// true:window�@false:�ٽ�ذ�
	if (DxLib_Init() == -1) {
		return false;					// DXײ���؏���������
	}
	SetDrawScreen(DX_SCREEN_BACK);

	SetUseZBuffer3D(true);	// Z�ޯ̧��L���ɂ���
	SetWriteZBuffer3D(true);// Z�ޯ̧�ւ̏������݂�L���ɂ���

	//SetUseLighting(false);	// ײèݸޖ���
	SetLightDirection(VGet(0.5f, -0.5f, 0.5f));		// ���s���̌���
	SetBackgroundColor(137,189,222);

	// �ް�ٰ��(�֐��߲��)��GameInit�ɕύX
	//gLoopPtr = &GameTask::GameInit;

	// ----- �e�ݽ�ݽ
	//camera = new Camera();
	//field = new Field();

	gameMode = GAME_INIT;
	return 0;
}

int GameTask::GameInit()
{
	player = new Player();
 	field = new Field();
	camera = new Camera(player);
	
	return 0;
}

int GameTask::GameTitle(void)
{
	//DrawString(0, 0, "TITLE", 0xffff00);
	//DrawString(SCREEN_SIZE_X / 2 - 90, SCREEN_SIZE_Y / 2 + 100, "PRESS SPACE BUTTON", 0xffffff);
	//DrawFormatString(915, 32, 0xffffff, "ObjCnt = %d", ObjCnt);
	ChangeFont("Bauhaus 93");
	SetFontSize(50);
	// ����������
	//DrawString(SCREEN_SIZE_X / 2 - 310, SCREEN_SIZE_Y / 2 - 50, "RACINGE of MIKAN",0xff0000); 
	ChangeFont("MS�S�V�b�N");
	SetFontSize(10);
	SetFontSize(16);
	return 0;
}

int GameTask::GameMain()
{
	//DrawLine3D(VGet(500.0f, 0.0f, 0.0f), VGet(-500.0f, 0.0f, 0.0f), 0xff0000);// X(��)
	////DrawLine3D(VGet(0.0f, 500.0f, 0.0f), VGet(0.0f, -500.0f, 0.0f), 0x00ff00);// Y(��)
	//DrawLine3D(VGet(0.0f, 0.0f, 500.0f), VGet(0.0f, 0.0f, -500.0f), 0x0000ff);// Z(��)

	camera->Update();
	field->Update();
	field->Render();
	player->Update();
	player->Render();
	

	// �B�`��Ɏg�p���鼬�޳ϯ�߂̐ݒ������
	SetUseShadowMap(0, -1);

	//DrawString(0, 0, "MAIN", 0xffff00);
	////DrawFormatString(900, 0, 0xff0000, "�r�b�n�q�d = %d", score);
	//DrawFormatString(915, 32, 0xffffff, "ObjCnt = %d", ObjCnt);
	//DrawString(0, 30, "SPACE:��ʑJ��", 0xffffff);

	return 0;
}

int GameTask::GameOver()
{
	//DrawString(0, 0, "OVER", 0xffff00);
	return 0;
}

int GameTask::GameClear()
{
	//DrawString(0, 0, "CLEAR", 0xffff00);
	return 0;
}
int GameTask::Update()
{
	trgKey = 0;
	newKey = 0;

	//DrawFormatString(915, 32, 0xffffff, "ObjCnt = %d", ObjCnt);

	KeyMng::GetInstance().Update();
	//int rtnID = (this->*gLoopPtr)();	// �ް�ٰ��(�֐��߲���)
	//return rtnID;

	if (CheckHitKey(KEY_INPUT_SPACE))
	{
		newKey = 1;
	}
	trgKey = ~oldKey & newKey;			// �ضް��

	if ((oldKey == 0) && (newKey == 1))
	{
		trgKey = 1;
	}
	oldKey = newKey;

	switch (gameMode) {
	case GAME_INIT:
		GameInit();
		//gLoopPtr = &GameTask::GameMain;
		gameMode = GAME_TITLE;
		break;
	case GAME_TITLE:
		GameTitle();
		if (trgKey)
		{
			//gLoopPtr = &GameTask::GameMain;
			gameMode = GAME_MAIN;
		}
		break;
	case GAME_MAIN:
		GameMain();
		if (trgKey)
		{
			//gLoopPtr = &GameTask::GameMain;
			gameMode = GAME_CLEAR;
		}
		break;
	case GAME_CLEAR:
		GameClear();
		if (trgKey)
		{
			//gLoopPtr = &GameTask::GameMain;
			gameMode = GAME_OVER;
		}
		break;
	case GAME_OVER:
		GameOver();
		if (trgKey)
		{
			//gLoopPtr = &GameTask::GameMain;
			delete camera;
			delete field;
			delete player;
			gameMode = GAME_INIT;
		}
		break;
	default:
		break;
	};
	return 0;
}