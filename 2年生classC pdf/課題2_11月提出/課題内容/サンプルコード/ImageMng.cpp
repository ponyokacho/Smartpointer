#include <DxLib.h>
#include "ImageMng.h"

// ======================================================================================
// SetID
// @param1 : 命名するidName
// @param2 : 登録するﾌｧｲﾙ名
// ======================================================================================
const int& ImageMng::SetID(std::string id_name, std::string f_name)
{
	if(imgMap.find(id_name) == imgMap.end())
	{
		//imgMap[id_name].resize(1);	// 要素数を1に設定
		imgMap[id_name] = LoadGraph(f_name.c_str());
	}
	return imgMap[id_name];	// LoadGraph用のimgMapに登録してidを返す
}

// ======================================================================================
// SetID
// @id_name : 命名するidName
// @f_name  : 登録するﾌｧｲﾙ名
// @divSize : 1ﾏｽのｻｲｽﾞ
// @divCnt  : 縦横の個数
//
// ======================================================================================
const std::vector<int>& ImageMng::SetID(std::string id_name, std::string f_name, VECTOR2 divSize, VECTOR2 divCnt)
{
	if(imgMap.find(id_name) == imgMap.end())
	{
		imgDivMap[id_name].resize(divCnt.x*divCnt.y);	// 要素数をdivCnt.x*divCnt.yに設定

		LoadDivGraph(f_name.c_str(),
				divCnt.x*divCnt.y,
				divCnt.x,
				divCnt.y,
				divSize.x,
				divSize.y,
				&imgDivMap[id_name][0], true);
	}
	return imgDivMap[id_name];	// LoadDivGraph用のimgDivMapに登録してidを返す
}

// ======================================================================================
// DrawImage
// @idName  : 描画するidName
// @x       : 描画する左上のX座標
// @y       : 描画する左上のY座標 
// @flag    : 透過ﾌﾗｸﾞ
//
// ======================================================================================
void ImageMng::DrawImage(std::string id_name, int x, int y, bool flag)
{
	// LoadGraph用のimgMapで処理する

	// ｴﾗｰﾁｪｯｸ�@ 指定したIDの登録が無かったら無視
	if(imgMap.find(id_name) == imgMap.end()){
		return;
	}
	DrawGraph(x, y, imgMap[id_name], flag);	// LoadGraph用のimgMapから描画
}

// ======================================================================================
// DrawImage ※ｱﾆﾒ版
// @idName  : 描画するidName
// @x       : 描画する左上のX座標
// @y       : 描画する左上のY座標
// @animNo  : 描画するｱﾆﾒｺﾏ番号
// @flag    : 透過ﾌﾗｸﾞ
//
// ======================================================================================
void ImageMng::DrawImage(std::string id_name, int x, int y, int animNo, bool flag)
{
	// LoadDivGraph用のimgDivMapで処理する

	// ｴﾗｰﾁｪｯｸ�@ 指定したIDの登録が無かったら無視
	if (imgDivMap.find(id_name) == imgDivMap.end()) {
		return;
	}

	// ｴﾗｰﾁｪｯｸ�A ｱﾆﾒ番号が0以下、最大ｱﾆﾒ数を超えていたら無視
	if(0 > animNo || imgDivMap[id_name].size() <= animNo){
		return;
	}

	DrawGraph(x, y, imgDivMap[id_name][animNo], flag);	// LoadDivGraph用のimgDivMapから描画
}

ImageMng::ImageMng()
{
}

ImageMng::~ImageMng()
{
}
