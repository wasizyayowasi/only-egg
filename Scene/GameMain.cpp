#include "GameMain.h"

#include "../object/Map.h"
#include "../object/Player.h"
#include "../object/Camera.h"
#include "../object/FryPan.h"
#include "../object/Bacon.h"
#include "../object/Arrow.h"
#include "../object/Recovery.h"
#include "../object/Interim.h"
#include "../object/ItemBase.h"

#include "GameEnd.h"
#include "SceneManager.h"
#include "PauseScene.h"
#include "GameOver.h"

#include "../util/SoundManager.h"
#include "../util/Model.h"
#include "../util/game.h"
#include "../util/InputState.h"

#include "EffekseerForDXLib.h"
#include <cmath>

namespace {
	//画像ファイルパス
	const char* const pictograph_file_name = "data/graph/pictoGraph2.png";
	const char* const pictographDisk_file_name = "data/graph/pictoGraphDisk.png";
	const char* const menu_graph = "data/graph/menuGraph1.png";
	const char* const result_graph = "data/graph/result.png";
	const char* const clock_graph = "data/graph/clock.png";
	const char* const gameOver_file_name = "data/graph/gameover.png";
	const char* const option_file_name = "data/graph/optionDisplay.png";
	//モデルファイルパス
	const char* const bacon_file_name = "data/object/bacon.mv1";
	const char* const arrow_file_name = "data/object/arrow.mv1";
	const char* const bane_file_name = "data/object/bane.mv1";
	//音源ファイルパス
	const char* const paradise_file_name = "data/sound/BGM/bgm1.mp3";
	const char* const sundial_file_name = "data/sound/BGM/bgm2.mp3";
	const char* const the_solid_earth_file_name = "data/sound/BGM/bgm3.mp3";
	const char* const death_file_name = "data/sound/BGM/bgm4.mp3";
	//エッフェクシア
	const char* const sprite_file_name = "data/effekseer/Simple_Sprite_BillBoard.efkefc";
	//複製ベーコン数
	constexpr int duplication_bacon_num = 1;
	//パッドの右スティックの傾き
	constexpr float slope = 400.0f;
	//傾くスピード
	constexpr float rot_speed = 0.05f;
	//絵文字のサイズ
	constexpr int pictograph_Billborad_size = 80;
	constexpr int pictograph_size = 100;
	//絵文字の角度
	constexpr float pictograph_angle = 0.0f;
	//絵文字表示タイム
	constexpr int display_time = 300;
	//数字の画像数
	constexpr int clockGraphNum_ = 16;
	//ばねの数
	constexpr int bane_num = 10;
	
}

GameMain::GameMain(SceneManager& manager, int stageNum) : SceneBase(manager),updateFunc_(&GameMain::fadeInUpdate),stageNum_(stageNum)
{
	{//生成
		map_ = std::make_shared<Map>(stageNum_);
		player_ = std::make_shared<Player>();
		camera_ = std::make_shared<Camera>();
		fryPan_ = std::make_shared<FryPan>(stageNum_);
		arrow_ = std::make_shared<Arrow>();
		item_.push_back(std::make_shared<Interim>());
		item_.push_back(std::make_shared<Recovery>());
		item_.push_back(std::make_shared<Bacon>(bacon_file_name));
		item_.push_back(std::make_shared<Bacon>(item_[2]->getModelHandle()));
		if (stageNum_ == 2) {
			bane_.push_back(std::make_shared<Model>(bane_file_name));
			for (int i = 0; i < bane_num; i++) {
				bane_.push_back(std::make_shared<Model>(bane_[0]->getModelHandle()));
			}
		}
	}
	
	itemNum = item_.size();

	itemSetPosition();
	

	{//画像ロード
		pictographDiskHandle_ = LoadGraph(pictographDisk_file_name);
		menugraphHandle_ = LoadGraph(menu_graph);
		resultgraphHandle_ = LoadGraph(result_graph);
		gameOverHandle_ = LoadGraph(gameOver_file_name);
		optiongraphHandle_ = LoadGraph(option_file_name);
		LoadDivGraph(pictograph_file_name, 8, 8, 1, pictograph_size, pictograph_size, pictographHandle_);
		LoadDivGraph(clock_graph, clockGraphNum_, clockGraphNum_, 1, pictograph_Billborad_size, pictograph_Billborad_size, clockgraphHandle_);
	}

	effectHandle_ = LoadEffekseerEffect(sprite_file_name, 20.0f);
	for (auto enable : effectIsEnable_) {
		enable = false;
	}

	player_->setMapData(map_);
	for (auto& bane : bane_) {
		bane->setScale({ 30,30,30 });
	}

	MV1SetupCollInfo(player_->getModel(), -1, 8, 8, 8);
	MV1SetupCollInfo(fryPan_->getModel(), -1, 8, 8, 8);

	ChangeLightTypePoint({ 0,500,0 }, 2000.0f, 0.0f, 0.006f, 0.0f);

	SoundManager::getInstance().setBGMRate(0.0f);
	switch (stageNum_) {
	case 0:
		SoundManager::getInstance().playMusic(paradise_file_name);
		break;
	case 1:
		SoundManager::getInstance().playMusic(the_solid_earth_file_name);
		break;
	case 2:
		SoundManager::getInstance().playMusic(sundial_file_name);
		break;
	}


}

GameMain::~GameMain()
{
	MV1TerminateCollInfo(player_->getModel(), -1);
	MV1TerminateCollInfo(fryPan_->getModel(), -1);
	for (auto& item : item_) {
		MV1TerminateCollInfo(item->getModelHandle(), -1);
	}
	DeleteGraph(pictographDiskHandle_);
	DeleteGraph(menugraphHandle_);
	DeleteGraph(resultgraphHandle_);
	DeleteGraph(gameOverHandle_);
	for (auto& graph : pictographHandle_) {
		DeleteGraph(graph);
	}
	for (auto& graph : clockgraphHandle_) {
		DeleteGraph(graph);
	}
//	DeleteEffekseerEffect(effectHandle_);
}

/// <summary>
/// メインシーンの更新
/// </summary>
/// <param name="input"></param>
void GameMain::update(const InputState& input)
{
	(this->*updateFunc_)(input);
}

/// <summary>
/// 描画
/// </summary>
void GameMain::draw()
{

	{//グリッド線表示　※消去予定
		for (float x = -500.0f; x <= 500.0f; x += 100.0f)
		{
			VECTOR start = VGet(x, 0.0f, -500.0f);
			VECTOR end = VGet(x, 0.0f, 500.0f);
			DrawLine3D(start, end, GetColor(0, 255, 0));
		}
		for (float z = -500.0f; z <= 500.0f; z += 100.0f)
		{
			VECTOR start = VGet(-500.0f, 0.0f, z);
			VECTOR end = VGet(500.0f, 0.0f, z);
			DrawLine3D(start, end, GetColor(255, 255, 0));
		}
	}

	{//各オブジェクトの描画
		map_->draw();
		player_->draw();
		fryPan_->draw();
		arrow_->draw();
		if (stageNum_ == 2) {
			for (auto& bane : bane_) {
				bane->draw();
			}
		}
	
		for (auto& item : item_) {
			item->draw();
		}
	}
	
	//絵文字円盤の描画
	if (pictographFlag_) {
		DrawRotaGraphF(Game::kScreenWidth / 2, Game::kScreenHeight / 2, 3.0f, 0.0f, pictographDiskHandle_, true);
		DrawCircleAA(Game::kScreenWidth / 2 + stickRot.x * 200, Game::kScreenHeight / 2 + stickRot.y * 200, 20, 32, 0xff0000, true);
	}

	{//テレポートカウントダウン表示
		VECTOR pictographPos;
		pictographPos = { player_->getPos().x,player_->getPos().y + 70,player_->getPos().z };
		DrawBillboard3D(pictographPos, 0.5f, 0.5f, pictograph_Billborad_size, pictograph_angle, clockgraphHandle_[player_->teleportCountDown()], true);
	}
	

	{//絵文字の座標、描画
		VECTOR pictographPos;
		pictographPos = { player_->getPos().x - 60,player_->getPos().y + 70,player_->getPos().z };
		DrawBillboard3D(pictographPos, 0.5f, 0.5f, pictograph_Billborad_size, pictograph_angle, pictographHandle_[pictographNum_], true);
	}

	{//時計
		//時計の背景
		DrawBox(1700, 0, Game::kScreenWidth, 100, 0xffffff, true);
		//数字の描画
		DrawGraph(1700, 10, clockgraphHandle_[clearTimeMinute_ / 10], true);
		DrawGraph(1730, 10, clockgraphHandle_[clearTimeMinute_ % 10], true);
		DrawGraph(1755, 10, clockgraphHandle_[10], true);
		DrawGraph(1780, 10, clockgraphHandle_[clearTimeSecond_ / 10], true);
		DrawGraph(1810, 10, clockgraphHandle_[clearTimeSecond_ % 10], true);
	}

	//オプション画像
	DrawGraph(0, 0, optiongraphHandle_, true);

	for (int i = 0; i < 4;i++) {
		if (effectIsEnable_[i]) {
			DrawEffekseer3D_Begin();
			DrawEffekseer3D_Draw(playEffectHandle_[i]);
		}
	}

	SetDrawBlendMode(DX_BLENDMODE_ALPHA, fadeValue_);
	//画面全体を真っ黒に塗りつぶす
	DrawBox(0, 0, Game::kScreenWidth, Game::kScreenHeight, fadeColor_, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}

/// <summary>
/// 絵文字選択更新
/// </summary>
/// <param name="input">key入力</param>
void GameMain::pictographUpdate(const InputState& input)
{
	GetJoypadDirectInputState(DX_INPUT_PAD1, &stick_);

	if (input.isPressed(InputType::rightTrigger)) {

		pictographFlag_ = true;

		stickRot.x = static_cast<float>(stick_.Rx);
		stickRot.y = static_cast<float>(stick_.Ry);
		stickRot.z = static_cast<float>(stick_.Rz);

		float size = 0.0f;
		size = VSize(stickRot);
		if (size > 0.0f) {
			stickRot = VNorm(stickRot);
		}
		else {
			pictographNum_ = -1;
		}

		size = std::atan2(stickRot.y, stickRot.x);
		size = size * 180 / 3.14f;

		//FIXME:もっといいやり方があるはず
		if (size > 0 && size < 45) {
			pictographNum_ = 0;
			pictographDisplayCount_ = 0;
		}
		else if (size > 45 && size < 90) {
			pictographNum_ = 1;
			pictographDisplayCount_ = 0;
		}
		else if (size > 90 && size < 135) {
			pictographNum_ = 2;
			pictographDisplayCount_ = 0;
		}
		else if (size > 135 && size < 180) {
			pictographNum_ = 3;
			pictographDisplayCount_ = 0;
		}
		else if (size < 0 && size > -45) {
			pictographNum_ = 4;
			pictographDisplayCount_ = 0;
		}
		else if (size < -45 && size > -90) {
			pictographNum_ = 5;
			pictographDisplayCount_ = 0;
		}
		else if (size < -90 && size > -135) {
			pictographNum_ = 6;
			pictographDisplayCount_ = 0;
		}
		else if (size < -135 && size > -180) {
			pictographNum_ = 7;
			pictographDisplayCount_ = 0;
		}
	}
}

void GameMain::itemSetPosition()
{

	if (stageNum_ == 2) {
		bane_[0]->setPos({ 990,1600,-600 });
		bane_[1]->setPos({ 1474,2200,-1055 });
		bane_[2]->setPos({ 1750, 2900 ,-1545 });
		bane_[3]->setPos({ -355 ,1000 ,-2770 });
		bane_[4]->setPos({ -510,1600,-3400 });
		bane_[5]->setPos({ -820,650,-6525 });
		bane_[6]->setPos({ -950,1000,-7390 });
		bane_[7]->setPos({ -1580,1700,-7350 });
		bane_[8]->setPos({ -2270 ,1200,-7340 });
		bane_[9]->setPos({ -2800,1900,-7340 });
		bane_[10]->setPos({ 348,0,810 });
	}

	for (int i = 0; i < itemNum; i++) {
		if (i == 0) {
			switch (stageNum_) {
			case 0:
				item_[i]->setPos({ 2820,2415,795 });
				//item_[i]->setPos({ 200,0,0});
				break;
			case 1:
				item_[i]->setPos({ -690,2755,970 });
				break;
			case 2:
				item_[i]->setPos({ 1405,2985,-3000 });
				break;
			}
		}

		if (i == 1) {
			switch (stageNum_) {
			case 0:
				item_[i]->setPos({ 3290,2415,795 });
				//item_[i]->setPos({ 200,0,200 });
				break;
			case 1:
				item_[i]->setPos({ -5,1680,-1170 });
				break;
			case 2:
				item_[i]->setPos({ -720,1945,-5460 });
				break;
			}
		}

		if (i == 2) {
			switch (stageNum_) {
			case 0:
				item_[i]->setPos({ 2155,1065,945 });
				//item_[i]->setPos({ 200,0,400 });
				break;
			case 1:
				item_[i]->setPos({ 390,1300,-3460 });
				break;
			case 2:
				item_[i]->setPos({ -4000,1450,-5115 });
				break;
			}
		}

		if (i == 3) {
			switch (stageNum_) {
			case 0:
				item_[i]->setPos({ 7205,1200,-2095 });
				//item_[i]->setPos({ 200,0,600 });
				break;
			case 1:
				item_[i]->setPos({ -865,2940,2445 });
				break;
			case 2:
				item_[i]->setPos({ 1450,2245,-2725 });
				break;
			}
		}
	}
}

void GameMain::checkCollision()
{
	int baneNum = bane_num + 1;

	//当たり判定用変数
	MV1_COLL_RESULT_POLY_DIM itemCollisionResult[4];
	MV1_COLL_RESULT_POLY_DIM baneCollisionResult[bane_num + 1];
	//関数を使用し当たっているかの情報を変数に代入する
	for (int i = 0; i < itemNum; i++) {
		if (item_[i]->isEnabled()) {
			itemCollisionResult[i] = MV1CollCheck_Capsule(item_[i]->getModelHandle(), item_[i]->getFrameIndex(), player_->getPos(), player_->getlastPos(), player_->getRadius());
		}
	}

	if (stageNum_ == 2) {
		for (int i = 0; i < baneNum; i++) {
			baneCollisionResult[i] = MV1CollCheck_Capsule(bane_[i]->getModelHandle(), bane_[i]->getColFrameIndex(), player_->getPos(), player_->getlastPos(), player_->getRadius());
		}
	}


	//フライパンの中心とプレイヤーの当たり判定
	MV1_COLL_RESULT_POLY_DIM fryPanResult;
	MV1RefreshCollInfo(player_->getModel(), -1);
	fryPanResult = MV1CollCheck_Sphere(player_->getModel(), -1, fryPan_->getPos(), 60);

	if (fryPanResult.HitNum > 0) {
		player_->stateChange();
		player_->setPos(fryPan_->getPos());
		SoundManager::getInstance().play("bake");
		updateFunc_ = &GameMain::fadeOutUpdate;
	}

	//ベーコンとプレイヤーの当たり判定
	for (int i = 0; i < itemNum; i++) {
		if (itemCollisionResult[i].HitNum > 0) {
			item_[i]->setEnabled();
			effectTime_ = 0;
			effectIsEnable_[i] = true;
			if (i != 0) {
				if (i == 1) {
					player_->cure();
				}
				SoundManager::getInstance().play("cure");
			}
			else {
				player_->chengeCheckpoint(item_[i]->getPos());
				SoundManager::getInstance().play("interim");
			}
		}
	}

	if (stageNum_ == 2) {
		for (int i = 0; i < baneNum; i++) {
			if (baneCollisionResult[i].HitNum > 0) {
				player_->FlyAway();
				SoundManager::getInstance().play("bane");
			}
		}
	}

	//当たり判定情報の初期化
	MV1CollResultPolyDimTerminate(fryPanResult);
	if (stageNum_ == 2) {
		for (int i = 0; i < baneNum; i++) {
			MV1CollResultPolyDimTerminate(baneCollisionResult[i]);
		}
	}
	for (int i = 0; i < itemNum; i++) {
		if (item_[i]->isEnabled()) {
			MV1CollResultPolyDimTerminate(itemCollisionResult[i]);
		}
	}
}

void GameMain::effectUpdate()
{
	if (effectTime_ % 60 == 0) {
		for (int i = 0; i < 4; i++) {
			playEffectHandle_[i] = PlayEffekseer3DEffect(effectHandle_);
		}
	}

	if (effectTime_ > 59) {
		for (int i = 0; i < 4; i++) {
			StopEffekseer3DEffect(playEffectHandle_[i]);
			effectIsEnable_[i] = false;
		}
	}

	for (int i = 0; i < 4; i++) {
		SetPosPlayingEffekseer3DEffect(playEffectHandle_[i], item_[i]->getPos().x, item_[i]->getPos().y, item_[i]->getPos().z);
	}


	UpdateEffekseer3D();

	++effectTime_;
}

/// <summary>
/// フェードイン処理
/// </summary>
/// <param name="input"></param>
void GameMain::fadeInUpdate(const InputState& input)
{
	SoundManager::getInstance().setBGMRate(static_cast<float>(60 - fadeTimer_) / 60.0f);
	fadeValue_ = static_cast <int>(255 * (static_cast<float>(fadeTimer_) / static_cast<float>(fadeInterval_)));
	if (--fadeTimer_ == 0) {
		updateFunc_ = &GameMain::normalUpdate;
		fadeValue_ = 0;
	}
}

/// <summary>
/// 更新
/// </summary>
/// <param name="input"></param>
void GameMain::normalUpdate(const InputState& input)
{
	deadFlag_ = false;

	effectUpdate();

	//絵文字表示時間
	{
		pictographDisplayCount_++;
		if (pictographDisplayCount_ > display_time) {
			pictographNum_ = -1;
			pictographDisplayCount_ = (std::min)(pictographDisplayCount_, display_time);
		}
	}
	
	pictographFlag_ = false;

	//絵文字更新
	pictographUpdate(input);

	{//各オブジェクトの更新
		if (!deadFlag_) {
			player_->update(input);
			camera_->update(input, player_, pictographFlag_, stick_);
			arrow_->update(fryPan_->getPos(), player_->getPos());
			for (auto& item : item_) {
				item->update();
			}
		}
	}
	
	//プレイヤーの状態を取得しtrueであればゲームを
	//中断させるフラグを立てる
	if (player_->getState()) {
		deadFlag_ = true;
	}

	//当たり判定
	checkCollision();
	
	//メイン処理を終了させる
	if (deadFlag_) {
		updateFunc_ = &GameMain::fadeOutUpdate;
	}

	//ポーズ画面に移動
	if (input.isTriggered(InputType::pause)) {
		manager_.pushScene(new PauseScene(manager_, stageNum_, player_, menugraphHandle_,gameOverHandle_));
	}

	//TODO:時間の処理の変更
	{
		tempTime_++;
		if (tempTime_ > 59) {
			clearTimeSecond_++;
			tempTime_ = 0;
		}
		if (clearTimeSecond_ > 59) {
			clearTimeMinute_++;
			clearTimeSecond_ = 0;
		}
	}
	
	SetLightPosition({ player_->getPos().x,player_->getPos().y + 200,player_->getPos().z });

}

/// <summary>
/// フェードアウト処理
/// </summary>
/// <param name="input"></param>
void GameMain::fadeOutUpdate(const InputState& input)
{
	float remainingHp_ = static_cast<float>(player_->getHp()) / 400.0f;
	int invalidBaconNum = 0;
	for (int i = 2; i < itemNum;i++) {
		if (!item_[i]->isEnabled()) {
			invalidBaconNum++;
		}
	}

	if (deadFlag_) {
		manager_.pushScene(new GameOver(manager_, stageNum_,gameOverHandle_));
		SoundManager::getInstance().playMusic(death_file_name);
	}
	else {
		manager_.pushScene(new GameEnd(manager_,stageNum_,resultgraphHandle_, clockgraphHandle_,clearTimeSecond_,clearTimeMinute_, remainingHp_, invalidBaconNum));
	}

}
