#pragma once

class VECTOR2
{
public:
	int x;
	int y;

	// ‰Šú‰»(‰Šú’l‚È‚µ)
	VECTOR2()
	{
		x = 0;
		y = 0;
	}

	// ‰Šú‰»(‰Šú’l‚ ‚è)
	VECTOR2(int x, int y)
	{
		this->x = x;
		this->y = y;
	} 
	~VECTOR2(){}

	// ----- ‘ã“ü‰‰Zq
	VECTOR2& operator=(const VECTOR2& vec)
	{
		this->x = vec.x;
		this->y = vec.y;
		return *this;
	}

	// ----- ”äŠr‰‰Zq
	bool operator==(const VECTOR2& vec) const
	{
		return ((this->x == vec.x) && (this->y == vec.y));
	}
	bool operator!=(const VECTOR2& vec) const
	{
		return !((this->x == vec.x) && (this->y == vec.y));
	}

	// ----- ’P€‰‰Zq
	VECTOR2& operator+=(const VECTOR2& vec)
	{
		this->x += vec.x;
		this->y += vec.y;
		return *this;
	}
	VECTOR2& operator-=(const VECTOR2& vec)
	{
		this->x -= vec.x;
		this->y -= vec.y;
		return *this;
	}
	VECTOR2& operator*=(int k)
	{
		this->x *= k;
		this->y *= k;
		return *this;
	}
	VECTOR2& operator/=(int k)
	{
		this->x /= k;
		this->y /= k;
		return *this;
	}
};