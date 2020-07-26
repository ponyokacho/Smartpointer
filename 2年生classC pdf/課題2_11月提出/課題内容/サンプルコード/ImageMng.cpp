#include <DxLib.h>
#include "ImageMng.h"

// ======================================================================================
// SetID
// @param1 : ��������idName
// @param2 : �o�^����̧�ٖ�
// ======================================================================================
const int& ImageMng::SetID(std::string id_name, std::string f_name)
{
	if(imgMap.find(id_name) == imgMap.end())
	{
		//imgMap[id_name].resize(1);	// �v�f����1�ɐݒ�
		imgMap[id_name] = LoadGraph(f_name.c_str());
	}
	return imgMap[id_name];	// LoadGraph�p��imgMap�ɓo�^����id��Ԃ�
}

// ======================================================================================
// SetID
// @id_name : ��������idName
// @f_name  : �o�^����̧�ٖ�
// @divSize : 1Ͻ�̻���
// @divCnt  : �c���̌�
//
// ======================================================================================
const std::vector<int>& ImageMng::SetID(std::string id_name, std::string f_name, VECTOR2 divSize, VECTOR2 divCnt)
{
	if(imgMap.find(id_name) == imgMap.end())
	{
		imgDivMap[id_name].resize(divCnt.x*divCnt.y);	// �v�f����divCnt.x*divCnt.y�ɐݒ�

		LoadDivGraph(f_name.c_str(),
				divCnt.x*divCnt.y,
				divCnt.x,
				divCnt.y,
				divSize.x,
				divSize.y,
				&imgDivMap[id_name][0], true);
	}
	return imgDivMap[id_name];	// LoadDivGraph�p��imgDivMap�ɓo�^����id��Ԃ�
}

// ======================================================================================
// DrawImage
// @idName  : �`�悷��idName
// @x       : �`�悷�鍶���X���W
// @y       : �`�悷�鍶���Y���W 
// @flag    : �����׸�
//
// ======================================================================================
void ImageMng::DrawImage(std::string id_name, int x, int y, bool flag)
{
	// LoadGraph�p��imgMap�ŏ�������

	// �װ�����@ �w�肵��ID�̓o�^�����������疳��
	if(imgMap.find(id_name) == imgMap.end()){
		return;
	}
	DrawGraph(x, y, imgMap[id_name], flag);	// LoadGraph�p��imgMap����`��
}

// ======================================================================================
// DrawImage ����Ҕ�
// @idName  : �`�悷��idName
// @x       : �`�悷�鍶���X���W
// @y       : �`�悷�鍶���Y���W
// @animNo  : �`�悷���Һϔԍ�
// @flag    : �����׸�
//
// ======================================================================================
void ImageMng::DrawImage(std::string id_name, int x, int y, int animNo, bool flag)
{
	// LoadDivGraph�p��imgDivMap�ŏ�������

	// �װ�����@ �w�肵��ID�̓o�^�����������疳��
	if (imgDivMap.find(id_name) == imgDivMap.end()) {
		return;
	}

	// �װ�����A ��Ҕԍ���0�ȉ��A�ő��Ґ��𒴂��Ă����疳��
	if(0 > animNo || imgDivMap[id_name].size() <= animNo){
		return;
	}

	DrawGraph(x, y, imgDivMap[id_name][animNo], flag);	// LoadDivGraph�p��imgDivMap����`��
}

ImageMng::ImageMng()
{
}

ImageMng::~ImageMng()
{
}
