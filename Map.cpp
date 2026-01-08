#include "Map.h"
#include <Novice.h>
#include <fstream>
#include "json.hpp" 

using json = nlohmann::json;

Map::Map() {
	// 初期化（全部0にする）
	for (int y = 0; y < kMapHeight; y++) {
		for (int x = 0; x < kMapWidth; x++) {
			mapData[y][x] = 0;
		}
	}
}

void Map::LoadMapFromLDtk(const char* fileName) {
	std::ifstream file(fileName);
	if (!file.is_open()) {
		Novice::ScreenPrintf(0, 0, "Error: File not found.");
		return;
	}
	json data;
	file >> data;

	auto& level = data["levels"][0];
	auto& layers = level["layerInstances"];

	for (auto& layer : layers) {
		if (layer["__identifier"] == "IntGrid") {
			auto& csvData = layer["intGridCsv"];
			for (int i = 0; i < csvData.size(); i++) {
				int x = i % kMapWidth;
				int y = i / kMapWidth;
				if (y < kMapHeight && x < kMapWidth) {
					mapData[y][x] = csvData[i];
				}
			}
			break;
		}
	}
}

void Map::Draw(int textureHandle) {
	for (int y = 0; y < kMapHeight; y++) {
		for (int x = 0; x < kMapWidth; x++) {
			if (mapData[y][x] != 0) {
				Novice::DrawSprite(
					x * kTileSize, y * kTileSize,
					textureHandle, 1.0f, 1.0f, 0.0f, 0xFFFFFFFF
				);
			}
		}
	}
}