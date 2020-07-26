#pragma once
#include <vector>
#include <map>
#include "VECTOR2.h"

// ----- �ݸ���ݸ׽��KeyCheck�׽���`����
class ImageMng
{
private:
	// ----- �ݽ�׸��Q
	ImageMng();		// ��̫�ĺݽ�׸���private�ɂ��ĊO�����琶���ł��Ȃ��l�ɂ���
	ImageMng(const ImageMng&){}					// ----- ��߰�ݽ�׸���private��
	ImageMng& operator=(const ImageMng&){}		// ----- ������Z�q�̵��ްײ�ނ�private��
	~ImageMng();	// �޽�׸�

	// �摜�o�^�pmap�z��
	// std::map<���̌^,�A�z�z��̌^> �A�z�z��;
	std::map<std::string, int> imgMap;
	std::map<std::string, std::vector<int>> imgDivMap;

public:
	// ----- ImageMng��޼ު�Ă̎��̂�Ԃ�(�ݸ����)
	static ImageMng& GetInstance(){
		static ImageMng imgInstance;	// ImageMng�̎��̂𐶐��BimgInstance�ɕۑ�����
		return imgInstance;
	}

	const int& SetID(std::string idName, std::string fName);
	const std::vector<int>& SetID(std::string idName, std::string fName, VECTOR2 divSize, VECTOR2 divCnt);
	void DrawImage(std::string idName, int x, int y, bool flag);
	void DrawImage(std::string idName, int x, int y, int animNo, bool flag);
};
