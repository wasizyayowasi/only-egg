#include "PauseScene.h"
#include "SceneManager.h"
#include "SceneTitle.h"
#include "StageSelect.h"
#include "GameOver.h"
#include "GameMain.h"

#include "DxLib.h"

#include "../object/Player.h"

#include "../util/InputState.h"
#include "../util/game.h"

#include<algorithm>

PauseScene::PauseScene(SceneManager& manager, int stageNum, std::shared_ptr<Player> player,int handle, int gameoverHandle):SceneBase(manager),stageNum_(stageNum), player_(player),handle_(handle), gameoverHandle_(gameoverHandle)
{
	
}

PauseScene::~PauseScene()
{
}

/// <summary>
/// 更新
/// </summary>
/// <param name="input"></param>
void PauseScene::update(const InputState& input)
{
	if (input.isTriggered(InputType::pause)) {
		manager_.popScene();
	}

	if (input.isTriggered(InputType::up)) {
		cursorNum_--;
		cursorNum_ = (std::max)(cursorNum_, 0);
	}
	else if (input.isTriggered(InputType::down)) {
		cursorNum_++;
		cursorNum_ = (std::min)(cursorNum_, 5);
	}
	
	if (input.isTriggered(InputType::next)) {
		switch (cursorNum_) {
		case 0:
			manager_.popScene();
			break;
		case 1:
			player_->resetInitial();
			manager_.popScene();
			break; 
		case 2:
			manager_.pushScene(new GameOver(manager_,stageNum_, gameoverHandle_));
			break;
		case 3:
			break;
		case 4:
			manager_.changeScene(new SceneTitle(manager_));
			break;
		}
	}

}

/// <summary>
/// 描画
/// </summary>
void PauseScene::draw()
{
	SetDrawBlendMode(DX_BLENDMODE_MULA, 196);//乗算合成
	//ポーズウィンドウセロファン(黒い)
	DrawBox(0, 0, Game::kScreenWidth, Game::kScreenHeight, 0x000000, true);

	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);//通常描画に戻す

	DrawGraph(0, 0, handle_, true);

	DrawString(500, cursorNum_ * 130 + 280, "右", 0xff0000);

}
