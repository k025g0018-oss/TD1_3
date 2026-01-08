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
	status_.jumpPower=10.0f;
	status_.radius = 64.0f;

	status_.height = 64.0f;
	status_.width = 64.0f;

}

void Player::UpdatePlayer(char keys[256], char preKeys[256], int  mapData[kMapHeight][kMapWidth]){

	if (status_.isActive) {
		MovePlayer(keys, preKeys,mapData);
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

    // 左右移動
    if (keys[DIK_A]) {
        status_.pos.x -= status_.Speed;
    }
    if (keys[DIK_D]) {
        status_.pos.x += status_.Speed;
    }

    // ジャンプ（押した瞬間）
    if (!status_.isJumop) {
        if (keys[DIK_SPACE] && !preKeys[DIK_SPACE]) {
            status_.isJumop = true;
            status_.Velocity.y = -status_.jumpPower;
        }
    }

    // 重力
    status_.Velocity.y += 0.38f;
    status_.pos.y += status_.Velocity.y;

    // ===== 床当たり判定 =====
    float footY = status_.pos.y + status_.height-0.5f;
    float leftX = status_.pos.x ;
    float rightX = status_.pos.x + status_.width;
    float topY = status_.pos.y;

    int tileBottomY = (int)(footY / kTileSize);
    int tileLeftX = (int)(leftX / kTileSize);
    int tileRightX = (int)(rightX / kTileSize);
    int tileTopY = (int)(topY / kTileSize);


    //下のタイルの座標系さんと当たり判定
#pragma region 下のタイルの当たり判定と補正
    // 配列範囲チェック
    if (tileBottomY >= 0 && tileBottomY < kMapHeight &&
        tileLeftX >= 0 && tileRightX < kMapWidth) {

        // 足元にブロックがあるか？
        if (mapData[tileBottomY][tileLeftX] != 0 ||
            mapData[tileBottomY][tileRightX] != 0) {

            // 地面の上に補正
            status_.pos.y = tileBottomY * kTileSize - status_.height;
            status_.Velocity.y = 0.0f;
            status_.isJumop = false;
        }
    }

#pragma endregion
    
    //右のタイルの座標系さんと当たり判定
#pragma region 右のタイルの当たり判定と補正
    // 配列範囲チェック
    if (tileBottomY >= 0 && tileBottomY < kMapHeight &&
        tileLeftX >= 0 && tileRightX < kMapWidth) {

        // 足元にブロックがあるか？
        if (mapData[tileTopY][tileRightX] != 0 ||
            mapData[tileBottomY][tileRightX] != 0) {

            int rightIndex = (int)(status_.pos.x / kTileSize);
            float left = (float)(rightIndex + 1) * kTileSize;

            status_.pos.x = left - status_.radius + 0.01f;
        }
    }

#pragma endregion

#pragma region 左のタイルの当たり判定と補正
    // 配列範囲チェック
    if (tileBottomY >= 0 && tileBottomY < kMapHeight &&
        tileLeftX >= 0 && tileRightX < kMapWidth) {

        // 足元にブロックがあるか？
        if (mapData[tileTopY][tileLeftX] != 0 ||
            mapData[tileBottomY][tileLeftX] != 0) {

            int leftIndex = (int)(status_.pos.x / kTileSize);
            float right = (float)leftIndex * kTileSize;
            // 地面の上に補正
            status_.pos.x = right + status_.radius;
        }
    }

#pragma endregion

}

void Player::Gravity() {

	status_.Velocity.y += 0.38f;
	status_.pos.y += status_.Velocity.y;
}