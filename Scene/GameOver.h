#pragma once
#include "SceneBase.h"


class GameOver : public SceneBase
{
public:

	GameOver(SceneManager& manager,int stageNum ,int handle);
	virtual ~GameOver();

	void update(const InputState& input);
	void draw();

private:

	int stageNum_ = 0;

	int handle_ = -1;

	int cursorNum_ = 0;
};

