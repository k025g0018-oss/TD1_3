#pragma once
#include "Vector2.h"

class Player 
{

public:
    // --- プレイヤーステータス ---
    struct PlayerStatus
    {
        //プレイヤーの位置
        Vector2 pos;

        //プレイヤーの加速度
        Vector2 acceleration;
        Vector2 Velocity;//重力系
        float Speed;

        Vector2 scale;//大きさ
        Vector2 dir;//プレイヤーの向き
        
        bool isActive;//生存フラグ
        float jumpPower;//ジャンプ力
        bool isJumop;//ジャンプフラグ
    }status_;//status＿で宣言

    Player();
    void InitPlayer();
    void UpdatePlayer(char keys[256], char preKeys[256]);
    void DrawPlayer();

private:

    void MovePlayer(char keys[256], char preKeys[256]);
    void Gravity();


};

