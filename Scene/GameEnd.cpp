#include "GameEnd.h"
#include "SceneManager.h"
#include "StageSelect.h"
#include "GameMain.h"

#include "../util/game.h"
#include "../util/InputState.h"
#include "DxLib.h"
#include <algorithm>

GameEnd::GameEnd(SceneManager& manager, int resultHandle, int figureHandle[], int secondTime, int minuteTime,float remainingHp) : SceneBase(manager), resultHandle_(resultHandle), secondTime_(secondTime), minuteTime_(minuteTime), remainingHp_(remainingHp)
{
	for (int i = 0; i < 13;i++) {
		figureHandle_[i] = figureHandle[i];
	}
}

GameEnd::~GameEnd()
{
	
}

/// <summary>
/// 更新
/// </summary>
/// <param name="input"></param>
void GameEnd::update(const InputState& input)
{
	if (input.isTriggered(InputType::left)) {
		cursorNum_--;
		cursorNum_ = (std::max)(cursorNum_, 0);
	}
	else if (input.isTriggered(InputType::right)) {
		cursorNum_++;
		cursorNum_ = (std::min)(cursorNum_, 1);
	}

	if (input.isTriggered(InputType::next)) {
		switch (cursorNum_) {
		case 0:
			manager_.changeScene(new GameMain(manager_));
			break;
		case 1:
			manager_.changeScene(new StageSelect(manager_));
			break;
		}
	}
}

/// <summary>
/// 描画
/// </summary>
void GameEnd::draw()
{

	SetDrawBlendMode(DX_BLENDMODE_MULA, 196);//乗算合成
	//ポーズウィンドウセロファン(黒い)
	DrawBox(0, 0, Game::kScreenWidth, Game::kScreenHeight, 0x000000, true);

	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);//通常描画に戻す
	
//	DrawBox(0, 0, Game::kScreenWidth, Game::kScreenHeight, 0xffffff, true);

	DrawBox(610, 780, 955, 860, 0xffffff, true);
	DrawBox(1010, 780,1355, 860, 0xffffff, true);
	DrawBox(cursorNum_ * 400 + 610, 780, cursorNum_ * 400 + 955, 860, 0xff0000, true);

	DrawGraph(0, 0, resultHandle_, true);

	int hpPercentRemaining = static_cast<int>(remainingHp_ * 100);
	//残りHP％の表示
	DrawGraph(Game::kScreenWidth / 2 +  70, 370, figureHandle_[hpPercentRemaining / 10], true);
	DrawGraph(Game::kScreenWidth / 2 + 100, 370, figureHandle_[hpPercentRemaining % 10], true);
	DrawGraph(Game::kScreenWidth / 2 + 140, 370, figureHandle_[11], true);
	//クリアタイム
	DrawGraph(Game::kScreenWidth / 2 +  70, 295, figureHandle_[minuteTime_ / 10], true);
	DrawGraph(Game::kScreenWidth / 2 + 100, 295, figureHandle_[minuteTime_ % 10], true);
	DrawGraph(Game::kScreenWidth / 2 + 125, 295, figureHandle_[10], true);
	DrawGraph(Game::kScreenWidth / 2 + 150, 295, figureHandle_[secondTime_ / 10], true);
	DrawGraph(Game::kScreenWidth / 2 + 180, 295, figureHandle_[secondTime_ % 10], true);

	if (hpPercentRemaining >= 50) {
		DrawGraph(Game::kScreenWidth / 2 + 110, Game::kScreenHeight / 2 - 10, figureHandle_[12], true);
	}

	DrawString(0, 0, "GameEnd", 0xffffff);

}

