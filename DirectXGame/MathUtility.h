#pragma once
#include <KamataEngine.h>
#include <math/Matrix4x4.h>
#include <cmath>

KamataEngine::Vector3& operator+=(KamataEngine::Vector3& a, const KamataEngine::Vector3& b);
KamataEngine::Vector3& operator-=(KamataEngine::Vector3& a, const KamataEngine::Vector3& b);
KamataEngine::Matrix4x4 operator*(const KamataEngine::Matrix4x4& lv, const KamataEngine::Matrix4x4& rv);

const KamataEngine::Vector3 operator+(const KamataEngine::Vector3& v1, const KamataEngine::Vector3& v2);
const KamataEngine::Vector3 operator-(const KamataEngine::Vector3& v1, const KamataEngine::Vector3& v2);
const KamataEngine::Vector3 operator*(const KamataEngine::Vector3& v, float scalar);
