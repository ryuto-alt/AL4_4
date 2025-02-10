#include "Player.h"
#include <cassert>
#include "math/Vector3.h"
#include "MathUtility.h"
#include "Affin.h"
#include "imgui.h"

using namespace KamataEngine;

Player::Player()
{
}

Player::~Player()
{
	for (PlayerBullet* bullet : bullets_) {
	  delete bullet;
	}
}

KamataEngine::Vector3 Player::GetWorldPosition()
{
	KamataEngine::Vector3 worldPos;

	worldPos.x = worldTransform_.matWorld_.m[3][0];
	worldPos.y = worldTransform_.matWorld_.m[3][1];
	worldPos.z = worldTransform_.matWorld_.m[3][2];

	return worldPos;
}

void Player::Initialize(KamataEngine::Model* model, uint32_t textureHandle, KamataEngine::Vector3 position) {
	assert(model);

	//引数の内容をメンバ変数に記録
	model_ = model;
	textureHandle_ = textureHandle;

	//ワールド変換の初期化
	worldTransform_.Initialize();
	worldTransform_.translation_ = position;

	//シングルトンインスタンスを取得
	input_ = KamataEngine::Input::GetInstance();
}

void Player::Update() {

	//キャラクターの移動ベクトル
	Vector3 move = { 0,0,0 };

	//キャラクターの移動速度
	const float kCharacterSpeed = 0.2f;

	const float rotationSpeed = 0.1f;

	//押した方向で移動ベクトルを変更
	if (input_->PushKey(DIK_A)) {
		move.x -= kCharacterSpeed;
	}
	if (input_->PushKey(DIK_D)) {
		move.x += kCharacterSpeed;
	}
	if (input_->PushKey(DIK_S)) {
		move.y -= kCharacterSpeed;
	}
	if (input_->PushKey(DIK_W)) {
		move.y += kCharacterSpeed;
	}

	//旋回処理
	if (input_->PushKey(DIK_LEFT)) {
		worldTransform_.rotation_.y += rotationSpeed;
	}
	if (input_->PushKey(DIK_RIGHT)) {
		worldTransform_.rotation_.y -= rotationSpeed;
	}
	//移動
	worldTransform_.translation_ += move;

	worldTransform_.UpdateMatrix();

	//攻撃呼び出し
	Attack();

	//弾更新
	for (PlayerBullet* bullet : bullets_) {
		bullet->Update();
	}

	//死んだ弾を削除
	bullets_.remove_if([](PlayerBullet* bullet) {
		if (bullet->IsDead()) {
			delete bullet;
			return true;
		}
		return false;
	});
}

void Player::Draw(KamataEngine::Camera* camera) {

	model_->Draw(worldTransform_, *camera, textureHandle_);

	//弾描画
	for (PlayerBullet* bullet : bullets_) {
		bullet->Draw(*camera);
	}
}

void Player::Attack() {
	// 弾を生成し、初期化
	if (input_->TriggerKey(DIK_SPACE)) {
		// 自機のワールド座標を取得
		KamataEngine::Vector3 position = GetWorldPosition(); // ワールド座標を取得する関数を利用

		// 発射方向の初期ベクトルを指定
		KamataEngine::Vector3 velocity = { 0, 0, 1 };

		// 発射方向をワールド空間に変換
		velocity = TransformNormal(velocity, worldTransform_.matWorld_);

		// 弾の生成と初期化
		PlayerBullet* newBullet = new PlayerBullet();
		newBullet->Initialize(model_, position, velocity);

		// 弾を登録する
		bullets_.push_back(newBullet);
	}
}

void Player::OnCollision()
{
}

void Player::SetParent(const KamataEngine::WorldTransform* parent)
{
	worldTransform_.parent_ = parent;
}
