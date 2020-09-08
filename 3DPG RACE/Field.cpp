#include "GameTask.h"
#include "Field.h"
#include "Player.h"

Field::Field()
{
	Init();
}

Field::~Field()
{
}

void Field::Init()
{
	floorTestModel = MV1LoadModel("model/Circuit/tukubaV01.mv1");
	floorTestPos = VGet(0.0f, 0.0f, 0.0f);
 	MV1SetPosition(floorTestModel, floorTestPos);
	MV1SetScale(floorTestModel, VGet(2.0f, 2.0f, 2.0f));

	// �n�ʑS�̺̂ؼޮݏ����\�z(��غ�ݑS����)
	MV1SetupCollInfo(floorTestModel, -1, 8, 8, 8);

	blueTarget = VGet(0.0f, 0.0f, 0.0f);

	ChangeLightTypeDir(VGet(0.0f, -1.0f, 0.0f));
}

void Field::Update()
{

	VECTOR StartPos, EndPos;

	// ---------- �ړ�
	//if (KeyMng::GetInstance().newKey[P1_UP])blueTarget.z += 7.0f;
	//if (KeyMng::GetInstance().newKey[P1_DOWN])blueTarget.z -= 7.0f;
	//if (KeyMng::GetInstance().newKey[P1_RIGHT])blueTarget.x += 7.0f;
	//if (KeyMng::GetInstance().newKey[P1_LEFT])blueTarget.x -= 7.0f;

	//�C��ʏ�̍��W�ɊY������3D��ԏ�� Near, Far�ʂ̍��W���擾
 	StartPos = VGet(blueTarget.x, blueTarget.y + 500.0f, blueTarget.z);		//  Start���W
	EndPos = VGet(blueTarget.x, blueTarget.y - 500.0f, blueTarget.z);		// End���W
	//DrawLine3D(StartPos, EndPos, 0xffff00);

	hitPos = MV1CollCheck_Line(floorTestModel, -1, StartPos, EndPos);
	blueTarget.y = hitPos.HitPosition.y;
}

void Field::Render(VECTOR carPos)
{
	// �º��
	//DrawCone3D(VGet(blueTarget.x, blueTarget.y + 200, blueTarget.z), blueTarget, 100, 4, 0x0000ff, 0xffffff, true);

	MV1DrawModel(floorTestModel);

	count++;
	if (count % 30 == 0)
	{
		p.push_back(make_shared<Point>(carPos));
	}

	for (auto i : p)
	{
		//DrawSphere3D(i->point, 50, 10, 0xff0000, 0xff0000, true);
	}
}
