#pragma once


// プレイヤーのコマンドの種類
enum class CommandType {
    None,
    MoveRight,      // ずっと右
    CheckWallJump,  // 前に壁があったらジャンプ
    CheckCliffJump  // 下がブロックじゃなかったらジャンプ
};