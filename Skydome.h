#pragma once
#include "Model.h"
#include "WorldTransform.h"
#include <cassert>

class Skydome {
public:
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(Model* model, uint32_t textureHandle, ViewProjection* viewProjection);

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

	private:
		// ワールド返還データ
	    WorldTransform worldTransform_;

	    ViewProjection* viewProjection_ = nullptr;



		// モデル
	    Model* model_ = nullptr;

		// テクスチャハンドル
	    uint32_t textureHandle_ = 0u;

};
