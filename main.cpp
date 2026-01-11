#include <Novice.h>
#define _USE_MATH_DEFINES
#include <assert.h>
#include "Map.h"
#include <math.h>
#include "Player.h"
#include "Game.h"
// (^▽^)/あ


#include "Vector2.h"
#include "SceneManager.h"
#include "Game.h"
#include "Router.h"

// (^▽^)/あ
// (^▽^)/あ

const char kWindowTitle[] = "TD1_3";

//////////
/// 構造体
//////////

// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(_In_ HINSTANCE, _In_opt_ HINSTANCE, _In_ LPSTR, _In_ int) {

	//////////
	/// 変数の宣言
	//////////
	
	// 画面
	Vector2 screenSize;
	screenSize.x = 1980.0f;
	screenSize.y = 1080.0f;

	// ライブラリの初期化
	Novice::Initialize(kWindowTitle, static_cast<int>(screenSize.x), static_cast<int>(screenSize.y));

	// キー入力結果を受け取る箱
	char keys[256] = {0};
	char preKeys[256] = {0};

	// スクリーンモード
	bool screenMode = false;
  
	Player* player = new Player();
	// ゲームシーンマネージャーを生成
	SceneManager* sceneManager = new SceneManager();

	Map* map = new Map();
	//Game* game = new Game();


	Router* router[250] = { 0 };
	
	player->InitPlayer();
	map->LoadMapFromLDtk("./mapTest9999.ldtk",{"IntGrid","HalfBlock"});
	//int textureBlock = Novice::LoadTexture("./block.png");

	for (int i = 0; i < 250; i++) {
		router[i] = new Router(i, map->mapData);
	}

	// ウィンドウの×ボタンが押されるまでループ
	while (Novice::ProcessMessage() == 0) {
		// フレームの開始
		Novice::BeginFrame();

		// キー入力を受け取る
		memcpy(preKeys, keys, 256);
		Novice::GetHitKeyStateAll(keys);

		///
		/// ↓更新処理ここから
		///

		// ゲームシーンの管理
		sceneManager->Run();

		// スクリーン
		if(keys[DIK_P]&& !preKeys[DIK_P]){
			screenMode = !screenMode;
		}

		if (screenMode) {
			Novice::SetWindowMode(kFullscreen);
		} else {
			Novice::SetWindowMode(kWindowed);
		}
		/*for (int i = 0; i < 250; i++) {
			router[i]->UpdateRouter(map->mapData);
		}*/
		//////////
		/// 自機
		//////////

	

		//////////
		/// 座標変換
		//////////

		///
		/// ↑更新処理ここまで
		///

		///
		/// ↓描画処理ここから
		///

		//////////
		/// 自機
		//////////
		
		map->Draw(/*textureBlock*/);
		
		
		//////////
		/// デバッグ処理
		//////////




		///
		/// ↑描画処理ここまで
		///

		// フレームの終了
		Novice::EndFrame();

		// ESCキーが押されたらループを抜ける
		if (preKeys[DIK_ESCAPE] == 0 && keys[DIK_ESCAPE] != 0) {
			break;
		}
	}

	// 解放エクササイズ
	delete sceneManager;

	delete map;

	delete player;

	for (int i = 0; i < 250; i++) {
		delete router[i];
	}
	// ライブラリの終了
	Novice::Finalize();
	return 0;
}
