#include "ScrollCamera.h"
#include <Novice.h>
#include "Game.h"
#include "const.h" // マップ計算

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

    // マップ全体の横幅を計算 (例: 100枚 * 64px = 6400px)
    float mapWorldWidth = static_cast<float>(kMapWidth * kTileSize);

    // 左端の制限
    if (offset_.x < 0) offset_.x = 0;

    // 右端の制限（マップの本当の端で止まるようにする）
    float maxScroll = mapWorldWidth - kScreenWidth;
    if (offset_.x > maxScroll) {
        offset_.x = maxScroll;
    }

    // Y軸は今回スクロールしないなら0固定
    offset_.y = 0;
}