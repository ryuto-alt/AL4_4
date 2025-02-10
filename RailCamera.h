#pragma once
#include <KamataEngine.h>
#include <DirectXMath.h>
#include <imgui.h>
#include <3d/Camera.h>
#include "Affin.h"

class RailCamera
{
public:
	void Initialize(KamataEngine::Vector3 position,
		KamataEngine::Camera* camera, float radian);
	void Update();
	const KamataEngine::WorldTransform& GetWorldTransform() const;

	const KamataEngine::Camera* GetCamera();

private:
	KamataEngine::WorldTransform worldTransform_;
	KamataEngine::Camera* camera_ = nullptr;
//	float farZ_ = 1000.0f; // カメラの遠方クリップ距離
};

