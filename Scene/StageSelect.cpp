#include "StageSelect.h"
#include "SceneManager.h"
#include "GameMain.h"

#include "../util/game.h"
#include "../util/InputState.h"

#include <algorithm>
#include "DxLib.h"

StageSelect::StageSelect(SceneManager& manager):SceneBase(manager),updateFunc_(&StageSelect::fadeInUpdate)
{

}

StageSelect::~StageSelect()
{

}

/// <summary>
/// �X�V
/// </summary>
/// <param name="input"></param>
void StageSelect::update(const InputState& input)
{
	(this->*updateFunc_)(input);
}

/// <summary>
/// �`��
/// </summary>
void StageSelect::draw()
{
	DrawString(0, 0, "stageSelect", 0xffffff);

	DrawBox(75 + selectNum * 300, Game::kScreenHeight / 2 - 125, 125 + selectNum * 300 + 200, Game::kScreenHeight / 2 + 125, 0xffffff, true);

	int x = 100;
	for (int i = 0; i < 4; i++) {
		DrawBox(x, Game::kScreenHeight / 2 - 100, x + 200, Game::kScreenHeight / 2 + 100, 0x448844, true);
		x += 300;
	}

	SetDrawBlendMode(DX_BLENDMODE_ALPHA, fadeValue_);
	//��ʑS�̂�^�����ɓh��Ԃ�
	DrawBox(0, 0, Game::kScreenWidth, Game::kScreenHeight, fadeColor_, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

}

/// <summary>
/// �t�F�[�h�C������
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
/// �X�V
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
/// �t�F�[�h�A�E�g����
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
