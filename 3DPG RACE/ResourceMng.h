#pragma once
#include <string>
#include <map>
#include <vector>

#define IMAGE_ID(X) ResourceMng::GetInstance().SetImage(X)
#define DIV_IMAGE_ID(NAME,ALLNUM,SEP_X,SEP_Y,SRC_X,SRC_Y,WIDTH,HEIGHT,ARRAY) ResourceMng::GetInstance().SetDivImage(NAME,ALLNUM,SEP_X,SEP_Y,SRC_X,SRC_Y,WIDTH,HEIGHT,ARRAY)		//@DIV_IMAGE_ID(Ì§²Ù¼,Sª,XÌª,YÌª,¶ãÀWX,¶ãÀWY,æÌ¡,æÌc,i[zñ)
#define SOUND_ID(X) ResourceMng::GetInstance().SetSound(X)

using namespace std;

class ResourceMng
{
public:
	static ResourceMng& GetInstance() {
		static ResourceMng resInst;
		return resInst;
	}
	const int& SetImage(string);		// æo^AIDðÔ·
	const int* SetDivImage(string imagePath, size_t allnum, 
		int sepNumX, int sepNumY, int srcX, int srcY, int width,
		int height, int ary[]);		// ¡æo^AIDðÔ·
	const int& SetSound(string);		// ¹o^AIDðÔ·
private:
	map<string,int>mp;
	map<string, int>mpDiv;
	vector<int>v;
};

