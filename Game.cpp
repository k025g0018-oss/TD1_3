#include "Game.h"
#include <Novice.h>
#include "Player.h"
#include "ScrollCamera.h"

// コンストラクタ
Game::Game() {
	// ゲーム画面
	gameScreen.position = {0.0f, 0.0f};
	gameScreen.size = {1400.0f, 1080.0f};
	// コマンド画面
	commandScreen.position = {1400.0f, 0.0f};
	commandScreen.size = {580.0f, 1080.0f};
}

// デストラクタ
Game::~Game() {
	// 特に解放するリソースはない
}

// 更新
void Game::Update() {
	// プレイヤーの座標をカメラに渡す

}

// 描画
void Game::Draw() {
	// ゲーム画面
	Novice::DrawBox(
		static_cast<int>(gameScreen.position.x),
		static_cast<int>(gameScreen.position.y),
		static_cast<int>(gameScreen.size.x),
		static_cast<int>(gameScreen.size.y),
		0.0f,
		0x000000FF,
		kFillModeSolid
	);


	// コマンド画面
	Novice::DrawBox(
		static_cast<int>(commandScreen.position.x),
		static_cast<int>(commandScreen.position.y),
		static_cast<int>(commandScreen.size.x),
		static_cast<int>(commandScreen.size.y),
		0.0f,
		0x00FF00FF,
		kFillModeSolid
	);
}