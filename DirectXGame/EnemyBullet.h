#pragma once
#include "Enemy.h"
#include "GameScene.h"
#include "Affin.h"
#include "MathUtility.h"
#include <algorithm>
#include <vector>

class EnemyBullet
{
public:

	//getter
	//ワールド座標を返す
	KamataEngine::Vector3 GetWorldPosition();
	KamataEngine::Vector3 GetVelocity() const { return velocity_; }

	//setter
	void SetVelocity(KamataEngine::Vector3 velocity) {
		velocity_ = velocity;
	}

	void Initialize(KamataEngine::Model* model,
		KamataEngine::Vector3& position,
		KamataEngine::Vector3& velocity);
	void EnemyBulletUpdate();
	void EnemyBulletDraw(KamataEngine::Camera& camera);
	bool IsDead()const { return isDead_; }

	//定数でオブジェクトの半径の数値を返す
	float GetCollisionRadius() const {
		return 1.0f;
	}

	//衝突したら呼び出されるコールバック関数
	void OnCollision();

	KamataEngine::Model* model_ = nullptr;
	KamataEngine::WorldTransform worldTransform_;
	uint32_t textureHandle_;

private:
	KamataEngine::Vector3 position_;
	KamataEngine::Vector3 velocity_ = { 1,1,1 };

	//寿命
	static const int32_t kLifeTime = 60 * 5;
	//デスタイマー
	int32_t deathTimer_ = kLifeTime;
	//デスフラグ
	bool isDead_ = false;
};

