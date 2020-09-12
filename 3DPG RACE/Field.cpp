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

	// ’n–Ê‘S‘Ì‚ÌºØ¼Ş®İî•ñ‚ğ\’z(ÎßØºŞİ‘SŒŸõ)
	MV1SetupCollInfo(floorTestModel, -1, 8, 8, 8);

	blueTarget = VGet(0.0f, 0.0f, 0.0f);

	ChangeLightTypeDir(VGet(0.0f, -1.0f, 0.0f));
	_goal._goalStart = VGet(-9000.0f, 100.0f, 43650.0f);
	_goal._goalEnd = VGet(-10800.0f, 100.0f, 39000.0f);

}

void Field::Update()
{

	VECTOR StartPos, EndPos;

	// ---------- ˆÚ“®
	//if (KeyMng::GetInstance().newKey[P1_UP])blueTarget.z += 7.0f;
	//if (KeyMng::GetInstance().newKey[P1_DOWN])blueTarget.z -= 7.0f;
	//if (KeyMng::GetInstance().newKey[P1_RIGHT])blueTarget.x += 7.0f;
	//if (KeyMng::GetInstance().newKey[P1_LEFT])blueTarget.x -= 7.0f;

	//‡C‰æ–Êã‚ÌÀ•W‚ÉŠY“–‚·‚é3D‹óŠÔã‚Ì Near, Far–Ê‚ÌÀ•W‚ğæ“¾
 	StartPos = VGet(blueTarget.x, blueTarget.y + 500.0f, blueTarget.z);		//  StartÀ•W
	EndPos = VGet(blueTarget.x, blueTarget.y - 500.0f, blueTarget.z);		// EndÀ•W
	//DrawLine3D(StartPos, EndPos, 0xffff00);

	hitPos = MV1CollCheck_Line(floorTestModel, -1, StartPos, EndPos);
	blueTarget.y = hitPos.HitPosition.y;
}

void Field::Render(VECTOR carPos)
{
	// Âº°İ
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

bool Field::HitCollision(VECTOR a, VECTOR b, VECTOR wallA, VECTOR wallB)
{
	bool rtnFlag = false;
	auto hitCheck = MV1CollCheck_Capsule(floorTestModel, -1, a, b, 80.0f);

	if (hitCheck.HitNum > 0)
	{
		// “–‚½‚Á‚½ƒ|ƒŠƒSƒ“‚Ì”‚¾‚¯ŒJ‚è•Ô‚µ
		for (int i = 0; i < hitCheck.HitNum; i++)
		{
			// “–‚½‚Á‚½ƒ|ƒŠƒSƒ“‚ğ•`‰æ
			/*DrawTriangle3D(
				hitCheck.Dim[i].Position[0],
				hitCheck.Dim[i].Position[1],
				hitCheck.Dim[i].Position[2], GetColor(0, 255, 255), true);*/

			_field = FieldStatus::Turf;
		}
		rtnFlag = true;
	}

	//‘¤–Ê‚Æ‚ÌÕ“Ë”»’f
	auto wallCheck = MV1CollCheck_Capsule(floorTestModel, -1, wallA, wallB, 100.0f);
	if (wallCheck.HitNum > 0)
	{
		// “–‚½‚Á‚½ƒ|ƒŠƒSƒ“‚Ì”‚¾‚¯ŒJ‚è•Ô‚µ
		for (int i = 0; i < wallCheck.HitNum; i++)
		{
			// “–‚½‚Á‚½ƒ|ƒŠƒSƒ“‚ğ•`‰æ
			/*DrawTriangle3D(
				wallCheck.Dim[i].Position[0],
				wallCheck.Dim[i].Position[1],
				wallCheck.Dim[i].Position[2], GetColor(255, 0, 255), true);*/
			_field = FieldStatus::Wall;
		}

		lpGameTask.SetCollisionPos(wallCheck.Dim->Position[0]);

		rtnFlag = true;

	}

	return rtnFlag;
}
