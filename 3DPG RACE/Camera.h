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

	VECTOR pos;		// ��׍��W
	VECTOR rol;		// ��׉�]
	VECTOR up;		// ��׏����
	VECTOR target;	// ��ג����_
	VECTOR mov;

	int cnt;

	float fov;		// ����p 
	float c_near;	// ��O�د��
	float c_far;	// ���د��
	float ySize;
	Player& player;
	std::shared_ptr<Player>_player;
};
