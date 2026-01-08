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
    float footY = status_.pos.y + status_.height;
    float leftX = status_.pos.x + 2;
    float rightX = status_.pos.x + status_.width - 2;

    int tileY = (int)(footY / kTileSize);
    int tileLeftX = (int)(leftX / kTileSize);
    int tileRightX = (int)(rightX / kTileSize);

    // 配列範囲チェック
    if (tileY >= 0 && tileY < kMapHeight &&
        tileLeftX >= 0 && tileRightX < kMapWidth) {

        // 足元にブロックがあるか？
        if (mapData[tileY][tileLeftX] != 0 ||
            mapData[tileY][tileRightX] != 0) {

            // 地面の上に補正
            status_.pos.y = tileY * kTileSize - status_.height;
            status_.Velocity.y = 0.0f;
            status_.isJumop = false;
        }
    }
}

void Player::Gravity() {

	status_.Velocity.y += 0.38f;
	status_.pos.y += status_.Velocity.y;
}