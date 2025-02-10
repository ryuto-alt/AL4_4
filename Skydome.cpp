#include "Skydome.h"

void Skydome::Initialize(KamataEngine::Model* model,
	KamataEngine::Camera* camera,KamataEngine::Vector3 position)
{
	worldTransform_.Initialize();
	model_ = model;
	camera_ = camera;
	position_ = position;
	worldTransform_.scale_ = { 1000.0f,1000.0f,1000.0f };
}

void Skydome::Update()
{
	worldTransform_.UpdateMatrix();
}

void Skydome::Draw()
{
	model_->Draw(worldTransform_, *camera_);
}
