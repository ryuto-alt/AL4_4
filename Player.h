#pragma once
#include "3d/Model.h"
#include "3d/WorldTransform.h"
#include "3d/camera.h"
#include "GameScene.h"
#include <input/Input.h>
#include <algorithm> 
#include "PlayerBullet.h"
#include <list>
#include <DirectXMath.h>

class PlayerBullet;
class GameScene;

class Player {

public:
	//コンストラクタ
	Player();
	//デストラクタ
	~Player();

	//getter
	//ワールド座標を返す
	KamataEngine::Vector3 GetWorldPosition();
	//弾リストを取得
//	const std::list<PlayerBullet*>& GetBullets() const { return gameScene_->playerBullets_; }

	void Initialize(KamataEngine::Model* model, uint32_t textureHandle, KamataEngine::Vector3 position);
	void Update();
	void Draw(KamataEngine::Camera* camera);

	//攻撃
	void Attack();

	//定数でオブジェクトの半径の数値を返す
	float GetCollisionRadius() const {
		return 1.0f;
	}

	//衝突したら呼び出されるコールバック関数
	void OnCollision();
	
	//軸となるワールドトランスフォームをセット
	void SetParent(const KamataEngine::WorldTransform* parent);

	GameScene* GetGameScene() const { return gameScene_; }
	void SetGameScene(GameScene* gameScene) { gameScene_ = gameScene; }

private:

	//ワールド変換データ
	KamataEngine::WorldTransform worldTransform_;

	//モデル
	KamataEngine::Model* model_ = nullptr;

	//位置
	KamataEngine::Vector3 position_;

	//テクスチャハンドル
	uint32_t textureHandle_ = 0u;

	//キー入力
	KamataEngine::Input* input_ = nullptr;

	GameScene* gameScene_ = nullptr;

	//スケールのベクトル
	KamataEngine::Vector3 scale_;
};