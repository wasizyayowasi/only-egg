#include "SoundSettingScene.h"
#include "DxLib.h"
#include "../util/SoundManager.h"
#include "../util/InputState.h"
#include "SceneManager.h"

namespace {
	constexpr int init_wait_interval = 60;//キー入力待ち間隔初期値
	constexpr int wait_interval_decrement_period = 5;//入力街間隔が落ちていく間隔
	const char* const egg_file_name = "data/graph/egg.png";
	const char* const sound_file_name = "data/graph/sound.png";
}

SoundSettingScene::SoundSettingScene(SceneManager&manager) : SceneBase(manager)
{
	eggHandle_ = LoadGraph(egg_file_name);
	soundHandle_ = LoadGraph(sound_file_name);
}

SoundSettingScene::~SoundSettingScene()
{
	DeleteGraph(eggHandle_);
	DeleteGraph(soundHandle_);
}

void SoundSettingScene::update(const InputState& input)
{
	auto& soundMgr = SoundManager::getInstance();

	if (!decision_) {
		if (input.isTriggered(InputType::left)) {
			selectNum_ = 0;
		}
		else if (input.isTriggered(InputType::right)) {
			selectNum_ = 1;
		}
	}

	if (input.isTriggered(InputType::next)) {
		decision_ = true;
		color_ = 0x00ff00;
	}

	if (selectNum_ == 0) {
		//BGM
		if (decision_) {
			AccelerateChangeBGMVolume(input, InputType::up, +50);//上で増える
			AccelerateChangeBGMVolume(input, InputType::down, -50);//下で減る
		}
	}
	else {
		//SE
		if (decision_) {
			AccelerateChangeSEVolume(input, InputType::up, +50);//右で増える
			AccelerateChangeSEVolume(input, InputType::down, -50);//左で減る
		}

	}
	

	
	if (input.isTriggered(InputType::pause)) {
		manager_.popScene();
		soundMgr.saveSoundConfig();
	}
	if (input.isTriggered(InputType::prev)) {
		if (!decision_) {
			manager_.popScene();
			soundMgr.saveSoundConfig();
		}
		else {
			decision_ = false;
			color_ = 0xff0000;
		}
		
	}

}

void SoundSettingScene::draw()
{

	constexpr int pw_width = 450;							//キーコンフィグ枠の幅
	constexpr int pw_height = 350;							//キーコンフィグ枠の高さ
	constexpr int pw_start_x = (640 - pw_width) / 2 + 50;	//キーコンフィグ枠に左
	constexpr int pw_start_y = (480 - pw_height) / 2 + 50;	//キーコンフィグ枠上

	DrawGraph(0, 0, soundHandle_, true);

	DrawBox(selectNum_ * 900 + 300, 130, selectNum_ * 900 + 820, 950, color_, false);

	auto& soundMgr = SoundManager::getInstance();

	int BGMVolume = soundMgr.getBGMVolume() / 50;
	for (int i = 0; i < BGMVolume; i++) {
		DrawGraph(500, -i * 140 + 800,eggHandle_, true);
	}

	int SEVolume = soundMgr.getSEVolume() / 50;
	for (int i = 0; i < SEVolume; i++) {
		DrawGraph(1400, -i * 140 + 800, eggHandle_, true);
	}
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
