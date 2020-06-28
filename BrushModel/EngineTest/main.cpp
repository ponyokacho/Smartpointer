#include "DxLib.h"
#include "GameTask.h"

int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_  HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int)
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