#include "SceneTitle.h"
#include "SceneManager.h"
#include "StageSelect.h"

#include "../util/game.h"
#include "../util/InputState.h"
#include "DxLib.h"

SceneTitle::SceneTitle(SceneManager& manager) : SceneBase(manager), updateFunc_(&SceneTitle::fadeInUpdate)
{

}

SceneTitle::~SceneTitle()
{
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
	DrawString(0, 0, "SceneTitle", 0xffffff);

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
	if (input.isTriggered(InputType::next)) {
		updateFunc_ = &SceneTitle::fadeOutUpdate;
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
