#pragma once
#include "Player.h"
#include <3d/Camera.h>
#include <3d/WorldTransform.h>
#include "math/Vector3.h"
#include <cassert>
#include "GameScene.h"
#include "MathUtility.h"


class PlayerBullet
{
public:

	//getter
	//ワールド座標を返す
	KamataEngine::Vector3 GetWorldPosition();

	void Initialize(KamataEngine::Model* model, 
		KamataEngine::Vector3& position,
		KamataEngine::Vector3& velocity);
	void Update();
	void Draw(KamataEngine::Camera& viewProjection);

	bool IsDead()const { return isDead_; }

	float GetCollisionRadius() const {
		return 1.0f;
	}

	//衝突したら呼び出されるコールバック関数
	void OnCollision();

private:
	//ワールド変換データ
	KamataEngine::WorldTransform worldTransform_;

	//モデル
	KamataEngine::Model* model_ = nullptr;

	//テクスチャハンドル
	uint32_t textureHandle_ = 0u;

	//速度
	KamataEngine::Vector3 velocity_;

	//寿命
	static const int32_t kLifeTime = 60 * 5;
	//デスタイマー
	int32_t deathTimer_ = kLifeTime;
	//デスフラグ
	bool isDead_ = false;
};

