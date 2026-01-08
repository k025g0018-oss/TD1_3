#include "Game.h"
#include <Novice.h>
#include "Map.h"

Game::Game() {
	player = new Player();
	map = new Map();
	Initialize();
}

Game::~Game() {
	delete player;
	delete map;
}

void Game::Initialize() {
	// エリア定義
	gameArea = { 0, 0, 1400, 1080 };
	paletteArea = { 1400, 0, 580, 400 };     // 右上：パレット
	programArea = { 1400, 400, 580, 680 };   // 右下：プログラム置き場

	isRunning = false;
	player->InitPlayer();
	commandList.clear();

	map->LoadMapFromLDtk("./mapTest9999.ldtk");


	// ★パレットエリアにボタンを配置
	float btnX = 1450;
	float btnW = 400;
	float btnH = 60;

	btnRight = { btnX, 50,  btnW, btnH, ">> Move Right",    CommandType::MoveRight,      0x44AAFFFF }; // 青系
	btnWallJump = { btnX, 130, btnW, btnH, "?? Wall -> Jump",  CommandType::CheckWallJump,  (int)0xFFAA44FF }; // オレンジ系
	btnCliffJump = { btnX, 210, btnW, btnH, "?? Cliff -> Jump", CommandType::CheckCliffJump, (int)0xFFAA44FF }; // オレンジ系

	// スタート・リセットボタン
	btnStart = { 1450, 300, 180, 80, "START >", (CommandType)-1, (int)0xFF4444FF };
	btnReset = { 1670, 300, 180, 80, "STOP []", (CommandType)-1, 0x44FF44FF };
}

void Game::Update(char keys[256], char preKeys[256]) {

	int mouseX, mouseY;
	Novice::GetMousePosition(&mouseX, &mouseY);
	bool isClick = Novice::IsTriggerMouse(0);

	// ==========================================
	// モード分岐
	// ==========================================
	if (isRunning) {
		// --- 実行モード ---
		player->UpdateByCommands(commandList, map->mapData);

		// ストップボタン判定
		if (isClick) {
			if (mouseX >= btnReset.x && mouseX <= btnReset.x + btnReset.w &&
				mouseY >= btnReset.y && mouseY <= btnReset.y + btnReset.h) {
				isRunning = false;
				player->InitPlayer();
			}
		}
	} else {
		// --- 編集モード ---
		player->UpdatePlayer(keys, preKeys, map->mapData);

		if (isClick) {
			// 1. パレットのボタンを押してコマンドを追加
			if (mouseX >= btnRight.x && mouseX <= btnRight.x + btnRight.w && mouseY >= btnRight.y && mouseY <= btnRight.y + btnRight.h) {
				commandList.push_back(btnRight.cmdType);
			}
			if (mouseX >= btnWallJump.x && mouseX <= btnWallJump.x + btnWallJump.w && mouseY >= btnWallJump.y && mouseY <= btnWallJump.y + btnWallJump.h) {
				commandList.push_back(btnWallJump.cmdType);
			}
			if (mouseX >= btnCliffJump.x && mouseX <= btnCliffJump.x + btnCliffJump.w && mouseY >= btnCliffJump.y && mouseY <= btnCliffJump.y + btnCliffJump.h) {
				commandList.push_back(btnCliffJump.cmdType);
			}

			// 2. スタートボタン
			if (mouseX >= btnStart.x && mouseX <= btnStart.x + btnStart.w && mouseY >= btnStart.y && mouseY <= btnStart.y + btnStart.h) {
				isRunning = true;
				player->InitPlayer();
			}

			// 3. プログラムエリアのブロックをクリックして削除
			// 下の方に表示されているブロックほど、リストの後ろの方にある
			float blockY = programArea.y + 50;
			for (int i = 0; i < commandList.size(); i++) {
				// ブロックの当たり判定
				if (mouseX >= 1450 && mouseX <= 1450 + 400 &&
					mouseY >= blockY && mouseY <= blockY + 50) {

					// このコマンドを削除する
					commandList.erase(commandList.begin() + i);
					break; // 削除したらループを抜ける（indexがずれるため）
				}
				blockY += 60; // 次のブロックの位置へ
			}
		}
	}
}

void Game::Draw() {
	// --- 背景 ---
	Novice::DrawBox((int)gameArea.x, (int)gameArea.y, (int)gameArea.w, (int)gameArea.h, 0.0f, 0x000000FF, kFillModeSolid);

	// パレットエリア背景
	Novice::DrawBox((int)paletteArea.x, (int)paletteArea.y, (int)paletteArea.w, (int)paletteArea.h, 0.0f, 0x333333FF, kFillModeSolid);
	// プログラムエリア背景
	Novice::DrawBox((int)programArea.x, (int)programArea.y, (int)programArea.w, (int)programArea.h, 0.0f, 0x222222FF, kFillModeSolid);

	// 区切り線
	Novice::DrawBox(1400, 398, 580, 4, 0.0f, WHITE, kFillModeSolid);


	// --- ゲーム画面 ---
	for (int y = 0; y < kMapHeight; y++) {
		for (int x = 0; x < kMapWidth; x++) {
			if (mapData[y][x] != 0) Novice::DrawBox(x * kTileSize, y * kTileSize, kTileSize, kTileSize, 0.0f, WHITE, kFillModeSolid);
		}
	}

	int Texture = Novice::LoadTexture("./block.png");
	map->Draw(Texture);

	player->DrawPlayer();


	// --- UIボタン描画 ---
	auto DrawBtn = [](Button& b) {
		Novice::DrawBox((int)b.x, (int)b.y, (int)b.w, (int)b.h, 0.0f, b.color, kFillModeSolid);
		Novice::ScreenPrintf((int)b.x + 20, (int)b.y + 20, b.label);
		};

	DrawBtn(btnRight);
	DrawBtn(btnWallJump);
	DrawBtn(btnCliffJump);
	DrawBtn(btnStart);
	DrawBtn(btnReset);

	// --- プログラムリストのブロック描画 ---
	Novice::ScreenPrintf(1420, 410, "--- YOUR PROGRAM (Click to Delete) ---");

	float blockY = programArea.y + 50;

	int currentIndex = -1;
	if (isRunning) {
		currentIndex = player->GetCurrentCommandIndex();
	}

	for (int i = 0; i < commandList.size(); i++) {
		unsigned int color = 0xFFFFFFFF;
		const char* text = "";

		// コマンドの種類によって色と文字を変える
		switch (commandList[i]) {
		case CommandType::MoveRight:
			color = 0x44AAFFFF; // 青
			text = "Move Right";
			break;
		case CommandType::CheckWallJump:
			color = 0xFFAA44FF; // オレンジ
			text = "If Wall -> Jump";
			break;
		case CommandType::CheckCliffJump:
			color = 0xFFAA44FF; // オレンジ
			text = "If Cliff -> Jump";
			break;
		}
		// もし今実行しているコマンドなら、色を「赤」に変えて目立たせる！
		if (i == currentIndex) {
			color = 0xFF0000FF; // 赤色
		}

		// ブロック描画
		Novice::DrawBox(1450, (int)blockY, 400, 50, 0.0f, color, kFillModeSolid);
		Novice::ScreenPrintf(1470, (int)blockY + 15, "%d: %s", i + 1, text);

		// さらに左側に矢印「▶」を出して強調する
		if (i == currentIndex) {
			Novice::ScreenPrintf(1420, (int)blockY + 15, "->");
			// 少しずらして枠っぽく描画する演出などを入れてもOK
			Novice::DrawBox(1445, (int)blockY - 5, 410, 60, 0.0f, RED, kFillModeWireFrame);
		}

		// つながりの矢印
		if (i < commandList.size() - 1) {
			Novice::DrawTriangle(1650, (int)blockY + 50, 1630, (int)blockY + 60, 1670, (int)blockY + 60, WHITE, kFillModeSolid);
		}

		blockY += 60; // 次の表示位置へ
	}

	// 実行中などのステータス表示
	if (isRunning) {
		Novice::ScreenPrintf(10, 10, "RUNNING...");
		// 画面全体に枠を表示して実行中っぽくする
		Novice::DrawBox(0, 0, 1400, 1080, 0.0f, 0xFF000044, kFillModeSolid);
	} else {
		Novice::ScreenPrintf(10, 10, "EDIT MODE");
	}

	int mx, my;
	Novice::GetMousePosition(&mx, &my);
	Novice::ScreenPrintf(0, 0, "Mouse: %d, %d", mx, my);
	Novice::ScreenPrintf(0, 20, "BtnRight X: %d", (int)btnRight.x);

}