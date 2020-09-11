#pragma once

#include <array>
#include <memory>

constexpr VECTOR TARGET_OFFSET = { 0.0f,110.0f,0.0f };

class Player;

class Camera
{
public:
	//Camera() {};
	Camera(std::shared_ptr<Player>);

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
	Player& player;
	std::shared_ptr<Player>_player;
};
