#include "StageSelect.h"
#include "SceneManager.h"
#include "GameMain.h"

#include "../util/game.h"
#include "../util/InputState.h"

#include <algorithm>
#include <fstream>
#include <iostream>
#include <string>
#include <stdio.h>
#include <sstream>

#include "DxLib.h"

StageSelect::StageSelect(SceneManager& manager):SceneBase(manager),updateFunc_(&StageSelect::fadeInUpdate)
{
	externalFileLoading();
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
	DrawString(0, 0, "stageSelect", 0xffffff);

	DrawBox(385 + selectNum * 300, Game::kScreenHeight / 2 - 125, 435 + selectNum * 300 + 200, Game::kScreenHeight / 2 + 125, 0xffffff, true);

	int x = Game::kScreenWidth / 2 - 550;
	for (int i = 0; i < 4; i++) {
		DrawBox(x, Game::kScreenHeight / 2 - 100, x + 200, Game::kScreenHeight / 2 + 100, 0x448844, true);
		x += 300;
	}

	SetDrawBlendMode(DX_BLENDMODE_ALPHA, fadeValue_);
	//画面全体を真っ黒に塗りつぶす
	DrawBox(0, 0, Game::kScreenWidth, Game::kScreenHeight, fadeColor_, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

}

void StageSelect::externalFileLoading()
{
	std::ifstream reading_file;
	std::string filename = "mapInformation.txt";
	reading_file.open(filename, std::ios::in);

	std::string reading_line_buffer;
	while (std::getline(reading_file, reading_line_buffer)) {
		std::cout << reading_line_buffer << std::endl;

		std::istringstream ss;
		ss = std::istringstream(reading_line_buffer);

	}

	
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
	}
	else if (input.isTriggered(InputType::right)) {
		selectNum++;
		selectNum = (std::min)(selectNum, 3);
	}

	if (input.isTriggered(InputType::next)) {
		updateFunc_ = &StageSelect::fadeOutUpdate;
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
