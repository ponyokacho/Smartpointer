#pragma once

class Player;

class Camera
{
public:
	Camera(Player*);
	~Camera(){}

	void Init(void);
	void Update(void);

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
	Player* player;
};
