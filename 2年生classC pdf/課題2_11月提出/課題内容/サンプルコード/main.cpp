#include "DxLib.h"
#include "GameTask.h"

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
	// ���я���
	GameTask::GetInstance().SystemInit();

	// ---------- �ް�ٰ��
	while (ProcessMessage() == 0 && CheckHitKey(KEY_INPUT_ESCAPE) == 0)
	{
		ClsDrawScreen();

		GameTask::GetInstance().Update();

		ScreenFlip();
	}
	DxLib_End();
	return 0;
}