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

	int m_waitInterval = 60;	//値が変化するインターバル
	int m_pressTime = 0;		//押しっぱなしの時間

	bool decision_ = false;

	/// <summary>
	/// BGMボリュームを加速しつつ変更する
	/// </summary>
	/// <param name="inputType">キー入力値</param>
	/// <param name="changeVal">変化の値</param>
	void AccelerateChangeBGMVolume(const InputState& input, InputType inputType, int changeVal);
	void AccelerateChangeSEVolume(const InputState& input, InputType inputType, int changeVal);
};

