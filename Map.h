#pragma once
#include "Const.h"
#include <vector>
#include <string>

// 一応最大ブロック種類の定義
const int kMaxBlocksType = 100;

class Map {
public:
	// マップデータ
	int mapData[kMapHeight][kMapWidth];

	// ブロックごとの画像を保存
	int blockTextures[kMaxBlocksType];

	// コンストラクタ
	Map();

	// 初期化
	void Initialize();

	// LDtk読み込み
	void LoadMapFromLDtk(const char* fileName,const std::vector<std::string>& layerName);

	// 描画
	void Draw();
};