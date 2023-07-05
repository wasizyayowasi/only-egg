#include "GameMain.h"
#include "SceneManager.h"
#include "GameEnd.h"

#include "../object/Map.h"
#include "../object/Player.h"
#include "../object/Camera.h"
#include "../object/FryPan.h"

#include "../Scene/PauseScene.h"

#include "../util/Model.h"
#include "../util/game.h"
#include "../util/InputState.h"

#include <cmath>

namespace {
	//ファイルパス
	const char* const pictograph_file_name = "data/graph/pictoGraph2.png";
	const char* const pictographDisk_file_name = "data/graph/pictoGraphDisk.png";
	const char* const menu_graph = "data/graph/menuGraph.png";
	const char* const result_graph = "data/graph/result.png";
	const char* const clock_graph = "data/graph/clock.png";
	//パッドの右スティックの傾き
	constexpr float slope = 400.0f;
	//傾くスピード
	constexpr float rot_speed = 0.05f;
	//絵文字のサイズ
	constexpr int pictograph_Billborad_size = 80.0f;
	constexpr int pictograph_size = 100;
	//絵文字の角度
	constexpr float pictograph_angle = 0.0f;
	//絵文字表示タイム
	constexpr int display_time = 300;
	//数字の画像数
	constexpr int clockGraphNum_ = 13;
}

GameMain::GameMain(SceneManager& manager, int stageNum) : SceneBase(manager),updateFunc_(&GameMain::fadeInUpdate),stageNum_(stageNum)
{
	//カメラ関連
	{
		/////////////// 3D関連の設定 /////////////
		// Zバッファを使用する
		SetUseZBuffer3D(true);
		// Zバッファへの書き込みを行う
		SetWriteZBuffer3D(true);
		// ポリゴンの裏面を描画しない
		SetUseBackCulling(true);

		//////////////// カメラの設定 //////////////////
		// カメラからどれだけ離れたところ( Near )から、 どこまで( Far )のものを描画するかを設定
		SetCameraNearFar(5.0f, 2800.0f);
		// カメラの位置、どこを見ているかを設定する
		SetCameraPositionAndTarget_UpVecY(cameraPos_, VGet(0, 0, 0));
		// カメラの視野角を設定(ラジアン)
		SetupCamera_Perspective(60.0f * DX_PI_F / 180.0f);
	}
	
	{//生成
		map_ = std::make_shared<Map>(stageNum_);
		player_ = std::make_shared<Player>();
		camera_ = std::make_shared<Camera>();
		fryPan_ = std::make_shared<FryPan>();
	}
	
	pictographDiskHandle_ = LoadGraph(pictographDisk_file_name);
	menugraphHandle_ = LoadGraph(menu_graph);
	resultgraphHandle_ = LoadGraph(result_graph);
	LoadDivGraph(pictograph_file_name, 8, 8, 1, pictograph_size, pictograph_size, pictographHandle_);
	LoadDivGraph(clock_graph, clockGraphNum_, clockGraphNum_, 1, pictograph_Billborad_size, pictograph_Billborad_size, clockgraphHandle_);

	player_->setMapData(map_);

	MV1SetupCollInfo(player_->getModel(), -1, 8, 8, 8);
	MV1SetupCollInfo(fryPan_->getModel(), -1, 8, 8, 8);

	SetGlobalAmbientLight(GetColorF(0.5f, 0.5f, 0.5f, 0.0f));

	ChangeLightTypePoint({ 0,500,0 }, 2000.0f, 0.0f, 0.006f, 0.0f);

}

GameMain::GameMain(SceneManager& manager) : SceneBase(manager), updateFunc_(&GameMain::fadeInUpdate)
{
	//カメラ関連
	{
		/////////////// 3D関連の設定 /////////////
		// Zバッファを使用する
		SetUseZBuffer3D(true);
		// Zバッファへの書き込みを行う
		SetWriteZBuffer3D(true);
		// ポリゴンの裏面を描画しない
		SetUseBackCulling(true);

		//////////////// カメラの設定 //////////////////
		// カメラからどれだけ離れたところ( Near )から、 どこまで( Far )のものを描画するかを設定
		SetCameraNearFar(5.0f, 2800.0f);
		// カメラの位置、どこを見ているかを設定する
		SetCameraPositionAndTarget_UpVecY(cameraPos_, VGet(0, 0, 0));
		// カメラの視野角を設定(ラジアン)
		SetupCamera_Perspective(60.0f * DX_PI_F / 180.0f);
	}

	map_ = std::make_shared<Map>(stageNum_);
	player_ = std::make_shared<Player>();
	camera_ = std::make_shared<Camera>();
	fryPan_ = std::make_shared<FryPan>();

	player_->setMapData(map_);
	pictographDiskHandle_ = LoadGraph(pictographDisk_file_name);
	menugraphHandle_ = LoadGraph(menu_graph);
	resultgraphHandle_ = LoadGraph(result_graph);
	LoadDivGraph(pictograph_file_name, 8, 8, 1, pictograph_size, pictograph_size, pictographHandle_);
	LoadDivGraph(clock_graph, clockGraphNum_, clockGraphNum_, 1, pictograph_Billborad_size, pictograph_Billborad_size, clockgraphHandle_);

	MV1SetupCollInfo(player_->getModel(), -1, 8, 8, 8);
	MV1SetupCollInfo(fryPan_->getModel(), -1, 8, 8, 8);

	SetGlobalAmbientLight(GetColorF(0.5f, 0.5f, 0.5f, 0.0f));

	ChangeLightTypePoint({ 0,500,0 }, 2000.0f, 0.0f, 0.006f, 0.0f);

}

GameMain::~GameMain()
{
	MV1TerminateCollInfo(player_->getModel(), -1);
	MV1TerminateCollInfo(fryPan_->getModel(), -1);
	DeleteGraph(pictographDiskHandle_);
	DeleteGraph(menugraphHandle_);
	DeleteGraph(resultgraphHandle_);
	for (auto& graph : pictographHandle_) {
		DeleteGraph(graph);
	}
	for (auto& graph : clockgraphHandle_) {
		DeleteGraph(graph);
	}
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
	//DrawString(0, 0, "GameMain", 0xffffff);

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

	map_->draw();
	player_->draw();
	fryPan_->draw();

	//絵文字円盤の描画
	if (pictographFlag_) {
		DrawRotaGraphF(Game::kScreenWidth / 2, Game::kScreenHeight / 2, 3.0f, 0.0f, pictographDiskHandle_, true);
		DrawCircleAA(Game::kScreenWidth / 2 + stickRot.x * 200, Game::kScreenHeight / 2 + stickRot.y * 200, 20, 32, 0xff0000, true);
	}

	{//絵文字の座標、描画
		VECTOR pictographPos;
		pictographPos = { player_->GetPos().x - 60,player_->GetPos().y + 70,player_->GetPos().z };

		DrawBillboard3D(pictographPos, 0.5f, 0.5f, pictograph_Billborad_size, pictograph_angle, pictographHandle_[pictographNum_], true);
	}

	DrawFormatString(0, 115, 0xffffff, "%d:%d", clearTimeMinute_, clearTimeSecond_);

	DrawBox(1700, 0, Game::kScreenWidth, 100, 0xffffff, true);

	DrawGraph(1700, 10, clockgraphHandle_[clearTimeMinute_ / 10], true);
	DrawGraph(1730, 10, clockgraphHandle_[clearTimeMinute_ % 10], true);
	DrawGraph(1755, 10, clockgraphHandle_[10], true);
	DrawGraph(1780, 10, clockgraphHandle_[clearTimeSecond_ / 10], true);
	DrawGraph(1810, 10, clockgraphHandle_[clearTimeSecond_ % 10], true);

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

/// <summary>
/// フェードイン処理
/// </summary>
/// <param name="input"></param>
void GameMain::fadeInUpdate(const InputState& input)
{
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

	if (!deadFlag_) {
		player_->update(input);
		camera_->update(input,player_,pictographFlag_,stick_);
	}

	if (player_->getState()) {
		deadFlag_ = true;
	}

	MV1_COLL_RESULT_POLY_DIM result;
	result = MV1CollCheck_Capsule(fryPan_->getModel(), fryPan_->getFrameIndex(), player_->GetPos(), player_->GetlastPos(),player_->getRadius());

	if (result.HitNum > 0) {
		if (result.Dim->Normal.y > 0.000001) {
			player_->stateChange();
			player_->setPos(fryPan_->getPos());
			updateFunc_ = &GameMain::fadeOutUpdate;
		}
	}

	if (deadFlag_) {
		updateFunc_ = &GameMain::fadeOutUpdate;
	}

	if (input.isTriggered(InputType::pause)) {
		manager_.pushScene(new PauseScene(manager_,player_, menugraphHandle_));
	}

	if (input.isTriggered(InputType::down)) {
		updateFunc_ = &GameMain::fadeOutUpdate;
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
	
	VECTOR temp;
	temp =  VSub(player_->GetPos(), fryPan_->getPos());
	float size = VSize(temp);
	if (size > 0.0f) {
		temp = VNorm(temp);
	}



	SetLightPosition({ player_->GetPos().x,player_->GetPos().y + 200,player_->GetPos().z });
}

/// <summary>
/// フェードアウト処理
/// </summary>
/// <param name="input"></param>
void GameMain::fadeOutUpdate(const InputState& input)
{
	float remainingHp_ = static_cast<float>(player_->getHp()) / 400.0f;
	manager_.pushScene(new GameEnd(manager_,resultgraphHandle_, clockgraphHandle_,clearTimeSecond_,clearTimeMinute_, remainingHp_));

	/*fadeValue_ = static_cast <int>(255 * (static_cast<float>(fadeTimer_) / static_cast<float>(fadeInterval_)));
	if (++fadeTimer_ == fadeInterval_) {
		manager_.changeScene(new GameEnd(manager_));
		return;
	}*/
}
