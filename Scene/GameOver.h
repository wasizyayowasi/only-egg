#pragma once
#include "SceneBase.h"


class GameOver : public SceneBase
{
public:

	GameOver(SceneManager& manager,int handle);
	virtual ~GameOver();

	void update(const InputState& input);
	void draw();

private:

	int handle_ = -1;

	int cursorNum_ = 0;
};

