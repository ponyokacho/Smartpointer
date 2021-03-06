#pragma once

#include <memory>

constexpr VECTOR TARGET_OFFSET = { 0.0f,110.0f,0.0f };

class Player;

class Camera
{
public:
	Camera(std::shared_ptr<Player>);

	~Camera(){}

	void Init(void);
	void Update(void);

	void SetCamera(std::shared_ptr<Player> p);

	VECTOR pos;		// ｶﾒﾗ座標
	VECTOR rol;		// ｶﾒﾗ回転
	VECTOR up;		// ｶﾒﾗ上方向
	VECTOR target;	// ｶﾒﾗ注視点
	VECTOR mov;

	int cnt;

	float fov;		// 視野角 
	float c_near;	// 手前ｸﾘｯﾌﾟ
	float c_far;	// 奥ｸﾘｯﾌﾟ
	float ySize;
	Player& player;
	std::shared_ptr<Player>_player;
};
