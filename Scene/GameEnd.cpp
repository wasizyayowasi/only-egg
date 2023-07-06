#include "GameEnd.h"
#include "SceneManager.h"
#include "StageSelect.h"
#include "GameMain.h"

#include "../util/game.h"
#include "../util/InputState.h"
#include "DxLib.h"
#include <algorithm>

GameEnd::GameEnd(SceneManager& manager, int resultHandle, int figureHandle[], int secondTime, int minuteTime,float remainingHp,int baconNum) : 
	SceneBase(manager),
	resultHandle_(resultHandle),
	secondTime_(secondTime),
	minuteTime_(minuteTime), 
	remainingHp_(remainingHp),
	baconNum_(baconNum)
{
	for (int i = 0; i < 16;i++) {
		figureHandle_[i] = figureHandle[i];
	}

	for (auto& rewards : questRewards) {
		rewards = false;
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

	if (minuteTime_ < 7) {
		if (minuteTime_ <= 6) {
			questRewards[0] = true;
		}
		else if(minuteTime_ == 7 && secondTime_ == 0) {
			questRewards[0] = true;
		}
	}

	if (remainingHp_ * 100 > 50) {
		questRewards[1] = true;
	}

	if (baconNum_ == 2) {
		questRewards[2] = true;
	}

}

/// <summary>
/// 描画
/// </summary>
void GameEnd::draw()
{

	SetDrawBlendMode(DX_BLENDMODE_ADD, 100);//乗算合成
	//ポーズウィンドウセロファン(黒い)
	DrawBox(Game::kScreenWidth / 4, Game::kScreenHeight / 10, Game::kScreenWidth / 4 * 3, Game::kScreenHeight / 6 * 5, 0xffffff, true);

	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);//通常描画に戻す

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
	//ベーコン獲得数
	DrawGraph(Game::kScreenWidth / 2 +  80, 595, figureHandle_[baconNum_], true);
	DrawGraph(Game::kScreenWidth / 2 + 110, 595, figureHandle_[13], true);
	DrawGraph(Game::kScreenWidth / 2 + 140, 595, figureHandle_[2], true);

	int x = Game::kScreenWidth / 2 - 80;
	//星
	for (auto& rewards : questRewards) {
		if (rewards) {
			DrawGraph(x, 670, figureHandle_[14], true);
		}
		else {
			DrawGraph(x, 670, figureHandle_[15], true);
		}
		x += 80;
	}

	DrawGraph(Game::kScreenWidth / 2 + 140, 595, figureHandle_[2], true);

	if (questRewards[0]) {
		DrawGraph(Game::kScreenWidth / 2 + 110, Game::kScreenHeight / 2 - 75, figureHandle_[12], true);
	}
	if (questRewards[1]) {
		DrawGraph(Game::kScreenWidth / 2 + 110, Game::kScreenHeight / 2 - 10, figureHandle_[12], true);
	}

}

