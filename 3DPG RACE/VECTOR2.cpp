#include "VECTOR2.h"

VECTOR2::VECTOR2()
{
}


VECTOR2::~VECTOR2()
{
}


VECTOR2 operator*(const VECTOR2& p, const VECTOR2& v)
{
	VECTOR2 tmp;
	tmp.x = p.x * v.x;
	tmp.y = p.y * v.y;
	return tmp;
}

VECTOR2 operator*(const VECTOR2& p, const float& f)
{
	VECTOR2 tmp;
	tmp.x = p.x * f;
	tmp.y = p.y * f;
	return tmp;
}

VECTOR2 operator*(const float & f, const VECTOR2 & p)
{
	VECTOR2 tmp;
	tmp.x = p.x * f;
	tmp.y = p.y * f;
	return tmp;
}

VECTOR2 operator/(const VECTOR2& p, const float& f)
{
	VECTOR2 tmp;
	tmp.x = p.x / f;
	tmp.y = p.y / f;
	return tmp;
}

VECTOR2 operator-(const VECTOR2& p, const VECTOR2& v)
{
	VECTOR2 tmp;
	tmp.x = p.x - v.x;
	tmp.y = p.y - v.y;
	return tmp;
}

VECTOR2 operator+(const VECTOR2& p, const VECTOR2& v)
{
	VECTOR2 tmp;
	tmp.x = p.x + v.x;
	tmp.y = p.y + v.y;
	return tmp;
}

bool operator==(const VECTOR2 & p, const VECTOR2 & t)
{
	return ((int)p.x == (int)t.x && (int)p.y == (int)t.y);
}

bool operator!=(const VECTOR2 & p, const VECTOR2 & t)
{
	return ((int)p.x != (int)t.x && (int)p.y != (int)t.y);
}

bool operator>=(const VECTOR2 & p, const VECTOR2 & t)
{
	return (p.x >= t.x && p.y >= t.y);
}

bool operator<=(const VECTOR2 & p, const VECTOR2 & t)
{
	return (p.x <= t.x && p.y <= t.y);
}
