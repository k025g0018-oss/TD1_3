#pragma once
#include "Vector2.h"
#include "const.h"
#include "Command.h" 
#include <vector>    
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
        
        float height;
        float width;

        bool isActive;//生存フラグ
        float jumpPower;//ジャンプ力
        bool isJumop;//ジャンプフラグ
        float radius;
    }status_;//status＿で宣言


    Player();
    void InitPlayer();
    void UpdatePlayer(char keys[256], char preKeys[256], int  mapData[kMapHeight][kMapWidth]);
    // コマンドで動かせる
   void UpdateByCommands(const std::vector<CommandType>& commands, int mapData[kMapHeight][kMapWidth]);
    void DrawPlayer();

    // 今どのコマンドを実行中か
    int GetCurrentCommandIndex() const { return cmdIndex; }

private:

    void MovePlayer(char keys[256], char preKeys[256], int  mapData[kMapHeight][kMapWidth]);
    void Gravity();

    // コマンドの処理関数
    // アクション関数
    void ActionMoveRight();
    void ActionTryJump();

    // センサー関数
    bool IsWallAhead(int mapData[kMapHeight][kMapWidth]);
    bool IsCliffAhead(int mapData[kMapHeight][kMapWidth]);

	// マップの当たり判定関数
	void isGrounded(int mapData[kMapHeight][kMapWidth]);
	void isRightWall(int mapData[kMapHeight][kMapWidth]);
	void isLeftWall(int mapData[kMapHeight][kMapWidth]);
	void isTopWall(int mapData[kMapHeight][kMapWidth]);

    // コマンド実行時のインデックス
    int cmdIndex;

};

