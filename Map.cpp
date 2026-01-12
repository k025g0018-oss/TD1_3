#include "Map.h"
#include <Novice.h>
#include <fstream>
#include "json.hpp" 
#include "Vector2.h"

using json = nlohmann::json;

Map::Map() {
	// 初期化（全部0にする）
	for (int y = 0; y < kMapHeight; y++) {
		for (int x = 0; x < kMapWidth; x++) {
			mapData[y][x] = 0;
		}
	}

	// 画像配列の初期化
	for (int i = 0;i < kMaxBlocksType;i++) {
		blockTextures[i] = 0;
	}
}

void Map::Initialize() {
	/*--------------------------
	ここでブロックの画像を貼ってね
	-----------------------------*/
	blockTextures[1] = Novice::LoadTexture("./block.png");
	blockTextures[2] = Novice::LoadTexture("./halfBlock.png");// ハーフブロック用
	blockTextures[3] = Novice::LoadTexture("./halfBlock.png");// ルーター用

}

// LDtk読み込み
void Map::LoadMapFromLDtk(const char* fileName, const std::vector<std::string>& layerNames) {
	std::ifstream file(fileName);
	if (!file.is_open()) return;

	json data;
	file >> data;

	// いったんマップをクリア
	for (int y = 0; y < kMapHeight; y++) {
		for (int x = 0; x < kMapWidth; x++) {
			mapData[y][x] = 0;
		}
	}

	auto& level = data["levels"][0];
	auto& layers = level["layerInstances"];

	// LDtkの全レイヤーをチェック
	for (auto& layer : layers) {
		std::string currentLayerName = layer["__identifier"];

		// 読み込みたいレイヤー名リストの中に、今のレイヤー名が含まれているか確認
		bool isTargetLayer = false;
		for (const std::string& target : layerNames) {
			if (currentLayerName == target) {
				isTargetLayer = true;
				break;
			}
		}

		// 対象のレイヤーならデータを読み込む
		if (isTargetLayer) {
			auto& csvData = layer["intGridCsv"];
			for (int i = 0; i < csvData.size(); i++) {
				int id = csvData[i];

				// 0(空気)じゃない場合のみ書き込む
				// これにより、「Ground」の上に「Item」が重なっていても消えずに合成されます
				if (id != 0) {
					int x = i % kMapWidth;
					int y = i / kMapWidth;
					if (y < kMapHeight && x < kMapWidth) {
						mapData[y][x] = id;
					}
				}
			}
		}
	}
}

void Map::Draw(Vector2 offset) {
	for (int y = 0; y < kMapHeight; y++) {
		for (int x = 0; x < kMapWidth; x++) {

			int id = mapData[y][x];
			if (id > 0 && id < kMaxBlocksType && blockTextures[id] != 0) {

				Novice::DrawSprite(
					(int)(x * kTileSize - offset.x), (int)(y * kTileSize - offset.y),
					blockTextures[id], 
					1.0f, 1.0f, 0.0f, 0xFFFFFFFF
				);
			}

			//ルーターのデバック表示
			if (mapData[y][x] == 3) {
			
				Novice::DrawBox(

					(int)(x * kTileSize - offset.x), (int)(y * kTileSize - offset.y),
					kTileSize, kTileSize,
					0.0f,
					RED,
					kFillModeSolid

				);

			}

		}

		// Map.cpp の Draw内に追加してデバッグ
		Novice::ScreenPrintf(0, 100, "MapData[10][10]: %d", mapData[10][10]);
	}
}