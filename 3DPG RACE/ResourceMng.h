#pragma once
#include <string>
#include <map>
#include <vector>

#define IMAGE_ID(X) ResourceMng::GetInstance().SetImage(X)
#define DIV_IMAGE_ID(NAME,ALLNUM,SEP_X,SEP_Y,SRC_X,SRC_Y,WIDTH,HEIGHT,ARRAY) ResourceMng::GetInstance().SetDivImage(NAME,ALLNUM,SEP_X,SEP_Y,SRC_X,SRC_Y,WIDTH,HEIGHT,ARRAY)		//�@DIV_IMAGE_ID(̧�ٖ�,�S������,X�̕�����,Y�̕�����,������WX,������WY,�摜�̉���,�摜�̏c��,�i�[�z��)
#define SOUND_ID(X) ResourceMng::GetInstance().SetSound(X)

using namespace std;

class ResourceMng
{
public:
	static ResourceMng& GetInstance() {
		static ResourceMng resInst;
		return resInst;
	}
	const int& SetImage(string);		// �摜�o�^�AID��Ԃ�
	const int* SetDivImage(string imagePath, size_t allnum, 
		int sepNumX, int sepNumY, int srcX, int srcY, int width,
		int height, int ary[]);		// ���摜�o�^�AID��Ԃ�
	const int& SetSound(string);		// ���o�^�AID��Ԃ�
private:
	map<string,int>mp;
	map<string, int>mpDiv;
	vector<int>v;
};

