#pragma once
#include "SceneBase.h"

class GameEnd : public SceneBase
{
public:
	GameEnd(SceneManager& manager,int resultHandle,int figureHandle[], int secondTime, int minuteTime,float remainingHp);
	virtual ~GameEnd();

	void update(const InputState& input);
	void draw();

private:

	int resultHandle_ = 0;
	int figureHandle_[13];
	int cursorNum_ = 0;
	int minuteTime_ = 0;
	int secondTime_ = 0;

	float remainingHp_ = 0.0f;

	static constexpr int fadeInterval_ = 60;
	int fadeTimer_ = fadeInterval_;
	int fadeValue_ = 255;
	int fadeColor_ = 0x000000;
};

