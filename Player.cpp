#include "Player.h"
#include "Novice.h"
#include "Map.h"
#include "const.h"

Player::Player() {
	InitPlayer();
}

void Player::InitPlayer() {
	status_.pos.x = 300.0f;
	status_.pos.y = 300.0f;
	status_.acceleration.x = 0.0f;
	status_.acceleration.y = 0.0f;
	status_.Velocity.x = 0.0f;
	status_.Velocity.y = +9.8f;
	status_.Speed = 5.0f;
	status_.scale.x = 64.0f;
	status_.scale.y = 64.0f;
	status_.isActive = true;
	status_.isJumop = false;
	status_.jumpPower = 13.0f;
	status_.radius = 64.0f;

	status_.height = 64.0f;
	status_.width = 64.0f;

}

void Player::UpdatePlayer(char keys[256], char preKeys[256], int  mapData[kMapHeight][kMapWidth]) {

	if (status_.isActive) {
		MovePlayer(keys, preKeys, mapData);
	}
}

void Player::DrawPlayer() {
	Novice::DrawBox(static_cast<int>(status_.pos.x), static_cast<int>(status_.pos.y),
		static_cast<int>(status_.scale.x), static_cast<int>(status_.scale.y),
		0.0f, WHITE, kFillModeSolid);
}

//------------------------------------------------------------------------------------------------------
//プライベート関数など
//------------------------------------------------------------------------------------------------------


void Player::MovePlayer(char keys[256], char preKeys[256],
	int mapData[kMapHeight][kMapWidth]) {

	// ジャンプ（押した瞬間）
	if (!status_.isJumop) {
		if (keys[DIK_SPACE] && !preKeys[DIK_SPACE]) {
			status_.isJumop = true;
			status_.Velocity.y = -status_.jumpPower;
		}
	}
	

	// --- 左右移動の処理 ---
	if (keys[DIK_D]) {
		status_.pos.x += status_.Speed;
	}
	if (keys[DIK_A]) {
		status_.pos.x -= status_.Speed;
	}

	// 左右の当たり判定と補正
	float leftX = status_.pos.x;
	float rightX = status_.pos.x + status_.width;
	float topY = status_.pos.y;
	float bottomY = status_.pos.y + status_.height;

	int tileLeftX = (int)(leftX / kTileSize);
	int tileRightX = (int)((rightX - 0.1f) / kTileSize); // 0.1f引いて右端ギリギリを判定
	int tileTopY = (int)(topY / kTileSize);
	int tileBottomY = (int)((bottomY - 0.1f) / kTileSize);


#pragma region 右のタイルの当たり判定と補正]

	// 右壁の判定
	// 右壁の判定
	if (keys[DIK_D]) {
		// 右上の点か右下の点が壁なら
		if (mapData[tileTopY][tileRightX] != 0 || mapData[tileBottomY][tileRightX] != 0) {
			status_.pos.x = (float)(tileRightX * kTileSize) - status_.width;
		}
	}
	// 左壁の判定
	if (keys[DIK_A]) {
		// 左上の点か左下の点が壁なら
		if (mapData[tileTopY][tileLeftX] != 0 || mapData[tileBottomY][tileLeftX] != 0) {
			status_.pos.x = (float)(tileLeftX + 1) * kTileSize;
		}
	}

#pragma endregion
	Gravity();
	//下のタイルの座標系さんと当たり判定
#pragma region 下のタイルの当たり判定と補正
// 上下の当たり判定と補正（最新のX座標を使って再計算）
	leftX = status_.pos.x;
	rightX = status_.pos.x + status_.width;
	topY = status_.pos.y;
	bottomY = status_.pos.y + status_.height;

	tileLeftX = (int)(leftX / kTileSize);
	tileRightX = (int)((rightX - 0.1f) / kTileSize);
	tileTopY = (int)(topY / kTileSize);
	tileBottomY = (int)((bottomY - 0.1f) / kTileSize);

	// 下方向（床）の判定
	if (status_.Velocity.y > 0) {
		if (mapData[tileBottomY][tileLeftX] != 0 || mapData[tileBottomY][tileRightX] != 0) {
			status_.pos.y = (float)(tileBottomY * kTileSize) - status_.height;
			status_.Velocity.y = 0.0f;
			status_.isJumop = false;
		}
	}

	if (status_.pos.y >= 1080 - status_.height) {
		status_.pos.y = 0;
	}

#pragma endregion



}

void Player::Gravity() {

	status_.Velocity.y += 0.38f;
	status_.pos.y += status_.Velocity.y;
}