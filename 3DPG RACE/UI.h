#pragma once
#include "VECTOR2.h"
#include <array>

class UI {
public:
	UI();
	~UI();

	void Update();
	void Draw();

private:
	struct EngineRpm {
		float rpm = 0.0f;
		float Normalize = 0.0f;
		VECTOR2 centerPos = { 0.0f,0.0f };
		float tachoSize = 0.0f;
	};
	struct Speed {
		int speedNum[10];
		int bs[2];

		float speed = 0.0f;
		float Normalize = 0.0f;
		VECTOR2 centerPos = { 0.0f,0.0f };
		float tachoSize = 0.0f;
		float size = 0.0f;

		int speed100 = 0;
		int speed10 = 0;
		int speed1 = 0;

		float maxSpeed = 0.0f;

		float accelBox = 0.0f;
		float brakeBox = 0.0f;

		VECTOR2 pos = { 0.0f,0.0f };

		VECTOR2 absPos = { 0.0f,0.0f };
		bool absFlag = false;

		VECTOR2 brakeBarPos = { 0.0f,0.0f };
		VECTOR2 accelBarPos = { 0.0f,0.0f };

		float numOffset = 0.0f;
		float kmhOffset = 0.0f;
	};

	struct Gear {
		VECTOR2 pos = { 0.0f,0.0f };
		VECTOR2 shiftIndiPos = { 0.0f,0.0f };

		float size = 0.0f;
		float shiftIndiSize = 0.0f;

		int gear[11];
		int gearNum = 0;

		int mt[2] = { 0,0 };
		int at[2] = { 0,0 };

		VECTOR2 tmPos = { 0.0f,0.0f };
		float tmSize = 0.0f;
		float tmOffset = 0.0f;
		bool transmission = false;
	};

	struct InnerBox {
		VECTOR2 pos = { 0.0f,0.0f };
		float rot = 0.0f;
	};

	EngineRpm r;
	Speed s;
	Gear g;
	InnerBox ib;

	bool view = false;

	bool flash = false;
	int flashCnt = 0;
};