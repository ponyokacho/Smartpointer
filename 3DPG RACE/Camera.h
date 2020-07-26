#pragma once

class Player;

class Camera
{
public:
	Camera(Player*);
	~Camera(){}

	void Init(void);
	void Update(void);

	VECTOR pos;		// ¶Ò×À•W
	VECTOR rol;		// ¶Ò×‰ñ“]
	VECTOR up;		// ¶Ò×ã•ûŒü
	VECTOR target;	// ¶Ò×’‹“_
	VECTOR mov;

	int cnt;

	float fov;		// ‹–ìŠp 
	float c_near;	// è‘O¸Ø¯Ìß
	float c_far;	// ‰œ¸Ø¯Ìß
	float ySize;
	Player* player;
};
