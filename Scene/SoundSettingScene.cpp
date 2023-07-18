#include "SoundSettingScene.h"
#include<DxLib.h>
#include "../util/SoundManager.h"
#include "../util/InputState.h"
#include "SceneManager.h"

namespace {
	constexpr int init_wait_interval = 60;//キー入力待ち間隔初期値
	constexpr int wait_interval_decrement_period = 5;//入力街間隔が落ちていく間隔
}

SoundSettingScene::SoundSettingScene(SceneManager&manager) : SceneBase(manager)
{
}

SoundSettingScene::~SoundSettingScene()
{
}

void SoundSettingScene::update(const InputState& input)
{
	auto& soundMgr = SoundManager::getInstance();
	//BGM
	AccelerateChangeBGMVolume(input, InputType::up, +1);//上で増える
	AccelerateChangeBGMVolume(input, InputType::down, -1);//下で減る

	//SE
	AccelerateChangeSEVolume(input, InputType::right, +1);//右で増える
	AccelerateChangeSEVolume(input, InputType::left, -1);//左で減る

	if (input.isTriggered(InputType::pause)) {
		manager_.popScene();
		soundMgr.saveSoundConfig();
	}
}

void SoundSettingScene::draw()
{

	constexpr int pw_width = 450;							//キーコンフィグ枠の幅
	constexpr int pw_height = 350;							//キーコンフィグ枠の高さ
	constexpr int pw_start_x = (640 - pw_width) / 2 + 50;	//キーコンフィグ枠に左
	constexpr int pw_start_y = (480 - pw_height) / 2 + 50;	//キーコンフィグ枠上

	//サウンドセッティングウィンドウ背景
	DrawBox(pw_start_x, pw_start_y, pw_start_x + pw_width, pw_start_y + pw_height, 0x884444, true);

	//キーコンフィグメッセージ
	DrawString(pw_start_x + 10, pw_start_y + 10, "SoundSetting...", 0xffffaa);

	auto& soundMgr = SoundManager::getInstance();
	//BGMの設定
	DrawFormatString(pw_start_x + 20, pw_start_y + 50,  0xffffaa, "BGM Volume = %d",soundMgr.getBGMVolume() );

	//SEの設定
	DrawFormatString(pw_start_x + 20, pw_start_y + 70, 0xffffaa, "SE Volume = %d", soundMgr.getSEVolume());

	//サウンドセッティングウィンドウ枠
	DrawBox(pw_start_x, pw_start_y, pw_start_x + pw_width, pw_start_y + pw_height, 0xffffff, false);
}

void SoundSettingScene::AccelerateChangeBGMVolume(const InputState& input,InputType inputType, int changeVal)
{
	auto& soundMgr = SoundManager::getInstance();

	if (input.isPressed(inputType)) {
		if (input.isTriggered(inputType)) {//押した瞬間
			m_pressTime = 0;
			m_waitInterval = init_wait_interval;
		}

		if (m_pressTime % m_waitInterval == 0) {
			soundMgr.setBGMVolume((std::max)((std::min)(soundMgr.getBGMVolume() + changeVal, 255),0));
		}

		if (m_pressTime % wait_interval_decrement_period == 0) {
			m_waitInterval = (std::max)(m_waitInterval - 1, 1);
		}

		m_pressTime++;
	}
}

void SoundSettingScene::AccelerateChangeSEVolume(const InputState& input, InputType inputType, int changeVal)
{
	auto& soundMgr = SoundManager::getInstance();

	if (input.isPressed(inputType)) {
		if (input.isTriggered(inputType)) {//押した瞬間
			m_pressTime = 0;
			m_waitInterval = init_wait_interval;
		}

		if (m_pressTime % m_waitInterval == 0) {
			soundMgr.setSEVolume((std::max)((std::min)(soundMgr.getSEVolume() + changeVal, 255), 0));
		}

		if (m_pressTime % wait_interval_decrement_period == 0) {
			m_waitInterval = (std::max)(m_waitInterval - 1, 1);
		}

		m_pressTime++;
	}
}
