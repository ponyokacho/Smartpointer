// äwçZóp 10.0.16299.0
#include "DxLib.h"
#include "GameTask.h"

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
	// ºΩ√—èàóù
	GameTask::GetInstance().SystemInit();

	// ---------- πﬁ∞—Ÿ∞Ãﬂ
	while (ProcessMessage() == 0 && CheckHitKey(KEY_INPUT_ESCAPE) == 0)
	{
		ClsDrawScreen();

		GameTask::GetInstance().Update();

		ScreenFlip();
	}
	DxLib_End();
	return 0;
}