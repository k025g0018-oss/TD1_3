#include "Game.h"
#include <Novice.h>

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