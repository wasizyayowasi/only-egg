#include "SceneTitle.h"
#include "SceneManager.h"
#include "StageSelect.h"
#include "SoundSettingScene.h"
#include "EffekseerForDXLib.h"

#include "../util/SoundManager.h"
#include "../util/game.h"
#include "../util/InputState.h"

#include <algorithm>

#include "DxLib.h"

SceneTitle::SceneTitle(SceneManager& manager) : SceneBase(manager), updateFunc_(&SceneTitle::fadeInUpdate)
{
	handle_ = LoadGraph("data/graph/title.png");
}

SceneTitle::~SceneTitle()
{
	DeleteGraph(handle_);
}

/// <summary>
/// 更新
/// </summary>
/// <param name="input"></param>
void SceneTitle::update(const InputState& input)
{
	(this->*updateFunc_)(input);
}

/// <summary>
/// 描画
/// </summary>
void SceneTitle::draw()
{

	DrawBoxAA(649.5f, selectNum_ * 131.0f + 609.5f, 649.5f + 620.0f, selectNum_ * 131.0f + 709.5f, 0xff0000, true);

	DrawGraph(0, 0, handle_, true);


	SetDrawBlendMode(DX_BLENDMODE_ALPHA, fadeValue_);
	//画面全体を真っ黒に塗りつぶす
	DrawBox(0, 0, Game::kScreenWidth, Game::kScreenHeight, fadeColor_, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}

/// <summary>
/// フェードイン処理
/// </summary>
/// <param name="input"></param>
void SceneTitle::fadeInUpdate(const InputState& input)
{
	fadeValue_ = static_cast <int>(255 * (static_cast<float>(fadeTimer_) / static_cast<float>(fadeInterval_)));
	if (--fadeTimer_ == 0) {
		updateFunc_ = &SceneTitle::normalUpdate;
		fadeValue_ = 0;
	}
}

/// <summary>
/// 更新
/// </summary>
/// <param name="input"></param>
void SceneTitle::normalUpdate(const InputState& input)
{

	if (input.isTriggered(InputType::up)) {
		selectNum_ -= 1;
		selectNum_ = (std::max)(selectNum_, 0);
		SoundManager::getInstance().play("select");
	}
	if (input.isTriggered(InputType::down)) {
		selectNum_ += 1;
		selectNum_ = (std::min)(selectNum_, 2);
		SoundManager::getInstance().play("select");
	}

	if (input.isTriggered(InputType::next)) {
		SoundManager::getInstance().play("decision");
		switch (selectNum_)
		{
		case 0:
			updateFunc_ = &SceneTitle::fadeOutUpdate;
			break;
		case 1:
			manager_.pushScene(new SoundSettingScene(manager_));
			break;
		case 2:
			Effkseer_End();
			DxLib_End();
			break;
		}
	}

}

/// <summary>
/// フェードアウト処理
/// </summary>
/// <param name="input"></param>
void SceneTitle::fadeOutUpdate(const InputState& input)
{
	fadeValue_ = static_cast <int>(255 * (static_cast<float>(fadeTimer_) / static_cast<float>(fadeInterval_)));
	if (++fadeTimer_ == fadeInterval_) {
		manager_.changeScene(new StageSelect(manager_));
		return;
	}
}
