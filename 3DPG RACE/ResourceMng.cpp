#include "DxLib.h"
#include "ResourceMng.h"
#include <assert.h>

const int& ResourceMng::SetImage(string imagePath)
{
	// 画像名検索
	if (mp.find(imagePath) == mp.end())
	{
		mp[imagePath] = LoadGraph(imagePath.c_str());
	}
	return mp[imagePath];
}

const int* ResourceMng::SetDivImage(string imagePath, size_t allnum, int sepNumX, int sepNumY, int srcX, int srcY, int width, int height, int ary[])
{
	int count = 0;

	if (mp.find(imagePath) == mp.end())
	{
		mp[imagePath] = LoadGraph(imagePath.c_str());

		for (int i = 0; i < sepNumY; i++)
		{
			for (int j = 0; j < sepNumX; j++)
			{
				// パス名のあとに数字を付けて管理(0スタート)
				mpDiv[imagePath + std::to_string(count)] = DerivationGraph(srcX + (j * width), srcY + (i * height), width, height, mp[imagePath]);
				count++;
			}

			if (allnum + 1 == count)
			{
				break;
			}
		}
		for (size_t i = 0; i < allnum; i++)
		{
			ary[i] = mpDiv[imagePath + std::to_string(i)];
		}
		return ary;
	}
	else/* if(mpDiv.find(imagePath + std::to_string(0)) != mp.end())*/	// RunTime Error
	{
		v.clear();
		for (size_t i = 0; i < allnum; i++)
		{
			v.push_back(mpDiv[imagePath + std::to_string(i)]);
			ary[i] = v[i];
		}
		return ary;
	}

	return nullptr;
}

const int & ResourceMng::SetSound(string soundPath)
{
	// 音名検索
	if (mp.find(soundPath) == mp.end())
	{
		mp[soundPath] = LoadSoundMem(soundPath.c_str());
	}
	return mp[soundPath];
}
