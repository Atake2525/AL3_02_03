#include "GameScene.h"
#include "TextureManager.h"
#include <cassert>





GameScene::GameScene() {}

GameScene::~GameScene() { 
	delete modelSkydome_; 
	for (std::vector<WorldTransform*>& worldtransformBlockLine : worldTransformBlocks_) {
		for (WorldTransform* worldTransformBlock : worldtransformBlockLine ) {
			delete worldTransformBlock;
		}
	}
	worldTransformBlocks_.clear();
	
	delete skyDome_;
	delete player_;
	delete debugCamera_;
}

void GameScene::Initialize() {

	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();

	model_ = Model::Create();

	modelSkydome_ = Model::CreateFromOBJ("skydome", true);
	texturehandle_ = TextureManager::Load("inshipPlayer_front_1.png");

	// 背景クラスの生成
	skyDome_ = new Skydome();
	// 背景クラスの初期化
	skyDome_->Initialize(modelSkydome_, texturehandle_, &viewProjection_);

	// プレイヤー
	player_ = new Player();
	// プレイヤーキャラの初期化
	player_->Initialize(model_, texturehandle_, &viewProjection_);

	//デバッグカメラの作成
	debugCamera_ = new DebugCamera(1280, 720);

	viewProjection_.Initialize();

	texturehandle_ = TextureManager::Load("cube/cube.jpg");

	//要素数
	const uint32_t kNumBlockVirtical = 10;
	const uint32_t kNumBlockHorizontal = 20;
	//ブロック1個分の増幅
	const float kBlockWidth = 2.0f;
	const float kBlockHeight = 2.0f;
	//要素数を変更する
	//列数を設定(縦方向のブロック数)
	worldTransformBlocks_.resize(kNumBlockVirtical);
	for (uint32_t i = 0; i < kNumBlockVirtical; i++) {
		//1列の要素数を設定(横方向のブロック数)
		worldTransformBlocks_[i].resize(kNumBlockHorizontal);
	}

	//キューブの生成
	for (uint32_t i = 0; i < kNumBlockVirtical; i++) {
		for (uint32_t j = 0; j < kNumBlockHorizontal; j++) {
			if (0 == (i + j) %2)
				continue;
			worldTransformBlocks_[i][j] = new WorldTransform();
			worldTransformBlocks_[i][j]->Initialize();
			worldTransformBlocks_[i][j]->translation_.x = kBlockWidth * j;
			worldTransformBlocks_[i][j]->translation_.y = kBlockHeight* i;
		}
	}

}

void GameScene::Update() {

	//ブロックの更新
	for (std::vector<WorldTransform*>& worldtransformBlockLine : worldTransformBlocks_) {
		for (WorldTransform* worldTransformBlock : worldtransformBlockLine) {
			if (!worldTransformBlock)
				continue;
			// アフィン変換行列の作成
			worldTransformBlock->matWorld_ = MakeAffineMatrix(worldTransformBlock->scale_, worldTransformBlock->rotation_, worldTransformBlock->translation_);
			// 定数バッファに転送する
			worldTransformBlock->TransferMatrix();
		}
	}

	player_->Update();

	skyDome_->Update();
	
	//デバッグカメラの更新
	debugCamera_->Update();

	#ifdef _DEBUG
	if (input_->TriggerKey(DIK_0)) {
		isDebugCameraActive_ = !isDebugCameraActive_;
	}
#endif  DEBUG
	if (isDebugCameraActive_) {
		debugCamera_->Update();
		viewProjection_.matView = debugCamera_->GetViewProjection().matView;
		viewProjection_.matProjection = debugCamera_->GetViewProjection().matProjection;
		//ビュープロジェクション行列の転送
		viewProjection_.TransferMatrix();
	}else
	{
	       //ビュープロジェクション行列の更新と転送
		viewProjection_.UpdateMatrix();
	}
}

void GameScene::Draw() {

	// コマンドリストの取得
	ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandList();

#pragma region 背景スプライト描画
	// 背景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに背景スプライトの描画処理を追加できる
	/// </summary>

	// スプライト描画後処理
	Sprite::PostDraw();
	// 深度バッファクリア
	dxCommon_->ClearDepthBuffer();
#pragma endregion

#pragma region 3Dオブジェクト描画
	// 3Dオブジェクト描画前処理
	Model::PreDraw(commandList);

	/// <summary>
	/// ここに3Dオブジェクトの描画処理を追加できる
	/// </summary>
	//ブロックの描画
	for (std::vector<WorldTransform*>& worldtransformBlockLine : worldTransformBlocks_) {
		for (WorldTransform* worldTransformBlock : worldtransformBlockLine) {
			if (!worldTransformBlock)
				continue;
			model_->Draw(*worldTransformBlock, viewProjection_, texturehandle_);
		}
	}
	player_->Draw();
	skyDome_->Draw();

	// 3Dオブジェクト描画後処理
	Model::PostDraw();
#pragma endregion

#pragma region 前景スプライト描画
	// 前景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに前景スプライトの描画処理を追加できる
	/// </summary>

	// スプライト描画後処理
	Sprite::PostDraw();

#pragma endregion
}
