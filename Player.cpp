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

			// 移動コマンドは「実行したら終わり」なので次へ
			cmdIndex++;

			// ★ポイント：
			// もし「MoveRight」の直後に「CheckWall」がある場合、
			// 同じフレーム内で即座にチェックを開始したいので、
			// ここで再帰呼び出し（またはループ）してもいいですが、
			// 今回はシンプルに「次のフレームから次のコマンドを実行」にします。
			break;

			// ---------------------------------------------------
			// 壁があったらジャンプ：
			// 「壁が来るまで待機」する。
			// ---------------------------------------------------
		case CommandType::CheckWallJump:
			// とりあえず右には進み続ける（MoveRightの効果を持続させるため）
			// ※もしMoveRightブロックなしでも進ませたいならここでも pos.x += speed する
			status_.pos.x += status_.Speed;

			// 壁チェック
			if (IsWallAhead(mapData)) {
				// 壁があった！ -> ジャンプ実行
				ActionTryJump();

				// このブロックの役目は終わったので次へ進む
				// （これで「一回だけ」が実現できます）
				cmdIndex++;
			} else {
				// 壁がない -> まだこのブロックにとどまる
				// cmdIndex を増やさないことで、次のフレームもこのチェックを行います。
			}
			break;

			// ---------------------------------------------------
			// 崖があったらジャンプ：
			// 「崖が来るまで待機」する。
			// ---------------------------------------------------
		case CommandType::CheckCliffJump:
			status_.pos.x += status_.Speed;

			if (IsCliffAhead(mapData)) {
				ActionTryJump();
				cmdIndex++; // 役目を終えたので次へ
			}
			break;
		}
	} else {
		// コマンドリストが全部終わった後の挙動
		// 例：そのまま右に進み続けるなら
		status_.pos.x += status_.Speed;
	}

	// 物理演算（重力）
	// 物理演算があるならここで
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