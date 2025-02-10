#include "3d/Model.h"
#include "3d/WorldTransform.h"
#include "3d/camera.h"
#include "MathUtility.h"
#include "Affin.h"

using namespace KamataEngine;

void KamataEngine::WorldTransform::UpdateMatrix()
{
	matWorld_ = MakeAffineMatrix(
		scale_,
		rotation_,
		translation_
	);

	if (parent_) {
		matWorld_ = matWorld_* parent_->matWorld_;
	}

    TransferMatrix();
}

void KamataEngine::WorldTransform::NoTransferUpdateMatrix()
{
	matWorld_ = MakeAffineMatrix(
		scale_,
		rotation_,
		translation_
	);

	if (parent_) {
		matWorld_ =matWorld_* parent_->matWorld_;
	}

}
