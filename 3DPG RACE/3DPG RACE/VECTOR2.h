#pragma once
#include <cmath>
#include <vector>

class VECTOR2
{
public:
	VECTOR2();
	VECTOR2(float px, float py) : x(px), y(py) {}
	~VECTOR2();
	float x, y;

	// ŽÎ•Ó‚ð‹‚ß‚é
	float Magnitude()const
	{
		return sqrt(x * x + y * y);
	}

	//³‹K‰»
	VECTOR2 Normalize()const
	{
		auto z = Magnitude();
		return VECTOR2(x / z, y / z);
	}
	float VectorDot(VECTOR2& start, VECTOR2& end)
	{
		float s = 0.0f;

		s = start.x * end.x + start.y * end.y;

		return s;
	}


private:

};
VECTOR2 operator*(const VECTOR2&, const VECTOR2&);
VECTOR2 operator*(const VECTOR2&, const float&);
VECTOR2 operator*(const float&, const VECTOR2&);
VECTOR2 operator/(const VECTOR2&, const float&);
VECTOR2 operator-(const VECTOR2&, const VECTOR2&);
VECTOR2 operator+(const VECTOR2&, const VECTOR2&);
bool operator==(const VECTOR2&, const VECTOR2&);
bool operator!=(const VECTOR2&, const VECTOR2&);
bool operator>=(const VECTOR2&, const VECTOR2&);
bool operator<=(const VECTOR2&, const VECTOR2&);
