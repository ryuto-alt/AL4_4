#pragma once
#include <KamataEngine.h>
#include <list>
#include <3d/Camera.h>
#include <3d/WorldTransform.h>
#include <sstream>
#include "Player.h"
#include "PlayerBullet.h"
#include "Enemy.h"
#include "EnemyBullet.h"
#include "Skydome.h"
#include "RailCamera.h"

using namespace KamataEngine;

class Player;
class PlayerBullet;
class Enemy;
class EnemyBullet;
class Skydome;
class RailCamera;

class GameScene {
public:

	//コンストラクタ
	GameScene();
	//デストラクタ
	~GameScene();

	KamataEngine::WorldTransform worldTransform_;

	//初期化
	void Initialze();

	//毎フレーム処理
	void Update();

	//描画
	void Draw();

	//衝突判定と応答
	void CheckAllCollisions();

	//敵発生
	void AddEnemy(KamataEngine::Vector3 position);
	//敵弾を追加する
	void AddEnemyBullet(EnemyBullet* enemyBullet);
	void AddPlayerBullet(PlayerBullet* playerBullet);

	//敵発生データの読み込み
	void LoadEnemyPopData();

	//敵発生コマンドの更新
	void UpdateEnemyPopCommands();

	//弾リストの取得
	std::list<PlayerBullet*> playerBullets_ = {};
	std::list<EnemyBullet*> enemyBullets_ = {};

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

	//Enemy* enemy_ = nullptr;
	std::list<Enemy*> enemies_;

	Skydome* skydome_;

	RailCamera* railCamera_ = nullptr;


	//敵発生コマンド
	std::stringstream enemyPopCommands;
	std::string word;
	//待ち時間
	int32_t waitTime_;
	bool waitFlag_;

};