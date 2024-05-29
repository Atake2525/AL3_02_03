#include "Player.h"
#include "TextureManager.h"
#include <cassert>

void Player::Initialize(Model* model, uint32_t textureHandle, ViewProjection* viewProjection) {
	assert(model);

	model_ = model;
	textureHandle_ = textureHandle;

	worldTransform_.Initialize();
	viewProjection_ = viewProjection;
	worldTransform_.translation_.x = 2;
	worldTransform_.translation_.y = 2;
};

void Player::Update() { 
	worldTransform_.UpdateMatrix();
	
};

void Player::Draw() { model_->Draw(worldTransform_, *viewProjection_, textureHandle_); };
