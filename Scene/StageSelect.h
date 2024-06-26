#pragma once
#include "SceneBase.h"

class StageSelect : public SceneBase
{
public:
	StageSelect(SceneManager& manager);
	virtual ~StageSelect();

	void update(const InputState& input);
	void draw();

private:

	int handle_ = -1;

	void fadeInUpdate(const InputState& input);
	void normalUpdate(const InputState& input);
	void fadeOutUpdate(const InputState& input);

	static constexpr int fadeInterval_ = 60;
	int fadeTimer_ = fadeInterval_;
	int fadeValue_ = 255;
	int fadeColor_ = 0x000000;

	void (StageSelect::* updateFunc_)(const InputState& input);

	int selectNum = 0;

};

