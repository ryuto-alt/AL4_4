#pragma once
#include <KamataEngine.h>
#include "EnemyBullet.h"
#include "Affin.h"
#include "MathUtility.h"
#include <algorithm> 
#include <list>
#include <vector>
#include <DirectXMath.h>

class EnemyBullet;
class Player;
class GameScene;

class Enemy
{
public:
	Enemy();
	~Enemy();

	//setter
	void SetPlayer(Player* player) { player_ = player; }

	//getter
	//ワールド座標を返す
	KamataEngine::Vector3 GetWorldPosition();
	//弾リストを取得
	//const std::list<EnemyBullet*>& GetBullets()const { return gameScene_->enemyBullets_; }

	void Initialize(KamataEngine::Model* model,
		uint32_t textureHandle, KamataEngine::Vector3 position);
	void PhaseInitialize();
	void Update();
	void Draw(KamataEngine::Camera* camera);

	//一定間隔で弾を発射
	void Fire();

	float GetCollisionRadius() const {
		return 1.0f;
	}

	//衝突したら呼び出されるコールバック関数
	void OnCollision();

	void SetGameScene(GameScene* gameScene) { gameScene_ = gameScene; }

private:
	//ワールド変換データ
	KamataEngine::WorldTransform worldTransform_;

	//モデル
	KamataEngine::Model* model_ = nullptr;

	//テクスチャハンドル
	uint32_t textureHandle_ = 0u;

	KamataEngine::Vector3 position_ = { 0,0,0 };
	//std::list<EnemyBullet*> bullets_;

	int32_t fireTimer = 60;

	//enum行動パターンクラス
	enum class Phase {
		Approach,
		Leave,
	};

	Phase phase_ = Phase::Approach;

	Player* player_ = nullptr;
	GameScene* gameScene_ = nullptr;
};
