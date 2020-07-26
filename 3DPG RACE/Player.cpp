#include "math.h"
#include "GameTask.h"
#include "KeyMng.h"
#include "Player.h"

Player::Player()
{
	Init();
	GameTask::GetInstance().addObjCnt();
}

Player::~Player()
{
	GameTask::GetInstance().removeObjCnt();
}


void Player::Init()
{
	bodyModel = MV1LoadModel("model/ae86Cp.mv1");
	wheelFRModel = MV1LoadModel("model/ae86RWheel.mv1");
	wheelFLModel = MV1LoadModel("model/ae86LWheel.mv1");
	wheelRRModel = MV1LoadModel("model/ae86RWheel.mv1");
	wheelRLModel = MV1LoadModel("model/ae86LWheel.mv1");
	boxModel = MV1LoadModel("model/box.mv1");
	boxRModel = MV1LoadModel("model/box.mv1");
	boxCarModel = MV1LoadModel("model/box.mv1");
	boxCamModel = MV1LoadModel("model/box.mv1");

	carRolY = 0.0f;	
	wheelFRRolX = 0.0f;
	wheelFLRolX = 0.0f;
	wheelFRolY = 0.0f;
	wheelRRRolX = 0.0f;
	wheelRLRolX = 0.0f;
	boxRolY = 0.0f;
	boxRRolY = 0.0f;

	carScl = VGet(1.0f,1.0f,1.0f);
	carMat = MGetScale(carScl);
	carPos = VGet(0.0f,0.0f,0.0f);

	wheelScl = VGet(1.0f,1.0f,1.0f);
	wheelFRMat = MGetScale(wheelScl);
	wheelFLMat = MGetScale(wheelScl);
	wheelRRMat = MGetScale(wheelScl);
	wheelRLMat = MGetScale(wheelScl);
	wheelFRPos = VGet(0.0f, 0.0f, 0.0f);
	wheelFLPos = VGet(0.0f, 0.0f, 0.0f);
	wheelRRPos = VGet(0.0f, 0.0f, 0.0f);
	wheelRLPos = VGet(0.0f, 0.0f, 0.0f);

	boxScl = VGet(1.0f, 1.0f, 1.0f);		// ﾎｲｰﾙと同じ拡大率にする
	boxMat = MGetScale(boxScl);
	boxPos = VGet(-9564.0f,0.0f,41682.0f);

	boxFRScl = VGet(1.0f, 1.0f, 1.0f);
	boxFRMat = MGetScale(boxFRScl);
	boxFRPos = VGet(0.0f, 0.0f, 0.0f);

	boxFLScl = VGet(1.0f, 1.0f, 1.0f);
	boxFLMat = MGetScale(boxFLScl);
	boxFLPos = VGet(0.0f, 0.0f, 0.0f);

	boxRScl = VGet(1.0f, 1.0f, 1.0f);
	boxRMat = MGetScale(boxRScl);
	boxRPos = VGet(0.0f, 0.0f, 0.0f);

	boxCarScl = VGet(1.0f, 1.0f, 1.0f);
	boxCarMat = MGetScale(boxCarScl);
	boxCarPos = VGet(0.0f, 0.0f, 0.0f);

	boxCamScl = VGet(1.0f,1.0f,1.0f);
	boxCamMat = MGetScale(boxCamScl);
	boxCamPos = VGet(0.0f,0.0f,0.0f); 

	wheelFOffset = VGet(135.0f, 65.0f, 480.0f);
	wheelROffset = VGet(135.0f, 65.0f, 0.0f);

	bodyOffset = VGet(0.0f, 0.0f, 0.0f);

	boxCamOffset = VGet(0.0f, 300.0f, -900.0f);

	speed = 0;
	udFlag = false;
	rolFlag = false;

	dirVec = VGet(0.0f, 0.0f, 0.0f);
	frontWheelDeg = VGet(0.0f, 0.0f, 0.0f);
	
	LRFlag = 0;		// -1:右 0:なし 1:左(カメラ用)
	sum = 0;

	dirCrossFlag = 0;	// -1:右 0:なし 1:左

	//MV1SetScale(bodyModel, carScl);
	//MV1SetPosition(bodyModel, carPos);
}

//---ﾎﾞﾃﾞｨにﾎｲｰﾙを追従させる
void Player::Update()
{
	// 向いている方向のﾍﾞｸﾄﾙと前に進むスピードのﾍﾞｸﾄﾙを合成する
	// 車の前方向(黄色)
	//DrawLine3D(boxPos, VGet(boxPos.x + front.x * (speed * 100), boxPos.y + front.y * (speed * 100), boxPos.z + front.z * (speed * 100)), 0xffff00);
	// ﾀｲﾔの切れ角(紫)
	//DrawLine3D(boxPos, VGet(boxPos.x + frontWheel.x * 900, boxPos.y + frontWheel.y * 900, boxPos.z + frontWheel.z * 900), 0xcd619b);

	//dirVec = VAdd(front, frontWheel);		
	// 進む方向ﾍﾞｸﾄﾙ(水色)、徐々に紫のﾍﾞｸﾄﾙ側に寄せていく
	//DrawLine3D(boxPos, VGet(boxPos.x + dirVec.x * (speed * 100), boxPos.y + dirVec.y * (speed * 100), boxPos.z + dirVec.z * (speed * 100)),0xfffff);


	// 左右どちらに曲がるべきかの判定
	dirCross = Cross(frontWheel, front);

	// 切れ角制限
	dirLim = Dot(frontWheel, front);
	frontDeg = VGet(front.x * (180 / DX_PI_F), front.y * (180 / DX_PI_F), front.z * (180 / DX_PI_F));
	frontWheelDeg = VGet(frontWheel.x * (180 / DX_PI_F), frontWheel.y * (180 / DX_PI_F), frontWheel.z * (180 / DX_PI_F));

	// rolAngle(車の曲がる量)はｽﾋﾟｰﾄﾞと反比例（y=a/x x≠0,y≠0,aは比例定数）
	if (speed != 0)
	{
		//rolAngle = 1.5f / speed;
		rolAngle = speed / 25;		// test
	}
	else
	{
		rolAngle = 0;
	}


	if (KeyMng::GetInstance().newKey[P1_UP])
	{
		wheelFRRolX -= 3.0f;
		wheelFLRolX -= 3.0f;
		wheelRRRolX -= 3.0f;
		wheelRLRolX -= 3.0f;

		// boxPos.xとboxPos.zが向いている方向ﾍﾞｸﾄﾙ
		speed += 0.15f;
		if (speed >= 70.0f)
		{
			speed = 70.0f;
		}

		fVec = VGet(boxPos.x + front.x, boxPos.y + front.y, boxPos.z + front.z);
		wheelAngle = VGet(boxPos.x + frontWheel.x, boxPos.y + frontWheel.y, boxPos.z + frontWheel.z);

		// 黄色を徐々に紫に近づける？

		//// rolAngle分ずつ回転（つまりrolAngleが "0"（停止状態）にならない限りY軸回転は止まらない）
		if (dirCross < 0)	// dirCrossが右に行ってた場合
		{
			//dirCrossFlag = -1;
			//if (dirCross > -0.1)
			//{
			//	dirCross = 0;
			//}
			//else
			{
				boxRolY = (DX_PI_F / 180) * rolAngle;
			}
		}
		else if (dirCross > 0)	// dirCrossが左に行ってた場合
		{
			//dirCrossFlag = 1;
			//if (dirCross < 0.1)
			//{
			//	dirCross = 0;
			//}
			//else
			{
				boxRolY = -(DX_PI_F / 180) * rolAngle;
			}
		}
		else
		{
			dirCrossFlag = 0;
			dirCross = 0;
			rolAngle = 0;
			boxRolY = 0;
		}
	}
	// 要修正(ﾊﾞｯｸする方向,挙動)
	else if (KeyMng::GetInstance().newKey[P1_DOWN])
	{
		if (speed < 0)
		{
			wheelFRRolX += 3.0f;
			wheelFLRolX += 3.0f;
			wheelRRRolX += 3.0f;
			wheelRLRolX += 3.0f;
		}
		else
		{
			wheelFRRolX -= 1.5f;
			wheelFLRolX -= 1.5f;
			wheelRRRolX -= 1.5f;
			wheelRLRolX -= 1.5f;
		}

		// boxPos.xとboxPos.zが向いている方向ﾍﾞｸﾄﾙ
		speed -= 0.15f;
		if (speed <= -20.0f)
		{
			speed = -20.0f;
		}
		
		if (dirCross < 0)
		{
			boxRolY = (DX_PI_F / 180) * rolAngle;
			rolFlag = true;
		}
		else if (dirCross > 0)
		{
			boxRolY = -(DX_PI_F / 180) * rolAngle;
			rolFlag = true;
		}
	}
	else  // ｱｸｾﾙ抜いたとき
	{
		if (speed > 0)
		{
			speed -= 0.10f;
			wheelFRRolX -= 1.5f;
			wheelFLRolX -= 1.5f;
			wheelRRRolX -= 1.5f;
			wheelRLRolX -= 1.5f;
		}
		else if (speed < 0)
		{
			speed += 0.10f;
			wheelFRRolX += 1.5f;
			wheelFLRolX += 1.5f;
			wheelRRRolX += 1.5f;
			wheelRLRolX += 1.5f;
		}

		if (dirCross < 0)
		{
			if (dirCross > -0.1)
			{
				dirCross = 0;
			}
			else
			{
				boxRolY = (DX_PI_F / 180) * rolAngle;
			}
			rolFlag = true;
		}
		else if (dirCross > 0)
		{
			if (dirCross < 0.1)
			{
				dirCross = 0;
			}
			else
			{
				boxRolY = -(DX_PI_F / 180) * rolAngle;
			}
			rolFlag = true;
		}
		//boxRolY = 0;
		udFlag = false;
	}

	if (KeyMng::GetInstance().newKey[P1_RIGHT])
	{
		//if (frontWheel.x < front.x + 0.6f)
		//if (frontWheelDeg.x < frontDeg.x + 44.0f)
		if(dirCross > -0.8)
		{
			//---ﾎｲｰﾙのY軸回転
			wheelFRolY += (DX_PI_F / 180) * 1.5f;
			boxRRolY = wheelFRolY;
		}
		LRFlag = 1;
	}
	else if (KeyMng::GetInstance().newKey[P1_LEFT])
	{
		//if (frontWheel.x > front.x - 0.6f)
		//if (frontWheelDeg.x > frontDeg.x - 44.0f)
		if (dirCross < 0.8)
		{
			//---ﾎｲｰﾙのY軸回転
			wheelFRolY += -(DX_PI_F / 180) * 1.5f;
			boxRRolY = wheelFRolY;
		}
		LRFlag = -1;
	}
	else
	{
		if (LRFlag == 1)
		{
			wheelFRolY += -(DX_PI_F / 180) * 1.0f;
		}
		else if (LRFlag == -1)
		{
			wheelFRolY += (DX_PI_F / 180) * 1.0f;
		}
		boxRRolY = wheelFRolY;
		LRFlag = 0;
	}

	boxPos.x += front.x * speed;
	boxPos.z += front.z * speed;

	//---回転->移動の順を守る
	//---回転

	boxRMat = MGetIdent();
	boxFRMat = MGetIdent();
	boxFLMat = MGetIdent();

	//---本体
	MATRIX tmp;

	yAxis = VTransform(VGet(0.0f, 1.0f, 0.0f), boxMat);
	tmp = MGetRotAxis(yAxis, boxRolY);
	boxMat = MMult(boxMat, tmp);

	yAxis = VTransform(VGet(0.0f, 1.0f, 0.0f), boxRMat);
	MATRIX tmpRB = MGetRotAxis(yAxis, boxRRolY);
	boxRMat = MMult(boxRMat, tmpRB);

	yAxis = VTransform(VGet(0.0f, 1.0f, 0.0f), boxCarMat);
	MATRIX tmpCar = MGetRotAxis(yAxis, boxRolY);
	boxCarMat = MMult(boxCarMat,tmpCar);

	yAxis = VTransform(VGet(0.0f, 1.0f, 0.0f), boxFRMat);
	MATRIX wheelFR = MGetRotAxis(yAxis, wheelFRolY);
	boxFRMat = MMult(boxFRMat, wheelFR);

	yAxis = VTransform(VGet(0.0f, 1.0f, 0.0f), boxFLMat);
	MATRIX wheelFL = MGetRotAxis(yAxis, wheelFRolY);
	boxFLMat = MMult(boxFLMat, wheelFL);

	//yAxis = VTransform(VGet(0.0f, 1.0f, 0.0f), carMat);
	//carMat = MMult(carMat, tmpRB);

	//---ﾎｲｰﾙ
	xAxis = VTransform(VGet(1.0f, 0.0f, 0.0f),boxFRMat);
	MATRIX tmpR = MGetRotAxis(xAxis, wheelFRRolX);
	wheelFRMat = MMult(boxFRMat, tmpR);

	xAxis = VTransform(VGet(1.0f, 0.0f, 0.0f), boxFLMat);
	MATRIX tmpL = MGetRotAxis(xAxis, wheelFLRolX);
	wheelFLMat = MMult(boxFLMat, tmpL);

	xAxis = VTransform(VGet(1.0f, 0.0f, 0.0f), boxMat);
	MATRIX tmpRR = MGetRotAxis(xAxis, wheelRRRolX);
	wheelRRMat = MMult(boxMat, tmpRR);

	xAxis = VTransform(VGet(1.0f, 0.0f, 0.0f), boxMat);
	MATRIX tmpRL = MGetRotAxis(xAxis, wheelRLRolX);
	wheelRLMat = MMult(boxMat, tmpRL);

	//---移動
	front = VTransform(VGet(0.0f, 0.0f, 1.0f), boxMat);
	frontWheel = VTransform(VGet(0.0f, 0.0f, 1.0f), boxRMat);

	//---ｵﾌｾｯﾄ
	//---FR
	VECTOR wheelFRTmp = VTransform(VGet(wheelFOffset.x, -wheelFOffset.y, wheelFOffset.z),boxMat);

	wheelFRPos.x = boxPos.x + wheelFRTmp.x;
	wheelFRPos.y = boxPos.y + wheelFRTmp.y;
	wheelFRPos.z = boxPos.z + wheelFRTmp.z;

	//---FL
	VECTOR wheelFLTmp = VTransform(VGet(-wheelFOffset.x, -wheelFOffset.y, wheelFOffset.z), boxMat);

	wheelFLPos.x = boxPos.x + wheelFLTmp.x;
	wheelFLPos.y = boxPos.y + wheelFLTmp.y;
	wheelFLPos.z = boxPos.z + wheelFLTmp.z;

	//---RR
	VECTOR wheelRRTmp = VTransform(VGet(wheelROffset.x, -wheelROffset.y,wheelROffset.z), boxMat);

	wheelRRPos.x = boxPos.x + wheelRRTmp.x;
	wheelRRPos.y = boxPos.y + wheelRRTmp.y;
	wheelRRPos.z = boxPos.z + wheelRRTmp.z;

	//---RL
	VECTOR wheelRLTmp = VTransform(VGet(-wheelROffset.x, -wheelROffset.y, wheelROffset.z), boxMat);

	wheelRLPos.x = boxPos.x + wheelRLTmp.x;
	wheelRLPos.y = boxPos.y + wheelRLTmp.y;
	wheelRLPos.z = boxPos.z + wheelRLTmp.z;

	//---boxROff
	VECTOR boxROff = VTransform(VGet(0.0f, 0.0f , wheelFOffset.z / 2), boxRMat);

	boxRPos.x = boxPos.x + boxROff.x;
	boxRPos.y = boxPos.y + boxROff.y;
	boxRPos.z = boxPos.z + boxROff.z;

	// boxCarOff
	VECTOR boxCarOff = VTransform(VGet(0.0f, 0.0f, wheelFOffset.z / 2), boxCarMat);

	boxCarPos.x = boxPos.x + boxCarOff.x;
	boxCarPos.y = boxPos.y + boxCarOff.y;
	boxCarPos.z = boxPos.z + boxCarOff.z;

	//---boxCamOff
	VECTOR boxCamOff = VTransform(VGet(boxCamOffset.x, boxCamOffset.y, boxCamOffset.z), boxMat);

	// カメラずらし
	if (LRFlag == -1)
	{
		sum -= 0.5f;
		if (sum < -60)
		{
			sum = -60;
		}
	}
	else if (LRFlag == 1)
	{
		sum += 0.5f;
		if (sum > 60)
		{
			sum = 60;
		}
	}
	else
	{
		if (sum > 0)
		{
			sum -= 0.5f;
			if (sum <= 0)
			{
				sum = 0;
			}
		}
		else if(sum < 0)
		{
			sum += 0.5f;
			if (sum >= 0)
			{
				sum = 0;
			}
		}
	}

	boxCamPos.x = boxPos.x + (boxCamOff.x + sum);
	boxCamPos.y = boxPos.y + boxCamOff.y;
	boxCamPos.z = boxPos.z + boxCamOff.z;


	//---合成
	tmp = MMult(boxMat, MGetTranslate(VGet(boxPos.x, boxPos.y, boxPos.z)));
	tmpRB = MMult(boxRMat, MGetTranslate(VGet(boxRPos.x, boxRPos.y, boxRPos.z)));
	tmpCar = MMult(boxCarMat, MGetTranslate(VGet(boxCarPos.x, boxCarPos.y, boxCarPos.z)));
	wheelFRMat = MMult(wheelFRMat, MGetTranslate(VGet(wheelFRPos.x, wheelFRPos.y, wheelFRPos.z)));
	wheelFLMat = MMult(wheelFLMat, MGetTranslate(VGet(wheelFLPos.x, wheelFLPos.y, wheelFLPos.z)));
	wheelRRMat = MMult(wheelRRMat, MGetTranslate(VGet(wheelRRPos.x, wheelRRPos.y, wheelRRPos.z)));
	wheelRLMat = MMult(wheelRLMat, MGetTranslate(VGet(wheelRLPos.x, wheelRLPos.y, wheelRLPos.z)));
	boxCamMat = MMult(boxMat, MGetTranslate(VGet(boxCamPos.x, boxCamPos.y, boxCamPos.z)));
	//carMat = MMult(carMat, MGetTranslate(VGet(carPos.x, carPos.y, carPos.z)));
	//carMat = MMult(carMat, tmp);
	//wheelFRMat = MMult(wheelFRMat, carMat);
	//wheelFLMat = MMult(wheelFLMat, carMat);
	//wheelRRMat = MMult(wheelRRMat, carMat);
	//wheelRLMat = MMult(wheelRLMat, carMat);

	//---ﾓﾃﾞﾙにｾｯﾄ
	MV1SetMatrix(boxRModel, tmpRB);
	MV1SetMatrix(boxModel, tmp);
	MV1SetMatrix(boxCarModel, tmpCar);
	MV1SetMatrix(bodyModel, tmpCar);
	MV1SetMatrix(wheelFRModel, wheelFRMat);
	MV1SetMatrix(wheelFLModel, wheelFLMat);
	MV1SetMatrix(wheelRRModel, wheelRRMat);
	MV1SetMatrix(wheelRLModel, wheelRLMat);
	MV1SetMatrix(boxCamModel, boxCamMat);
}

void Player::Render()
{
	//DrawFormatString(0, 100, 0xffffff,"wheelFRolY: %.1f",wheelFRolY);
	//DrawFormatString(0, 115, 0xffffff,"boxRolY: %.1f", boxRolY);
	//DrawFormatString(0, 130, 0xffffff, "boxPos: ( %.1f, %.1f, %.1f)", boxPos.x,boxPos.y,boxPos.z);
	//DrawFormatString(0, 145, 0xffffff, "boxRRolY: %.1f", boxRRolY);
	//DrawFormatString(0, 160, 0xffffff, "dirCross: %.1f", dirCross);
	//DrawFormatString(0, 175, 0xffffff, "dirLim: %.1f", dirLim);
	//DrawFormatString(0, 190, 0xffffff, "front:( %.1f, %.1f, %.1f)", front.x, front.y, front.z);
	//DrawFormatString(0, 205, 0xffffff, "frontWheel:( %.1f, %.1f, %.1f)", frontWheel.x, frontWheel.y, frontWheel.z);
	//DrawFormatString(0, 220, 0xffffff, "frontDeg:( %.1f, %.1f, %.1f)", frontDeg.x, frontDeg.y, frontDeg.z);
	//DrawFormatString(0, 235, 0xffffff, "frontWheelDeg:( %.1f, %.1f, %.1f)", frontWheelDeg.x, frontWheelDeg.y, frontWheelDeg.z);
	//DrawFormatString(0, 250, 0xffffff, "rolAngle:%.1f", rolAngle);
	//DrawFormatString(0, 265, 0xffffff, "speed:%.1f", speed);
	//DrawFormatString(0, 280, 0xffffff, "playerPos:( %.1f, %.1f, %.1f)", boxPos.x, boxPos.y, boxPos.z);
	//DrawString(0, 295, "(操作方法)十字キー:ｱｸｾﾙ・ﾌﾞﾚｰｷ・右ﾊﾝﾄﾞﾙ・左ﾊﾝﾄﾞﾙ", 0xff0000);
	//MV1DrawModel(boxRModel);
	//MV1DrawModel(boxModel);
	//MV1DrawModel(boxCarModel);
	MV1DrawModel(bodyModel);
	MV1DrawModel(wheelFRModel);
	MV1DrawModel(wheelFLModel);
	MV1DrawModel(wheelRRModel);
	MV1DrawModel(wheelRLModel);
	MV1DrawModel(boxCamModel);
}

// 外積
float Player::Cross(VECTOR va, VECTOR vb)
{
	return ((va.z * vb.x) - (vb.z * va.x));
}

// 内積
float Player::Dot(VECTOR va, VECTOR vb)
{
	return ((va.x * vb.x) + (va.z * vb.z));
}
