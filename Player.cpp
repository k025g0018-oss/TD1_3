#include "Player.h"
#include "Novice.h"

Player::Player() {
	InitPlayer();
}

void Player::InitPlayer() {
	status_.pos.x = 300.0f;
	status_.pos.y = 300.0f;
	status_.acceleration.x = 0.0f;
	status_.acceleration.y = 0.0f;
	status_.Velocity.x = 0.0f;
	status_.Velocity.y = +9.8f;
	status_.Speed = 5.0f;
	status_.scale.x = 64.0f;
	status_.scale.y = 64.0f;
	status_.isActive = true;
	status_.isJumop = false;
	status_.jumpPower=10.0f;
}

void Player::UpdatePlayer(char keys[256], char preKeys[256]){

	if (status_.isActive) {
		MovePlayer(keys, preKeys);
	}
}

void Player::DrawPlayer() {
	Novice::DrawBox(static_cast<int>(status_.pos.x), static_cast<int>(status_.pos.y),
		static_cast<int>(status_.scale.x), static_cast<int>(status_.scale.y),
		0.0f, WHITE, kFillModeSolid);
}

//------------------------------------------------------------------------------------------------------
//プライベート関数など
//------------------------------------------------------------------------------------------------------


void Player::MovePlayer(char keys[256], char preKeys[256]) {

	if (!status_.isJumop) {
		if (keys[DIK_SPACE] && preKeys[DIK_SPACE]) {
			status_.isJumop = true;
			status_.Velocity.y =-status_.jumpPower;
		}
	}

	if (status_.pos.y >= 1080) {
		status_.pos.y = 0;
	}
	Gravity();
}

void Player::Gravity() {

	status_.Velocity.y += 0.38f;
	status_.pos.y += status_.Velocity.y;
}


