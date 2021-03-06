#include "DxLib.h"
#include "GameTask.h"
#include "ImageMng.h"
#include "KeyMng.h"
#include "Camera.h"
#include "Player.h"
#include "Enemy.h"
#include "Primitive.h"
#include "Polygon.h"

int GameTask::SCREEN_SIZE_X = 1024;
int GameTask::SCREEN_SIZE_Y = 600;

int GameTask::Update()
{
	KeyMng::GetInstance().Update();
	//int rtnID = (this->*gLoopPtr)();	// ｹﾞｰﾑﾙｰﾌﾟ(関数ﾎﾟｲﾝﾀｰ)
	//return rtnID;

	switch(gameMode){
		case GAME_INIT:
			GameInit();
			if((KeyMng::GetInstance().newKey[P1_RA])
			){
				//gLoopPtr = &GameTask::GameMain;
				gameMode = GAME_MAIN;
			}
			break;
		case GAME_MAIN:
			GameMain();
			break;
		case GAME_CLEAR:
			GameClear();
			break;
		case GAME_OVER:
			GameOver();
			break;
		default:
			break;
	};
	return 0;
}

int GameTask::SystemInit()
{
	SetWindowText("C++Sample");
	SetGraphMode(SCREEN_SIZE_X, SCREEN_SIZE_Y, 16);			// 800×600ﾄﾞｯﾄ65536色ﾓｰﾄﾞに設定
	ChangeWindowMode(true);				// true:window　false:ﾌﾙｽｸﾘｰﾝ
	if (DxLib_Init() == -1) {
		return false;					// DXﾗｲﾌﾞﾗﾘ初期化処理
	}
	SetDrawScreen(DX_SCREEN_BACK);

	SetUseZBuffer3D(true);	// Zﾊﾞｯﾌｧを有効にする
	SetWriteZBuffer3D(true);// Zﾊﾞｯﾌｧへの書き込みを有効にする

	SetUseLighting(false);	// ﾗｲﾃｨﾝｸﾞ無効

	// ｹﾞｰﾑﾙｰﾌﾟ(関数ﾎﾟｲﾝﾀ)をGameInitに変更
	//gLoopPtr = &GameTask::GameInit;
	gameMode = GAME_INIT;
	return 0;
}

int GameTask::GameInit()
{
	// ----- 1枚画像の登録
	ImageMng::GetInstance().SetID("playerImage", "image/stop.png");

	// ----- ｱﾆﾒｰｼｮﾝの画像登録
	animCnt = 0;
	VECTOR2 divSize = {96, 64};
	VECTOR2 divCnt = {4, 1};
	ImageMng::GetInstance().SetID("pRunImage", "image/run.png", divSize, divCnt);

	cam = new Camera();

	poly = new Polygon_();

/*
	player = new Player();

	prim = new Primitive();
*/
	int model = MV1LoadModel("model/enemy/Teki.mv1");
	for(int i = 0; i < 20; i++){
		enemy[i] = new Enemy(model);
	}


	DrawString(0, 0, "INIT", 0xffff00);
	DrawString(0, 16, "LSHIFT START", 0xffffff);
	return 0;
}

int GameTask::GameMain()
{
	DrawLine3D(VGet(500.0f, 0.0f, 0.0f), VGet(-500.0f, 0.0f, 0.0f), 0xff0000);// X(赤)
	DrawLine3D(VGet(0.0f, 500.0f, 0.0f), VGet(0.0f, -500.0f, 0.0f), 0x00ff00);// Y(緑)
	DrawLine3D(VGet(0.0f, 0.0f, 500.0f), VGet(0.0f, 0.0f, -500.0f), 0x0000ff);// Z(青)

	cam->Update();

	poly->Update();
	poly->Render();
/*
	prim->Render();

	// 三角形ﾎﾟﾘｺﾞﾝ
	VERTEX3D Vertex1[3];
	VERTEX3D Vertex2[3];

	//int texture;
	//texture = LoadGraph("texture/sample.png");
	int texture = ImageMng::GetInstance().SetID("sample", "texture/sample.png");

	// ----- �@
	Vertex1[0].pos = VGet(100.0f, 300.0f, 0.0f);
	Vertex1[0].norm = VGet(0.0f, 0.0f, -1.0f);	// 法線
	Vertex1[0].dif = GetColorU8(255, 255, 255, 255);
	Vertex1[0].spc = GetColorU8(255, 255, 255, 255);
	Vertex1[0].u = 0.0f;	// 横
	Vertex1[0].v = 0.0f;	// 縦

	// ----- �A
	Vertex1[1].pos = VGet(250.0f, 100.0f, 0.0f);
	Vertex1[1].norm = VGet(0.0f, 0.0f, -1.0f);	// 法線
	Vertex1[1].dif = GetColorU8(255, 255, 255, 255);
	Vertex1[1].spc = GetColorU8(255, 255, 255, 255);
	Vertex1[1].u = 1.0f;	// 横
	Vertex1[1].v = 1.0f;	// 縦

	// ----- �B
	Vertex1[2].pos = VGet(100.0f, 100.0f, 0.0f);
	Vertex1[2].norm = VGet(0.0f, 0.0f, -1.0f);	// 法線
	Vertex1[2].dif = GetColorU8(255, 255, 255, 255);
	Vertex1[2].spc = GetColorU8(255, 255, 255, 255);
	Vertex1[2].u = 0.0f;	// 横
	Vertex1[2].v = 1.0f;	// 縦

	// ﾎﾟﾘｺﾞﾝ描画
	//DrawPolygon3D(Vertex1, 1, DX_NONE_GRAPH, false);
	DrawPolygon3D(Vertex1, 1, texture, true);

	// ----- �@
	Vertex2[0].pos = VGet(100.0f, 300.0f, 0.0f);
	Vertex2[0].norm = VGet(0.0f, 0.0f, -1.0f);	// 法線
	Vertex2[0].dif = GetColorU8(255, 255, 255, 255);
	Vertex2[0].spc = GetColorU8(255, 255, 255, 255);
	Vertex2[0].u = 0.0f;	// 横
	Vertex2[0].v = 0.0f;	// 縦

	// ----- �A
	Vertex2[1].pos = VGet(250.0f, 300.0f, 0.0f);
	Vertex2[1].norm = VGet(0.0f, 0.0f, -1.0f);	// 法線
	Vertex2[1].dif = GetColorU8(255, 255, 255, 255);
	Vertex2[1].spc = GetColorU8(255, 255, 255, 255);
	Vertex2[1].u = 1.0f;	// 横
	Vertex2[1].v = 0.0f;	// 縦

	// ----- �B
	Vertex2[2].pos = VGet(250.0f, 100.0f, 0.0f);
	Vertex2[2].norm = VGet(0.0f, 0.0f, -1.0f);	// 法線
	Vertex2[2].dif = GetColorU8(255, 255, 255, 255);
	Vertex2[2].spc = GetColorU8(255, 255, 255, 255);
	Vertex2[2].u = 1.0f;	// 横
	Vertex2[2].v = 1.0f;	// 縦

	DrawPolygon3D(Vertex2, 1, texture, true);

	//player->Update();
	//player->Render();
*/


	for(int i = 0; i < 20; i++){
		enemy[i]->Update();
		enemy[i]->Render();
	}


	DrawString(0, 0, "MAIN", 0xffff00);
	return 0;
}

int GameTask::GameOver()
{
	return 0;
}

int GameTask::GameClear()
{
	return 0;
}