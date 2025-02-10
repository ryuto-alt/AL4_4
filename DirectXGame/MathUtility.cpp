#include "MathUtility.h"

using namespace KamataEngine;

Vector3& operator+=(Vector3& a, const Vector3& b) {

	a.x += b.x;
	a.y += b.y;
	a.z += b.z;

	return a;
}
Vector3& operator-=(Vector3& a, const Vector3& b) {

	a.x -= b.x;
	a.y -= b.y;
	a.z -= b.z;

	return a;
}
KamataEngine::Matrix4x4 operator*(const KamataEngine::Matrix4x4& lv, const KamataEngine::Matrix4x4& rv)
{
	Matrix4x4 result = {}; // 結果を格納する行列を初期化
	// 行列積を計算
	for (int i = 0; i < 4; ++i) {
		for (int j = 0; j < 4; ++j) {
			result.m[i][j] = 0.0f;
			for (int k = 0; k < 4; ++k) {
				result.m[i][j] += lv.m[i][k] * rv.m[k][j];
			}
		}
	}
	return result;
}


const Vector3 operator+(const Vector3& v1, const Vector3& v2) {

	Vector3 temp(v1);
	return temp += v2;
}

const Vector3 operator-(const Vector3& v1, const Vector3& v2) {

	Vector3 temp(v1);

	return temp -= v2;
}

const Vector3 operator*(const Vector3& v, float scalar) {
	return Vector3(v.x * scalar, v.y * scalar, v.z * scalar);
}

