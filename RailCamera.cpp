#include "RailCamera.h"

void RailCamera::Initialize(
	KamataEngine::Vector3 position,
	KamataEngine::Camera* camera, float radian)
{
	//引数でワールド座標を受け取ってワールドトランスフォームに設定
	worldTransform_.translation_ = position;
	//引数で回転角を受け取ってワールドトランスフォームに設定
	worldTransform_.rotation_ = 
		KamataEngine::Vector3(0.0f, radian, 0.0f);
	//カメラの初期化
	camera_ = camera;

	//カメラのfarZを適切な値に変更する
	//if (camera_ != nullptr) {
	//	camera_->SetFarZ(); // 適切な値を設定。例えば1000.0f。
	//}
	////カメラの初期化
	//camera_ = camera;
}

void RailCamera::Update()
{
	//ワールドトランスフォームの座標の数値を加算したりする(移動)
	//worldTransform_.translation_.x += 0.1f;
	//ワールドトランスフォームの角度の数値を加算したりする(回転)
//	worldTransform_.rotation_.y += 0.01f;
    //ワールドトランスフォームのワールド行列再計算
	worldTransform_.NoTransferUpdateMatrix();

	//カメラオブジェクトのワールド行列からビュー行列を計算する
	camera_->matView = Inverse(worldTransform_.matWorld_);

}

const KamataEngine::WorldTransform& RailCamera::GetWorldTransform() const {
	return worldTransform_;
}

const KamataEngine::Camera* RailCamera::GetCamera()
{
	return camera_;
}
