#include "GameTask.h"
#include "UI.h"
#include "ResourceMng.h"

UI::UI()
{
	s.maxSpeed = lpGameTask.GetMaxSpeed();
	DIV_IMAGE_ID("image/gear_num_red.png", 11, 11, 1, 0, 0, 60, 60,g.gear);
	DIV_IMAGE_ID("image/gear_num.png", 10, 10, 1, 0, 0, 60, 60, s.speedNum);
	DIV_IMAGE_ID("image/mt.png", 2, 2, 1, 0, 0, 60, 60, g.mt);
	DIV_IMAGE_ID("image/at.png", 2, 2, 1, 0, 0, 60, 60, g.at);
	DIV_IMAGE_ID("image/abs.png", 2, 2, 1, 0, 0, 60, 60, s.bs);
}

UI::~UI()
{
}

void UI::Update()
{
	r.rpm = lpGameTask.GetActualRpm();
	s.speed = lpGameTask.GetSpeed();
	view = lpGameTask.GetView();
	g.gearNum = lpGameTask.GetGearNum();
	s.accelBox = lpGameTask.GetRT();
	s.brakeBox = lpGameTask.GetLT();
	g.transmission = lpGameTask.GetTransmission();
	s.absFlag = lpGameTask.GetABSFlag();

	r.Normalize = r.rpm / ACTUAL_MAX_RPM;
	s.Normalize = s.speed / s.maxSpeed;

	s.speed100 = (int)s.speed / 100;
	s.speed10 = ((int)s.speed / 10) % 10;
	s.speed1 = (int)s.speed % 10;

	if (view)
	{
		r.centerPos = VECTOR2(SCREEN_SIZE_X / 2 - 125, SCREEN_SIZE_Y / 2 + 210);
		s.centerPos = VECTOR2(SCREEN_SIZE_X / 2 + 125, SCREEN_SIZE_Y / 2 + 210);

		r.tachoSize = 0.25f;
		s.tachoSize = 0.25f;
		s.size = 0.4f;

		s.pos = VECTOR2(SCREEN_SIZE_X / 2.0f, SCREEN_SIZE_Y - 95.0f);
		s.numOffset = 15.0f;
		s.kmhOffset = 22.0f;

		s.absPos = VECTOR2(SCREEN_SIZE_X / 2, SCREEN_SIZE_Y - 57.0f);

		s.accelBarPos = VECTOR2(SCREEN_SIZE_X / 2 + 25.0f, SCREEN_SIZE_Y - 40.0f);
		s.brakeBarPos = VECTOR2(SCREEN_SIZE_X / 2 - 25.0f, SCREEN_SIZE_Y - 40.0f);

		g.pos = VECTOR2(SCREEN_SIZE_X / 2, SCREEN_SIZE_Y - 125.0f);
		g.size = 0.4f;
		g.shiftIndiPos = VECTOR2(SCREEN_SIZE_X / 2.0f, SCREEN_SIZE_Y - 125.0f);
		g.shiftIndiSize = 0.25f;

		g.tmPos = VECTOR2(SCREEN_SIZE_X / 2, SCREEN_SIZE_Y - 22.0f);
		g.tmSize = 0.4f;
		g.tmOffset = 7.0f;

		ib.pos = VECTOR2(SCREEN_SIZE_X / 2, SCREEN_SIZE_Y - 40.0f);
		ib.rot = 0.0f;
	}
	else
	{
		//r.centerPos = VECTOR2(SCREEN_SIZE_X / 2 - 375, SCREEN_SIZE_Y / 2 + 210);
		//s.centerPos = VECTOR2(SCREEN_SIZE_X / 2 - 225, SCREEN_SIZE_Y / 2 + 230);

		r.centerPos = VECTOR2(SCREEN_SIZE_X / 2 - 200, SCREEN_SIZE_Y / 2 + 210);
		s.centerPos = VECTOR2(SCREEN_SIZE_X / 2 + 200, SCREEN_SIZE_Y / 2 + 210);

		r.tachoSize = 0.25f;
		s.tachoSize = 0.25f;
		s.size = 0.50f;

		s.pos = VECTOR2(SCREEN_SIZE_X / 2.0f, SCREEN_SIZE_Y - 25.0f);
		s.numOffset = 20.0f;
		s.kmhOffset = 30.0f;

		s.absPos = VECTOR2(SCREEN_SIZE_X / 2 + 65.0f, SCREEN_SIZE_Y - 40.0f);

		s.accelBarPos = VECTOR2(SCREEN_SIZE_X / 2 + 88.0f, SCREEN_SIZE_Y - 40.0f);
		s.brakeBarPos = VECTOR2(SCREEN_SIZE_X / 2 - 88.0f, SCREEN_SIZE_Y - 40.0f);

		g.pos = VECTOR2(SCREEN_SIZE_X / 2, SCREEN_SIZE_Y - 60.0f);
		g.size = 0.45f;
		g.shiftIndiPos = VECTOR2(SCREEN_SIZE_X / 2.0f, SCREEN_SIZE_Y - 60.0f);
		g.shiftIndiSize = 0.3f;

		g.tmPos = VECTOR2(SCREEN_SIZE_X / 2 - 65.0f, SCREEN_SIZE_Y - 40.0f);
		g.tmSize = 0.4f;
		g.tmOffset = 15.0f;

		ib.pos = VECTOR2(SCREEN_SIZE_X / 2, SCREEN_SIZE_Y - 40.0f);
		ib.rot = 90.0f * (PI / 180.0f);
	}
}

void UI::Draw()
{
	DrawRotaGraph(r.centerPos.x,r.centerPos.y, r.tachoSize, 0.0f, IMAGE_ID("image/tacho_rpm.png"), true);
	DrawRotaGraph(s.centerPos.x,s.centerPos.y, s.tachoSize, 0.0f, IMAGE_ID("image/tacho_speed.png"), true);

	DrawRotaGraph(ib.pos.x, ib.pos.y, 0.25f, ib.rot, IMAGE_ID("image/innerbox.png"), true);

	// 針
	DrawRotaGraph(r.centerPos.x, r.centerPos.y, r.tachoSize, r.Normalize * (180.0f * (PI / 180.0f)), IMAGE_ID("image/tacho_rpm_ptr.png"), true);
	DrawRotaGraph(s.centerPos.x, s.centerPos.y, s.tachoSize, s.Normalize * (265.0f * (PI / 180.0f)), IMAGE_ID("image/tacho_speed_ptr.png"), true);

	// スピード系
	if (s.speed > 100.0f)
	{
		DrawRotaGraph(s.pos.x - (s.numOffset * 2.0f), s.pos.y, s.size, 0.0f, s.speedNum[s.speed100], true);
	}
	if (s.speed > 10.0f)
	{
		DrawRotaGraph(s.pos.x - s.numOffset, s.pos.y, s.size, 0.0f, s.speedNum[s.speed10], true);
	}
	DrawRotaGraph(s.pos.x, s.pos.y, s.size, 0.0f, s.speedNum[s.speed1], true);
	DrawRotaGraph(s.pos.x + s.kmhOffset, s.pos.y, s.size, 0.0f, IMAGE_ID("image/kmh.png"), true);

	DrawBox(s.accelBarPos.x + 5.0f, s.accelBarPos.y + 35.0f, s.accelBarPos.x - 5.0f, (s.accelBarPos.y + 35.0f) - (70.0f * s.accelBox), 0xffffff,true);
	DrawRotaGraph(s.accelBarPos.x, s.accelBarPos.y, 0.25f, 0.0f, IMAGE_ID("image/bar_outline.png"), true);
	DrawBox(s.brakeBarPos.x + 5.0f, s.brakeBarPos.y + 35.0f, s.brakeBarPos.x - 5.0f, (s.brakeBarPos.y + 35.0f) - (70.0f * s.brakeBox), 0xffffff, true);
	DrawRotaGraph(s.brakeBarPos.x, s.brakeBarPos.y, 0.25f, 180.0f * (PI / 180.0f), IMAGE_ID("image/bar_outline.png"), true);

	// シフト系
	if (r.rpm > 7000.0f)
	{
		flashCnt++;
		if ((flashCnt / 5) % 3 == 0)
		{
			DrawRotaGraph(g.shiftIndiPos.x, g.shiftIndiPos.y, g.shiftIndiSize, 0.0f, IMAGE_ID("image/shiftIndi_3.png"), true);
		}
		flash = true;
	}
	else if (r.rpm > 6500.0f)
	{
		DrawRotaGraph(g.shiftIndiPos.x, g.shiftIndiPos.y, g.shiftIndiSize, 0.0f, IMAGE_ID("image/shiftIndi_3.png"), true);
	}
	else if (r.rpm > 6000.0f)
	{
		DrawRotaGraph(g.shiftIndiPos.x, g.shiftIndiPos.y, g.shiftIndiSize, 0.0f, IMAGE_ID("image/shiftIndi_2.png"), true);
	}
	else if (r.rpm > 5500.0f)
	{
		DrawRotaGraph(g.shiftIndiPos.x, g.shiftIndiPos.y, g.shiftIndiSize, 0.0f, IMAGE_ID("image/shiftIndi_1.png"), true);
	}
	else
	{
		flashCnt = 0;
		flash = false;
	}

	if (g.gearNum != -1)
	{
		if (flash)
		{
			if ((flashCnt / 5) % 2 == 0)
			{
				DrawRotaGraph(g.pos.x, g.pos.y, g.size, 0.0f, g.gear[g.gearNum + 1], true);
			}
		}
		else
		{
			DrawRotaGraph(g.pos.x, g.pos.y, g.size, 0.0f, g.gear[g.gearNum + 1], true);
		}
	}
	else
	{
		DrawRotaGraph(g.pos.x, g.pos.y, g.size, 0.0f, g.gear[10], true);
	}

	if (g.transmission)
	{
		DrawRotaGraph(g.tmPos.x, g.tmPos.y - g.tmOffset, g.tmSize, 0.0f, g.at[0], true);
		DrawRotaGraph(g.tmPos.x, g.tmPos.y + g.tmOffset, g.tmSize, 0.0f, g.mt[1], true);
	}
	else
	{
		DrawRotaGraph(g.tmPos.x, g.tmPos.y + g.tmOffset, g.tmSize, 0.0f, g.mt[0], true);
		DrawRotaGraph(g.tmPos.x, g.tmPos.y - g.tmOffset, g.tmSize, 0.0f, g.at[1], true);
	}

	if (s.absFlag)
	{
		DrawRotaGraph(s.absPos.x, s.absPos.y, g.tmSize, 0.0f, s.bs[0], true);
	}
	else
	{
		DrawRotaGraph(s.absPos.x, s.absPos.y, g.tmSize, 0.0f, s.bs[1], true);
	}
}
