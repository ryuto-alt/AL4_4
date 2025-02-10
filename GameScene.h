#pragma once
#include <KamataEngine.h>
#include "Player.h"
#include "Enemy.h"
#include "Skydome.h"
#include "RailCamera.h"

using namespace KamataEngine;

class Player;
class Enemy;
class Skydome;
class RailCamera;

class GameScene {
public:

	//コンストラクタ
	GameScene();
	//デストラクタ
	~GameScene();

	//初期化
	void Initialze();

	//毎フレーム処理
	void Update();

	//描画
	void Draw();

	//衝突判定と応答
	void CheckAllCollisions();

private:

	//メンバ変数
	DirectXCommon* dxCommon_ = nullptr;
	Input* input_ = nullptr;
	Audio* audio_ = nullptr;

	//テクスチャハンドル
	uint32_t textureHandle_ = 0;
	uint32_t enemyTextureHandle_ = 0;
	//モデル
	Model* model_ = nullptr;
	//天球のモデル
	Model* modelSkydome_ = nullptr;

	//ビュープロ(カメラ)
	Camera camera_;

	//プレイヤー
	Player* player_ = nullptr;

	Enemy* enemy_ = nullptr;

	Skydome* skydome_;

	RailCamera* railCamera_ = nullptr;
};