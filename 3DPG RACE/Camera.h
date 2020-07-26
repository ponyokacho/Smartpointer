#pragma once

class Player;

class Camera
{
public:
	Camera(Player*);
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
	Player* player;
};
