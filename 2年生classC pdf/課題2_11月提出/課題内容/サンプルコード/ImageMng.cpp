#include <DxLib.h>
#include "ImageMng.h"

// ======================================================================================
// SetID
// @param1 : –½–¼‚·‚éidName
// @param2 : “o˜^‚·‚éÌ§²Ù–¼
// ======================================================================================
const int& ImageMng::SetID(std::string id_name, std::string f_name)
{
	if(imgMap.find(id_name) == imgMap.end())
	{
		//imgMap[id_name].resize(1);	// —v‘f”‚ğ1‚Éİ’è
		imgMap[id_name] = LoadGraph(f_name.c_str());
	}
	return imgMap[id_name];	// LoadGraph—p‚ÌimgMap‚É“o˜^‚µ‚Äid‚ğ•Ô‚·
}

// ======================================================================================
// SetID
// @id_name : –½–¼‚·‚éidName
// @f_name  : “o˜^‚·‚éÌ§²Ù–¼
// @divSize : 1Ï½‚Ì»²½Ş
// @divCnt  : c‰¡‚ÌŒÂ”
//
// ======================================================================================
const std::vector<int>& ImageMng::SetID(std::string id_name, std::string f_name, VECTOR2 divSize, VECTOR2 divCnt)
{
	if(imgMap.find(id_name) == imgMap.end())
	{
		imgDivMap[id_name].resize(divCnt.x*divCnt.y);	// —v‘f”‚ğdivCnt.x*divCnt.y‚Éİ’è

		LoadDivGraph(f_name.c_str(),
				divCnt.x*divCnt.y,
				divCnt.x,
				divCnt.y,
				divSize.x,
				divSize.y,
				&imgDivMap[id_name][0], true);
	}
	return imgDivMap[id_name];	// LoadDivGraph—p‚ÌimgDivMap‚É“o˜^‚µ‚Äid‚ğ•Ô‚·
}

// ======================================================================================
// DrawImage
// @idName  : •`‰æ‚·‚éidName
// @x       : •`‰æ‚·‚é¶ã‚ÌXÀ•W
// @y       : •`‰æ‚·‚é¶ã‚ÌYÀ•W 
// @flag    : “§‰ßÌ×¸Ş
//
// ======================================================================================
void ImageMng::DrawImage(std::string id_name, int x, int y, bool flag)
{
	// LoadGraph—p‚ÌimgMap‚Åˆ—‚·‚é

	// ´×°Áª¯¸‡@ w’è‚µ‚½ID‚Ì“o˜^‚ª–³‚©‚Á‚½‚ç–³‹
	if(imgMap.find(id_name) == imgMap.end()){
		return;
	}
	DrawGraph(x, y, imgMap[id_name], flag);	// LoadGraph—p‚ÌimgMap‚©‚ç•`‰æ
}

// ======================================================================================
// DrawImage ¦±ÆÒ”Å
// @idName  : •`‰æ‚·‚éidName
// @x       : •`‰æ‚·‚é¶ã‚ÌXÀ•W
// @y       : •`‰æ‚·‚é¶ã‚ÌYÀ•W
// @animNo  : •`‰æ‚·‚é±ÆÒºÏ”Ô†
// @flag    : “§‰ßÌ×¸Ş
//
// ======================================================================================
void ImageMng::DrawImage(std::string id_name, int x, int y, int animNo, bool flag)
{
	// LoadDivGraph—p‚ÌimgDivMap‚Åˆ—‚·‚é

	// ´×°Áª¯¸‡@ w’è‚µ‚½ID‚Ì“o˜^‚ª–³‚©‚Á‚½‚ç–³‹
	if (imgDivMap.find(id_name) == imgDivMap.end()) {
		return;
	}

	// ´×°Áª¯¸‡A ±ÆÒ”Ô†‚ª0ˆÈ‰ºAÅ‘å±ÆÒ”‚ğ’´‚¦‚Ä‚¢‚½‚ç–³‹
	if(0 > animNo || imgDivMap[id_name].size() <= animNo){
		return;
	}

	DrawGraph(x, y, imgDivMap[id_name][animNo], flag);	// LoadDivGraph—p‚ÌimgDivMap‚©‚ç•`‰æ
}

ImageMng::ImageMng()
{
}

ImageMng::~ImageMng()
{
}
