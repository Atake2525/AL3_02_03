#pragma once

#include <cmath>
#include "Matrix4x4.h"
#include "Vector3.h"

// 1, x軸回転行列
Matrix4x4 MakeRotateXMatrix(float radian);

// 2, y軸回転行列
Matrix4x4 MakeRotateYMatrix(float radian);

// 3, z軸回転行列
Matrix4x4 MakeRotateZMatrix(float radian);

Matrix4x4 Multiply(const Matrix4x4& m1, const Matrix4x4& m2);

// ３次元アフィン変換行列
Matrix4x4 MakeAffineMatrix(const Vector3& scale, const Vector3& rotate, const Vector3& translate);
