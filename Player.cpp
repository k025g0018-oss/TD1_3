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
	status_.jumpPower = 20.0f;
	status_.radius = 64.0f;

	status_.height = 64.0f;
	status_.width = 64.0f;

	cmdIndex = 0;

}

void Player::UpdatePlayer(char keys[256], char preKeys[256], int  mapData[kMapHeight][kMapWidth]) {

	if (status_.isActive) {
		MovePlayer(keys, preKeys, mapData);
	}
}

// コマンドで動かせるプレイヤー
void Player::UpdateByCommands(const std::vector<CommandType>& commands, int mapData[kMapHeight][kMapWidth]) {
	if (!status_.isActive) return;

	// まだ実行すべきコマンドが残っているか？
	if (cmdIndex < commands.size()) {

		CommandType currentCmd = commands[cmdIndex];

		switch (currentCmd) {

			// ---------------------------------------------------
			// 右へ進む：
			// 「右へ動く力」を与えたら、即座に次のコマンドへ進む。
			// 力は残り続けるので、結果として「ずっと右へ」になる。
			// ---------------------------------------------------
		case CommandType::MoveRight:
			status_.pos.x += status_.Speed;

			cmdIndex++;

			break;

			// ---------------------------------------------------
			// 壁があったらジャンプ：
			// 「壁が来るまで待機」する。
			// ---------------------------------------------------
		case CommandType::CheckWallJump:
			status_.pos.x += status_.Speed;

			// 壁があったらジャンプ
			if (!status_.isJumop) {
				if (IsWallAhead(mapData)) {
					ActionTryJump();
					cmdIndex++;
				}
			}
			break;

			// ---------------------------------------------------
			// 崖があったらジャンプ：
			// 「崖が来るまで待機」する。
			// ---------------------------------------------------
		case CommandType::CheckCliffJump:
			status_.pos.x += status_.Speed;

			// ★重要修正：地面にいるときだけチェックする！
			if (!status_.isJumop) {
				if (IsCliffAhead(mapData)) {
					ActionTryJump();
					cmdIndex++; // ジャンプしたので役目終了、次へ
				}
			}
			break;
		}
	}
	else {
		// コマンドがなくなった後
		status_.pos.x += status_.Speed;
	}

	isRightWall(mapData);
	isLeftWall(mapData);


	Gravity();//重力処理

	//下のタイルの座標系さんと当たり判定
	isGrounded(mapData);
	isTopWall(mapData);
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

	isRightWall(mapData);
	isLeftWall(mapData);
	Gravity();
	//下のタイルの座標系さんと当たり判定
	if (status_.pos.y >= 1080 - status_.height) {
		status_.pos.y = 0;
	}
	isGrounded(mapData);
	isTopWall(mapData);

}

void Player::Gravity() {

	status_.Velocity.y += 0.98f;
	status_.pos.y += status_.Velocity.y;
}

/*--------------------
コマンドでうごかす処理
---------------------*/

void Player::ActionMoveRight() {
	status_.pos.x += status_.Speed;
}

void Player::ActionTryJump() {
	if (!status_.isJumop) {
		status_.isJumop = true;
		status_.Velocity.y = -status_.jumpPower;
	}
}

// 前に壁があるかチェック
bool Player::IsWallAhead(int mapData[kMapHeight][kMapWidth]) {
	// 自分の右端 + 5ピクセル先を見る
	int checkX = (int)(status_.pos.x + status_.width + 5.0f) / kTileSize;
	int checkY = (int)(status_.pos.y + status_.height / 2.0f) / kTileSize;

	if (checkX >= 0 && checkX < kMapWidth && checkY >= 0 && checkY < kMapHeight) {
		if (mapData[checkY][checkX] != 0) return true; // 壁あり
	}
	return false;
}


// 足元が崖かチェック
bool Player::IsCliffAhead(int mapData[kMapHeight][kMapWidth]) {
	// 自分の右端 + 5ピクセル先を見る
	int checkX = (int)(status_.pos.x + status_.width + 5.0f) / kTileSize;
	// 足元 + 5ピクセル下を見る
	int checkY = (int)(status_.pos.y + status_.height + 5.0f) / kTileSize;

	if (checkX >= 0 && checkX < kMapWidth && checkY >= 0 && checkY < kMapHeight) {
		if (mapData[checkY][checkX] == 0) return true; // 0なら崖
	}
	return false;
}

//マップチップの当たり判定関数
#pragma region マップの当たり判定関数




void Player::isGrounded(int mapData[kMapHeight][kMapWidth]) {
	//足元にマップがあるかの確認
	// 左右の当たり判定と補正
	float leftX = status_.pos.x;
	float rightX = status_.pos.x + status_.width;
	float bottomY = status_.pos.y + status_.height;

	int tileLeftX = (int)(leftX / kTileSize);
	int tileRightX = (int)((rightX - 0.1f) / kTileSize); // 0.1f引いて右端ギリギリを判定
	int tileBottomY = (int)((bottomY - 0.1f) / kTileSize);

	// 下方向（床）の判定
	if (status_.Velocity.y > 0) {
		if (mapData[tileBottomY][tileLeftX] != 0 || mapData[tileBottomY][tileRightX] != 0) {
			status_.pos.y = (float)(tileBottomY * kTileSize) - status_.height;
			status_.Velocity.y = 0;
			status_.isJumop = false;
		}
	}
}

//右壁の当たり判定
void Player::isRightWall(int mapData[kMapHeight][kMapWidth]) {
	float rightX = status_.pos.x + status_.width;
	float topY = status_.pos.y;
	float bottomY = status_.pos.y + status_.height;

	// 座標から右側にあるタイル番号を算出
	int tileRightX = (int)((rightX - 0.1f) / kTileSize);
	int tileTopY = (int)(topY / kTileSize);
	int tileBottomY = (int)((bottomY - 0.1f) / kTileSize);

	// 壁（0以外）に当たっていたら
	if (mapData[tileTopY][tileRightX] != 0 || mapData[tileBottomY][tileRightX] != 0) {
		// 【修正点】
		// tileRightX * kTileSize は「壁の左端」の座標です。
		// そこから「プレイヤーの横幅」を引いた位置が、正しい停止位置になります。
		status_.pos.x = (float)(tileRightX * kTileSize) - status_.width;
	}
}
//左壁の当たり判定
void Player::isLeftWall(int mapData[kMapHeight][kMapWidth]) {
	float leftX = status_.pos.x;
	float topY = status_.pos.y;
	float bottomY = status_.pos.y + status_.height;

	int tileLeftX = (int)(leftX / kTileSize);
	int tileTopY = (int)(topY / kTileSize);
	int tileBottomY = (int)((bottomY - 0.1f) / kTileSize);

	if (mapData[tileTopY][tileLeftX] != 0 || mapData[tileBottomY][tileLeftX] != 0) {
		// 【修正点】左に押し出すのではなく、タイルの右側（+1）へ押し戻す
		status_.pos.x = (float)((tileLeftX + 1) * kTileSize);
	}
}

void Player::isTopWall(int mapData[kMapHeight][kMapWidth]) {

	// プレイヤーの左右端と上端の座標
	float leftX = status_.pos.x;
	float rightX = status_.pos.x + status_.width;
	float topY = status_.pos.y;

	// 座標からタイル番号を算出
	int tileLeftX = (int)(leftX / kTileSize);
	int tileRightX = (int)((rightX - 0.1f) / kTileSize); // 右端ギリギリを判定
	int tileTopY = (int)(topY / kTileSize);

	// 上方向（頭上）の判定：上昇中（Velocity.y < 0）のみチェック
	if (status_.Velocity.y < 0) {
		// 頭上の左端または右端にタイル（0以外）があるか
		if (mapData[tileTopY][tileLeftX] != 0 || mapData[tileTopY][tileRightX] != 0) {

			// 【修正】位置をタイルの「下端」に押し戻す
			// (tileTopY + 1) * kTileSize は、衝突したタイルの下のラインの座標
			status_.pos.y = (float)((tileTopY + 1) * kTileSize);

			// 上昇速度をゼロにする（頭をぶつけて止まる）
			status_.Velocity.y = 0;
		}
	}
}


#pragma endregion
