#pragma once
#include <cassert>
#include <cmath>
#include <math/Matrix4x4.h>
#include <math/Vector3.h>

KamataEngine::Matrix4x4 MakeAffineMatrix(const KamataEngine::Vector3& scale, const KamataEngine::Vector3& rot, const KamataEngine::Vector3& translate);

KamataEngine::Matrix4x4 MatrixMultiply(KamataEngine::Matrix4x4& m1, KamataEngine::Matrix4x4& m2);

KamataEngine::Vector3 TransformNormal(const KamataEngine::Vector3& v, const KamataEngine::Matrix4x4& m);

KamataEngine::Vector3 Normalize(const KamataEngine::Vector3& v);

KamataEngine::Matrix4x4 Inverse(const KamataEngine::Matrix4x4& mat);