#include "GameTask.h"
#include "KeyMng.h"
#include "DxLib.h"
#include "Player.h"
#include "Field.h"
#include "Camera.h"
#include "ResourceMng.h"
#include "Engine.h"
#include "DriveTrain.h"
#include "Tire.h"
#include "UI.h"
#include "TitleScene.h"
#include "ResultScene.h"
#include "OptionScene.h"

#include <vector>
#include <memory>

GameTask *GameTask::s_Instance = nullptr;

void GameTask::Create(void)
{
	if (!s_Instance)
	{
		s_Instance = new GameTask();
	}
}

int GameTask::SystemInit()
{
	SetWindowText ("レッツゴー！くるまもどき");
	SetGraphMode(SCREEN_SIZE_X, SCREEN_SIZE_Y, 16);
	ChangeWindowMode(true);
	if (DxLib_Init() == -1) {
		return -1;
	}
	SetDrawScreen(DX_SCREEN_BACK);
	gLoopPtr = &GameTask::GameTitle;

	GameInit();
	return 0;
}

void GameTask::GameInit()
{
	e.push_back(make_shared<Engine>());
	d.push_back(make_shared<DriveTrain>());
	t.push_back(make_shared<Tire>());

	for (int j = 0; j < 2; j++)
	{
		p.push_back(make_shared<Player>(j));
	}

	c.push_back(make_shared<Camera>(p.back()));
	f.push_back(make_shared<Field>());
	
	u.push_back(make_shared<UI>());

	ChangeVolumeSoundMem(255 * 80 / 100, SOUND_ID("sounds/shift_change.wav"));

	_title = std::make_unique<TitleScene>();
	_title->SetPos(VGet(-18000.0f, 100.0f, 43000.0f));
	_result = std::make_unique<ResultScene>();
	_result->SetPos(VGet(-18000.0f, 100.0f, 43000.0f));

	_option = std::make_unique<OptionScene>();

	_playerRanking.resize(3);
	for (int i = 0; i < 3; i++)
	{
		_playerRanking[0][i] = 0;
		_playerRanking[1][i] = 0;
		_playerRanking[2][i] = 0;

	}
	_raceRanking.resize(3);
	for (int i = 0; i < 3; i++)
	{
		_raceRanking[0][i] = 0;
		_raceRanking[1][i] = 0;
		_raceRanking[2][i] = 0;

	}

}


void GameTask::Update()
{
	KeyMng::GetInstance().Update();

	DrawBox(0, 0, SCREEN_SIZE_X, SCREEN_SIZE_Y, 0x9DCCff, true);

	(this->*gLoopPtr)();

}


void GameTask::GameTitle()
{
	if (_fadeFlag)
	{
		if (FadeOut())
		{
			_fadeFlag = false;
		}
	}

	if (!CheckSoundMem(SOUND_ID("sounds/bgm_maoudamashii_neorock73.mp3")))
	{
		PlaySoundMem(SOUND_ID("sounds/bgm_maoudamashii_neorock73.mp3"), DX_PLAYTYPE_LOOP);
	}
	_titleCamera = VGet(-15000, 1500, 40000);//VGet(1000, 1000, 1500);

	ChangeFont("Bauhaus 93");
	SetFontSize(50);
	ChangeFont("MSゴシック");
	SetFontSize(10);
	SetFontSize(16);


	bodyPos = _title->GetPos();
	SetCameraPositionAndTarget_UpVecY(_titleCamera, bodyPos);

	for (auto field : f)
	{
		field->Update();
		field->Render(carPos);
	}

	_title->Draw();
	_title->Update();

	if (Fade(bodyPos, -10000.0f, 255, 0))
	{	
		//Init();
		if (!CheckSoundMem(SOUND_ID("sounds/menu1.mp3")))
		{
			PlaySoundMem(SOUND_ID("sounds/menu1.mp3"), DX_PLAYTYPE_BACK);
		}
		_fadeinFlag = true;
	}

	if (_fadeinFlag)
	{
		if (FadeIn())
		{
			for (auto i : d)
			{
				i->StopIdoling();
			}

			_fadeinFlag = false;
			_fadeFlag = true;
			gLoopPtr = &GameTask::GameOption;
			if (CheckSoundMem(SOUND_ID("sounds/bgm_maoudamashii_neorock73.mp3")))
			{
				StopSoundMem(SOUND_ID("sounds/bgm_maoudamashii_neorock73.mp3"));
			}
		}
	}

	SetDrawBlendMode(DX_BLENDMODE_ALPHA, _bright[0]);
	DrawBox(0, 0, SCREEN_SIZE_X, SCREEN_SIZE_Y, 0x000000, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

}

void GameTask::GameOption()
{
	if (_fadeFlag)
	{
		if (FadeOut())
		{
			_fadeFlag = false;
		}
	}
	if (!CheckSoundMem(SOUND_ID("sounds/bgm_maoudamashii_cyber38.mp3")))
	{
		PlaySoundMem(SOUND_ID("sounds/bgm_maoudamashii_cyber38.mp3"), DX_PLAYTYPE_LOOP);
	}
	DrawBox(0, 0, SCREEN_SIZE_X, SCREEN_SIZE_Y, 0x000000, true);
	DrawString(0, 0, "GameOption", 0xffffff);

	//-12800,200,37650,
	_titleCamera = VGet(-13000, 700, 34100);
	bodyPos = VGet(-12000, 200, 37650);


	SetCameraPositionAndTarget_UpVecY(_titleCamera, bodyPos);

	for (auto field : f)
	{
		field->Update();
		field->Render(carPos);
	}

	_option->Draw();
	_option->Update();


	if (!_fadeFlag)
	{
		if (KeyMng::GetInstance().trgKey[P1_PAUSE])
		{
			if (!CheckSoundMem(SOUND_ID("sounds/menu1.mp3")))
			{
				PlaySoundMem(SOUND_ID("sounds/menu1.mp3"), DX_PLAYTYPE_BACK);
			}
			_optionFlag = true;
		}
	}

	if (_optionFlag)
	{
		if (FadeIn())
		{
			for (auto i : p)
			{
				i->Init();
			}
			for (auto i : e)
			{
				i->Init();
			}
			for (auto i : d)
			{
				i->Init();
			}
			for (auto i : t)
			{
				i->Init();
			}
			for (int j = 0; j < _raceCnt.size(); j++)
			{
				_raceCnt[j] = 0;
			}
			//ｻｲｽﾞ変更
			_setGhost.resize(3);
			_getGhost.resize(3);


			_optionFlag = false;
			_fadeFlag = true;
			_startCnt = 3;
			_startFlag = false;

			OpenGhostData();
			OpenRaceTime();

			_ghostTime = 0;
			gLoopPtr = &GameTask::GameUpdate;
			_option->SetOption(SelectOption::ABS);
			if (CheckSoundMem(SOUND_ID("sounds/bgm_maoudamashii_cyber38.mp3")))
			{
				StopSoundMem(SOUND_ID("sounds/bgm_maoudamashii_cyber38.mp3"));
			}
		}
	}

	SetDrawBlendMode(DX_BLENDMODE_ALPHA, _bright[0]);
	DrawBox(0, 0, SCREEN_SIZE_X, SCREEN_SIZE_Y, 0x000000, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

}

void GameTask::GameUpdate()
{
	if (_fadeFlag)
	{
		if (FadeOut())
		{
			_fadeFlag = false;
		}
	}
	if (!CheckSoundMem(SOUND_ID("sounds/bgm_maoudamashii_neorock81.mp3")))
	{
		PlaySoundMem(SOUND_ID("sounds/bgm_maoudamashii_neorock81.mp3"), DX_PLAYTYPE_LOOP);
	}

	FPS();

	auto getOldPos = p[0]->GetCarPos();


	if (_startFlag)
	{
		
		if (_ghostTime < _getGhost[_ghostLap].size())
		{
			p[0]->SetCarPos(_getGhost[_ghostLap][_ghostTime]._pos);
			p[0]->SetVec(_getGhost[_ghostLap][_ghostTime]._vec);
			p[0]->SetDeg(_getGhost[_ghostLap][_ghostTime]._deg.x, _getGhost[_ghostLap][_ghostTime]._deg.y, 0.0f, _getGhost[_ghostLap][_ghostTime]._deg.z);
			p[0]->SetSpeed(_getGhost[_ghostLap][_ghostTime].speed);
		}
		else
		{
			_ghostTime = 0;
		}
		
		_ghostTime++;
		_raceTime++;
		AddCountRaceTime(_raceCnt);

		Control();
	}
	else
	{
		gearNum = -1;

	}

	if (clutch > 1.0f)
	{
		clutch = 1.0f;
	}

	if (_collisionFlag)
	{
		_collisionFlag = false;
	}

	DrawBox(0, 0, SCREEN_SIZE_X, SCREEN_SIZE_Y, 0x9DCCff, true);
	//DrawFormatString(0, 0, 0xffffff, "%d", fps);

	for (auto i : p)
	{
		if (!t.empty())
		{
			(*i).Update();
			i->Render();
		}

		if (i == p[p.size() - 1])
		{
			if (_startFlag)
			{
				//保存
				_ghost._pos = i->GetCarPos();
				_ghost._vec = i->GetVec();

				//
				_ghost._deg.x = i->GetDeg().yaw;
				_ghost._deg.y = i->GetDeg().pitch;
				_ghost._deg.z = i->GetDeg().roll;

				_ghost.speed = i->GetSpeed();

				_setGhost[_ghostLap].emplace_back(_ghost);

				auto check = i->GetHitBox();

				if (f[0]->HitCollision(check._startPos, check._endPos, check._wallStart, check._wallEnd))
				{

					if (f[0]->GetField() == FieldStatus::Wall)
					{
						_status = CollisionStatus::Wall;

						auto deg = p[p.size() - 1]->GetDeg();
						auto Cpos = p[p.size() - 1]->GetCarPos();
						auto wallCheck = VGet(_collisionPos.x - Cpos.x, _collisionPos.y - Cpos.y, _collisionPos.y - Cpos.y);
						wallCheck = VNorm(wallCheck);

						if (_checkPos.x > 0.0f)
						{
							i->SetDeg(deg.yaw + (wallCheck.x * 0.1f), deg.pitch, deg.oldPitch, deg.roll);
						}
						if(_checkPos.x < 0.0f)
						{
							i->SetDeg(deg.yaw - (wallCheck.x * 0.1f), deg.pitch, deg.oldPitch, deg.roll);
						}

					}
					if (f[0]->GetField() == FieldStatus::Turf)
					{
						_status = CollisionStatus::Turf;
						brake = 1.0f;
					}
				}

				auto goalCheck = f[0]->GetGoal();
				if (i->HitGoalCollision(goalCheck._goalStart, goalCheck._goalEnd))
				{
					if (!_rapRagFlag)
					{
						_playerRanking[_rap] = _raceCnt;

						auto raceCntMax = (_playerRanking[_rap][0] * 0.01f) + (_playerRanking[_rap][1] * 0.1f) + (_playerRanking[_rap][2])
							+ (_playerRanking[_rap][3] * 10) + (_playerRanking[_rap][4] * 100);

						auto rankingMax = (_raceRanking[_rap][0] * 0.01f) + (_raceRanking[_rap][1] * 0.1f) + (_raceRanking[_rap][2])
							+ (_raceRanking[_rap][3] * 10) + (_raceRanking[_rap][4] * 100);
						if (rankingMax == 0)
						{
							rankingMax = 9999;
						}
						if (raceCntMax < rankingMax)
						{
							_ghostSetFlag = true;
							UploadGhostData(_rap);
							UploadRaceTime(_rap);
							OpenRaceTime();
						}
						_raceCnt = { 0,0,0,0,0,0 };
						_rap++;
						if (_ghostLap < 2)
						{
							_ghostLap++;
						}
						_rapRagFlag = true;
					}
					
					if (_rap >= 3)
					{
						_resultFlag = true;
					}
				}
			}

			auto carPos = i->GetCarPos();
			auto viewPosPoint = i->GetViewPoint(1);
			if (carPos.x < viewPosPoint.x && carPos.z < viewPosPoint.z)
			{
				_rapRagFlag = false;
			}

		}
	}


	for (auto i : c)
	{
		i->Update();
	}
	for (auto i : f)
	{
		i->Update();
		i->Render(carPos);
	}

	for (auto i : e)
	{
		(*i).Update();
		i->Draw();
	}
	for (auto i : d)
	{
		i->SetSoundVol(volume);
		(*i).Update();
		i->Draw();
	}
	for (auto i : t)
	{
		(*i).Update();
		i->Draw();

		np = i->GetPitchLoad();
		nr = i->GetRollLoad();
	}

	for (auto i : u)
	{
		i->Update();
		i->Draw();
	}



	if (!_startFlag)
	{
		if (_bright[1] >= 255)
		{
			_bright[1] = 0;
			_startCnt--;
		}
		else
		{
			if (_bright[1] == 10)
			{
				PlaySoundMem(SOUND_ID("sounds/start_count.mp3"), DX_PLAYTYPE_BACK);
			}
			_bright[1] += 2;
		}
		SetFontSize(100);
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, _bright[1]);
		if (_startCnt > 0)
		{
			DrawFormatString(SCREEN_SIZE_X / 2 - 40 + 1, 101, 0x000000, "%d", _startCnt);
			DrawFormatString(SCREEN_SIZE_X / 2 - 40, 100, 0xffffff, "%d", _startCnt);
		}
		else
		{
			_startFlag = true;
			gearNum = 0;
		}
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

		SetFontSize(16);

	}
	if (_resultFlag)
	{
		if (_bright[2] < 255)
		{
			_bright[2] += 5;
		}
		else
		{
			_bright[2] = 0;
			_resultFlag = false;
			for (auto i : d)
			{
				i->StopIdoling();
			}
			if (CheckSoundMem(SOUND_ID("sounds/bgm_maoudamashii_neorock81.mp3")))
			{
				StopSoundMem(SOUND_ID("sounds/bgm_maoudamashii_neorock81.mp3"));
			}
			gLoopPtr = &GameTask::GameResult;
		}
	}

	SetFontSize(30);
	DrawFormatString(1, 1, 0x000000, "%d%d:%d%d:%d%d", _raceCnt[5], _raceCnt[4], _raceCnt[3], _raceCnt[2], _raceCnt[1], _raceCnt[0]);
	DrawFormatString(0, 0, 0xffffff, "%d%d:%d%d:%d%d", _raceCnt[5], _raceCnt[4], _raceCnt[3], _raceCnt[2], _raceCnt[1], _raceCnt[0]);

	SetFontSize(25);

	for (int i = 0; i < _playerRanking.size(); i++)
	{
		DrawFormatString(1, 51 + (20 * i), 0x000000, "%d%d:%d%d:%d%d", _playerRanking[i][5], _playerRanking[i][4], _playerRanking[i][3], _playerRanking[i][2], _playerRanking[i][1], _playerRanking[i][0]);
		DrawFormatString(0, 50 + (20 * i), 0xffffff, "%d%d:%d%d:%d%d", _playerRanking[i][5], _playerRanking[i][4], _playerRanking[i][3], _playerRanking[i][2], _playerRanking[i][1], _playerRanking[i][0]);
	}

	for (int i = 0; i < _raceRanking.size(); i++)
	{
		DrawFormatString(121, 51 + (20 * i), 0x000000, "%d%d:%d%d:%d%d", _raceRanking[i][5], _raceRanking[i][4], _raceRanking[i][3], _raceRanking[i][2], _raceRanking[i][1], _raceRanking[i][0]);
		DrawFormatString(120, 50 + (20 * i), 0xffffff, "%d%d:%d%d:%d%d", _raceRanking[i][5], _raceRanking[i][4], _raceRanking[i][3], _raceRanking[i][2], _raceRanking[i][1], _raceRanking[i][0]);
	}
	SetFontSize(16);

	SetDrawBlendMode(DX_BLENDMODE_ALPHA, _bright[2]);
	DrawBox(0, 0, SCREEN_SIZE_X, SCREEN_SIZE_Y, 0x000000, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	auto checkDeg = p[p.size() - 1]->GetDeg();

	auto carPos = p[p.size() - 1]->GetCarPos();
	_checkPos = VGet(getOldPos.x - carPos.x, getOldPos.y - carPos.y, getOldPos.y - carPos.y);
	_checkPos = VNorm(_checkPos);

	auto WallcheckPos = VGet(_collisionPos.x - carPos.x, _collisionPos.y - carPos.y, _collisionPos.y - carPos.y);
	WallcheckPos = VNorm(WallcheckPos);

	DrawFormatString(300, 10, 0xffffff, "RAP:(%d)", _rap);

	//DrawFormatString(0, 220, 0xffffff, "check:x(%.2f),y(%.2f),z(%.2f)", carPos.x, carPos.y, carPos.z);
	//DrawFormatString(0, 140, 0xffffff, "vectorSpeed:x(%.2f),y(%.2f),z(%.2f)", checkDeg.yaw, checkDeg.pitch, checkDeg.roll);
	DrawFormatString(0, 180, 0xffffff, "_rapRagFlag:(%d)", _rapRagFlag);
	//DrawFormatString(0, 260, 0xffffff, "WallcheckPos:x(%.2f),y(%.2f),z(%.2f)", WallcheckPos.x, WallcheckPos.y, WallcheckPos.z);

	if (_fadeFlag)
	{
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, _bright[0]);
		DrawBox(0, 0, SCREEN_SIZE_X, SCREEN_SIZE_Y, 0x000000, true);
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	}

}

void GameTask::AddCountRaceTime(std::array<int, 6> & time)
{
	if (time[0]++ >= 9)
	{

		time[0] = 0;
		if (time[1]++ >= 9)
		{
			time[1] = 0;
			if (time[2]++ >= 9)
			{
				time[2] = 0;
				if (time[3]++ >= 9)
				{
					time[3] = 0;
					if (time[4]++ >= 6)
					{
						time[4] = 0;
						if (time[5]++ >= 6)
						{
							time[5] = 0;
						}
					}
				}
			}
		}
	}
}

void GameTask::Control()
{
	GetJoypadXInputState(DX_INPUT_PAD1, &input);

	shift = (shiftUp || shiftDown);

	if (shift)
	{
		accel = 0.0f;
		gearNum = -1;

		int tmp = 0.0f;
		if (shiftUp)
		{
			tmp = 30;
		}
		else
		{
			tmp = 10;
		}

		shiftCnt++;
		if (shiftCnt == tmp)
		{
			shiftCnt = 0;
			gearNum = saveGearNum;
			shiftUp = false;
			shiftDown = false;
		}

		if (shiftCnt == 1)
		{
			if (!CheckSoundMem(SOUND_ID("sounds/shift_change.wav")))
			{
				PlaySoundMem(SOUND_ID("sounds/shift_change.wav"), DX_PLAYTYPE_BACK);
			}
		}
	}
	else
	{
		accel = input.RightTrigger;
		throttlePercent = 1.0f / 255.0f;
		accel *= throttlePercent;
	}

	volume = pow(1.025f, (accel * 100)) + 90;

	brake = input.LeftTrigger;
	brake *= throttlePercent;

	if (gearNum == -1)
	{
		clutch = 0.0f;
	}
	else
	{
		clutch = 1.0f;
	}

	steering = input.ThumbLX;
	steeringPercent = 1.0f / 32767;
	steering *= steeringPercent;
	if (steering < 0.15f && steering > -0.15f)
	{
		steering = 0.0f;
	}

	if (transmission)
	{
		if (!shift)
		{
			if (KeyMng::GetInstance().trgKey[P1_LB])
			{
				if (gearNum < -1)
				{
					gearNum = -1;
				}
				if (gearNum > 0)
				{
					gearNum--;
					shiftDown = true;
					saveGearNum = gearNum;
				}
			}
			if (KeyMng::GetInstance().trgKey[P1_RB])
			{
				gearNum++;
				if (gearNum > MAX_GEAR - 1)
				{
					gearNum = MAX_GEAR - 1;
				}
				else
				{
					shiftUp = true;
					saveGearNum = gearNum;
				}
			}
		}
	}
	else
	{
		if (!shift)
		{
			if (gearNum != -1)
			{
				if (gearMaxSpeed[gearNum] - 10.0f < speed)
				{
					if (gearNum != MAX_GEAR - 1)
					{
						gearNum++;
						shiftUp = true;
						saveGearNum = gearNum;
					}
				}

				shiftDownTiming = 20.0f * gearNum;

				if (gearMinSpeed[gearNum] + shiftDownTiming > speed)
				{
					if (gearNum != 0)
					{
						gearNum--;
						shiftDown = true;
						saveGearNum = gearNum;
						if (gearNum < -1)
						{
							gearNum = -1;
						}
					}
				}
			}
		}
	}
}

void GameTask::GameResult()
{
	if (_fadeFlag)
	{
		if (FadeOut())
		{
			_fadeFlag = false;
		}
	}
	if (!CheckSoundMem(SOUND_ID("sounds/bgm_maoudamashii_cyber36.mp3")))
	{
		PlaySoundMem(SOUND_ID("sounds/bgm_maoudamashii_cyber36.mp3"), DX_PLAYTYPE_LOOP);
	}

	_titleCamera = VGet(-15000, 1500, 48000);//VGet(1000, 1000, 1500);

	SetFontSize(16);

	bodyPos = _result->GetPos();
	SetCameraPositionAndTarget_UpVecY(_titleCamera, bodyPos);

	for (auto field : f)
	{
		field->Update();
		field->Render(carPos);
	}

	_result->Draw();
	_result->Update();

	auto raceCntMax = 0.0f;
	auto rankingMax = 0.0f;

	if (Fade(bodyPos, -10000.0f, 255, 50))
	{
		if (!CheckSoundMem(SOUND_ID("sounds/decision27.mp3")))
		{
			PlaySoundMem(SOUND_ID("sounds/decision27.mp3"), DX_PLAYTYPE_BACK);
		}
		_resultFadeFlag = true;
	}

	if (_resultFadeFlag)
	{
		if (FadeIn())
		{
			_resultFadeFlag = false;
			bool rankingChangeFlag = false;
			/*for (int i = 0; i < _raceRanking.size(); i++)
			{
				raceCntMax = (_playerRanking[i][0] * 0.01f) + (_playerRanking[i][1] * 0.1f) + (_playerRanking[i][2])
					       + (_playerRanking[i][3] * 10) + (_playerRanking[i][4] * 100);

				rankingMax = (_raceRanking[i][0] * 0.01f) + (_raceRanking[i][1] * 0.1f) + (_raceRanking[i][2])
						   + (_raceRanking[i][3] * 10) + (_raceRanking[i][4] * 100);
				if (rankingMax == 0)
				{
					rankingMax = 9999;
				}
				if (raceCntMax < rankingMax)
				{
					_ghostSetFlag = true;
					UploadGhostData(i);
				}
			}*/
			
			_resultFlag = false;
			_startFlag = false;
			_result->SetFlag(false);
			_fadeFlag = true;
			_abs.flag = true;
			_abs.power = 1;
			transmission = false;
			speed = 0.0f;
			gLoopPtr = &GameTask::GameTitle;
			_title->SetPos(VGet(-18000.0f, 100.0f, 43000.0f));
			_result->SetPos(VGet(-18000.0f, 100.0f, 43000.0f));
			_bright[0] = 0;
			if (CheckSoundMem(SOUND_ID("sounds/bgm_maoudamashii_cyber36.mp3")))
			{
				StopSoundMem(SOUND_ID("sounds/bgm_maoudamashii_cyber36.mp3"));
			}
		}
	}

	if (_result->GetFlag())
	{
		SetFontSize(50);
		DrawFormatString(401, 151, 0x000000, "%d:%d%d:%d%d", _raceCnt[4], _raceCnt[3], _raceCnt[2], _raceCnt[1], _raceCnt[0]);
		DrawFormatString(400, 150, 0xffffff, "%d:%d%d:%d%d", _raceCnt[4], _raceCnt[3], _raceCnt[2], _raceCnt[1], _raceCnt[0]);
		SetFontSize(16);
	}

	SetDrawBlendMode(DX_BLENDMODE_ALPHA, _bright[0]);
	DrawBox(0, 0, SCREEN_SIZE_X, SCREEN_SIZE_Y, 0x000000, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}

void GameTask::FPS()
{
	// 現在のシステム時間を取得
	NowTime = GetNowHiPerformanceCount();

	// 前回取得した時間からの経過時間を秒に変換してセット
	// ( GetNowHiPerformanceCount で取得できる値はマイクロ秒単位なので 1000000 で割ることで秒単位になる )
	deltaTime = (NowTime - Time) / 1000000.0f;

	// 今回取得した時間を保存
	Time = NowTime;

	// FPS関係の処理( 1秒経過する間に実行されたメインループの回数を FPS とする )
	FPSCounter++;
	if (NowTime - FPSCheckTime > 1000000)
	{
		fps = FPSCounter;
		FPSCounter = 0;
		FPSCheckTime = NowTime;
	}
}

void GameTask::UploadGhostData(int i)
{
	float size = 0.0f;
	float nondata = 0.0f;
	VECTOR2 nonVec2 = { 0.5f, 0.5f };

	Ghost data;
	size = _getGhost.size();

	if (_ghostSetFlag)
	{
		size = _setGhost[i].size();
	}

	FILE* file;

	if (i == 0)
	{
		fopen_s(&file, "Data/GhostData1.map", "wb+");
	}
	else if (i == 1)
	{
		fopen_s(&file, "Data/GhostData2.map", "wb+");
	}
	else
	{
		fopen_s(&file, "Data/GhostData3.map", "wb+");
	}

	if (!file)
	{
		return;
	}
	auto Csize = sizeof(Ghost);
	int Not = 0;
	fwrite(&size, sizeof(float), 1, file);
	fwrite(&Not, sizeof(int), 1, file);
	fwrite(&nonVec2, sizeof(VECTOR2), 1, file);

	for (int j = 0; j < size; ++j)
	{
		Ghost ghost;
		ghost = _setGhost[i][j];
		fwrite(&ghost._pos, sizeof(VECTOR), 1, file);
		fwrite(&nondata, sizeof(float), 1, file);

		fwrite(&ghost._vec, sizeof(VECTOR), 1, file);
		fwrite(&nondata, sizeof(float), 1, file);

		fwrite(&ghost._deg, sizeof(VECTOR), 1, file);
		fwrite(&nondata, sizeof(float), 1, file);

		fwrite(&ghost.speed, sizeof(float), 1, file);
		fwrite(&nondata, sizeof(float), 1, file);
		fwrite(&nondata, sizeof(float), 1, file);
		fwrite(&nondata, sizeof(float), 1, file);


	}

	_ghostSetFlag = false;
	fclose(file);
}

void GameTask::UploadRaceTime(int i)
{
	float size = 0.0f;
	float nondata = 0.0f;
	VECTOR2 nonVec2 = { 0.5f, 0.5f };
	FILE* file;

	if (i == 0)
	{
		fopen_s(&file, "Data/timeRanking1.map", "wb+");
	}
	else if (i == 1)
	{
		fopen_s(&file, "Data/timeRanking2.map", "wb+");
	}
	else
	{
		fopen_s(&file, "Data/timeRanking3.map", "wb+");
	}

	int dummy = 0;

	fwrite(&_playerRanking[i][0], sizeof(int), 1, file);
	fwrite(&_playerRanking[i][1], sizeof(int), 1, file);
	fwrite(&_playerRanking[i][2], sizeof(int), 1, file);
	fwrite(&_playerRanking[i][3], sizeof(int), 1, file);
	fwrite(&_playerRanking[i][4], sizeof(int), 1, file);
	fwrite(&dummy, sizeof(int), 1, file);
	fwrite(&dummy, sizeof(int), 1, file);
	fwrite(&dummy, sizeof(int), 1, file);

	fclose(file);
}

void GameTask::OpenGhostData()
{
	FILE* file;
	float getSize = 0.0f;
	float nondata = 0.5f;
	int Not = 0;
	VECTOR2 nonVec2 = { 0.0f, 0.0f };

	fopen_s(&file, "Data/GhostData1.map", "rb");

	if (!file)
	{
		return;
	}

	fread(&getSize, sizeof(float), 1, file);
	fread(&Not, sizeof(int), 1, file);
	fread(&nonVec2, sizeof(VECTOR2), 1, file);

	_getGhost[0].resize(getSize);

	for (int i = 0; i < _getGhost[0].size(); ++i)
	{
		fread(&_getGhost[0][i]._pos, sizeof(VECTOR), 1, file);
		fread(&nondata, sizeof(float), 1, file);

		fread(&_getGhost[0][i]._vec, sizeof(VECTOR), 1, file);
		fread(&nondata, sizeof(float), 1, file);

		fread(&_getGhost[0][i]._deg, sizeof(VECTOR), 1, file);
		fread(&nondata, sizeof(float), 1, file);

		fread(&_getGhost[0][i].speed, sizeof(float), 1, file);
		fread(&nondata, sizeof(float), 1, file);
		fread(&nondata, sizeof(float), 1, file);
		fread(&nondata, sizeof(float), 1, file);

	}

	fopen_s(&file, "Data/GhostData2.map", "rb");

	if (!file)
	{
		return;
	}

	fread(&getSize, sizeof(float), 1, file);
	fread(&Not, sizeof(int), 1, file);
	fread(&nonVec2, sizeof(VECTOR2), 1, file);

	_getGhost[1].resize(getSize);

	for (int i = 0; i < _getGhost[1].size(); ++i)
	{
		fread(&_getGhost[1][i]._pos, sizeof(VECTOR), 1, file);
		fread(&nondata, sizeof(float), 1, file);

		fread(&_getGhost[1][i]._vec, sizeof(VECTOR), 1, file);
		fread(&nondata, sizeof(float), 1, file);

		fread(&_getGhost[1][i]._deg, sizeof(VECTOR), 1, file);
		fread(&nondata, sizeof(float), 1, file);

		fread(&_getGhost[1][i].speed, sizeof(float), 1, file);
		fread(&nondata, sizeof(float), 1, file);
		fread(&nondata, sizeof(float), 1, file);
		fread(&nondata, sizeof(float), 1, file);

	}


	fopen_s(&file, "Data/GhostData3.map", "rb");

	if (!file)
	{
		return;
	}

	fread(&getSize, sizeof(float), 1, file);
	fread(&Not, sizeof(int), 1, file);
	fread(&nonVec2, sizeof(VECTOR2), 1, file);

	_getGhost[2].resize(getSize);

	for (int i = 0; i < _getGhost[2].size(); ++i)
	{
		fread(&_getGhost[2][i]._pos, sizeof(VECTOR), 1, file);
		fread(&nondata, sizeof(float), 1, file);

		fread(&_getGhost[2][i]._vec, sizeof(VECTOR), 1, file);
		fread(&nondata, sizeof(float), 1, file);

		fread(&_getGhost[2][i]._deg, sizeof(VECTOR), 1, file);
		fread(&nondata, sizeof(float), 1, file);

		fread(&_getGhost[2][i].speed, sizeof(float), 1, file);
		fread(&nondata, sizeof(float), 1, file);
		fread(&nondata, sizeof(float), 1, file);
		fread(&nondata, sizeof(float), 1, file);

	}
}

void GameTask::OpenRaceTime()
{
	FILE* file;
	fopen_s(&file, "Data/timeRanking1.map", "rb");
	if (!file)
	{
		return;
	}

	_raceRanking.resize(3);
	for (int i = 0; i < 3; i++)
	{
		_raceRanking[0][i] = 0;
		_raceRanking[1][i] = 0;
		_raceRanking[2][i] = 0;

	}

	int dummy = 0;

	for (int i = 0; i < _raceRanking.size(); i++)
	{
		if (i == 1)
		{
			fopen_s(&file, "Data/timeRanking2.map", "rb");
		}
		else if(i == 2)
		{
			fopen_s(&file, "Data/timeRanking3.map", "rb");
		}

		if (!file)
		{
			return;
		}

		fread(&_raceRanking[i][0], sizeof(int), 1, file);
		fread(&_raceRanking[i][1], sizeof(int), 1, file);
		fread(&_raceRanking[i][2], sizeof(int), 1, file);
		fread(&_raceRanking[i][3], sizeof(int), 1, file);
		fread(&_raceRanking[i][4], sizeof(int), 1, file);
		fread(&dummy, sizeof(int), 1, file);
		fread(&dummy, sizeof(int), 1, file);
		fread(&dummy, sizeof(int), 1, file);

	}

	fclose(file);
}
//フェードインアウト
bool GameTask::Fade(VECTOR pos, float target, int fadeInMax, int fadeOutMax)
{
	lpKeyMng.Update();
	bool rtnFlag = false;
	if (pos.x > target)
	{
		if (!_titleFlag)
		{
			if (_bright[0] < fadeInMax)
			{
				_bright[0] += 10;
			}
			else
			{
				_titleFlag = true;
			}
		}
		else
		{
			if (_bright[0] > fadeOutMax)
			{
				_bright[0] -= 5;
			}
			else
			{
				if (KeyMng::GetInstance().newKey[P1_SPACE])
				{
					_titleFlag = false;
					rtnFlag = true;
				}
			}
		}
	}

	SetDrawBlendMode(DX_BLENDMODE_ALPHA, _bright[0]);
	DrawBox(0, 0, SCREEN_SIZE_X, SCREEN_SIZE_Y, 0x000000, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	return rtnFlag;
}

bool GameTask::FadeIn()
{
	bool rtnFlag = false;
	if (_bright[0] < 255)
	{
		_bright[0] += 5;
	}
	else
	{
		rtnFlag = true;
	}

	SetDrawBlendMode(DX_BLENDMODE_ALPHA, _bright[0]);
	DrawBox(0, 0, SCREEN_SIZE_X, SCREEN_SIZE_Y, 0x000000, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	return rtnFlag;
}

bool GameTask::FadeOut()
{
	bool rtnFlag = false;
	if (_bright[0] > 0)
	{
		_bright[0] -= 5;
	}
	else
	{
		rtnFlag = true;
	}

	SetDrawBlendMode(DX_BLENDMODE_ALPHA, _bright[0]);
	DrawBox(0, 0, SCREEN_SIZE_X, SCREEN_SIZE_Y, 0x000000, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	return rtnFlag;
}

const bool& GameTask::GetCollisionFlag()
{
	return _collisionFlag;
}

void GameTask::SetCollisionFlag(bool flag)
{
	_collisionFlag = flag;
}

const CollisionStatus& GameTask::GetField()
{
	return _status;
}