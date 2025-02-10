#pragma once
#include <KamataEngine.h>

class Skydome
{
public:
	void Initialize(KamataEngine::Model* model,
		KamataEngine::Camera* camera,
		KamataEngine::Vector3 position);
	void Update();
	void Draw();

private:
	KamataEngine::WorldTransform worldTransform_;
	KamataEngine::Model* model_ = nullptr;
	KamataEngine::Model* modelSkydome_ = nullptr;
	KamataEngine::Camera* camera_ = nullptr;
	KamataEngine::Vector3 position_;
};
