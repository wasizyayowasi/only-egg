#include "SoundSettingScene.h"
#include<DxLib.h>
#include "../util/SoundManager.h"
#include "../util/InputState.h"
#include "SceneManager.h"

namespace {
	constexpr int init_wait_interval = 60;//�L�[���͑҂��Ԋu�����l
	constexpr int wait_interval_decrement_period = 5;//���͊X�Ԋu�������Ă����Ԋu
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
	AccelerateChangeBGMVolume(input, InputType::up, +1);//��ő�����
	AccelerateChangeBGMVolume(input, InputType::down, -1);//���Ō���

	//SE
	AccelerateChangeSEVolume(input, InputType::right, +1);//�E�ő�����
	AccelerateChangeSEVolume(input, InputType::left, -1);//���Ō���

	if (input.isTriggered(InputType::pause)) {
		manager_.popScene();
		soundMgr.saveSoundConfig();
	}
}

void SoundSettingScene::draw()
{

	constexpr int pw_width = 450;							//�L�[�R���t�B�O�g�̕�
	constexpr int pw_height = 350;							//�L�[�R���t�B�O�g�̍���
	constexpr int pw_start_x = (640 - pw_width) / 2 + 50;	//�L�[�R���t�B�O�g�ɍ�
	constexpr int pw_start_y = (480 - pw_height) / 2 + 50;	//�L�[�R���t�B�O�g��

	//�T�E���h�Z�b�e�B���O�E�B���h�E�w�i
	DrawBox(pw_start_x, pw_start_y, pw_start_x + pw_width, pw_start_y + pw_height, 0x884444, true);

	//�L�[�R���t�B�O���b�Z�[�W
	DrawString(pw_start_x + 10, pw_start_y + 10, "SoundSetting...", 0xffffaa);

	auto& soundMgr = SoundManager::getInstance();
	//BGM�̐ݒ�
	DrawFormatString(pw_start_x + 20, pw_start_y + 50,  0xffffaa, "BGM Volume = %d",soundMgr.getBGMVolume() );

	//SE�̐ݒ�
	DrawFormatString(pw_start_x + 20, pw_start_y + 70, 0xffffaa, "SE Volume = %d", soundMgr.getSEVolume());

	//�T�E���h�Z�b�e�B���O�E�B���h�E�g
	DrawBox(pw_start_x, pw_start_y, pw_start_x + pw_width, pw_start_y + pw_height, 0xffffff, false);
}

void SoundSettingScene::AccelerateChangeBGMVolume(const InputState& input,InputType inputType, int changeVal)
{
	auto& soundMgr = SoundManager::getInstance();

	if (input.isPressed(inputType)) {
		if (input.isTriggered(inputType)) {//�������u��
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
		if (input.isTriggered(inputType)) {//�������u��
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
