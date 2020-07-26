#pragma once
#include <vector>
#include <map>
#include "VECTOR2.h"

// ----- ｼﾝｸﾞﾙﾄﾝｸﾗｽのKeyCheckｸﾗｽを定義する
class ImageMng
{
private:
	// ----- ｺﾝｽﾄﾗｸﾀ群
	ImageMng();		// ﾃﾞﾌｫﾙﾄｺﾝｽﾄﾗｸﾀをprivateにして外部から生成できない様にする
	ImageMng(const ImageMng&){}					// ----- ｺﾋﾟｰｺﾝｽﾄﾗｸﾀをprivate化
	ImageMng& operator=(const ImageMng&){}		// ----- 代入演算子のｵｰﾊﾞｰﾗｲﾄﾞをprivate化
	~ImageMng();	// ﾃﾞｽﾄﾗｸﾀ

	// 画像登録用map配列
	// std::map<ｷｰの型,連想配列の型> 連想配列名;
	std::map<std::string, int> imgMap;
	std::map<std::string, std::vector<int>> imgDivMap;

public:
	// ----- ImageMngｵﾌﾞｼﾞｪｸﾄの実体を返す(ｼﾝｸﾞﾙﾄﾝ)
	static ImageMng& GetInstance(){
		static ImageMng imgInstance;	// ImageMngの実体を生成。imgInstanceに保存する
		return imgInstance;
	}

	const int& SetID(std::string idName, std::string fName);
	const std::vector<int>& SetID(std::string idName, std::string fName, VECTOR2 divSize, VECTOR2 divCnt);
	void DrawImage(std::string idName, int x, int y, bool flag);
	void DrawImage(std::string idName, int x, int y, int animNo, bool flag);
};
