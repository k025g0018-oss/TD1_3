#pragma once
#include "Vector2.h"

struct GameScreen {
	Vector2 position;
	Vector2 size;
};

struct CommandScreen {
	Vector2 position;
	Vector2 size;
};

class Game {
public:
	// screen
	// ゲーム画面とコマンド設定画面を分ける
	// ゲーム(1400*1080),コマンド(580*1080),計1980*1080
	GameScreen gameScreen;
	CommandScreen commandScreen;

	Game();  // コンストラクタの宣言
	~Game(); // デストラクタの宣言
};

