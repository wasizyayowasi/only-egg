#pragma once

#include "SceneBase.h"

#include "../util/InputState.h"

class SoundSettingScene : public SceneBase
{
public:
	SoundSettingScene(SceneManager& manager);
	~SoundSettingScene();

	void update(const InputState& input);
	void draw();
private:

	int selectNum_ = 0;
	int color_ = 0xff0000;

	int eggHandle_ = 0;
	int soundHandle_ = 0;

	int m_waitInterval = 60;	//�l���ω�����C���^�[�o��
	int m_pressTime = 0;		//�������ςȂ��̎���

	bool decision_ = false;

	/// <summary>
	/// BGM�{�����[�����������ύX����
	/// </summary>
	/// <param name="inputType">�L�[���͒l</param>
	/// <param name="changeVal">�ω��̒l</param>
	void AccelerateChangeBGMVolume(const InputState& input, InputType inputType, int changeVal);
	void AccelerateChangeSEVolume(const InputState& input, InputType inputType, int changeVal);
};

