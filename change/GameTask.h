#pragma once

#include <vector>
#include <array>
#include <memory>
#include "DxLib.h"
#include "VECTOR2.h"

class Engine;
class DriveTrain;
class Tire;
class Player;
class Camera;
class Field;
class UI;
class TitleScene;
class ResultScene;
class OptionScene;

#define  lpGameTask GameTask::GetInstance()

using namespace std;

constexpr int SCREEN_SIZE_X(960);
constexpr int SCREEN_SIZE_Y(540);

constexpr float PI = 3.14159265;
constexpr float DT = 1.0f / 60.0f;

constexpr int MAX_GEAR = 5;
constexpr int IDOL_RPM = 1000;
constexpr int MAX_RPM = 7999;
constexpr int ACTUAL_MAX_RPM = 7700;

constexpr int MAX_THOUSAND = 7;

constexpr float BRAKE_POWER_MAX = 400.0f;
constexpr float TIRE_DIAMETER = 0.64; // m
constexpr float TIRE_PERIMETER = TIRE_DIAMETER * PI;
constexpr float ROLLING_FRICTION = 0.015;
constexpr float LOAD = 1250;		// kg

constexpr float CLUTCH_FRICTION = 0.25f;

constexpr float AIR_DENSITY = 0.125;
constexpr float FRONTAL_PROJECTED_AREA = 2.308; // m
constexpr float CD = 0.27;

constexpr float FINAL = 4.100;

constexpr float WHEEL_WEIGHT = 20; // kg ※要調整

constexpr float WHEEL_BASE = 2.57; //m
constexpr float HIGHT_OF_G = 0.46; //m
constexpr float TREAD = 1.53;	//m
constexpr float T_OFFSET_X = TREAD * 5;
constexpr float T_OFFSET_Y = WHEEL_BASE * 5;

constexpr float G_ACCELERATION = 9.8; // m/s^2
constexpr float ONE_TIRE_LOAD = (LOAD * G_ACCELERATION) / 4;

constexpr float MU = 0.8f;

constexpr float WHEEL_ANGLE_MAX = 33; // 度
constexpr float WHEEL_ANGLE_MAX_RAD = WHEEL_ANGLE_MAX * PI / 180;

constexpr float G_RATIO[6] = { 3.626,2.188,1.541,1.213,1.000,0.767 };


enum class CollisionStatus
{
	Non,
	Turf,
	Wall,
};

class GameTask
{
public:
	// ｼﾝｸﾞﾙﾄﾝ
	static void Create(void);
	static GameTask &GetInstance(void)
	{
		Create();
		return *s_Instance;
	}

	int SystemInit();
	void GameInit();
	void Update();
	void GameTitle();
	void GameOption();
	void TopTimeValue();
	void GameUpdate();
	void AddCountRaceTime(std::array<int, 6>& time);
	void Control();
	void GameResult();
	void FPS();

	void UploadGhostData(int i);

	void UploadRaceTime(int i);

	void OpenGhostData();

	void OpenRaceTime();

	bool Fade(VECTOR pos, float target, int fadeInMax, int fadeOutMax);

	bool FadeIn();

	bool FadeOut();

	const bool& GetCollisionFlag();

	void SetCollisionFlag(bool flag);

	const CollisionStatus& GetField();

	float GetPitchLoad()
	{
		return np;
	}
	float GetRollLoad()
	{
		return nr;
	}

	float GetBrake()
	{
		return brake * BRAKE_POWER_MAX;
	}

	void SetRpm(float r)
	{
		rpm = r;
	}
	float GetRpm()
	{
		return rpm;
	}

	void SetActualRpm(float rpm)
	{
		actualRpm = rpm;
	}
	float GetActualRpm()
	{
		return actualRpm;
	}

	void SetGearMaxSpeed(float speed,int i)
	{
		gearMaxSpeed[i] = speed;
	}
	float GetMaxSpeed()
	{
		return gearMaxSpeed[MAX_GEAR - 1];
	}

	void SetGearMinSpeed(float speed, int i)
	{
		gearMinSpeed[i] = speed;
	}

	void SetSpeed(float sp)
	{
		speed = sp;
	}
	float GetSpeed()
	{
		return speed;
	}

	bool GetShift()
	{
		return shift;
	}

	void SetView(bool v)
	{
		view = v;
	}
	bool GetView()
	{
		return view;
	}

	int GetGearNum()
	{
		return gearNum;
	}

	float GetRT()
	{
		return accel;
	}
	float GetLT()
	{
		return brake;
	}

	//option Get
	bool GetTransmission()
	{
		return transmission;
	}
	bool GetABSFlag()
	{
		return _abs.flag;
	}
	int GetABSPower()
	{
		return _abs.power;
	}

	//option Set
	void SetTransmission(bool flag)
	{
		transmission = flag;
	}
	void SetABSFlag(bool flag)
	{
		_abs.flag = flag;
	}
	void SetABSPower(int num)
	{
		_abs.power = num;
	}
	void AddABSPower()
	{
		_abs.power++;
	}
	void SubABSPower()
	{
		_abs.power--;
	}

	float GetDeltaTime()
	{
		return deltaTime;
	}

	void SetTireForce(VECTOR2 tf)
	{
		tireForce = tf;
	}
	VECTOR2 GetTireForce()
	{
		return tireForce;
	}

	void SetDirVec(VECTOR2 vec)
	{
		dirVec = vec;
	}
	VECTOR2 GetDirVec()
	{
		return dirVec;
	}

	void SetFrontWheelVec(VECTOR2 vec)
	{
		fWheelVec = vec;
	}
	VECTOR2 GetFrontWheelVec()
	{
		return fWheelVec;
	}

	void SetFrontWheelVecRot(VECTOR vec)
	{
		fWheelVecRot = vec;
	}
	VECTOR GetFrontWheelVecRot()
	{
		return fWheelVecRot;
	}

	void SetLRFlag(int flag)
	{
		lr = flag;
	}
	int GetLRFlag()
	{
		return lr;
	}

	void SetSteering(float steer)
	{
		steering = steer;
	}
	float GetSteering()
	{
		return steering;
	}
	
	void SetVectorSpeed(VECTOR vec)
	{
		vectorSpeed = vec;
	}
	VECTOR GetVectorSpeed()
	{
		return vectorSpeed;
	}

	void SetVectorSpeedRot(VECTOR vec)
	{
		vectorSpeedRot = vec;
	}
	VECTOR GetVectorSpeedRot()
	{
		return vectorSpeedRot;
	}

	void SetDirVecRot(VECTOR vec)
	{
		dirVecRot = vec;
	}
	VECTOR GetDirVecRot()
	{
		return dirVecRot;
	}

	void SetCarPos(VECTOR pos)
	{
		carPos = pos;
	}
	VECTOR GetCarPos()
	{
		return carPos;
	}

	void SetAcceleration(float accel)
	{
		acceleration = accel;
	}
	float GetAcceleration()
	{
		return acceleration;
	}

	void SetEngineTorque(float t)
	{
		engineTorque = t;
	}
	float GetEngineTorque()
	{
		return engineTorque;
	}

	void SetOnlyEngineVel(float v)
	{
		onlyEngineVel = v;
	}
	float GetOnlyEngineVel()
	{
		return onlyEngineVel;
	}

	float GetClutch()
	{
		return clutch;
	}

	void SetDriveTireVel(float d)
	{
		driveTireVel = d;
	}
	float GetDriveTireVel()
	{
		return driveTireVel;
	}

	void SetWheelTorque(float wt)
	{
		wheelTorque = wt;
	}
	float GetWheelTorque()
	{
		return wheelTorque;
	}

	void SetTireLock(bool tl)
	{
		tireLock = tl;
	}
	bool GetTireLock()
	{
		return tireLock;
	}

	void SetWheelAngle(float wa)
	{
		wheelAngle = wa;
	}
	float GetWheelAngle()
	{
		return wheelAngle;
	}

	void SetCollisionPos(VECTOR p)
	{
		_collisionPos = p;
	}

private:
	static GameTask *s_Instance;

	// ｹﾞｰﾑﾙｰﾌﾟ用関数ﾎﾟｲﾝﾀｰ
	void (GameTask::* gLoopPtr)(void);


	XINPUT_STATE input;

	vector<shared_ptr<Engine>>e;
	vector<shared_ptr<DriveTrain>>d;
	vector<shared_ptr<Tire>>t;

	vector<shared_ptr<Player>>p;
	vector<shared_ptr<Camera>>c;
	vector<shared_ptr<Field>>f;

	vector<shared_ptr<UI>>u;

	float accel = 0.0f;
	float brake = 0.0f;
	float throttlePercent = 0.0f;

	float clutch = 0.0f;

	float engineTorque = 0.0f;
	float rpm = 0.0f;

	int gearNum = 0;  // -1 = N
	array<float, 5>gearMinSpeed = { 0.0f };
	array<float, 5>gearMaxSpeed = { 0 };

	float steering = 0.0f;
	float steeringPercent = 0.0f;

	float onlyEngineVel = 0.0f;

	float driveTireVel = 0.0f;
	float wheelTorque = 0.0f;

	float speed = 0.0f;

	VECTOR2 pos = { 0.0f,0.0f };
	VECTOR2 dirVec = { 0.0f,0.0f };
	VECTOR dirVecRot = { 0.0f,0.0f,1.0f };
	VECTOR2 rotVec = { 0.0f ,0.0f };
	VECTOR2 tireForce = { 0.0f ,0.0f };
	VECTOR vectorSpeed = { 0.0f,0.0f,1.0f };
	VECTOR vectorSpeedRot = { 0.0f,0.0f,1.0f };
	VECTOR carPos = { 0.0f,0.0f,1.0f };

	VECTOR2 fWheelVec = { 0.0f ,0.0f };
	VECTOR fWheelVecRot = { 0.0f ,0.0f, 1.0f };

	float wheelAngle = 0.0f;
	int lr = 0;
	float deg = 0.0f;
	float acceleration = 0.0f;

	float np = 0.0f;
	float nr = 0.0f;

	bool shift = false;
	bool shiftUp = false;
	bool shiftDown = false;
	int shiftCnt = 0;
	int shiftDownTiming = 0.0f;

	int volume = 0;

	float actualRpm = 0.0f;
	int saveGearNum = 0;

	bool tireLock = 0.0f;
	bool view = false;

	LONGLONG NowTime;
	LONGLONG Time;
	int fps = 0;
	int FPSCounter = 0;
	LONGLONG FPSCheckTime = 0;
	float deltaTime = 0.0f;


	//フェードインorアウト
	bool _fadeFlag = false;

	//time計測
	int _raceTime = 0;
	std::array<int, 6> _raceCnt;
	std::array<int, 6> _playerRapCnt;
	std::array<int, 6> topTime;

	using _topRaceCnt = std::array<int, 6>;

	std::vector<_topRaceCnt> _playerRanking;
	std::vector<_topRaceCnt> _raceRanking;
	float _ghostTopTime = 0.0f;


	//Title変数
	std::unique_ptr<TitleScene> _title;
	VECTOR _titleCamera;
	VECTOR bodyPos;
	std::array<int, 3> _bright{ 0,0,0 };
	bool _brightFlag = false;
	bool _titleFlag = false;
	bool _fadeinFlag = false;

	//GameOption
	//
	struct ABS
	{
		bool flag = true;
		int power = 1; // 0,1,2
	};
	ABS _abs;
	bool transmission = false; // false:mt, true:at
	std::unique_ptr<OptionScene> _option;
	bool _optionFlag = false;

	//GameMain変数
	struct Ghost
	{
		VECTOR _pos;
		VECTOR _vec;
		VECTOR _deg;
		float speed;
	};
	Ghost _ghost;
	bool _ghostSetFlag = false;

	std::vector<std::vector<Ghost>> _setGhost;
	std::vector<std::vector<Ghost>> _getGhost;
	int _ghostTime = 0;
	int _ghostLap = 0;


	//レースのスタート関係
	bool _startFlag = false;
	int _startCnt = 3;
	int _knockbackTime = 0;
	bool _collisionFlag = false;
	CollisionStatus _status;

	int _rap = 0;
	int _rapRagTime = 0;
	bool _rapRagFlag = false;

	float setSteering = 0.0f;
	VECTOR _collisionPos;
	VECTOR _checkPos;


	//Result変数
	std::unique_ptr<ResultScene> _result;
	bool _resultFlag = false;
	bool _resultFadeFlag = false;



};

