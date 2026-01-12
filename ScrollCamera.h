#pragma once
#include "Vector2.h"

class ScrollCamera {
public:
	// コンストラクタ
	ScrollCamera();

	// デストラクタ
	~ScrollCamera();
	
	void Update(Vector2& playerPos);

	// ゲッター
	Vector2 GetOffset() const { return offset_; } // 描画時にこの値を引き算しちゃうわよ～

private:
	Vector2 offset_; // 描画をずらす量
	const float kScreenWidth = 1400.0f; // ゲーム画面の横幅
	const float kWorldWidth = 1980.0f;  // 全体の横幅
};

