#pragma once
#include "Vector2.h"

class ScrollCamera {
public:
	// コンストラクタ
	ScrollCamera();

	// デストラクタ
	~ScrollCamera();
	
	void Update(Vector2& playerPos);

private:
	// カメラの位置
	Vector2 worldPos_;
	float speed_;
	Vector2 offset_; // 基点
};

