#include "GameOver.h"
#include "SceneManager.h"
#include "StageSelect.h"
#include "GameMain.h"

#include "../util/InputState.h"
#include "../util/game.h"

#include "DxLib.h"

GameOver::GameOver(SceneManager& manager, int stageNum, int handle) : SceneBase(manager),stageNum_(stageNum), handle_(handle)
{

}

GameOver::~GameOver()
{

}

void GameOver::update(const InputState& input)
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
			manager_.changeScene(new GameMain(manager_,stageNum_));
			break;
		case 1:
			manager_.changeScene(new StageSelect(manager_));
			break;
		}
	}
}

void GameOver::draw()
{
	SetDrawBlendMode(DX_BLENDMODE_ADD, 150);//乗算合成
	//ポーズウィンドウセロファン(黒い)
	DrawBox(Game::kScreenWidth / 4, Game::kScreenHeight / 6, Game::kScreenWidth / 4 * 3, Game::kScreenHeight / 6 * 5, 0xffffff, true);

	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);//通常描画に戻す

	DrawBox(Game::kScreenWidth / 2 - 375, 630, Game::kScreenWidth / 2 - 30 , 710, 0xffffff, true);
	DrawBox(Game::kScreenWidth / 2 + 30 , 630, Game::kScreenWidth / 2 + 375, 710, 0xffffff, true);
	DrawBox(Game::kScreenWidth / 2 - 375 + cursorNum_ * 405, 630, Game::kScreenWidth / 2 - 30 + cursorNum_ * 405, 710, 0xff0000, true);

	DrawGraph(0, 0, handle_, true);

}
