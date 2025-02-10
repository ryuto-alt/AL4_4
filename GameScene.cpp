#include "GameScene.h"
#include <cassert>

GameScene::GameScene() {}

GameScene::~GameScene() {

	delete model_;
	delete player_;
	delete enemy_;
	delete modelSkydome_;
	delete railCamera_;
}

void GameScene::Initialze() {

	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();

	//ファイル名を指定してテクスチャを読み込む
	textureHandle_ = TextureManager::Load("./Resources/uvChecker.png");
	enemyTextureHandle_ = TextureManager::Load("./Resources/kamata.ico");

	modelSkydome_ = Model::CreateFromOBJ("skydome", true);

	//3Dモデルの生成
	model_ = Model::Create();

	//カメラの初期化
	camera_.Initialize();

	//自キャラの生成
	player_ = new Player();

	//自機をカメラからずらす処理
    //カメラの前方ベクトルを計算
	KamataEngine::Vector3 cameraForward(
		sinf(camera_.rotation_.y),  // Y軸回りの回転を反映
		0,
		cosf(camera_.rotation_.y)
	);
	float distanceOffset = 100.0f;
	// カメラの位置を基準に、自機を前方にずらす
	KamataEngine::Vector3 playerPosition = (0, 0, camera_.translation_ + cameraForward * distanceOffset);

	railCamera_ = new RailCamera();  // railCamera_をインスタンス化
	assert(railCamera_ != nullptr); // 確認
	float rotation = 0.0f;          // 初期回転角
	railCamera_->Initialize({ 0.0f, 0.0f, 0.0f }, &camera_, rotation);

	// プレイヤーの初期化
	player_->Initialize(model_, textureHandle_, playerPosition);
	player_->SetParent(&railCamera_->GetWorldTransform());

	//生成初期化
	enemy_ = new Enemy();
	enemy_->Initialize(model_, enemyTextureHandle_, { 0.0f,5.0f,30.0f });//ここで位置をいじる
	enemy_->PhaseInitialize();
	enemy_->SetPlayer(player_);

	//天球情報
	skydome_ = new Skydome();
	skydome_->Initialize(modelSkydome_, &camera_, { 0.0f,0.0f,0.0f });

}

void GameScene::Update() {

	//自キャラ更新
	player_->Update();
	//敵更新
	enemy_->Update();
	//弾とか敵とか自キャラとかの当たり判定
	CheckAllCollisions();
	//天球更新
	skydome_->Update();

	railCamera_->Update();
	camera_.matView = railCamera_->GetCamera()->matView;
	camera_.TransferMatrix();
}

void GameScene::Draw() {

	// コマンドリストの取得
	ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandList();

	// 背景スプライト描画前処理
	Sprite::PreDraw(commandList);
	// スプライト描画後処理
	Sprite::PostDraw();
	// 深度バッファクリア
	dxCommon_->ClearDepthBuffer();
	// 3Dオブジェクト描画前処理
	Model::PreDraw(commandList);

	//自キャラの描画
	player_->Draw(&camera_);
	//敵の描画
	/*enemy_->Draw(&camera_);*/
	//天球の描画
	skydome_->Draw();

	// 3Dオブジェクト描画後処理
	Model::PostDraw();
	// 前景スプライト描画前処理
	Sprite::PreDraw(commandList);
	// スプライト描画後処理
	Sprite::PostDraw();
}

void GameScene::CheckAllCollisions()
{
	KamataEngine::Vector3 posA, posB;

	//弾リストの取得
	const std::list<PlayerBullet*>& playerBullets = player_->GetBullets();
	const std::list<EnemyBullet*>& enemyBullets = enemy_->GetBullets();

#pragma region 自キャラと敵弾の当たり判定
	posA = player_->GetWorldPosition();
	
	//自キャラと敵弾すべての当たり判定
	for (EnemyBullet* bullet : enemyBullets) {
		//敵弾の座標を取る
		posB = bullet->GetWorldPosition();

		//posA_とposBの距離を求める
		float dx = posA.x - posB.x;
		float dy = posA.y - posB.y;
		float dz = posA.z - posB.z;
		float distance = std::sqrt(dx * dx + dy * dy + dz * dz);

		//球と球の交差判定
		//半径を比べる
		float combinedRadius =
			player_->GetCollisionRadius() + bullet->GetCollisionRadius();
		if (distance <= combinedRadius) {
			// 自キャラの衝突時コールバック関数を呼び出す
			player_->OnCollision();

			// 敵弾キャラの衝突時コールバック関数を呼び出す
			bullet->OnCollision();
		}
	}
#pragma endregion

#pragma region 自弾と敵キャラの当たり判定
	for (PlayerBullet* playerBullet : playerBullets) {
		posA = playerBullet->GetWorldPosition();

		// 敵キャラとの当たり判定
		posB = enemy_->GetWorldPosition(); // 敵キャラの位置を取得

		float dx = posA.x - posB.x;
		float dy = posA.y - posB.y;
		float dz = posA.z - posB.z;
		float distance = std::sqrt(dx * dx + dy * dy + dz * dz);

		float combinedRadius = playerBullet->GetCollisionRadius() + enemy_->GetCollisionRadius();
		if (distance <= combinedRadius) {
			playerBullet->OnCollision();
			enemy_->OnCollision();
		}
	}
#pragma endregion

#pragma region 自弾と敵弾の当たり判定
	for (PlayerBullet* playerBullet : playerBullets) {
		posA = playerBullet->GetWorldPosition();  // 自弾の位置を取得
		for (EnemyBullet* enemyBullet : enemyBullets) {  // 敵弾リストをループ
			posB = enemyBullet->GetWorldPosition();  // 敵弾の位置を取得

			// 自弾と敵弾の距離を計算する
			float dx = posA.x - posB.x;
			float dy = posA.y - posB.y;
			float dz = posA.z - posB.z;
			float distance = std::sqrt(dx * dx + dy * dy + dz * dz);

			// 衝突判定：球同士が当たっていれば
			float combinedRadius = playerBullet->GetCollisionRadius() + enemyBullet->GetCollisionRadius();
			if (distance <= combinedRadius) {
				// 自弾の衝突時コールバック関数を呼び出す
				playerBullet->OnCollision();

				// 敵弾の衝突時コールバック関数を呼び出す
				enemyBullet->OnCollision();
			}
		}
	}
#pragma endregion

}
