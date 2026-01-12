#include "ScrollCamera.h"
#include <Novice.h>
#include "Game.h"

// コンストラクタ
ScrollCamera::ScrollCamera() : offset_({0, 0}) {}

// デストラクタ
ScrollCamera::~ScrollCamera() {
}

// 更新
void ScrollCamera::Update(Vector2& playerPos) {
	// プレイヤーがゲーム面中央に来るようにオフセットを計算
	// offset=プレイヤー位置-画面の半分
	offset_.x = playerPos.x - (kScreenWidth / 2.0f);

    if (offset_.x < 0) offset_.x = 0;
    if (offset_.x > (kWorldWidth - kScreenWidth)) {
        offset_.x = kWorldWidth - kScreenWidth;
    }

    // Y軸は今回スクロールしないなら0固定
    offset_.y = 0;
}