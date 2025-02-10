#include "Enemy.h"
#include "Player.h"
#include "MathUtility.h"
#include "Affin.h"

using namespace KamataEngine;

Enemy::Enemy()
{
}

Enemy::~Enemy()
{
	
}

KamataEngine::Vector3 Enemy::GetWorldPosition()
{
	KamataEngine::Vector3 worldPos;

	worldPos.x = worldTransform_.translation_.x;
	worldPos.y = worldTransform_.translation_.y;
	worldPos.z = worldTransform_.translation_.z;

	return worldPos;
}

void Enemy::Initialize(KamataEngine::Model* model,
	uint32_t textureHandle, KamataEngine::Vector3 position)
{
	//引数の内容をメンバ変数に記録
	model_ = model;
	textureHandle_ = textureHandle;

	position_ = position;
	worldTransform_.translation_ = position_;

	//ワールド変換の初期化
	worldTransform_.Initialize();

	PhaseInitialize();
}

void Enemy::PhaseInitialize()
{
	switch (phase_) {
		// 接近フェーズ
	case Phase::Approach:
		worldTransform_.translation_.z = 30.0f;
		break;

		// 離脱フェーズ
	case Phase::Leave:
		worldTransform_.translation_.z = -30.0f;
		break;

	default:
		break;
	}
}

void Enemy::Update()
{
	float enemySpeed = 0.1f;

	worldTransform_.matWorld_ = MakeAffineMatrix(
		worldTransform_.scale_,
		worldTransform_.rotation_,
		worldTransform_.translation_
	);

	worldTransform_.TransferMatrix();

	//弾が発射されたらタイマーカウント開始
	Fire();
	fireTimer--;
	if (fireTimer == -1) {
		fireTimer = 60;
	}

	////弾更新
	//for (EnemyBullet* bullet : bullets_) {
	//	bullet->EnemyBulletUpdate();
	//}

	////死んだ弾を削除
	//gameScene_->enemyBullets_.remove_if([](EnemyBullet* bullet) {
	//	if (bullet->IsDead()) {
	//		delete bullet;
	//		return true;
	//	}
	//	return false;
	//	});

	// フェーズごとの動作
	switch (phase_) {
		// 接近フェーズ
	case Phase::Approach:
		worldTransform_.translation_.z -= enemySpeed;
		// 既定の位置に到達したら離脱
		if (worldTransform_.translation_.z < -30.0f) {
			phase_ = Phase::Leave;
			PhaseInitialize();
		}
		break;

		// 離脱フェーズ
	case Phase::Leave:
		worldTransform_.translation_.z += enemySpeed;
		break;
	}
}

void Enemy::Draw(KamataEngine::Camera* camera)
{
	//敵
	model_->Draw(worldTransform_, *camera, textureHandle_);
	//弾
	//for (EnemyBullet* bullet : bullets_) {
	//	bullet->EnemyBulletDraw(*camera,enemyBullet);
	//}
}

void Enemy::Fire()
{
	if (fireTimer > 0) {
		return; // まだクールダウン中なら発射しない
	}

	assert(player_);
	const float kBulletSpeed = 0.2f;

	// 自キャラのワールド座標を取得する
	KamataEngine::Vector3 playerPosition = player_->GetWorldPosition();
	// 敵キャラのワールド座標を取得する
	KamataEngine::Vector3 enemyPosition = worldTransform_.translation_;
	// 敵キャラから自キャラへの差分ベクトルを求める
	KamataEngine::Vector3 direction = playerPosition - enemyPosition;

	// ベクトルの正規化
	direction = Normalize(direction);

	// ベクトルの長さを速さに合わせる
	KamataEngine::Vector3 velocity = direction * kBulletSpeed;

	// 弾の生成と初期化
	EnemyBullet* newBullet = new EnemyBullet();
	newBullet->Initialize(model_, worldTransform_.translation_, velocity);

	// `GameScene` に弾を登録する
	if (gameScene_) {
		gameScene_->AddEnemyBullet(newBullet);
	}

	// 発射後、タイマーをリセット
	fireTimer = 60;
}

void Enemy::OnCollision()
{
}
