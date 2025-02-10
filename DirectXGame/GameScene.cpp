#include "GameScene.h"
#include <cassert>
#include <fstream>

GameScene::GameScene() {}

GameScene::~GameScene() {

	delete model_;
	delete player_;
	for (Enemy* enemy : enemies_) {
		delete enemy;
	}
	for (EnemyBullet* bullet : enemyBullets_) {
		delete bullet;
	}
	enemies_.clear();
	delete modelSkydome_;
	delete railCamera_;
}

void GameScene::Initialze() {

	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();

	//ファイル名を指定してテクスチャを読み込む
	textureHandle_ = TextureManager::Load("./Resources/uvChecker.png");

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
	player_->SetGameScene(this);

	//生成初期化
	enemies_.push_back(new Enemy());
	enemies_.back()->Initialize(model_, enemyTextureHandle_, { 0.0f, 5.0f, 30.0f });
	enemies_.back()->SetGameScene(this);
	enemies_.back()->PhaseInitialize();
	enemies_.back()->SetPlayer(player_);

	//天球情報
	skydome_ = new Skydome();
	skydome_->Initialize(modelSkydome_, &camera_, { 0.0f,0.0f,0.0f });
}

void GameScene::Update() {

	//自キャラ更新
	player_->Update();
	//敵更新
	for (Enemy* enemy : enemies_)
	{
		enemy->Update();
	}

	for (EnemyBullet* enemyBullet : enemyBullets_) {
		enemyBullet->EnemyBulletUpdate();
	}
	for (PlayerBullet* playerBullet : playerBullets_) {
		playerBullet->Update();
	}

	//弾とか敵とか自キャラとかの当たり判定
	CheckAllCollisions();
	//天球更新
	skydome_->Update();

	railCamera_->Update();
	camera_.matView = railCamera_->GetCamera()->matView;
	camera_.TransferMatrix();

	LoadEnemyPopData();
	UpdateEnemyPopCommands();
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
	for (Enemy* enemy : enemies_)
	{
		enemy->Draw(&camera_);
	}

	for (EnemyBullet* enemyBullet : enemyBullets_) {
		enemyBullet->EnemyBulletDraw(camera_);
	}

	for (PlayerBullet* playerBullet : playerBullets_) {
		playerBullet->Draw(camera_);
	}


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

#pragma region 自キャラと敵弾の当たり判定
	posA = player_->GetWorldPosition();

	//自キャラと敵弾すべての当たり判定
	for (EnemyBullet* bullet : enemyBullets_) {
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

#pragma region 自弾と敵キャラの当たり判定をすべての敵とすべての自弾の当たり判定
	for (PlayerBullet* playerBullet : playerBullets_) {
		posA = playerBullet->GetWorldPosition();

		// すべての敵キャラとの当たり判定
		for (Enemy* enemy : enemies_) {
			posB = enemies_.back()->GetWorldPosition(); // 敵キャラの位置を取得

			float dx = posA.x - posB.x;
			float dy = posA.y - posB.y;
			float dz = posA.z - posB.z;
			float distance = std::sqrt(dx * dx + dy * dy + dz * dz);

			float combinedRadius = playerBullet->GetCollisionRadius() + enemy->GetCollisionRadius();
			if (distance <= combinedRadius) {
				playerBullet->OnCollision();
				enemy->OnCollision();
			}
		}
	}
#pragma endregion

#pragma region 自弾と敵弾の当たり判定
	for (PlayerBullet* playerBullet : playerBullets_) {
		posA = playerBullet->GetWorldPosition();  // 自弾の位置を取得
		for (EnemyBullet* enemyBullet : enemyBullets_) {  // 敵弾リストをループ
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

	enemyBullets_.remove_if([](EnemyBullet* eb) {
		if (eb->IsDead()) {
			delete eb;
			eb = nullptr;
			return true;
		}
		else
		{
			return false;
		}
		});

	playerBullets_.remove_if([](PlayerBullet* pb) {
		if (pb->IsDead()) {
			delete pb;
			pb = nullptr;
			return true;
		}
		else
		{
			return false;
		}
		});
}

void GameScene::AddEnemy(KamataEngine::Vector3 position)
{
	Enemy* newEnemy = new Enemy();
	worldTransform_.translation_ = position;

	newEnemy->SetGameScene(this);
	newEnemy->Initialize(model_, enemyTextureHandle_,
		worldTransform_.translation_);
	newEnemy->SetPlayer(player_);

	enemies_.push_back(newEnemy);
}


void GameScene::AddEnemyBullet(EnemyBullet* enemyBullet)
{
	enemyBullets_.push_back(enemyBullet);
}

void GameScene::AddPlayerBullet(PlayerBullet* playerBullet)
{
	playerBullets_.push_back(playerBullet);
}

void GameScene::LoadEnemyPopData()
{
	std::ifstream file;
	file.open("Resources/enemyPop.csv");
	assert(file.is_open());
	enemyPopCommands << file.rdbuf();
	file.close();
}

void GameScene::UpdateEnemyPopCommands()
{
	//待機処理
	if (waitFlag_) {
		waitTime_--;
		if (waitTime_ <= 0) {
			//待機完了
			waitFlag_ = false;
		}
		return;
	}
	//１行分の文字列を入れる変数
	std::string line;

	while (getline(enemyPopCommands, line)) {

		//１行分の文字列をストリームに変換して解析しやすくする
		std::istringstream line_stream(line);

		//区切りで行の先頭文字列を取得
		getline(line_stream, word, ',');

		if (word.find("//") == 0) {
			//コメント行を飛ばす
			continue;
		}

		//POPコマンド
		if (word.find("POP") == 0) {
			//x
			getline(line_stream, word, ',');
			float x = (float)std::atof(word.c_str());
			//y
			getline(line_stream, word, ',');
			float y = (float)std::atof(word.c_str());
			//z
			getline(line_stream, word, ',');
			float z = (float)std::atof(word.c_str());

			//敵を発生させる
 			AddEnemy(KamataEngine::Vector3(x, y, z));
		}
		//WAITコマンド
		else if (word.find("WAIT") == 0) {
			getline(line_stream, word, ',');

			int32_t waitTime = std::stoi(word.c_str());

			//待機中開始
			//フラグ
			waitFlag_ = true;
			//タイマー
			waitTime_ = waitTime;

			break;
		}
	}
}