#include "WorldTransformEx.h"

using namespace KamataEngine;
using namespace KamataEngine::MathUtility;

void WorldTransformEx::UpdateMatrix() {
//World変換行列を計算しmatWorld_に格納する
	matWorld_ = MakeAffineMatrix();
//定数バッファへ転送
	TransferMatrix();

}

// アフィン変換行列
Matrix4x4 WorldTransformEx::MakeAffineMatrix() { 
	//ScaleMatrix
	Matrix4x4 matScale = MakeScaleMatrix(scale_);

	// RotationMatrix
	Matrix4x4 matRotX = MakeRotateXMatrix(rotation_.x);
	Matrix4x4 matRotY = MakeRotateYMatrix(rotation_.y);
	Matrix4x4 matRotZ = MakeRotateZMatrix(rotation_.z);
	Matrix4x4 matRot = matRotZ * matRotX * matRotY;

	// TranslateMatrix
	Matrix4x4 matTrans = MakeTranslateMatrix(translation_);

	//WorldMatrix
	Matrix4x4 matWorld = matScale * matRot * matTrans;

	return matWorld;
}
