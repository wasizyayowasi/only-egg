#pragma once
#include "SceneBase.h"

class SceneTitle : public SceneBase
{
public:
	SceneTitle(SceneManager& manager);
	virtual ~SceneTitle();

	void update(const InputState& input);
	void draw();

private:
	void fadeInUpdate(const InputState& input);
	void normalUpdate(const InputState& input);
	void fadeOutUpdate(const InputState& input);

	int handle_ = -1;

	int selectNum_ = 0;

	static constexpr int fadeInterval_ = 60;
	int fadeTimer_ = fadeInterval_;
	int fadeValue_ = 255;
	int fadeColor_ = 0x000000;

	void (SceneTitle::* updateFunc_)(const InputState& input);
};

