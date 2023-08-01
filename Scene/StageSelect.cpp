#include "StageSelect.h"
#include "SceneManager.h"
#include "GameMain.h"
#include "SceneTitle.h"

#include "../util/SoundManager.h"
#include "../util/game.h"
#include "../util/InputState.h"
#include "../util/SoundManager.h"

#include <algorithm>
#include <fstream>
#include <iostream>
#include <string>
#include <stdio.h>
#include <sstream>

#include "DxLib.h"

namespace {
	const char* const fileName = "data/graph/stageSelect.png";

	const char* const talkative_person_file_name = "data/sound/BGM/bgm5.mp3";
}

StageSelect::StageSelect(SceneManager& manager):SceneBase(manager),updateFunc_(&StageSelect::fadeInUpdate)
{

	handle_ = LoadGraph(fileName);

	SoundManager::getInstance().stopBGM();
	SoundManager::getInstance().playMusic(talkative_person_file_name);

}

StageSelect::~StageSelect()
{

}

/// <summary>
/// 更新
/// </summary>
/// <param name="input"></param>
void StageSelect::update(const InputState& input)
{
	(this->*updateFunc_)(input);
}

/// <summary>
/// 描画
/// </summary>
void StageSelect::draw()
{
	DrawBox(208 + selectNum * 380, Game::kScreenHeight / 4 + 25, 208 + selectNum * 380 + 360, Game::kScreenHeight / 4 + 25 + 304, 0x990000, true);

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
void StageSelect::fadeInUpdate(const InputState& input)
{
	fadeValue_ = static_cast <int>(255 * (static_cast<float>(fadeTimer_) / static_cast<float>(fadeInterval_)));
	if (--fadeTimer_ == 0) {
		updateFunc_ = &StageSelect::normalUpdate;
		fadeValue_ = 0;
	}
}

/// <summary>
/// 更新
/// </summary>
/// <param name="input"></param>
void StageSelect::normalUpdate(const InputState& input)
{
	if (input.isTriggered(InputType::left)) {
		selectNum--;
		selectNum = (std::max)(selectNum, 0);
		SoundManager::getInstance().play("select");
	}
	else if (input.isTriggered(InputType::right)) {
		selectNum++;
		selectNum = (std::min)(selectNum, 3);
		SoundManager::getInstance().play("select");
	}

	if (input.isTriggered(InputType::next)) {
		SoundManager::getInstance().play("decision");
		if (selectNum != 3) {
			updateFunc_ = &StageSelect::fadeOutUpdate;
		}
	}
	if (input.isTriggered(InputType::prev)) {
		manager_.changeScene(new SceneTitle(manager_));
	}
}

/// <summary>
/// フェードアウト処理
/// </summary>
/// <param name="input"></param>
void StageSelect::fadeOutUpdate(const InputState& input)
{
	fadeValue_ = static_cast <int>(255 * (static_cast<float>(fadeTimer_) / static_cast<float>(fadeInterval_)));
	if (++fadeTimer_ == fadeInterval_) {
		manager_.changeScene(new GameMain(manager_,selectNum));
		return;
	}
}
