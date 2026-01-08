#pragma once
#include "Const.h"

class Map {
public:
	// マップデータ（外部から参照できるようにpublicにする）
	int mapData[kMapHeight][kMapWidth];

	// コンストラクタ
	Map();

	// LDtk読み込み
	void LoadMapFromLDtk(const char* fileName);

	// 描画
	void Draw(int textureHandle);
};