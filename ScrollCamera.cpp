#include "ScrollCamera.h"
#include <Novice.h>
#include "Game.h"

// コンストラクタ


// デストラクタ


// 更新
void ScrollCamera::Update(Vector2& playerPos) {
	// プレイヤーを画面の中心
	worldPos_ = playerPos;
	offset_.x = worldPos_.x - (Game().gameScreen.size.x / 2);
}