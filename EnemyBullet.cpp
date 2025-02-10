#include "EnemyBullet.h"

KamataEngine::Vector3 EnemyBullet::GetWorldPosition()
{
	KamataEngine::Vector3 worldPos;

	worldPos.x = worldTransform_.translation_.x;
	worldPos.y = worldTransform_.translation_.y;
	worldPos.z = worldTransform_.translation_.z;

	return worldPos;
}

void EnemyBullet::Initialize(KamataEngine::Model* model,
	KamataEngine::Vector3& position,
	KamataEngine::Vector3& velocity)
{
	assert(model);

	model_ = model;
	textureHandle_ = TextureManager::Load("./Resources/cube/cube.jpg");

	worldTransform_.Initialize();

	worldTransform_.translation_ = position;

	velocity_ = velocity;
}

void EnemyBullet::Update()
{
	worldTransform_.UpdateMatrix();

	worldTransform_.translation_ += velocity_;
}

void EnemyBullet::Draw(KamataEngine::Camera& camera)
{
	model_->Draw(worldTransform_, camera,
		textureHandle_);
}

void EnemyBullet::OnCollision()
{
	isDead_ = true;
}
