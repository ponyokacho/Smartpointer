#pragma once

#include <vector>
#include <memory>
#include "DxLib.h"

#include "Engine.h"
#include "DriveTrain.h"
#include "Tire.h"

class Player;
class Camera;
class Field;

#define  lpGameTask GameTask::GetInstance()

constexpr int SCREEN_SIZE_X(960);
constexpr int SCREEN_SIZE_Y(540);

constexpr float PI = 3.14159265;
constexpr float DT = 1.0f / 60.0f;

constexpr int MAX_GEAR = 6;
constexpr int ACTUAL_MAX_RPM = 7700;

constexpr float BRAKE_POWER_MAX = 0;
constexpr float TIRE_DIAMETER = 0.64; // m
constexpr float TIRE_PERIMETER = TIRE_DIAMETER * PI;
constexpr float ROLLING_FRICTION = 0.015;
constexpr float LOAD = 1250;		// kg

constexpr float CLUTCH_FRICTION = 0.25f;

constexpr float AIR_DENSITY = 0.125;
constexpr float FRONTAL_PROJECTED_AREA = 2.308; // m
constexpr float CD = 0.27;

constexpr float FINAL = 4.100;

constexpr float WHEEL_WEIGHT = 20; // kg Å¶óví≤êÆ

constexpr float WHEEL_BASE = 2.57; //m
constexpr float HIGHT_OF_G = 0.46; //m
constexpr float TREAD = 1.53;	//m
constexpr float T_OFFSET_X = TREAD * 5;
constexpr float T_OFFSET_Y = WHEEL_BASE * 5;

constexpr float G_ACCELERATION = 9.8; // m/s^2
constexpr float ONE_TIRE_LOAD = (LOAD * G_ACCELERATION) / 4;

constexpr float MU = 0.8f;

constexpr float WHEEL_ANGLE_MAX = 33; // ìx
constexpr float WHEEL_ANGLE_MAX_RAD = 33 * PI / 180;

constexpr float G_RATIO[6] = { 3.626,2.188,1.541,1.213,1.000,0.767 };

class GameTask
{
public:
	// º›∏ﬁŸƒ›
	static void Create(void);
	static GameTask &GetInstance(void)
	{
		Create();
		return *s_Instance;
	}

	int SystemInit();
	void GameInit();
	void GameUpdate();
	void Control();
	
private:
	static GameTask *s_Instance;

	XINPUT_STATE input;

	vector<shared_ptr<Engine>>e;
	vector<shared_ptr<DriveTrain>>d;
	vector<shared_ptr<Tire>>t;

	std::vector<std::shared_ptr<Player>>p;
	std::vector<std::shared_ptr<Camera>>c;
	std::vector<std::shared_ptr<Field>>f;

	float accel = 0;
	float throttlePercent = 0.0f;

	float clutch = 0.0f;
	float clutchPercent = 0.0f;

	float engineTorque = 0.0f;
	float rpm = 0.0f;

	int gearNum = -1;  // -1 = N

	float steering = 0.0f;
	float steeringPercent = 0.0f;

	float onlyEngineVel = 0.0f;

	float driveTireVel = 0.0f;
	float wheelTorque = 0.0f;

	float speed = 0.0f;

	VECTOR2 pos = { 0.0f,0.0f };
	VECTOR2 dirVec = { 0.0f,0.0f };
	VECTOR2 rotVec = { 0.0f ,0.0f };
	VECTOR2 tireForce = { 0.0f ,0.0f };
	VECTOR vectorSpeed = { 0.0f,0.0f,0.0f };
	float wheelAngle = 0.0f;
};

