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

	//加速度計
	status_.acceleration.x = 0.0f;
	status_.acceleration.y = 0.0f;
	status_.Velocity.x = 0.0f;
	status_.Velocity.y = +9.8f;
	status_.Speed = 5.0f;

	//スケール
	status_.scale.x = 64.0f;
	status_.scale.y = 64.0f;

	status_.isActive = true;
	status_.isJumop = false;
	status_.jumpPower = 20.0f;
	status_.radius = 64.0f;

	//幅高さ
	status_.height = 64.0f;
	status_.width = 64.0f;

	//自由に動けるかの確認
	status_.isMoveFree = true;
	status_.isCommandMove = true;

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

	isRightWall(mapData, BLOCK);
	isLeftWall(mapData, BLOCK);


	Gravity();//重力処理

	//下のタイルの座標系さんと当たり判定
	isGrounded(mapData, BLOCK);//通常の地面判定
	isGrounded(mapData, HALF_FLOOR);  // ★ハーフ床の地面もチェック！

	isTopWall(mapData, BLOCK);
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
	if (status_.isMoveFree) {
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
	}
	isRightWall(mapData, BLOCK);
	isRightWall(mapData, HALF_FLOOR);//ハーフブロック判定

	isLeftWall(mapData, BLOCK);
	isLeftWall(mapData, HALF_FLOOR);
	//いまは左から当たるときは同じ処理なので特に変化なし

	Gravity();

	//下のタイルの座標系さんと当たり判定
	if (status_.pos.y >= 1080 - status_.height) {
		status_.pos.y = 0;
	}

	isGrounded(mapData, BLOCK);
	isGrounded(mapData, HALF_FLOOR);  // ★ハーフ床の地面もチェック！


	isTopWall(mapData, BLOCK);
	isTopWall(mapData, HALF_FLOOR);//ハーフブロック判定

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

// --- 地面との当たり判定 ---
void Player::isGrounded(int mapData[kMapHeight][kMapWidth], int mapId) {
	float leftX = status_.pos.x;
	float rightX = status_.pos.x + status_.width;
	float bottomY = status_.pos.y + status_.height;

	// 通常のタイルインデックス（mapDataを参照するための番号）
	int tileLeftX = (int)(leftX / kTileSize);
	int tileRightX = (int)((rightX - 0.1f) / kTileSize);
	int tileBottomY = (int)((bottomY - 0.1f) / kTileSize);

	if (status_.Velocity.y > 0) {

		// --- 通常ブロック ---
		if (mapId == BLOCK) {
			if (mapData[tileBottomY][tileLeftX] == mapId || mapData[tileBottomY][tileRightX] == mapId) {
				status_.pos.y = (float)(tileBottomY * kTileSize) - status_.height;
				status_.Velocity.y = 0;
				status_.isJumop = false;
			}
		}

		// --- 縦に左半分のハーフブロック ---
		else if (mapId == HALF_FLOOR) {
			// チェック対象のタイル（左端の足元と右側の足元）をループで回す
			int checkTilesX[] = { tileLeftX, tileRightX };

			for (int tx : checkTilesX) {
				// そのタイルがハーフ床（ID:2）なら
				if (mapData[tileBottomY][tx] == mapId) {

					// そのタイルの左端と中央のピクセル座標を計算
					float tileLeftEdgePixel = (float)(tx * kTileSize);
					float tileCenterXPixel = tileLeftEdgePixel + (kTileSize / 2.0f);

					// プレイヤーの左右端が、タイルの「左半分」のピクセル範囲に重なっているか
					if (rightX > tileLeftEdgePixel && leftX < tileCenterXPixel) {

						float floorY = (float)(tileBottomY * kTileSize);
						if (status_.pos.y + status_.height <= floorY + status_.Velocity.y) {
							status_.pos.y = floorY - status_.height;
							status_.Velocity.y = 0;
							status_.isJumop = false;
							return; // 1つでも乗っていれば判定終了
						}
					}
				}
			}
		}
	}
}

// --- 右壁の当たり判定 ---
void Player::isRightWall(int mapData[kMapHeight][kMapWidth], int mapId) {
	float rightX = status_.pos.x + status_.width;
	float topY = status_.pos.y;
	float bottomY = status_.pos.y + status_.height;

	int tileRightX = (int)((rightX - 0.1f) / kTileSize);
	int tileTopY = (int)(topY / kTileSize);
	int tileBottomY = (int)((bottomY - 0.1f) / kTileSize);

	if (mapData[tileTopY][tileRightX] == mapId || mapData[tileBottomY][tileRightX] == mapId) {
		if (mapId == BLOCK) {
			status_.pos.x = (float)(tileRightX * kTileSize) - status_.width;
		}
		else if (mapId == HALF_FLOOR) {
			// 補正なし：普通のブロックと同じ位置で止まる
			status_.pos.x = (float)(tileRightX * kTileSize) - status_.width;
		}
	}
}

// --- 左壁の当たり判定 ---
void Player::isLeftWall(int mapData[kMapHeight][kMapWidth], int mapId) {
	float leftX = status_.pos.x;
	float topY = status_.pos.y;
	float bottomY = status_.pos.y + status_.height;

	int tileLeftX = (int)(leftX / kTileSize);
	int tileTopY = (int)(topY / kTileSize);
	int tileBottomY = (int)((bottomY - 0.1f) / kTileSize);

	if (mapData[tileTopY][tileLeftX] == mapId || mapData[tileBottomY][tileLeftX] == mapId) {
		if (mapId == BLOCK) {
			status_.pos.x = (float)((tileLeftX + 1) * kTileSize);
		}
		else if (mapId == HALF_FLOOR) {
			float tileCenterX = (float)(tileLeftX * kTileSize) + (kTileSize / 2.0f);

			// 【重要】もしプレイヤーの左端が、タイルの真ん中（実体の右端）より
			// 少しでも左に入り込んでしまったら
			if (leftX < tileCenterX) {
				// 強引に真ん中まで戻す
				status_.pos.x = tileCenterX;
				// もし速度があるなら、ここで止める（左への速度をゼロにする）
				if (status_.Velocity.x < 0) status_.Velocity.x = 0;
			}
		}
	}
}

// --- 天井の当たり判定 ---
void Player::isTopWall(int mapData[kMapHeight][kMapWidth], int mapId) {
	float leftX = status_.pos.x;
	float rightX = status_.pos.x + status_.width;
	float topY = status_.pos.y;

	int tileLeftX = (int)(leftX / kTileSize);
	int tileRightX = (int)((rightX - 0.1f) / kTileSize);
	int tileTopY = (int)(topY / kTileSize);

	if (status_.Velocity.y < 0) {
		// まず、そのタイルがあるかチェック
		if (mapData[tileTopY][tileLeftX] == mapId || mapData[tileTopY][tileRightX] == mapId) {

			if (mapId == BLOCK) {
				status_.pos.y = (float)((tileTopY + 1) * kTileSize);
				status_.Velocity.y = 0;
			}
			else if (mapId == HALF_FLOOR) {
				// 足元判定と同じように、左半分に重なっているかチェック
				// (txをループさせる必要はなく、どっちかの足が左半分にかかればぶつかる判定)
				bool isHit = false;
				int checkX[] = { tileLeftX, tileRightX };
				for (int tx : checkX) {
					if (mapData[tileTopY][tx] == mapId) {
						float tLeft = (float)(tx * kTileSize);
						float tCenter = tLeft + (kTileSize / 2.0f);
						if (rightX > tLeft && leftX < tCenter) isHit = true;
					}
				}

				if (isHit) {
					status_.pos.y = (float)((tileTopY + 1) * kTileSize);
					status_.Velocity.y = 0;
				}
			}
		}
	}
}


#pragma endregion


void Player::CheckRouter(Router* router[], int count) {
	// ★1. まずは「圏外」のデフォルト状態にする
	// 全てのルーターから外れているときは、動けないように設定
	status_.isMoveFree = false;
	status_.isCommandMove = false;

	for (int i = 0; i < count; i++) {
		// 未生成のルーターを飛ばすガード句
		if (router[i] == nullptr) continue;
		if (router[i]->router_.pos.x < -5000.0f) continue;

		// ★2. 座標の計算（プレイヤーの中心 vs ルーターの中心）
		// 矩形(左上)と円の判定だとズレるので、プレイヤーの中心座標を作る
		float playerCenterX = status_.pos.x + status_.width / 2.0f;
		float playerCenterY = status_.pos.y + status_.height / 2.0f;

		float dx = router[i]->router_.pos.x - playerCenterX;
		float dy = router[i]->router_.pos.y - playerCenterY;
		float distSq = dx * dx + dy * dy;

		float r = router[i]->router_.radius;
		float br = router[i]->router_.bigRadius;

		// ★3. 判定（内側の円）
		if (distSq <= r * r) {
			status_.isMoveFree = true;
			status_.isCommandMove = true;
			return; // 最高の状態なので、これ以上他のルーターは見なくて良い
		}
		// ★4. 判定（外側の円：内側には入っていないが外側には入っている）
		else if (distSq <= br * br) {
			status_.isMoveFree = false;
			status_.isCommandMove = true;
			// 他のルーターでもっと良い条件（内側）があるかもしれないので、
			// ここでは return せずにループを続けるか、
			// 「最低でもコマンド移動はできる」というフラグだけ立てておく
		}
	}
}
